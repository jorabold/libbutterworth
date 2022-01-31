#include <libbutterworth/butterworth.h>
#include <libbutterworth/utils.h>

#include <complex>
#include <algorithm>
#include <exception>
#include <numeric>    // std::accumulate
#include <functional> // std::multiplies
#include <assert.h>
#include <limits>

butterworth::butterworth(int filter_order, std::vector<double> freq, filter_design::filter_type filter_type, double sampling_frequency)
    : m_filter_order{filter_order}, m_freq{freq}, m_filter_type(filter_type), m_sampling_frequency(sampling_frequency), m_sections{coefficients(filter_order, freq, filter_type, sampling_frequency)}
{
}

butterworth::~butterworth()
{
}

double butterworth::process(double sample)
{
    double result = sample;

    for (biquad &biquad : m_sections)
    {
        result = biquad.process(result);
    }
    return (result);
}

std::vector<double> butterworth::process(std::vector<double> samples)
{
    std::vector<double> result(samples);

    for (biquad &biquad : m_sections)
    {
        result = biquad.process(result);
    }
    return (result);
}

std::vector<biquad> butterworth::coefficients(int filter_order, std::vector<double> freq, filter_design::filter_type filter_type, double sampling_frequency)
{
    std::vector<double> Wn;

    for (double f : freq)
    {
        Wn.push_back(2 * f / sampling_frequency);
        if (Wn.back() <= 0 || Wn.back() >= 1)
        {
            throw std::invalid_argument("Digital filter critical frequencies in freq must be 0 < f < fs/2");
        }
    }

    // Get analog lowpass prototype
    filter_design::zpk zpk(filter_design::analog_lowpass(filter_order));

    // Pre-warp frequencies for digital filter design
    double fs = 2.0;
    std::vector<double> warped;
    for (double w : Wn)
    {
        warped.push_back(2 * fs * tan(PI * w / fs));
    }
    if (warped.size() != 1 &&
        (filter_type == filter_design::filter_type::lowpass ||
         filter_type == filter_design::filter_type::highpass))
    {
        throw std::invalid_argument("Must specify a single critical frequency for lowpass or highpass filter");
    }
    if (warped.size() != 2 &&
        (filter_type == filter_design::filter_type::bandpass ||
         filter_type == filter_design::filter_type::bandstop))
    {
        throw std::invalid_argument("Must specify two critical frequencies for bandpass or bandstop filter");
    }

    // transform to lowpass, bandpass, highpass, or bandstop
    switch (filter_type)
    {
    case filter_design::filter_type::lowpass:
    {
        zpk = lp2lp(zpk, warped.at(0));
        break;
    }
    case filter_design::filter_type::highpass:
    {
        zpk = lp2hp(zpk, warped.at(0));
        break;
    }
    case filter_design::filter_type::bandpass:
    {
        double passband_center = std::sqrt(warped.at(0) * warped.at(1));
        double passband_width = std::abs(warped.at(1) - warped.at(0));
        zpk = lp2bp(zpk, passband_center, passband_width);
        break;
    }
    case filter_design::filter_type::bandstop:
    {
        double stopband_center = std::sqrt(warped.at(0) * warped.at(1));
        double stopband_width = std::abs(warped.at(1) - warped.at(0));
        zpk = lp2bs(zpk, stopband_center, stopband_width);
        break;
    }
    default:
        throw std::invalid_argument("Filtertype not implemented!");
    }

    zpk = bilinear_transform(zpk, fs);
    return (zpk2sos(zpk));
}
