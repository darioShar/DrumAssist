#include "engine.h"


/** WARNING : some buffer size make the program bug (it seems that audioInfo analyses data out of m_buffer...).
 *  Understood : buffer size given to QAudioInput isn't necessarily the one that will be used : the latter can be smaller -> Seg Fault.
 *  Why ? because buffer size specified is not necessarily the one finally used in audioInput.
 *  Default for my system : 3840
 *  1920, 2560, 3072, 3840 etc work fine.
 *  Here, BufferSize will just be the size of the audio input buffer, not necessarily the one sent to audioInfo ;
 *  That will be specified by the refresh rate imposed.
 */
const int BufferSize = 3840/**4*4*2*/;


/** Frequency precision in the frequency spectrum
 *  Also the rate at which the audio buffer will
 *  be filled in Hz.
 *  WARNING : it is unnecessary to go under 2*samplerate / BufferSize,
 *  as the input buffer will be entirely full way before update -> loss of data.
 */
d_real frequencyPrecision = 10.0f;



Engine::Engine(const QAudioFormat * const format)
    :   m_format(format)
    ,   m_device(QAudioDeviceInfo::defaultInputDevice())
    ,   m_audioInput(nullptr)
    ,   m_input(nullptr)
    ,   m_mediaPlayer(nullptr)
    ,   m_audioProbe(nullptr)
    ,   m_output(nullptr)
    ,   m_soundCheck(nullptr)
    ,   m_audioInfo(nullptr)
    ,   m_listenAudio(false)
    ,   m_bufferSizeForUpdate(BufferSize)
    ,   m_audioMode(AudioMode::Microphone)
{
    initializeAllAudio();
}


