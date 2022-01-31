#include <libbutterworth/biquad.h>

biquad::biquad(double b0, double b1, double b2, double a1, double a2)
    : m_b0(b0), m_b1(b1), m_b2(b2), m_a1(a1), m_a2(a2), m_xn1(0), m_xn2(0), m_yn1(0), m_yn2(0)
{
}

biquad::biquad()
    : m_b0(0), m_b1(0), m_b2(0), m_a1(0), m_a2(0), m_xn1(0), m_xn2(0), m_yn1(0), m_yn2(0)
{
}

biquad::~biquad()
{
}

double biquad::process(double sample)
{
    // y[n] = b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] - a1 * y[n-1] - a2 * y[n-2]
    double yn = m_b0 * sample + m_b1 * m_xn1 + m_b2 * m_xn2 - m_a1 * m_yn1 - m_a2 * m_yn2;
    m_xn2 = m_xn1;
    m_xn1 = sample;
    m_yn2 = m_yn1;
    m_yn1 = yn;
    return yn;
}

std::vector<double> biquad::process(std::vector<double> samples)
{
    std::vector<double> result;
    for (double sample : samples)
    {
        result.push_back(this->process(sample));
    }
    return result;
}