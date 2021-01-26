#include "hra.h"
#include <iostream>


HRA::HRA()
    :   m_previousTotalBytesNeeded(0)
{
    loadDatabase();
}


HRA::~HRA() {

}


void HRA::loadDatabase() {
    m_databaseRawAudio.clear();
    m_databaseFFTs.clear();
    m_previousTotalBytesNeeded = 0;
    for (auto &audioPath : DatabaseRawAudioPaths) {
        QFile infile(audioPath);
        if (!infile.exists()) {
            qWarning(("Failed to open HRA file. Check presence of " + audioPath.toStdString() + ".\n").c_str());
            continue;
        }
        infile.open(QIODevice::ReadOnly);

        qint64 fileSize = infile.size();
        qWarning("%d",fileSize);
        QByteArray rawAudioData = infile.readAll();
        if (rawAudioData.size() != fileSize)
            qWarning(("Failed to entirely load " + audioPath.toStdString() + " file.\n").c_str());

        m_databaseRawAudio.push_back(rawAudioData);
    }
    /*
    std::ifstream infile(audioDataFile);
    if (!infile.is_open()) {qWarning("Failed to open HRA file.\n"); return;}
    std::vector<std::vector<double> > numbers;
    std::string temp;
    while (std::getline(infile, temp)) {
        std::istringstream buffer(temp);
        std::vector<double> line((std::istream_iterator<double>(buffer)),
                                 std::istream_iterator<double>());
        numbers.push_back(line);
    }
    for (auto& x : m_hitType)
        x.clear();
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < numbers[i].size(); j+=2) {
            m_hitType[i].push_back(std::make_pair((d_real)numbers[i][j], (d_real)numbers[i][j+1]));
        }
    }
    */
}

std::vector<d_real> HRA::analyseHit(std::vector<d_real> const& fs, QAudioFormat const& fsFormat, d_real rms) {
    // First we have to check that bin width is the same for this frequency spectrum and the whole database.
    // We assume the fs passed has been zero padded to a power of 2.
    d_real binWidth = (fsFormat.sampleRate()/2.0f) / (d_real)fs.size();
    d_real recordingTime = 1.0f / binWidth;
    d_real approxBytesNeeded = recordingTime * fsFormat.sampleRate() * fsFormat.sampleSize() / 8;
    int totalBytesNeeded = closest_power_of_2(approxBytesNeeded);
    if (qAbs(approxBytesNeeded - totalBytesNeeded) > 2) {
        qWarning("Frequency Spectrum has not been zero padded to a power of 2 !!!");
        return std::vector<d_real>(0,0);
    }

    // if we have to recalculate ffts.
    if (totalBytesNeeded != m_previousTotalBytesNeeded) {
        m_previousTotalBytesNeeded = totalBytesNeeded;
        QAudioFormat tempFormat;
        tempFormat.setByteOrder(DEFAULT_FORMAT_BYTE_ORDER);
        tempFormat.setChannelCount(DEFAULT_FORMAT_CHANNEL_COUNT);
        tempFormat.setCodec(DEFAULT_FORMAT_CODEC);
        tempFormat.setSampleRate(DEFAULT_FORMAT_SAMPLE_RATE);
        tempFormat.setSampleSize(DEFAULT_FORMAT_SAMPLE_SIZE);
        tempFormat.setSampleType(DEFAULT_FORMAT_SAMPLE_TYPE);

        AudioInfo audioAnalyzer(&tempFormat);
        m_databaseFFTs.clear();
        for (auto &rawAudioData : m_databaseRawAudio) {
            if (rawAudioData.size() > totalBytesNeeded)
                audioAnalyzer.setAudioData(QByteArray(rawAudioData.data(), totalBytesNeeded));
            else {
                zeroPad(rawAudioData, totalBytesNeeded);
                audioAnalyzer.setAudioData(rawAudioData);
            }
            audioAnalyzer.performFrequencySpectrumCalculation();
            m_databaseFFTs.push_back(audioAnalyzer.frequencySpectrum());
        }
    }

    // Run cross-correlation
    std::vector<d_real> crossCorrelationResults;
    for (auto &databaseFFT : m_databaseFFTs) {
        int samplesToAnalyze = qMin(MAX_FREQUENCY_FOR_ANALYSIS, DEFAULT_FORMAT_SAMPLE_RATE / 2) / binWidth;
        d_real crossCorrelation = 0.0f;
        d_real normalizationFactor = 0.0f;
        // We only do it for a shift of 0.
        for (int i = 0; i < samplesToAnalyze; i++) {
            crossCorrelation += fs[i] * databaseFFT[i];
            normalizationFactor += databaseFFT[i] * databaseFFT[i];
        }
        crossCorrelationResults.push_back((crossCorrelation / normalizationFactor) / rms);
    }

    return crossCorrelationResults;
}

/*
d_real HRA::analyseHit(std::vector<d_real> const& fs, d_real binWidthInHertz) {
    std::cout << binWidthInHertz << std::endl;;
    // On multiplie les valeurs de la fft par la hauteur relative d'une des fréquences caractéristique sur un intervalle donné (incertitude fréquentielle)
    d_real frequencyUncertainty = qMax(binWidthInHertz, minFrequencyUncertainty);
    d_real hitSimilarity[2] = {0.0, 0.0};
    for (int hitIndex = 0; hitIndex < 2; hitIndex++) {
        for (int i = 0; i < m_hitType[hitIndex].size(); i++) {
            int fsPos = (m_hitType[hitIndex][i].first- frequencyUncertainty) / binWidthInHertz;
            do {
                hitSimilarity[hitIndex] += m_hitType[hitIndex][i].second * fs[fsPos];
                fsPos++;
            } while (qAbs((fsPos * binWidthInHertz) - m_hitType[hitIndex][i].first) < frequencyUncertainty);
        }
    }


    std::cout << "Hit analysis : Harmonic, Centred ; ";
    for (auto& x : hitSimilarity) std::cout << x << ", ";
    std::cout << std::endl;


    return hitSimilarity[0] - hitSimilarity[1];
}
*/
