#ifndef __BIQUAD__H__
#define __BIQUAD__H__

#include <vector>
#include <complex>

class biquad
{
private:
    double m_b0, m_b1, m_b2, m_a1, m_a2;
    double m_xn1, m_xn2, m_yn1, m_yn2;

public:
    /** Construct second order section (biquad).
     * 
     * biquad filter expression (a0 = 1):
     * y[n] = b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] - a1 * y[n-1] - a2 * y[n-2]
     *
     * @param b0 coefficient for x[n]
     * @param b1 coefficient for x[n-1]
     * @param b2 coefficient for x[n-2]
     * @param a1 coefficient for x[n-1]
     * @param a2 coefficient for x[n-2]
     */
    biquad(double b0, double b1, double b2, double a1, double a2);

    /** Construct second order section (biquad) with all coefficients set to 0.
     *
     * biquad filter expression (a0 = 1):
     * y[n] = b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] - a1 * y[n-1] - a2 * y[n-2]
     *
     */
    biquad();
    ~biquad();

    /** Return filter coefficients of second order section (biquad).
     *
     * biquad filter expression (a0 = 1):
     * y[n] = b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] - a1 * y[n-1] - a2 * y[n-2]
     *
     * @return vector of coefficients (b0, b1, b2, a1, a2)
     */
    std::vector<double> get_coefficients() { return std::vector<double>{m_b0, m_b1, m_b2, m_a1, m_a2}; }

    /** Process a single sample (feed it into the biquad cascade and return output of last biquad).
     *
     * @param sample single signal sample
     * @return processed sample
     */
    double process(double sample);

    /** Process a multiple sample (feed them into the biquad cascade and return output of last biquad).
     *
     * @param samples signal samples
     * @return processed samples
     */
    std::vector<double> process(std::vector<double> samples);
};

#endif //!__BIQUAD__H__