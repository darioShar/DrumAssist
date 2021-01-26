#ifndef PROJECTDJEMBE_H
#define PROJECTDJEMBE_H

#include <QDialog>
#include <QWidget>
#include <QMessageBox>
#include <QtMultimedia>
#include "datatypes.h"
#include "engine.h"
#include "recordandcorrect.h"
#include "settings.h"


extern d_real minFrequencyShownOnSpectrum;


namespace Ui {
class ProjectDjembe;
}

class ProjectDjembe : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectDjembe(QWidget *parent = 0);
    ~ProjectDjembe();


public slots :
    void on_buttonQuit_clicked();
    void on_buttonSettings_clicked();
    void on_buttonStart_AutoCorrect_clicked();

    void updateFormat();
    void testSlot(d_real);

    // Updates microphone and arduino status (which is shown on a label).
    void updateStatus();


protected:
    Ui::ProjectDjembe*      ui;

    /** To hold windows created */
    RecordAndCorrect*       m_recordAndCorrect;
    Settings*               m_settingsWindow;

    /** Objects to manage audio and microcontroler */
    QAudioFormat            m_format;
    Engine*                 m_engine;
    AudioInfo*              m_audioInfo;
    ArduinoManager*         m_arduino;

    /** Get information about audio devices */
    QAudioRecorder*         m_audioRecorder;

    /** Objects for graphic display */
    PercussionSystem*       m_percussion;
    RenderArea*             m_volumeGraph;
    FrequencySpectrum*      m_freqSpectrum;
    QString                 m_shaders[6];
    int                     nbFile = 0; // to keep track of how many shaders files we have changed

    /** Not important */
    QTimer*                 m_timer;                // Timer object for status update
    QTimer*                 m_chronoProgBarUpload;  // Timer for progressBar update
    int                     m_valueProgBarUpload;   // Variable for progressBar value

};

#endif // PROJECTDJEMBE_H

