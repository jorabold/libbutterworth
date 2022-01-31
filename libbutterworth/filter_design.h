#ifndef __FILTER_DESIGN__H__
#define __FILTER_DESIGN__H__

#include <libbutterworth/biquad.h>

#include <vector>
#include <complex>

namespace filter_design
{
    enum class filter_type
    {
        lowpass,
        highpass,
        bandpass,
        bandstop
    };

    struct zpk
    {
        std::vector<std::complex<double>> zeros{};
        std::vector<std::complex<double>> poles{};
        double gain = 1; // system gain
    };

    /** Return poles for analog prototype of Butterworth lowpass (no zeros, gain is 1).
     *
     * The filter will have an angular (e.g., rad/s) cutoff frequency of 1
     *
     * @param filter_order order of filter
     * @return poles
     */
    filter_design::zpk analog_lowpass(int filter_order);

    /** Transform a lowpass filter prototype to a different frequency.
     *
     * Return an analog low-pass filter with cutoff frequency `wo`
     * from an analog low-pass filter prototype with unity cutoff frequency,
     * using zeros, poles, and gain ('zpk') representation.
     *
     * @param zpk Zeros, poles and system gain of the analog filter transfer function.
     * @param cutoff_frequency Desired cutoff, as angular frequency (e.g., rad/s).
     * @return Zeros, poles and system gain of the transformed low-pass filter transfer function.
     */
    filter_design::zpk lp2lp(filter_design::zpk zpk, double cutoff_frequency);

    /** Transform a lowpass filter prototype to a highpass filter.
     *
     * Return an analog high-pass filter with cutoff frequency `wo`
     * from an analog low-pass filter prototype with unity cutoff frequency,
     * using zeros, poles, and gain ('zpk') representation.
     *
     * @param zpk Zeros, poles and system gain of the analog filter transfer function.
     * @param cutoff_frequency Desired cutoff, as angular frequency (e.g., rad/s).
     * @return Zeros, poles and system gain of the transformed low-pass filter transfer function.
     */
    filter_design::zpk lp2hp(filter_design::zpk zpk, double cutoff_frequency);

    /** Transform a lowpass filter prototype to a bandpass filter.
     *
     * Return an analog band-pass filter with center frequency `wo` and
     * bandwidth `bw` from an analog low-pass filter prototype with unity
     * cutoff frequency, using zeros, poles, and gain ('zpk') representation.
     *
     * @param zpk Zeros, poles and system gain of the analog filter transfer function.
     * @param passband_center Desired passband center, as angular frequency (e.g., rad/s).
     * @param passband_width Desired passband width, as angular frequency (e.g., rad/s).
     * @return Zeros, poles and system gain of the transformed low-pass filter transfer function.
     */
    filter_design::zpk lp2bp(filter_design::zpk zpk, double passband_center, double passband_width);

    /** Transform a lowpass filter prototype to a bandstop filter.
     *
     * Return an analog band-stop filter with center frequency `wo` and
     * stopband width `bw` from an analog low-pass filter prototype with unity
     * cutoff frequency, using zeros, poles, and gain ('zpk') representation.
     *
     * @param zpk Zeros, poles and system gain of the analog filter transfer function.
     * @param stopband_center Desired stopband center, as angular frequency (e.g., rad/s).
     * @param stopband_width Desired stopband width, as angular frequency (e.g., rad/s).
     * @return Zeros, poles and system gain of the transformed low-pass filter transfer function.
     */
    filter_design::zpk lp2bs(filter_design::zpk zpk, double stopband_center, double stopband_width);

    /** Return a digital IIR filter from an analog one using a bilinear transform.
     *
     * Transform a set of poles and zeros from the analog s-plane to the digital
     * z-plane using Tustin's method, which substitutes ``(z-1) / (z+1)`` for
     * ``s``, maintaining the shape of the frequency response.
     *
     * @param zpk Zeros, poles and system gain of the analog filter transfer function.
     * @param sampling_frequency Sample rate, as ordinary frequency (e.g., hertz). No prewarping is
     *      done in this function.
     * @return Zeros, poles and system gain of the transformed digital filter transfer function.
     */
    filter_design::zpk bilinear_transform(filter_design::zpk zpk, double sampling_frequency);

