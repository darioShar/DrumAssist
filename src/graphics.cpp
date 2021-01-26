#include "graphics.h"


/**
 *
 *
 *
 *
 *
 *
 *
 *
 * @brief QSFMLCanvas::QSFMLCanvas
 * @param Parent
 * @param Position
 * @param Size
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

QSFMLCanvas::QSFMLCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size) :
        QWidget(Parent),
        myInitialized(false)
{
    // Mise en place de quelques options pour autoriser le rendu direct dans le widget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    // Changement de la police de focus, pour autoriser notre widget à capter les évènements clavier
    setFocusPolicy(Qt::StrongFocus);

    // Définition de la position et de la taille du widget
    move(Position);
    resize(Size);

    // Ajout du VSync
    setVerticalSyncEnabled(true);
}


#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif

void QSFMLCanvas::showEvent(QShowEvent*)
{
    if (!myInitialized)
    {
        // Sous X11, il faut valider les commandes qui ont été envoyées au serveur
        // afin de s'assurer que SFML aura une vision à jour de la fenêtre
        #ifdef Q_WS_X11
            XFlush(QX11Info::display());
        #endif

        // On crée la fenêtre SFML avec l'identificateur du widget
        sf::Window::create(reinterpret_cast<sf::WindowHandle>(winId()));

        // On laisse la classe dérivée s'initialiser si besoin
        OnInit();

        myInitialized = true;
    }
}

QPaintEngine* QSFMLCanvas::paintEngine() const
{
    return 0;
}

void QSFMLCanvas::paintEvent(QPaintEvent*)
{
}



/**
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * @brief FrequencySpectrum::FrequencySpectrum
 * @param Parent
 * @param Position
 * @param Size
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */


FrequencySpectrum::FrequencySpectrum(QSize Size) :
    QSFMLCanvas(0, QPoint(0,0), Size)
{
    m_windowSize = Size;
    isSpectrumInit = false;
    // It appears that the size given is slighlty off. Correcting a bit :
    /*m_windowSize = Size(m_windowSize.width() - 24, m_windowSize.height() - 30);*/
}

FrequencySpectrum::~FrequencySpectrum() {
    isSpectrumInit = false;
    //m_spectrum.clear();
}

void FrequencySpectrum::OnInit(){
    /* Initialize each rectangle size, position, color and origin(which we'll make bottom left).
    *  INFORMATION !!! setOrigin translate the object to the bottom, so that now when we set its 'y' size with
    *  'y' a negative number, it goes up from the bottom. */

    m_spectrum.resize(m_graphParameters.numBins);
    if (m_graphParameters.scale == GraphScale::Linear) {
        d_real binWidth = m_windowSize.width() / m_graphParameters.numBins;
        for (int i = 0; i < m_graphParameters.numBins; i++) {
            m_spectrum[i] = sf::RectangleShape(sf::Vector2f(binWidth - m_graphParameters.space,  0.0f));
            m_spectrum[i].setOrigin(0.0f, -m_windowSize.height());
            m_spectrum[i].move(binWidth * i, 0.0f);
            m_spectrum[i].setFillColor(sf::Color(255,255,255));
        }
    }
    else {
        d_real pos = 0.0f;
        d_real binWidth;
        d_real frequencyDifference = (m_graphParameters.finalFrequency - m_graphParameters.initialFrequency) / m_graphParameters.numBins;

        // Calculating reszing factor so that the whole graph is filled.
        d_real resizingFactor = m_windowSize.width() / log10(m_graphParameters.numBins);

        // First bin calculated out of loop because of log10(0.0) case.
        binWidth = log10(frequencyDifference);
        m_spectrum[0] = sf::RectangleShape(sf::Vector2f(binWidth - m_graphParameters.space, 0.0f));
        m_spectrum[0].setOrigin(0.0f, -m_windowSize.height());
        m_spectrum[0].move(pos, 0.0f);
        m_spectrum[0].setFillColor(sf::Color(255,255,255));
        pos += binWidth;

        for (int i = 1; i < m_graphParameters.numBins; i++) {
            binWidth = (log10f((i + 1) * frequencyDifference) - log10f(i * frequencyDifference)) * resizingFactor;
            m_spectrum[i] = sf::RectangleShape(sf::Vector2f(binWidth - m_graphParameters.space,  0.0f));
            m_spectrum[i].setOrigin(0.0f, -m_windowSize.height());
            m_spectrum[i].move(pos, 0.0f);
            m_spectrum[i].setFillColor(sf::Color(255,255,255));
            pos += binWidth;
        }
    }

    if (!m_shader.loadFromFile(SHADER_FILE, sf::Shader::Fragment)) {
        QMessageBox::warning(0, "ERROR", "Couldn't load fragment shader file !");
    }

    isSpectrumInit = true;
    qWarning() << "Spectrum Initialized.";
}

