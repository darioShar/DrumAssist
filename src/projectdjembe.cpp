#include "projectdjembe.h"
#include "recordandcorrect.h"
#include "ui_projectdjembe.h"


#define FATAL_ERROR(name) {qWarning() << (#name " object allocation has failed. Exiting program"); delete this;}


/** Well, the name is pretty explicit. */
d_real minFrequencyShownOnSpectrum = 10000.0f;


ProjectDjembe::ProjectDjembe(QWidget *parent)
    :   QDialog(parent)
    ,   ui(new Ui::ProjectDjembe)
    ,   m_recordAndCorrect(nullptr)
    ,   m_settingsWindow(nullptr)
    ,   m_engine(nullptr)
    ,   m_audioInfo(nullptr)
    ,   m_arduino(nullptr)
    ,   m_audioRecorder(nullptr)
    ,   m_volumeGraph(nullptr)
    ,   m_freqSpectrum(nullptr)
    ,   m_timer(nullptr)
    ,   m_chronoProgBarUpload(nullptr)
    ,   m_valueProgBarUpload(0)
{
    ui->setupUi(this);

    /* Setting up audio format that will be used everywhere */
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setChannelCount(1);
    m_format.setCodec("audio/pcm");
    m_format.setSampleRate(48000);
    m_format.setSampleSize(8*sizeof(audio_data_type));
    m_format.setSampleType(QAudioFormat::SignedInt);

    /* Audio engine */
    m_engine = new(std::nothrow) Engine(&m_format);
    if (!m_engine) FATAL_ERROR(Engine)

    /* Audio analysis */
    m_audioInfo = new(std::nothrow) AudioInfo(&m_format);
    if (!m_audioInfo) FATAL_ERROR(AudioInfo)

    /* Setting up arduino manager, connecting to labels. */
    m_arduino = new(std::nothrow) ArduinoManager;
    if (!m_arduino) FATAL_ERROR(ArduinoManager)
    connect(m_arduino, SIGNAL(arduinoPortName(QString)), ui->labelMCStatusRecord, SLOT(setText(QString)));
    if(!m_arduino->initialize())
        QMessageBox::warning(0, "Arduino Error", "Arduino not found ! Please check serial port connection.");

    /* Created in order to update microphone status */
    m_audioRecorder = new(std::nothrow) QAudioRecorder;
    if (!m_audioRecorder) FATAL_ERROR(AudioRecorder)

    /* Displays Percussion System */
    m_percussion = new(std::nothrow) PercussionSystem(ui->frameInfo->size());
    if (!m_percussion) FATAL_ERROR(PercussionSystem)
    else {
        m_percussion->QWidget::close();
        m_percussion->setParent(ui->frameInfo);
        m_percussion->show();
        m_percussion->update();
    }

    /* Displays volume */
    m_volumeGraph = new(std::nothrow) RenderArea;
    if (!m_volumeGraph) FATAL_ERROR(RenderArea)

    /* Displays frequency spectrum */
    m_freqSpectrum = new(std::nothrow) FrequencySpectrum;
    if (!m_freqSpectrum) FATAL_ERROR(FrequencySpectrum)
    else m_freqSpectrum->setGraphParameters(GraphParameters(0.0f, minFrequencyShownOnSpectrum,
                                                     (d_real)minFrequencyShownOnSpectrum / frequencyPrecision, GraphScale::Logarithmic));

    /* Starts a timer which will emit a signal once it times out, in order to update any kind of information we want (via connect function).*/
    m_timer = new(std::nothrow) QTimer;
    if (!m_timer) FATAL_ERROR(Timer1)
    m_timer->start(1000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateStatus()));

    /* Created in order to update progress bar status */
    m_chronoProgBarUpload = new(std::nothrow) QTimer;
    if (!m_chronoProgBarUpload) FATAL_ERROR(Timer2)


    /* Subwindows */
    m_recordAndCorrect = new(std::nothrow) RecordAndCorrect(nullptr, m_audioInfo, &m_format, m_freqSpectrum, m_volumeGraph, m_arduino);
    if (!m_recordAndCorrect) FATAL_ERROR(RecordAndCorrect)

    m_settingsWindow = new(std::nothrow) Settings(&m_format, this);
    if (!m_settingsWindow) FATAL_ERROR(SettingWindow)



    /* Connection ! */
    // update status label
    connect(m_recordAndCorrect, SIGNAL(closed(QString)), ui->labelStatus_AutoCorrect, SLOT(setText(QString)));
    connect(m_recordAndCorrect, SIGNAL(spectrumFreqChanged(int)), m_engine, SLOT(changePrecision(int)));
    // pass audio buffer from engine to audioInfo
    connect(m_engine, SIGNAL(audioBufferFilled(QByteArray const&)), m_audioInfo, SLOT(processAudioData(QByteArray const&)));
    // Settings
    connect(m_settingsWindow, SIGNAL(audioModeChanged(int)), m_engine, SLOT(changeAudioMode(int)));
    connect(m_settingsWindow, SIGNAL(generatedFrequencyChanged(int)), m_engine, SLOT(changeTone(int)));
    connect(m_settingsWindow, SIGNAL(audioCodecChanged()), this, SLOT(updateFormat()));
    connect(m_settingsWindow, SIGNAL(sampleRateChanged()), this, SLOT(updateFormat()));
    connect(m_settingsWindow, SIGNAL(sampleSizeChanged()), this, SLOT(updateFormat()));
    connect(m_settingsWindow, SIGNAL(sampleTypeChanged()), this, SLOT(updateFormat()));
    connect(m_settingsWindow, SIGNAL(windowFunctionChanged(int)), m_audioInfo, SLOT(setWindowFunction(int)));
}

