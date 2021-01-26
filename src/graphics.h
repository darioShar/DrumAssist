#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QAudioInput>
#include <QByteArray>
#include <QObject>
#include <QWidget>
#include <QTimer>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <QDebug>
#include <QPainter>
#include <QMessageBox>
#include <cmath>
#include "datatypes.h"

#define FRAME_TIME_MS 1000
#define SHADER_FILE "D:\\Desktop\\Programme C++\\ProjectDjembe\\shaders\\colorHeight1.frag"


/** SFML Base oject on which we'll derive our frequency spectrum */
class QSFMLCanvas : public QWidget, public sf::RenderWindow
{
    Q_OBJECT
public :
    QSFMLCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size);
    virtual ~QSFMLCanvas(){}

private :
    virtual void OnInit() = 0;
    virtual void OnUpdate() = 0;
    virtual QPaintEngine* paintEngine() const;
    virtual void showEvent(QShowEvent*);
    virtual void paintEvent(QPaintEvent*);

    bool   myInitialized;
};




/**
 *
 *
 *
 *
 *
 * For our frequency spectrum
 *
 *
 *
 *
 *
 * */

enum GraphScale {
    Linear,
    Logarithmic
};

/* Some Default Parameters*/
const int nbFrequencies = 100;
const GraphScale defaultScale = GraphScale::Linear;
const int Space = 0;

struct GraphParameters {
    GraphParameters(d_real init = 0.0f, d_real final = 4000.0f, int bins = nbFrequencies, GraphScale sc = defaultScale, int sp = Space)
        :   initialFrequency(init)
        ,   finalFrequency(final)
        ,   numBins(bins)
        ,   scale(sc)
        ,   space(sp)
    {}
    d_real       initialFrequency;
    d_real       finalFrequency;
    int         numBins;
    GraphScale  scale;
    int         space;
};


class FrequencySpectrum : public QSFMLCanvas
{
    Q_OBJECT
public :

    FrequencySpectrum(QSize = QSize(256, 256));
    ~FrequencySpectrum();

    void update();

    void reInitialize();

    void adaptSize(QSize size);
    QSize getSize();
    void setGraphParameters(GraphParameters const &);
    void changeShader(QString &shaderFile);

    void resizeAllHeight(d_real newHeight);
    bool setAllHeights(std::vector<d_real> const &newHeights, d_real resizingFactor = 1.0f, bool concave = 0, d_real concavingFactor = 2.0);
    std::vector<d_real> getAllHeights();

public slots :
    void displayFrequencySpectrum(std::vector<d_real> const&);

private :
    void OnInit();
    void OnUpdate();
    GraphParameters                 m_graphParameters;
    QSize                           m_windowSize;
    std::vector<sf::RectangleShape> m_spectrum;         // Holds all rectangles to be displayed.
    sf::Shader                      m_shader;           // Shader to change a rectangle color according to its height.
    bool                            isSpectrumInit;     // Makes sure we don't access m_spectrum while it hasn't been initialized.
};




/** Render Area for audio peak level */
class RenderArea : public QWidget
{
    Q_OBJECT

public:
    RenderArea(QWidget *parent = 0);

public slots :
    void setVolumeData(d_real level, d_real rms);
    void setLevel(d_real value);
    void setRMS(d_real value);

protected:
    void paintEvent(QPaintEvent *event);

private:
    d_real      m_level;
    d_real      m_rms;
    QPixmap     m_pixmap;
};



/** To represent the percussion */

class PercussionSystem : public QSFMLCanvas {
    QSize m_windowSize;

    sf::RectangleShape m_stick;
    sf::CircleShape m_percussion;
    sf::CircleShape m_hitPoint;
    sf::CircleShape m_nextHitPoint;

    d_real m_maxRotation;

    void OnInit();
    void OnUpdate();

public :
    PercussionSystem(QSize const &windowSize);
    ~PercussionSystem();

    void update();

    void initialize(QSize const &windowSize);
    void setStickPosition(d_real);
    void setNextHitPosition(d_real);
};

#endif // GRAPHICS_H