void FrequencySpectrum::OnUpdate(){
    clear(sf::Color::Black);
    for (int i = 0; i < m_graphParameters.numBins; i++) {
        // Do not forget that m_spectrum[i].getSize().y spits out a negative number
        // (opposite of the height, because of how we made the rectangles).
        m_shader.setParameter("height", -(d_real)m_spectrum[i].getSize().y / (d_real)m_windowSize.height());
        draw(m_spectrum[i], &m_shader);
    }
}

void FrequencySpectrum::update() {
    OnUpdate();
    display();
}

void FrequencySpectrum::reInitialize() {
    isSpectrumInit = false;
    m_spectrum.clear();
    m_spectrum.resize(m_graphParameters.numBins);
    /*
    for (int i = 0; i < m_nbFrequencies; i++) {
        // m_rectanglesWidth - x : x is the space between 2 consecutives rectangles.
        m_spectrum[i] = sf::RectangleShape(sf::Vector2f(m_rectanglesWidth - m_space,  0.0f));
        m_spectrum[i].setOrigin(0.0f, -m_windowSize.height());
        m_spectrum[i].move(m_rectanglesWidth * i, 0.0f);
        m_spectrum[i].setFillColor(sf::Color(255,255,255));
    }
    */
    if (m_graphParameters.scale == GraphScale::Linear) {
        d_real binWidth = m_windowSize.width() / m_graphParameters.numBins;
        for (int i = 0; i < m_graphParameters.numBins; i++) {
            m_spectrum[i] = sf::RectangleShape(sf::Vector2f(binWidth - m_graphParameters.space,  0.0f));
            m_spectrum[i].setOrigin(0.0f, -m_windowSize.height());
            m_spectrum[i].move(binWidth * i, 0.0f);
            m_spectrum[i].setFillColor(sf::Color(255,255,255));
        }
    }
    else {
        d_real pos = 0.0f;
        d_real binWidth;
        d_real frequencyDifference = (m_graphParameters.finalFrequency - m_graphParameters.initialFrequency) / m_graphParameters.numBins;

        // Calculating reszing factor so that the whole graph is filled.
        d_real resizingFactor = m_windowSize.width() / log10f(m_graphParameters.numBins);

        // First bin calculated out of loop because of log10(0.0f) case.
        binWidth = log10f(frequencyDifference);
        m_spectrum[0] = sf::RectangleShape(sf::Vector2f(binWidth - m_graphParameters.space, 0.0f));
        m_spectrum[0].setOrigin(0.0f, -m_windowSize.height());
        m_spectrum[0].move(pos, 0.0f);
        m_spectrum[0].setFillColor(sf::Color(255,255,255));
        pos += binWidth;

        for (int i = 1; i < m_graphParameters.numBins; i++) {
            binWidth = (log10f((i + 1) * frequencyDifference) - log10f(i * frequencyDifference)) * resizingFactor;
            m_spectrum[i] = sf::RectangleShape(sf::Vector2f(binWidth - m_graphParameters.space,  0.0f));
            m_spectrum[i].setOrigin(0.0f, -m_windowSize.height());
            m_spectrum[i].move(pos, 0.0f);
            m_spectrum[i].setFillColor(sf::Color(255,255,255));
            pos += binWidth;
        }
    }

    if (!m_shader.loadFromFile(SHADER_FILE, sf::Shader::Fragment)) {
        QMessageBox::warning(0, "ERROR", "Couldn't load fragment shader file !");
    }
    isSpectrumInit = true;
}