    /** Assert conjugate pairs exist, remove numbers with negative imaginary part and sort.
     *
     * @param z real and complex numbers to sort
     * @return real and complex numbers sorted by real and then imaginary parts.
     */
    std::pair<std::vector<std::complex<double>>, std::vector<std::complex<double>>> cplxpair(std::vector<std::complex<double>> z);

    /** Return polynomial transfer function representation from zeros and poles
     *
     * @param zpk_sos Zeros, poles and system gain of the analog filter transfer function.
     * @return biquad (second order section)
     */
    biquad zpk2tf(filter_design::zpk zpk_sos);

    /** Return second-order sections from zeros, poles, and gain of a system
     *
     * @param zpk Zeros, poles and system gain of the analog filter transfer function.
     * @return Vector of biquads (second order sections)
     *
     * Notes
     * -----
     * The algorithm used to convert ZPK to SOS format is designed to
     * minimize errors due to numerical precision issues. The pairing
     * algorithm attempts to minimize the peak gain of each biquadratic
     * section. This is done by pairing poles with the nearest zeros, starting
     * with the poles closest to the unit circle.
     *
     * Algorithms*
     *
     * The current algorithms are designed specifically for use with digital
     * filters. (The output coefficients are not correct for analog filters.)
     *
     * The steps in the ``pairing='nearest'`` and ``pairing='keep_odd'``
     * algorithms are mostly shared. The ``nearest`` algorithm attempts to
     * minimize the peak gain, while ``'keep_odd'`` minimizes peak gain under
     * the constraint that odd-order systems should retain one section
     * as first order. The algorithm steps and are as follows:
     *
     * As a pre-processing step, add poles or zeros to the origin as
     * necessary to obtain the same number of poles and zeros for pairing.
     * If ``pairing == 'nearest'`` and there are an odd number of poles,
     * add an additional pole and a zero at the origin.
     *
     * The following steps are then iterated over until no more poles or
     * zeros remain:
     *
     * 1. Take the (next remaining) pole (complex or real) closest to the
     *    unit circle to begin a new filter section.
     *
     * 2. If the pole is real and there are no other remaining real poles [#]_,
     *    add the closest real zero to the section and leave it as a first
     *    order section. Note that after this step we are guaranteed to be
     *    left with an even number of real poles, complex poles, real zeros,
     *    and complex zeros for subsequent pairing iterations.
     *
     * 3. Else:
     *
     *    1. If the pole is complex and the zero is the only remaining real
     *       zero*, then pair the pole with the *next* closest zero
     *       (guaranteed to be complex). This is necessary to ensure that
     *       there will be a real zero remaining to eventually create a
     *       first-order section (thus keeping the odd order).
     *
     *    2. Else pair the pole with the closest remaining zero (complex or
     *       real).
     *
     *    3. Proceed to complete the second-order section by adding another
     *       pole and zero to the current pole and zero in the section:
     *
     *       1. If the current pole and zero are both complex, add their
     *          conjugates.
     *
     *       2. Else if the pole is complex and the zero is real, add the
     *          conjugate pole and the next closest real zero.
     *
     *       3. Else if the pole is real and the zero is complex, add the
     *          conjugate zero and the real pole closest to those zeros.
     *
     *       4. Else (we must have a real pole and real zero) add the next
     *          real pole closest to the unit circle, and then add the real
     *          zero closest to that pole.
     *
     * .. [#] This conditional can only be met for specific odd-order inputs
     *      with the ``pairing == 'keep_odd'`` method.
     */
    std::vector<biquad> zpk2sos(filter_design::zpk zpk);
};

#endif //!__FILTER_DESIGN__H__