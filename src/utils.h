#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <cmath>
#include <limits>
#include <QByteArray>
#include "datatypes.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


/** Function to give the value of the sin wave of frequency f for the point t (time in s)
 *  t can be calculated by the ratio sample/sampleRate.
 * */
inline d_real sinFrequency(d_real frequency, d_real t) {
    return sin(2 * M_PI*frequency*t);
}


/** NOTE : works on little endian system only.  */
template<typename T>
bool toneGen(T* data, unsigned int bytesToWrite, unsigned int sampleRate,       // Where and How to write generated tone
             d_real initialFrequency, d_real finalFrequency, d_real amplitude)    // What to write as a generated tone
{
    //Some checking before we continue
    if (bytesToWrite % sizeof(T) != 0) {
        return false;
    }

    // Number of samples to write
    int samples = bytesToWrite / sizeof(T);

    d_real changingFrequency = initialFrequency;
    d_real shift = (finalFrequency - initialFrequency) / samples;
    for (int i = 0; i < samples; i++) {
        //d_real changingFrequency = ((i)* (finalFrequency - initialFrequency)/samples) + initialFrequency;
        changingFrequency += shift;
        data[i] = static_cast<T>(sinFrequency(changingFrequency, static_cast<d_real>(i) / static_cast<d_real>(sampleRate)) * amplitude * (std::numeric_limits<T>::max)());
    }
    return true;
}


template<typename T>
unsigned long long next_power_of_2(T value, typename std::enable_if<std::is_unsigned<T>::value && std::is_integral<T>::value, bool>::type* = 0) {
    for (int i = sizeof(T)*8 - 1; i >= 0; i--) {
        if (value & (T)1<<i) {
            if (value > (T)1<<i) {
                return 1ull << (i+1ull);
            }
            else {
                return (1ull << i);
            }
        }
    }
    // Value is 0 : return 2^(-max) =approx 0.
    return 0;
}

template<typename T>
unsigned long long next_power_of_2(T value, typename std::enable_if<std::is_signed<T>::value && std::is_integral<T>::value, bool>::type* = 0) {
    if (value > 0) {
        for (int i = sizeof(T)*8 - 1; i >= 0; i--) {
            if (value & (T)1<<i) {
                if (value > (T)1<<i) {
                    return 1ull << (i+1ull);
                }
                else {
                    return (1ull << i);
                }
            }
        }
    }
    return 0;
}

// double
template<typename T>
T next_power_of_2(T value, typename std::enable_if<std::is_floating_point<T>::value, bool>::type* = 0) {
    if (value > 0) {
        return pow(2.0, ceil(log2(value)));
    }
    return 0;
}



inline unsigned long long closest_power_of_2(double value) {
    unsigned long long up = next_power_of_2(value);
    unsigned long long down = pow(2.0, floor(log2(value)));
    return (up - value) < (value - down) ? up : down;
}


inline int zeroPad(QByteArray& tab, int totalBytes) {
    int bytesToadd = totalBytes - tab.size();
    if (bytesToadd > 0)
        tab.append(bytesToadd, 0);
    return tab.size();
}

#endif // UTILS_H
