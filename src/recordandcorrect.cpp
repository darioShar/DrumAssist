#include "recordandcorrect.h"
#include "ui_recordandcorrect.h"

/** Min and max RMS value to consider that there has been a hit */
d_real minRMSHitValue = 0.15;
d_real maxRMSHitValue = 0.45;

/** Min Volume value to consider that there has been a hit */
d_real minVolumeHitValue = 0.6;


RecordAndCorrect::RecordAndCorrect(QWidget *parent,
                                   const AudioInfo * const ai, const QAudioFormat * const af,
                                   FrequencySpectrum * const fs, RenderArea * const ra, ArduinoManager * const am)
    :   QDialog(parent)
    ,   ui(new Ui::RecordAndCorrect)
    ,   m_audioInfo(ai)
    ,   m_format(af)
    ,   m_canvas(ra)
    ,   m_frequencySpectrum(fs)
    ,   m_microControler(am)
{
    ui->setupUi(this);

    // Setting up QFrame in which we'll place frequency spectrum
    ui->frameSpectrum->setWindowTitle("Qt SFML");
    //ui->frameSpectrum->setGeometry(QRect(0,0,ui->groupBoxSpectrum->width(),ui->groupBoxSpectrum->height()));
    ui->frameSpectrum->show();

    // Setting up layout including our peak volume analyser and everything
    ui->layoutInput->addWidget(ra);
    repaint();
    update();

    //SFML Frequency spectrum window (specifying widget to put it into, point where to begin window and its size).
    m_frequencySpectrum->QWidget::close();
    m_frequencySpectrum->setParent(ui->frameSpectrum);
    m_frequencySpectrum->adaptSize(ui->frameSpectrum->size());
    m_frequencySpectrum->show();

    // Then connect everybody.
    connect(m_audioInfo, SIGNAL(FFTCalculationDone(std::vector<d_real> const&)),
            m_frequencySpectrum, SLOT(displayFrequencySpectrum(std::vector<d_real> const&)));
    connect(m_audioInfo, SIGNAL(volumeCalculationDone(d_real, d_real)),
            m_canvas, SLOT(setVolumeData(d_real,d_real)));
    connect(m_audioInfo, SIGNAL(allCalculationDone(AudioProperty  const&, std::vector<d_real>  const&)),
            this, SLOT(analyseAudio(AudioProperty const&, std::vector<d_real> const&)));
    connect(m_audioInfo, SIGNAL(calculationDone(AudioProperty const&)),
            this, SLOT(displayAudioInfo(AudioProperty const&)));

    // Setting up arduino manager, connecting to labels.
    connect(m_microControler, SIGNAL(arduinoPortName(QString)), ui->labelPort, SLOT(setText(QString)));

    // Start a timer which will emit a signal once it times out, in order to update any kind of information we want (via connect function).
    m_timer.start(1000);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateAnything()));
}

RecordAndCorrect::~RecordAndCorrect()
{
    close();
    update(); // Maybe ?
    delete ui;
    qWarning() << "Record And Correct window destroyed.";
}

void RecordAndCorrect::updateAnything(){
    //m_microcontroler->writeByteToArduino(static_cast<char>(static_cast<int>(m_engine->getFundamental()) % 181));
}

void RecordAndCorrect::displayAudioInfo(AudioProperty const& audioProp) {
    ui->lcdFundamental->display(audioProp.fundamental);
    ui->lcdDC->display(audioProp.DC * 1000);
}

void RecordAndCorrect::analyseAudio(AudioProperty const& ap, std::vector<d_real> const& fs) {
    Q_UNUSED(ap);
    std::vector<d_real> crossCorrelationResults = m_hra.analyseHit(fs, *m_format);
    // For now we'll only send correlation with harmonic hit (that is hit_1.raw file, crossCorrelationResults[1]).
    if (crossCorrelationResults.size() >= 2) {
        qWarning() << "Cross Correlation : " << crossCorrelationResults[0] << " " << crossCorrelationResults[1];
        ui->progressBarCC0->setValue(50*crossCorrelationResults[0]);
        ui->progressBarCC1->setValue(50*crossCorrelationResults[1]);
        if (crossCorrelationResults[0] >= 0.5f && crossCorrelationResults[1] >= 0.5f && ap.level >= 0.5 && ap.rms >= 0.1) {
            qWarning() << "Hit detected, with cross correlation being " << (((crossCorrelationResults[0] + crossCorrelationResults[1])/2.0f)/2.0f);
            if(!m_microControler->writeByteToArduino(
                        static_cast<char>(qMin(static_cast<int>((255.0f * ((crossCorrelationResults[0] + crossCorrelationResults[1])/2.0f)/2.0f)), 255)))
                    )
                qWarning() << "Failed to send corrections to Arduino !";
        }
    }

    /*d_real result = m_hra.analyseHit(fs, (m_format->sampleRate()/2.0) / (d_real)fs.size());
    if(result < -0.1f) {
        m_microControler->writeByteToArduino(1);
        qWarning() << "Frappe centrée détectée";
    }
    else if (result > 0.05f) {
        m_microControler->writeByteToArduino(1<<1);
        qWarning() << "Frappe harmonique détectée";
    }*/
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
 * WIDGETS HANDLING
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

void RecordAndCorrect::on_buttonReloadProtocol_clicked() {
    m_hra.loadDatabase();
    qWarning() << "Database reloaded.";
}

void RecordAndCorrect::on_buttonQuit_clicked() {
    this->close();
    emit closed("<font color='red'>Status : Stopped</font>");
}

void RecordAndCorrect::on_sliderFreq_valueChanged(int value) {
    //ui->labelFreq->setText(QString("Precision : ") + QString::number(value) + QString(" Hz"));
    emit(spectrumFreqChanged(value));
}