ProjectDjembe::~ProjectDjembe()
{
    delete m_chronoProgBarUpload;
    qWarning() << "1";
    delete m_timer;
    qWarning() << "2";
    delete m_arduino;
    qWarning() << "3";
    m_audioRecorder->stop();
    delete m_audioRecorder;
    qWarning() << "4";
    delete m_volumeGraph;
    delete m_freqSpectrum;
    qWarning() << "5";
    delete m_audioInfo;
    qWarning() << "6";
    delete m_recordAndCorrect;
    qWarning() << "7";
    delete m_settingsWindow;
    qWarning() << "8";
    delete m_engine;
    qWarning() << "9";
    delete ui;
    qWarning() << "10";
}


/*************************************************************************************************************/


void ProjectDjembe::on_buttonQuit_clicked() {
    m_recordAndCorrect->close();
    m_settingsWindow->close();
    this->close();
}

void ProjectDjembe::on_buttonSettings_clicked() {
    m_settingsWindow->show();
}

void ProjectDjembe::on_buttonStart_AutoCorrect_clicked() {
    m_recordAndCorrect->show();
    ui->labelStatus_AutoCorrect->setText("<font color='green'>Status : Started (Hit and Record)</font>");
}


/*************************************************************************************************************/

void ProjectDjembe::updateFormat() {
    // Format has changed, reintilaizing everything.
    m_audioInfo->initialize();
    m_engine->initializeAllAudio();
}


void ProjectDjembe::testSlot(d_real v) {
    qWarning() << v;
}

void ProjectDjembe::updateStatus() {
    m_percussion->update();
    // If there is a default mic connected, that is if the function doesn't send back an empty string.
    if (m_audioRecorder->defaultAudioInput() != "") {
        ui->labelMic->setText("<font color='green'>Using default recording device : " + m_audioRecorder->defaultAudioInput() + "</font>");
    }
    else {
        ui->labelMic->setText("<font color='red'>No microphone connected or recognized !</font>");
    }
    if (!m_arduino->isAvailable()) {
        m_arduino->initialize();
    }
}
