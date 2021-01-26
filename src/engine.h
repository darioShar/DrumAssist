#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include "audioinfo.h"
#include "graphics.h"
#include "utils.h"
#include "datatypes.h"
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QByteArray>
#include <QObject>
#include <QWidget>
#include <QTimer>
#include <cmath>
#include <QDebug>
#include <QAudioDeviceInfo>
#include <qendian.h>
#include <QMediaPlayer>
#include <QAudioProbe>
#include <QFileInfo>


#define ENGINE_DEBUG
#undef ENGINE_DEBUG


extern const int BufferSize;
extern d_real frequencyPrecision;


enum AudioMode {
    Microphone,
    PreRecorded,
    Generated
};


/** Audio Engine is a class that manages and syncs audio information and graphics output. */

class Engine : public QObject
{
    Q_OBJECT

public:
    Engine(const QAudioFormat* const format);
    ~Engine();

public slots:
    /** Reinitiliazing */
    void initializeAllAudio();

    /**     AUDIO DATA HANDLING */
    void readData();
    void processPreRecordedAudio(QAudioBuffer);
    void updateArtificialBuffer();

    /**     ERROR HANDLING */
    bool audioInputCheckError();
    void handleAudioOutputStateChange(QAudio::State);
    void checkAudioOutputBytesWritten(qint64);

    /**     ENGINE PARAMETERS */
    void setAudioListening(bool);

    void toggleSuspend();
    void changeAudioMode(int);
    void changeTone(int);
    void changeVolume(int value);
    void changePrecision(int value);
    void updateAudioDevice(int index);


signals :
    void audioBufferFilled(QByteArray const&);

private:
    /** Initializations */
    void initializeAudioInfo();
    void initializeAudioInput();
    void initializeAudioOutput();


    /** Audio input and output */
    const QAudioFormat* const m_format; // Default format for audio input and output

    /* Live Audio Data (Through Microphone)*/
    QAudioDeviceInfo    m_device;
    QAudioInput*        m_audioInput;
    QIODevice*          m_input;
    QByteArray          m_buffer;
    /* Generated Audio Data */
    Tone                m_tone;
    QByteArray          m_artificialBuffer;
    /* Pre-Recorded Audio Data */
    QMediaPlayer*       m_mediaPlayer;
    QAudioProbe*        m_audioProbe;
    QByteArray          m_preRecordedBuffer;
    /* Audio Output to check live audio or generated tone. Not for pre-recorded audio. */
    QIODevice*          m_output;
    QAudioOutput*       m_soundCheck;


    /** Audio Information */
    /* Only to detect a hit. Limits type of calculations
     * Maybe it is better to create another class dedicated to this task... */
    AudioInfo*          m_audioInfo;


    /** Affecting use of the engine during runtime*/
    bool                m_listenAudio;
    int                 m_bufferSizeForUpdate;  // Size a buffer should have to update according to the frequency precision we want.
    AudioMode           m_audioMode;            // To know if we use generated tone, live audio or pre-recorded.
};


#endif // AUDIOENGINE_H