Engine::~Engine() {
    // WARNING : memory leak with m_input and m_output, don't know how to fix it
    // Understood : they were pointers to internal objects of m_audioInput QAudioInput and m_soundCheck QAudioOutput.

    if(m_audioInput) {
        m_audioInput->stop();
        m_audioInput->disconnect(this);
        delete m_audioInput;
        qWarning() << "8.1";
    }
    if (m_mediaPlayer) {
        m_mediaPlayer->stop();
        delete m_mediaPlayer;
        qWarning() << "8.2";
    }
    if (m_audioProbe) {
        delete m_audioProbe;
        qWarning() << "8.3";
    }
    if(m_soundCheck) {
        m_soundCheck->stop();
        delete m_soundCheck;
        qWarning() << "8.4";
    }
    delete m_audioInfo;
    qWarning() << "8.5";
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
 * INITIALIZATION
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
 **/

void Engine::initializeAllAudio() {
    /*Initializing audioInfo*/
    initializeAudioInfo();

    /*Initializing Audio Input*/
    initializeAudioInput();

    /*Initializing Sound Checking*/
    initializeAudioOutput();
}

void Engine::initializeAudioInfo() {
    if (m_audioInfo)
        delete m_audioInfo;
    m_audioInfo  = new AudioInfo(m_format);
    m_bufferSizeForUpdate = m_audioInfo->sizeOfBufferForXHzPrecision(frequencyPrecision);
    qWarning() << "Audio Info initialized.";
}


void Engine::initializeAudioInput() {
    /* Resetting Audio Inputs and some format parameters (because of pre recorded format).*/
    //m_format.setSampleRate(48000); Erase that.

    if (m_audioInput) delete m_audioInput;
    m_audioInput = new QAudioInput(m_device, *m_format, this);
    m_audioInput->setBufferSize(BufferSize);
    m_input = m_audioInput->start();
    m_audioInput->suspend();

    if (m_mediaPlayer == nullptr) {
        m_mediaPlayer = new QMediaPlayer;
        m_mediaPlayer->setMedia(QUrl::fromLocalFile(QFileInfo("Audio\\Caisse Claire Qui Claque.wav").absoluteFilePath()));
    }
    m_mediaPlayer->stop();
    if (m_audioProbe == nullptr) {
        m_audioProbe = new QAudioProbe;
    }

    /* Initializing according to Audio Mode*/
    if (m_audioMode == AudioMode::Microphone) {
        if(audioInputCheckError()) {
            return;
        }
        if (m_audioInput->bufferSize() != BufferSize) {
            QMessageBox::warning(0, "Audio Input Error", "Audio Input is not using specified buffer size (using " \
                                 + QString::number(m_audioInput->bufferSize()) + " Bytes instead).");
        }
        m_audioInput->resume();
        connect(m_input, SIGNAL(readyRead()), this, SLOT(readData()));
    }
    else if (m_audioMode == AudioMode::PreRecorded){
        //m_format.setSampleRate(1058000); // Ouai c'est vraiment beaucoup ouai.
        connect(m_audioProbe, SIGNAL(audioBufferProbed(QAudioBuffer)), this, SLOT(processPreRecordedAudio(QAudioBuffer)));
        if(!m_audioProbe->setSource(m_mediaPlayer)) {
            QMessageBox::critical(0, "ERROR", "Couldn't load pre-recorded audio file !");
        }
        m_mediaPlayer->play();
        m_mediaPlayer->setVolume(100);
        if (m_mediaPlayer->state() == QMediaPlayer::PlayingState) {
            QMessageBox::information(0, "YES", "CE QMEDIAPLAYER DE MERDE DEVRAIT BIEN MARCHER !");
        }
        else {
            QMessageBox::information(0, "BORDEL", "Ce QMediaPlayer de merde ne marhce putain de pas !");
        }
    }
    else {
        updateArtificialBuffer();
    }
    qWarning() << "Audio input initialized.";
}


void Engine::initializeAudioOutput() {
    if (m_soundCheck == nullptr){
        // WARNING : if playing back audio to speaker next to mic (for instance on earphones), new audio data might get
        // corrupted by previous audio (we can hear same sound over and over again, until it fades out).
        m_soundCheck = new QAudioOutput(*m_format, this);
        // Setting buffer size of the soundCheck a bit more than needed so that we are sure that it'll get filled to BufferSize.
        // WARNING : Ihtink it causes some kind of audio drop out since well, audio buffer doesn't get filled all the way...
        m_soundCheck->setBufferSize(BufferSize);
        m_soundCheck->setVolume(1.0);
        m_output = m_soundCheck->start();
#ifdef ENGINE_DEBUG
        qWarning() << "Starting audio output.";
#endif
        if (m_soundCheck->bufferSize() != BufferSize) {
            QMessageBox::warning(0, "Audio Output Error", "Audio Ouput is not using specified buffer size (using " \
                                 + QString::number(m_soundCheck->bufferSize()) + " Bytes instead).");
        }
        connect(m_soundCheck, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleAudioOutputStateChange(QAudio::State)));
        connect(m_output, SIGNAL(bytesWritten(qint64)), this, SLOT(checkAudioOutputBytesWritten(qint64)));
    }

    m_soundCheck->suspend();
#ifdef ENGINE_DEBUG
    qWarning() << "Suspending audio output.";
#endif

    if(m_listenAudio) {
#ifdef ENGINE_DEBUG
        qWarning() << "Resuming audio output.";
#endif
        m_soundCheck->resume();
    }
    qWarning() << "Audio Output initialized.";
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
 * AUDIO DATA HANDLING
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


void Engine::readData() {
    /* We check if the audioinput buffer is full each time new payload of data has arrived to QIODevice object.
    * If it is, then we will analyse with audioInfo object what has been passed to the QIODevice object thanks to
    * the buffer we've been filling all along. And we can play the audio data to check if it is correct.
    * WARNING : there may be a problem here as the whole buffer may not have been fully transferred
    * to the QIODevice object.
    */


    m_buffer.append(m_input->readAll());
    if (m_buffer.size() < m_bufferSizeForUpdate) return;

    // If we want to listen to data recorded :
    if(m_listenAudio) {
        qWarning() << "NUMBER OF BYTES WRITTEN TO AUDIO OUTPUT :" << m_output->write(m_buffer);
        if(!m_output->waitForBytesWritten(/*1000/frequencyPrecision*/ 1000))
            qWarning() << "Sound buffer was not correctly uploaded to audio output buffer.";
        #ifdef ENGINE_DEBUG
        qWarning() << "Still" << m_soundCheck->bytesFree() << "Bytes free in audio output buffer.";
        #endif
    }

    audioBufferFilled(m_buffer);
    m_buffer.clear();
    m_buffer.reserve(BufferSize);
}


void Engine::processPreRecordedAudio(QAudioBuffer audioBuffer) {
    qWarning() << "YES PUTAIN";
    m_audioInfo->setAudioData(QByteArray((char*)audioBuffer.constData(), audioBuffer.byteCount()));
    m_audioInfo->performAllCalculations();
}


void Engine::updateArtificialBuffer() {
    m_artificialBuffer.resize(m_bufferSizeForUpdate);

    // filling artificial buffer with desired paramaters.
    toneGen(reinterpret_cast<audio_data_type*>(m_artificialBuffer.data()),
            m_bufferSizeForUpdate, m_format->sampleRate(), m_tone.initialFrequency, m_tone.finalFrequency, m_tone.amplitude);

    audioBufferFilled(m_artificialBuffer);
    // If we want to listen to data recorded :
    if(m_listenAudio) {
        toneGen(reinterpret_cast<audio_data_type*>(m_artificialBuffer.data()),
                BufferSize, m_format->sampleRate(), m_tone.initialFrequency, m_tone.finalFrequency, m_tone.amplitude);
        m_output->write(m_artificialBuffer.constData(), m_artificialBuffer.size());
        qWarning() << "Written";
        if(m_output->waitForBytesWritten(1000/frequencyPrecision))
            qWarning() << "Artificial buffer was uploaded to audio output buffer.";
        qWarning() << "Still" << m_soundCheck->bytesFree() << "Bytes free in audio output buffer.";
    }
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
 * ERROR HANDLING
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

bool Engine::audioInputCheckError() {
    if (m_audioInput) {
        if (m_audioInput->error()) {
            // Error has been detected
            QMessageBox::critical(0, "Audio Input Error", "Critical error : audio data cannot be retrieved !"  \
                                      " Error id : " + QString::number(m_audioInput->error()) + ".\nSwitching to Generated Tone Mode.");
            qWarning() << "Audio Input error is :" << m_audioInput->error();

            // Deleting audioInput
            m_audioInput->stop();
            m_audioInput->disconnect(this);
            delete m_audioInput;
            m_audioInput = nullptr;

            // Changing mode to generated tone
            changeAudioMode(AudioMode::Generated);
            return true;
        }
    }
    return false;
}

void Engine::handleAudioOutputStateChange(QAudio::State state) {
    switch(state) {
        case QAudio::IdleState :
            if (m_audioMode == AudioMode::Generated) {
#ifdef ENGINE_DEBUG
               qWarning() << "Updating artificial buffer for audio output.";
#endif
                updateArtificialBuffer();
            }
            break;
#ifdef ENGINE_DEBUG
        case QAudio::StoppedState :
            qWarning() << "Audio Output has been stopped.";
            break;
        case QAudio::ActiveState :
            qWarning() << "Audio Output in active state.";
            break;
        case QAudio::SuspendedState :
            qWarning() << "Audio Output in suspended state.";
            break;
#endif
    }
}

void Engine::checkAudioOutputBytesWritten(qint64 bytesWritten) {
    if (m_audioMode == AudioMode::Generated) {
        if (m_artificialBuffer.size() != bytesWritten) {
            qWarning() << "POSSIBILITY OF AUDIO DROPOUT WITH GENERATED TONE DATA !";
            qWarning() << QString::number(bytesWritten) << "BYTES WRITTEN INSTEAD OF" << QString::number(m_artificialBuffer.size()) << "EXPECTED.";
        }
    }
    else if (m_audioMode == AudioMode::Microphone){
        if (m_buffer.size() != bytesWritten) {
            qWarning() << "POSSIBILITY OF AUDIO DROPOUT WITH MICROPHONE DATA !";
            qWarning() << QString::number(bytesWritten) << "BYTES WRITTEN INSTEAD OF" << QString::number(m_buffer.size()) << "EXPECTED.";
        }
    }
    qWarning() << QString::number(bytesWritten) << "BYTES WRITTEN.";
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
 *
 * ENGINE PARAMETERS
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

void Engine::setAudioListening(bool value) {
    m_listenAudio = value;
    initializeAudioOutput();
}


void Engine::toggleSuspend()
{
    // toggle suspend/resume
    if (m_audioInput && (m_audioMode == AudioMode::Microphone)){
        if(audioInputCheckError()) {
            return;
        }
        if (m_audioInput->state() == QAudio::SuspendedState) {
            m_audioInput->resume();
        } else if (m_audioInput->state() == QAudio::ActiveState) {
            m_audioInput->suspend();
        } else if (m_audioInput->state() == QAudio::StoppedState) {
            m_audioInput->resume();
        } else if (m_audioInput->state() == QAudio::IdleState) {
            // no-op
        }
    }
}

void Engine::changeAudioMode(int audioMode) {
    // 0 is AudioMode::Microphone etc.
    m_audioMode = static_cast<AudioMode>(audioMode);
    initializeAudioInput();
}

void Engine::changeTone(int value) {
    if (m_audioMode == AudioMode::Generated) {
        m_tone.initialFrequency = value;
        m_tone.finalFrequency = value;
        updateArtificialBuffer();
    }
}
/*
void Engine::changeAudioCodec(int ac) {

    initializeAllAudio();
}

void Engine::changeSampleRate(int sr) {

    initializeAllAudio();
}

void Engine::changeSampleSize(int ss) {

    initializeAllAudio();
}

void Engine::changeSampleType(int st) {

    initializeAllAudio();
}
*/

void Engine::changeVolume(int value) {
    d_real volume = d_real(value) / 100;
    if (m_audioInput)
        m_audioInput->setVolume(volume);
    if (m_soundCheck)
        m_soundCheck->setVolume(volume);
    m_tone.amplitude = volume;
}

void Engine::changePrecision(int value) {
    if (m_audioMode == AudioMode::Microphone)
        audioInputCheckError();
    // Value on slider range from 2 to 120 : we map it from 1 to 60 by dividing by two.
    frequencyPrecision = (d_real)value / 2.0f;

    m_bufferSizeForUpdate = m_audioInfo->sizeOfBufferForXHzPrecision(frequencyPrecision);


    // Since the generated tone doesn't update itself :
    if(m_audioMode == AudioMode::Generated)
        updateArtificialBuffer();
}


void Engine::updateAudioDevice(int index) {
    if (m_audioInput) {
        m_audioInput->stop();
        m_audioInput->disconnect(this);
        delete m_audioInput;
        m_audioInput = nullptr;
    }

    //m_device = m_deviceBox->itemData(index).value<QAudioDeviceInfo>();
    initializeAudioInput();
}
