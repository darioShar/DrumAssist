#ifndef AUDIOINFO_H
#define AUDIOINFO_H

#include <QAudioInput>
#include <QByteArray>
#include <QObject>
#include <QDebug>
#include <qendian.h>
#include <cmath>
#include "FFTW/fftw3.h"
#include "datatypes.h"

#define FILL_ZERO_PADDING

#define AUDIOINFO_DEBUG
#undef AUDIOINFO_DEBUG

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


enum WindowFunction {
    Rectangular,
    HannWindow,
    BlackmanHarris
};

const WindowFunction DefaultWindowFunction = HannWindow;

struct Tone
{
    Tone(d_real initFreq = 440.0, d_real finalFreq = 440.0, d_real amp = 1.0)
    :   initialFrequency(initFreq), finalFrequency(finalFreq), amplitude(amp)
    { Q_ASSERT(finalFreq >= initFreq); }

    // Start and end frequencies for swept tone generation
    d_real   initialFrequency;
    d_real   finalFrequency;

    // Amplitude in range [0.0, 1.0]
    d_real   amplitude;
};

struct AudioProperty {
    AudioProperty(quint32 _maxAmp = 0, d_real _level = 0.0, d_real _rms = 0.0, d_real _fundamental = 0.0f, d_real _dc = 0.0f)
        :   maxAmplitude(_maxAmp)
        ,   level(_level)
        ,   rms(_rms)
        ,   fundamental(_fundamental)
        ,   DC(_dc) {}

    quint32                 maxAmplitude;
    d_real                   level;
    d_real                   rms;
    d_real                   fundamental;
    d_real                   DC;
};

/* This class takes care of audio information : stores it and proceeds to some calculations
* such as Peak Volume, RMS and Fast Fourrier Transform.
* How the class is used : first send it audio data with setAudioData().
* Then perform calculation wanted on this data (could be all of the calculations possible,
* but it could take some unnecessary time).
*/

class AudioInfo : public QObject
{
    Q_OBJECT

public:
    AudioInfo(const QAudioFormat* const format);
    ~AudioInfo();

    qint64 setAudioData(QByteArray const& newAudio);
    quint32 sizeOfBufferForXUs(quint32 durationUs);
    quint32 sizeOfBufferForXHzPrecision(d_real freqHz);

    AudioProperty audioProperty() const             { return m_audioProperty; }
    quint32 maxAmplitude() const                    { return m_audioProperty.maxAmplitude; }
    d_real level() const                             { return m_audioProperty.level; }
    d_real rms() const                               { return m_audioProperty.rms; }
    d_real fundamental() const                       { return m_audioProperty.fundamental; }
    d_real DC() const                                { return m_audioProperty.DC; }
    std::vector<d_real> frequencySpectrum() const    { return m_frequencySpectrum; }

public slots :
    void initialize();
    void processAudioData(QByteArray const&);
    void setWindowFunction(int);
    bool performAllCalculations();
    bool performVolumeCalculation();
    bool performFrequencySpectrumCalculation();

private:
    void calculateWindow();
    bool analyseForPeakVolume(const char *data, qint64 len);

    /** Data           */
    const QAudioFormat* const   m_format;
    qint64                      m_dataBytes;
    const void*                 m_data;
    QByteArray                  m_audioData;
    int                         m_numSamples;

    /** Data analysis  */
    WindowFunction              m_windowFunction;
    QVector<d_real>             m_window;
    d_real*                     in;
    fftwf_complex*              out;
    fftwf_plan                  my_plan;

    /** Audio properties */
    AudioProperty               m_audioProperty;
    std::vector<d_real>          m_frequencySpectrum;


signals:
    void calculationDone(AudioProperty const&);
    void volumeCalculationDone(d_real, d_real);
    void FFTCalculationDone(std::vector<d_real> const&);
    void allCalculationDone(AudioProperty const&, std::vector<d_real> const&);
};


#endif // AUDIOINFO_H
