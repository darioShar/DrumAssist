#include "audioinfo.h"
#include "utils.h"

/* Returns the size in bytes a buffer (or a chunk of data)
 *  should have so that it will be full after X µs of audio
 *  "listening".                                            */
quint32 AudioInfo::sizeOfBufferForXUs(quint32 durationUs) {
    return m_format->sampleRate() * durationUs / 1000000;
}

/* Returns the size in bytes a buffer (or a chunk of data)
 *  should have so that frequency precision after FFT would
 *  be of X Hz (which is the same thing as how many times
 *  it can be filled within 1 sec).                         */
quint32 AudioInfo::sizeOfBufferForXHzPrecision(d_real freqHz) {
    return (m_format->sampleSize()/8)*m_format->sampleRate()/freqHz;
}



AudioInfo::AudioInfo(const QAudioFormat * const format)
    :   m_format(format)
    ,   m_numSamples(0)
    ,   m_windowFunction(DefaultWindowFunction)
    ,   m_data(nullptr)
{
    initialize();
}



AudioInfo::~AudioInfo()
{
}

void AudioInfo::initialize() {
    switch (m_format->sampleSize()) {
    case 8:
        switch (m_format->sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_audioProperty.maxAmplitude = 255;
            break;
        case QAudioFormat::SignedInt:
            m_audioProperty.maxAmplitude = 127;
            break;
        default:
            break;
        }
        break;
    case 16:
        switch (m_format->sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_audioProperty.maxAmplitude = 65535;
            break;
        case QAudioFormat::SignedInt:
            m_audioProperty.maxAmplitude = 32767;
            break;
        default:
            break;
        }
        break;

    case 32:
        switch (m_format->sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_audioProperty.maxAmplitude = 0xffffffff;
            break;
        case QAudioFormat::SignedInt:
            m_audioProperty.maxAmplitude = 0x7fffffff;
            break;
        case QAudioFormat::Float:
            m_audioProperty.maxAmplitude = 0x7fffffff; // Kind of
        default:
            break;
        }
        break;

    default:
        break;
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
 * SETTING AUDIO DATA
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


qint64 AudioInfo::setAudioData(QByteArray const &newAudio){
    m_audioData.clear();
    m_audioData = newAudio;
    qint64 len = m_audioData.size();
#ifdef FILL_ZERO_PADDING
    quint32 bytesToZeroPad = next_power_of_2(len) - len;
    m_audioData.append(bytesToZeroPad, 0);
    len += bytesToZeroPad;
#endif
    Q_ASSERT(m_format->sampleSize() % 8 == 0);
    const int channelBytes = m_format->sampleSize() / 8;
    const int sampleBytes = m_format->channelCount() * channelBytes;
    Q_ASSERT(len % sampleBytes == 0);
    m_numSamples = len / sampleBytes;

    m_data = m_audioData.constData();
    m_dataBytes = len;

    return len;
}


void AudioInfo::setWindowFunction(int set) {
    m_windowFunction = static_cast<WindowFunction>(set);
    // if audio info contains audio data, recalculate spectrum.
    if (m_audioData.size() != 0) {
        performFrequencySpectrumCalculation();
    }
}


void AudioInfo::calculateWindow()
{
    // m_window is the window function.
    // each element of the array m_window will multiply the corresponding element of the audio data array
    // There are m_numSamples samples in the audio data array.
    m_window.resize(m_numSamples);
    switch (m_windowFunction) {
        case Rectangular:
            for (int i=0; i<m_numSamples; ++i) {
                m_window[i] = 1.0f;
            }
            break;
        case HannWindow:
            for (int i=0; i<m_numSamples; ++i) {
                m_window[i] = static_cast<d_real>(0.5f * (1.0f - cosf(static_cast<d_real>(2.0f * M_PI * i) / static_cast<d_real>(m_numSamples - 1))));
            }
            break;
        case BlackmanHarris :
            for (int i=0; i<m_numSamples; ++i) {
                m_window[i] = 0.35875f
                            - 0.48829f*cosf(static_cast<d_real>(2.0f * M_PI * i) / static_cast<d_real>(m_numSamples - 1))
                            + 0.14128f*cosf(static_cast<d_real>(4.0f * M_PI * i) / static_cast<d_real>(m_numSamples - 1))
                            - 0.01168f*cosf(static_cast<d_real>(6.0f * M_PI * i) / static_cast<d_real>(m_numSamples - 1));
            }
            break;
        default:
            Q_ASSERT(false);
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
 * PROCESSING AUDIO DATA
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

void AudioInfo::processAudioData(QByteArray const& ba) {
    setAudioData(ba);
    performAllCalculations();
}


bool AudioInfo::performAllCalculations(){
    calculateWindow();
    bool result = (performVolumeCalculation() && performFrequencySpectrumCalculation());
    if (result) {
        emit allCalculationDone(m_audioProperty, m_frequencySpectrum);
        return true;
    }
    return false;
}


bool AudioInfo::performVolumeCalculation(){
    // According to data type (given by m_format), we'll navigate through each value
    // to find max value, which is the peak volume + calculate rms.

    m_audioProperty.level = 0.0f;
    m_audioProperty.rms = 0.0f;

    if (m_audioProperty.maxAmplitude) {
        const int channelBytes = m_format->sampleSize() / 8;
        quint32 maxValue = 0;
        quint32 value = 0;
        const unsigned char *ptr = reinterpret_cast<const unsigned char *>(m_data);
        for (int i = 0; i < m_numSamples; ++i) {
            for (int j = 0; j < m_format->channelCount(); ++j) {

                if (m_format->sampleSize() == 8 && m_format->sampleType() == QAudioFormat::UnSignedInt) {
                    value = /**reinterpret_cast<const quint8*>(ptr)*/qFromLittleEndian<quint8>(ptr);
                } else if (m_format->sampleSize() == 8 && m_format->sampleType() == QAudioFormat::SignedInt) {
                    value = /*qAbs(*reinterpret_cast<const qint8*>(ptr))*/qFromLittleEndian<qint8>(ptr);
                } else if (m_format->sampleSize() == 16 && m_format->sampleType() == QAudioFormat::UnSignedInt) {
                    if (m_format->byteOrder() == QAudioFormat::LittleEndian)
                        value = qFromLittleEndian<quint16>(ptr);
                    else
                        value = qFromBigEndian<quint16>(ptr);
                } else if (m_format->sampleSize() == 16 && m_format->sampleType() == QAudioFormat::SignedInt) {
                    if (m_format->byteOrder() == QAudioFormat::LittleEndian)
                        value = qAbs(qFromLittleEndian<qint16>(ptr));
                    else
                        value = qAbs(qFromBigEndian<qint16>(ptr));
                } else if (m_format->sampleSize() == 32 && m_format->sampleType() == QAudioFormat::UnSignedInt) {
                    if (m_format->byteOrder() == QAudioFormat::LittleEndian)
                        value = qFromLittleEndian<quint32>(ptr);
                    else
                        value = qFromBigEndian<quint32>(ptr);
                } else if (m_format->sampleSize() == 32 && m_format->sampleType() == QAudioFormat::SignedInt) {
                    if (m_format->byteOrder() == QAudioFormat::LittleEndian)
                        value = qAbs(qFromLittleEndian<qint32>(ptr));
                    else
                        value = qAbs(qFromBigEndian<qint32>(ptr));
                } else if (m_format->sampleSize() == 32 && m_format->sampleType() == QAudioFormat::Float) {
                    value = qAbs(*reinterpret_cast<const float*>(ptr) * 0x7fffffff); // assumes 0-1.0
                }

                // Maybe that is better (overflow problem...)
                //m_audioProperty.rms = std::sqrtf(m_audioProperty.rms * m_audioProperty.rms + value*value);
                m_audioProperty.rms += ((d_real)value / (d_real)m_audioProperty.maxAmplitude)*((d_real)value / (d_real)m_audioProperty.maxAmplitude);
                maxValue = qMax(value, maxValue);
                ptr += channelBytes;
            }
        }

        // should be useless but well, one never knows.
        //maxValue = qMin(maxValue, m_audioProperty.maxAmplitude);
        m_audioProperty.level = d_real(maxValue) / m_audioProperty.maxAmplitude;
        m_audioProperty.rms /= (d_real)((d_real)m_numSamples * (d_real)m_format->channelCount());
        m_audioProperty.rms = std::sqrt(m_audioProperty.rms);

        /*
        emit volumeLevelCalculationDone(m_audioProperty.level);
        emit volumeRMSCalculationDone(m_audioProperty.rms);
        */

        emit volumeCalculationDone(m_audioProperty.level, m_audioProperty.rms);
        emit calculationDone(m_audioProperty);
        return true;
    }
    else {
        // analysis has failed because object not initialized.
        return false;
    }
}


bool AudioInfo::performFrequencySpectrumCalculation(){
    if(m_dataBytes != 0) {
        if(m_format->byteOrder() == QAudioFormat::LittleEndian/* && m_format->sampleSize() == 16*/ && m_format->sampleType() == QAudioFormat::SignedInt){
    #if defined (AUDIOINFO_DEBUG) && defined (FILL_ZERO_PADDING)
            qWarning() << "Performing FFT with buffer size of" << m_audioData.size() << ".Has been zero-padded";
    #elif defined (AUDIOINFO_DEBUG)
            qWarning() << "Performing FFT with buffer size of" << m_audioData.size();
    #endif

        /* PROBLEM TO SOLVE HERE :
         * understand how magintude scales through FFT espacially after applying window function
         * */
            /*OK*/

        int sampleSize = m_format->sampleSize() / 8;
        int length = (m_dataBytes/sampleSize);

        // Calculating window function
        calculateWindow();

        // in and out are fftw_complex*
        in = (d_real*) fftwf_malloc(sizeof(d_real)*length);
        out = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex)*length);
        const qint8* ptr = reinterpret_cast<const qint8*>(m_data);
        for (int i = 0; i < length; i++){
            // Applying window function
            switch (sampleSize) {
                case sizeof(qint8) :
                    in[i] = *reinterpret_cast<const qint8*>(ptr) * m_window[i];
                    break;
                case sizeof(qint16) :
                    in[i] = *reinterpret_cast<const qint16*>(ptr) * m_window[i];
                    break;
                case sizeof(qint32) :
                    in[i] = *reinterpret_cast<const qint32*>(ptr) * m_window[i];
                    break;
                default :
                    break;
            }
            ptr += sampleSize;
        }

        my_plan = fftwf_plan_dft_r2c_1d(length, in, out, FFTW_ESTIMATE);
        fftwf_execute(my_plan);

        // Scaling results
        d_real reducingFactor = 2.0f/(d_real)length;
        switch (m_windowFunction) {
            case WindowFunction::HannWindow :
                reducingFactor *= (1.0f/0.5f);
                break;
            case WindowFunction::BlackmanHarris :
                reducingFactor *= (1.0f/0.35875f);
                break;
            default :
                break;
        }

        d_real value = 0;
        d_real maxValue = 0;
        int maxValueIndex = 1;

        m_frequencySpectrum.resize(length/2);
        // We fisrt calculate for i = 0 out of the loop because it is the DC and it could be the highest value.
        m_audioProperty.DC = sqrt(out[0][0]*out[0][0] + out[0][1]*out[0][1])*reducingFactor/m_audioProperty.maxAmplitude;
        m_frequencySpectrum[0] = m_audioProperty.DC;
        for (int i = 1; i < length/2; i++){
            value = sqrt(out[i][0]*out[i][0] + out[i][1]*out[i][1])*reducingFactor/m_audioProperty.maxAmplitude;
            if (value > maxValue) {
                maxValue = value;
                maxValueIndex = i;
            }
            m_frequencySpectrum[i] = value;
        }
        // we obtain fundamental with : binNumber * sampleRate/samples.
        m_audioProperty.fundamental = maxValueIndex*(m_format->sampleRate()/(length));
    #ifdef AUDIOINFO_DEBUG
        qWarning() << "FFT Info : Fundamental is" << m_audioProperty.fundamental << "Hz.";
    #endif
        fftwf_free(in);
        fftwf_free(out);
        fftwf_destroy_plan(my_plan);

        emit FFTCalculationDone(m_frequencySpectrum);
        emit calculationDone(m_audioProperty);
        }
    }
    return true;
}
