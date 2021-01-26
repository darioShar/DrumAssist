#ifndef HRA_H
#define HRA_H

#include <QtGlobal>
#include <utility>
#include <vector>
#include <sstream>
#include <iterator>
#include <QFile>
#include <QDataStream>
#include "datatypes.h"
#include "utils.h"
#include "audioinfo.h"


/* This class will read important frequency values from a file corresponding to different hits.
 *  Will determine hit type accordingly once a fs will be passed to be analysed.
 *  File layout must look like this (all floating points) :
 *  (Harmonic hit) "1st important frequency" "its relative amplitude" "2nd important frequency" "its realtive amplitude" etc.
 *  (Centred Hit) idem
 *  (Excentred Hit) idem
*/
// No more !

/** This class will load raw audio data corrsponding to different hits from a database.
 *  Once a fft will be passed, it will perform fft on the whole database which was loaded,
 *  and then perform cross correlation on each fft sample for comparison. Have to define a minimum
 *  value which will correspond to a hit detected.
 *  DATABASE MUST HAVE BEEN NORMALIZED ( 0 dB !)
 * */

//constexpr d_real minFrequencyUncertainty = 8.0f;

// Max important frequency
#define MAX_FREQUENCY_FOR_ANALYSIS 4000

// Default format used in database
#define DEFAULT_FORMAT_BYTE_ORDER QAudioFormat::LittleEndian
#define DEFAULT_FORMAT_CHANNEL_COUNT 1
#define DEFAULT_FORMAT_CODEC "audio/pcm"
#define DEFAULT_FORMAT_SAMPLE_RATE 44100
#define DEFAULT_FORMAT_SAMPLE_SIZE 8*sizeof(audio_data_type)
#define DEFAULT_FORMAT_SAMPLE_TYPE QAudioFormat::SignedInt


static std::vector<QString> DatabaseRawAudioPaths = {
    QString("Database\\hit_0.raw"),
    QString("Database\\hit_1.raw")
};


class HRA {
    // 0 is harmonic, 1 is Centred
    //std::vector<std::pair<d_real, d_real>> m_hitType[2];

    std::vector<QByteArray> m_databaseRawAudio;
    std::vector<std::vector<d_real>> m_databaseFFTs;

    // To see if we have to re-calculate fft on the whole database
    int m_previousTotalBytesNeeded;

public :
    HRA();
    ~HRA();

    void loadDatabase();
    std::vector<d_real> analyseHit(std::vector<d_real> const& fs, QAudioFormat const &fsFormat, d_real rms = 1.0);
    //d_real analyseHit(std::vector<d_real> const& fs, d_real binWidthInHertz);

};

#endif // HRA_H
