#ifndef __BUTTERWORTH__H__
#define __BUTTERWORTH__H__

#include <libbutterworth/biquad.h>
#include <libbutterworth/filter_design.h>

#include <vector>
#include <complex>

class butterworth
{

private:
    // TODO: Store these and provide getters
    int m_filter_order;
    std::vector<double> m_freq;
    filter_design::filter_type m_filter_type;
    double m_sampling_frequency;
    std::vector<biquad> m_sections;
    std::vector<biquad> coefficients(int filter_order, std::vector<double> freq, filter_design::filter_type filter_type, double sampling_frequency);

public:
    /** Butterworth digital filter design.
     *
     * @param filter_order The order of the filter.
     * @param freq The critical frequency or frequencies. For lowpass and highpass filters, Wn is a scalar; 
     *             for bandpass and bandstop filters, freq is a length-2 sequence.
     *             For a Butterworth filter, this is the point at which the gain drops to 1/sqrt(2) that 
     *             of the passband (the "-3 dB point").
     *             `freq` are in the same units as `sampling_frequency`.  By default, `sampling_frequency` is 
     *             2 half-cycles/sample, so these are normalized from 0 to 1, where 1 is the Nyquist frequency. 
     *             (`freq` is thus in half-cycles / sample.)
     * @param filter_type The type of filter.
     * @param sampling_frequency The sampling frequency of the digital system.
     */
    butterworth(int filter_order, std::vector<double> freq, filter_design::filter_type filter_type, double sampling_frequency);
    ~butterworth();

    /** Get second order sections of filter
     *
     * @return vector of biquads (second order sections)
     */
    std::vector<biquad> get_sections() { return m_sections; }

    /** Process a single sample (feed it into the biquad cascade and return output of last biquad)
     *
     * @param sample single signal sample
     * @return processed sample
     */
    double process(double sample);

    /** Process a multiple sample (feed them into the biquad cascade and return output of last biquad)
     *
     * @param samples signal samples
     * @return processed samples
     */
    std::vector<double> process(std::vector<double> samples);
};

#endif //!__BUTTERWORTH__H__