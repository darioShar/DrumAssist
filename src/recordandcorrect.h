#ifndef RECORDANDCORRECT_H
#define RECORDANDCORRECT_H

#include "arduinomanager.h"
#include "graphics.h"
#include "audioinfo.h"
#include "datatypes.h"
#include "hra.h"
#include <QAudioInput>
#include <QByteArray>
#include <QtWidgets>
#include <QObject>
#include <QTimer>
#include <fstream>
#include <cmath>
#include <QDebug>
#include <QAudioDeviceInfo>
#include <qendian.h>
#include <algorithm>

extern d_real minRMSHitValue;
extern d_real maxRMSHitValue;
extern d_real minVolumeHitValue;


namespace Ui {
class RecordAndCorrect;
}

class RecordAndCorrect : public QDialog
{
    Q_OBJECT

public :
    explicit RecordAndCorrect(QWidget *parent = 0,
                              const AudioInfo* const = nullptr, const QAudioFormat* const = nullptr,
                              FrequencySpectrum* const = nullptr, RenderArea* const = nullptr,
                              ArduinoManager* const = nullptr);
    ~RecordAndCorrect();

public slots:
    void updateAnything();
    void displayAudioInfo(AudioProperty const&);
    void analyseAudio(AudioProperty const&, std::vector<d_real> const&);

    void on_buttonQuit_clicked();
    void on_buttonReloadProtocol_clicked();
    void on_sliderFreq_valueChanged(int);

signals :
    void closed(QString);
    void spectrumFreqChanged(int);

private :
    Ui::RecordAndCorrect *ui;

    /** Objects to manage audio and microcontroler */
    const QAudioFormat* const       m_format;
    const AudioInfo* const          m_audioInfo;
    ArduinoManager* const     m_microControler;

    /** Objects for graphic display */
    RenderArea* const               m_canvas;
    FrequencySpectrum* const        m_frequencySpectrum;

    /** Status update */
    QTimer                          m_timer;

    /** HRA */
    HRA                             m_hra;
};

#endif RECORDANDCORRECT_H