void FrequencySpectrum::changeShader(QString &shaderFile) {
    if (!m_shader.loadFromFile(shaderFile.toLocal8Bit().constData(), sf::Shader::Fragment)) {
        QMessageBox::warning(0, "ERROR", "Couldn't change fragment shader file !");
    }
}

void FrequencySpectrum::setGraphParameters(GraphParameters const &param) {
    m_graphParameters = param;
    reInitialize();
}


std::vector<d_real> FrequencySpectrum::getAllHeights(){
    std::vector<d_real> heights(m_graphParameters.numBins, 0.0f);
    if (isSpectrumInit){
        for (int i = 0; i < m_graphParameters.numBins; i++) {
            heights[i] = -m_spectrum[i].getSize().y;
        }
    }
    // if spectrum wasn't initialized, sends back a vector filled with 0.0f.
    return heights;
}

bool FrequencySpectrum::setAllHeights(std::vector<d_real> const &newHeights, d_real resizingFactor, bool concave, d_real concavingFactor){
    // If succesful, will return true, hence bool return type.
    if (isSpectrumInit && (newHeights.size() >= m_graphParameters.numBins)){
        if (concave) {
            for (int i = 0; i < m_graphParameters.numBins; i++) {
               m_spectrum[i].setSize(sf::Vector2f((float)m_spectrum[i].getSize().x,
                                                  -powf((float)(std::abs(newHeights[i])), 1.0f/concavingFactor))*(float)resizingFactor);
            }
        }
        else {
            for (int i = 0; i < m_graphParameters.numBins; i++) {
                m_spectrum[i].setSize(sf::Vector2f(m_spectrum[i].getSize().x, -newHeights[i]*resizingFactor*5.0f));
                //m_spectrum[i].setSize(sf::Vector2f(m_spectrum[i].getSize().x, -newHeights[i]*resizingFactor));
            }
        }
        return true;
    }
    return false;
}

void FrequencySpectrum::resizeAllHeight(d_real newHeight) {
    if (isSpectrumInit){
        for (int i = 0; i < m_graphParameters.numBins; i++) {
            m_spectrum[i].setSize(sf::Vector2f(m_spectrum[i].getSize().x, -newHeight));
        }
    }
}

void FrequencySpectrum::displayFrequencySpectrum(std::vector<d_real> const& fs) {
    setAllHeights(fs, m_windowSize.height());
    update();
}

void FrequencySpectrum::adaptSize(QSize size) {
    m_windowSize = size;
    setSize(sf::Vector2u(size.width(), size.height()));
    resize(size);
}

QSize FrequencySpectrum::getSize() {
    return m_windowSize;
}


/**
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * @brief RenderArea::RenderArea
 * @param parent
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */




RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    m_level = 0;
    m_rms = 0;
    setMinimumHeight(30);
    setMinimumWidth(200);
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    QPainter painterLevel(this);

    painterLevel.setPen(Qt::black);
    painterLevel.drawRect(QRect(painterLevel.viewport().left()+10,
                           painterLevel.viewport().top()+10,
                           painterLevel.viewport().right()-20,
                           painterLevel.viewport().bottom()-20));

    // Don't need this.
    /*if (m_level == 0.0)
        return;*/

    int posLevel = ((painterLevel.viewport().right()-20)-(painterLevel.viewport().left()+11))*m_level;
    painterLevel.fillRect(painterLevel.viewport().left()+11,
                     painterLevel.viewport().top()+10,
                     posLevel,
                     painterLevel.viewport().height()-21,
                     Qt::red);

    QPainter painterRMS(this);

    painterRMS.setPen(Qt::black);
    painterRMS.drawRect(QRect(painterRMS.viewport().left()+10,
                           painterRMS.viewport().top()+10,
                           painterRMS.viewport().right()-20,
                           painterRMS.viewport().bottom()-20));

    int posRMS = ((painterRMS.viewport().right()-20)-(painterRMS.viewport().left()+11))*m_rms;
    painterRMS.fillRect(painterRMS.viewport().left()+11,
                     painterRMS.viewport().top()+10,
                     posRMS,
                     painterRMS.viewport().height()-21,
                     Qt::darkRed);
}

