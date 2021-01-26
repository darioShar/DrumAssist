#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QAudioFormat>
#include "datatypes.h"

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QAudioFormat* const format, QWidget *parent = 0);
    ~Settings();

public slots :
    void on_comboBoxAudioMode_currentIndexChanged(int i);
    void on_sliderFrequency_valueChanged(int i);
    void on_comboBoxAudioCodec_currentIndexChanged(int i);
    void on_comboBoxSampleRate_currentIndexChanged(int i);
    void on_comboBoxSampleSize_currentIndexChanged(int i);
    void on_comboBoxSampleType_currentIndexChanged(int i);
    void on_comboBoxWindowFunction_currentIndexChanged(int i);

signals :
    void audioModeChanged(int);
    void generatedFrequencyChanged(int);
    void audioCodecChanged();
    void sampleRateChanged();
    void sampleSizeChanged();
    void sampleTypeChanged();
    void windowFunctionChanged(int);

private :
    Ui::Settings*           ui;
    QAudioFormat* const     m_format;
};

#endif // SETTINGS_H