void RenderArea::setVolumeData(d_real level, d_real rms){
    m_level = level;
    m_rms = rms;
    this->update();
}

void RenderArea::setLevel(d_real value)
{
    m_level = value;
    this->update();
}

void RenderArea::setRMS(d_real value)
{
    m_rms = value;
    this->update();
}



/**
 *
 *
 *
 *
 *
 *
 *
 *
 * Percussion
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * */


PercussionSystem::PercussionSystem(QSize const &windowSize)
    :   QSFMLCanvas(0, QPoint(0,0), windowSize)
    ,   m_windowSize(windowSize)
{
    initialize(m_windowSize);
}

PercussionSystem::~PercussionSystem() {

}

void PercussionSystem::initialize(QSize const &windowSize) {
    m_windowSize = windowSize;
    setSize(sf::Vector2u(windowSize.width(), windowSize.height()));
    resize(windowSize);

    // Setting objects properties
    m_stick.setSize(sf::Vector2f(m_windowSize.width() / 20.0f, m_windowSize.height() / 5.0f));
    m_stick.setFillColor(sf::Color::Black);
    m_stick.setOrigin(m_windowSize.width() / 40.0f, 0.0f);

    float radius = qMin(m_windowSize.width(), m_windowSize.height()) / 2.0f;
    float shortRadius = radius / 5.0f;

    m_percussion.setRadius(radius);
    m_percussion.setFillColor(sf::Color::White);
    m_percussion.setOutlineThickness(10.0f);
    m_percussion.setOutlineColor(sf::Color(255, 255, 0));
    m_percussion.setOrigin(radius, radius);

    m_hitPoint.setRadius(shortRadius);
    m_hitPoint.setFillColor(sf::Color::Red);

    m_nextHitPoint.setRadius(shortRadius);
    m_nextHitPoint.setFillColor(sf::Color::Red);

    // Setting objects positions
    m_stick.setPosition(m_windowSize.width() / 2.0f, m_windowSize.height() / 20.0f);
    m_hitPoint.setOrigin(m_stick.getPosition());
    m_nextHitPoint.setOrigin(m_stick.getPosition());

    m_percussion.setPosition(m_windowSize.width() / 2.0f, m_windowSize.height() / 20.0f);
    m_hitPoint.setPosition(m_windowSize.width() / 2.0f, radius);
    m_nextHitPoint.setPosition(m_windowSize.width() / 2.0f, radius);

    // And finally maxRotation
    m_maxRotation = atanf(sinf(acosf((radius - m_stick.getSize().y) / radius)) / m_stick.getSize().y) * 57.295779513;
}


void PercussionSystem::OnInit() {
    // I don't think that does anything.
    initialize(m_windowSize);
}

void PercussionSystem::OnUpdate() {
    clear(sf::Color::White);
    draw(m_stick);
    draw(m_percussion);
    draw(m_hitPoint);
    draw(m_nextHitPoint);
}

void PercussionSystem::update() {
    OnUpdate();
    display();
}

void PercussionSystem::setStickPosition(d_real rot) {
    m_stick.setRotation(rot * m_maxRotation);
    m_hitPoint.setRotation(rot * m_maxRotation);
}

void PercussionSystem::setNextHitPosition(d_real nextHit) {

}























