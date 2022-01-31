#include <libbutterworth/filter_design.h>

#include <gtest/gtest.h>

TEST(filter_design_test, analog_lowpass)
{
    const double EPSILON = 1.0e-8;

    // reference values created with signal.buttap(4)
    {
        int filter_order = 4;
        filter_design::zpk zpk{filter_design::analog_lowpass(filter_order)};
        EXPECT_TRUE(zpk.zeros.empty());
        EXPECT_NEAR(zpk.poles.at(0).real(), -0.38268343, EPSILON);
        EXPECT_NEAR(zpk.poles.at(0).imag(), +0.92387953, EPSILON);
        EXPECT_NEAR(zpk.poles.at(1).real(), -0.92387953, EPSILON);
        EXPECT_NEAR(zpk.poles.at(1).imag(), +0.38268343, EPSILON);
        EXPECT_NEAR(zpk.poles.at(2).real(), -0.92387953, EPSILON);
        EXPECT_NEAR(zpk.poles.at(2).imag(), -0.38268343, EPSILON);
        EXPECT_NEAR(zpk.poles.at(3).real(), -0.38268343, EPSILON);
        EXPECT_NEAR(zpk.poles.at(3).imag(), -0.92387953, EPSILON);
        EXPECT_NEAR(zpk.gain, 1.0, EPSILON);
    }

    // reference values created with signal.buttap(7)
    {
        int filter_order = 7;
        filter_design::zpk zpk{filter_design::analog_lowpass(filter_order)};
        EXPECT_TRUE(zpk.zeros.empty());
        EXPECT_NEAR(zpk.poles.at(0).real(), -0.22252093, EPSILON);
        EXPECT_NEAR(zpk.poles.at(0).imag(), +0.97492791, EPSILON);
        EXPECT_NEAR(zpk.poles.at(1).real(), -0.62348980, EPSILON);
        EXPECT_NEAR(zpk.poles.at(1).imag(), +0.78183148, EPSILON);
        EXPECT_NEAR(zpk.poles.at(2).real(), -0.90096887, EPSILON);
        EXPECT_NEAR(zpk.poles.at(2).imag(), +0.43388374, EPSILON);
        EXPECT_NEAR(zpk.poles.at(3).real(), -1.00000000, EPSILON);
        EXPECT_NEAR(zpk.poles.at(3).imag(), -0.00000000, EPSILON);
        EXPECT_NEAR(zpk.poles.at(4).real(), -0.90096887, EPSILON);
        EXPECT_NEAR(zpk.poles.at(4).imag(), -0.43388374, EPSILON);
        EXPECT_NEAR(zpk.poles.at(5).real(), -0.62348980, EPSILON);
        EXPECT_NEAR(zpk.poles.at(5).imag(), -0.78183148, EPSILON);
        EXPECT_NEAR(zpk.poles.at(6).real(), -0.22252093, EPSILON);
        EXPECT_NEAR(zpk.poles.at(6).imag(), -0.97492791, EPSILON);
        EXPECT_NEAR(zpk.gain, 1.0, EPSILON);
    }
}

TEST(filter_design_test, lp2lp)
{
    // Reference values gernerated with
    // signal.lp2lp_zpk(np.array([.1+1j,.2,.3]), np.array([.3+0j,.2,.1]), 1, 2)

    filter_design::zpk zpk;

    zpk.zeros = std::vector<std::complex<double>>{std::complex<double>(0.1, 1.0),
                                                  std::complex<double>(0.2, 0.0),
                                                  std::complex<double>(0.3, 0.0)};
    zpk.poles = std::vector<std::complex<double>>{std::complex<double>(0.3, 0.0),
                                                  std::complex<double>(0.2, 0.0),
                                                  std::complex<double>(0.1, 0.0),
                                                  std::complex<double>(0.0, -0.9)};
    double cutoff_frequency = 2;

    filter_design::zpk zpk_lp{filter_design::lp2lp(zpk, cutoff_frequency)};
    const double EPSILON = 1.0e-8;
    EXPECT_EQ(zpk_lp.zeros.size(), 3);
    EXPECT_NEAR(zpk_lp.zeros.at(0).real(), +0.20000000, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(0).imag(), +2.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(1).real(), +0.40000000, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(1).imag(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(2).real(), +0.60000000, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(2).imag(), +0.00000000, EPSILON);

    EXPECT_EQ(zpk_lp.poles.size(), 4);
    EXPECT_NEAR(zpk_lp.poles.at(0).real(), +0.60000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(0).imag(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(1).real(), +0.40000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(1).imag(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(2).real(), +0.20000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(2).imag(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(3).real(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(3).imag(), -1.80000000, EPSILON);

    EXPECT_NEAR(zpk_lp.gain, 2.0, EPSILON);
}

TEST(filter_design_test, lp2hp)
{
    // Reference values gernerated with
    // signal.lp2hp_zpk(np.array([.1+1j,.2,.3]), np.array([.3+0j,.2,.1,0.0-0.9j]), 1, 2)

    filter_design::zpk zpk;

    zpk.zeros = std::vector<std::complex<double>>{std::complex<double>(0.1, 1.0),
                                                  std::complex<double>(0.2, 0.0),
                                                  std::complex<double>(0.3, 0.0)};
    zpk.poles = std::vector<std::complex<double>>{std::complex<double>(0.3, 0.0),
                                                  std::complex<double>(0.2, 0.0),
                                                  std::complex<double>(0.1, 0.0),
                                                  std::complex<double>(0.0, -0.9)};
    double cutoff_frequency = 2;

    filter_design::zpk zpk_lp{filter_design::lp2hp(zpk, cutoff_frequency)};
    const double EPSILON = 1.0e-8;
    EXPECT_EQ(zpk_lp.zeros.size(), 4);
    EXPECT_NEAR(zpk_lp.zeros.at(0).real(), +0.1980198, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(0).imag(), -1.98019802, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(1).real(), +10.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(1).imag(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(2).real(), +6.66666667, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(2).imag(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(3).real(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(3).imag(), +0.00000000, EPSILON);

    EXPECT_EQ(zpk_lp.poles.size(), 4);
    EXPECT_NEAR(zpk_lp.poles.at(0).real(), +6.66666667, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(0).imag(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(1).real(), +10.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(1).imag(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(2).real(), +20.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(2).imag(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(3).real(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(3).imag(), +2.22222222, EPSILON);

    EXPECT_NEAR(zpk_lp.gain, 11.1111111111111, EPSILON);
}

TEST(filter_design_test, lp2bp)
{
    // Reference values gernerated with
    // signal.lp2bp_zpk(np.array([.1+1j,.2,]), np.array([.3+0j,.1,0.0-0.9j]), 1, 2, 1)

    filter_design::zpk zpk;

    zpk.zeros = std::vector<std::complex<double>>{std::complex<double>(0.1, 1.0),
                                                  std::complex<double>(0.2, 0.0)};
    zpk.poles = std::vector<std::complex<double>>{std::complex<double>(0.3, 0.0),
                                                  std::complex<double>(0.1, 0.0),
                                                  std::complex<double>(0.0, -0.9)};
    double bandpass_center = 2;
    double bandpass_width = 1;

    filter_design::zpk zpk_lp{filter_design::lp2bp(zpk, bandpass_center, bandpass_width)};
    const double EPSILON = 1.0e-8;
    EXPECT_EQ(zpk_lp.zeros.size(), 5);
    EXPECT_NEAR(zpk_lp.zeros.at(0).real(), +0.06213014, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(0).imag(), +2.56098208, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(1).real(), +0.10000000, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(1).imag(), +1.99749844, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(2).real(), +0.03786986, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(2).imag(), -1.56098208, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(3).real(), +0.10000000, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(3).imag(), -1.99749844, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(4).real(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(4).imag(), +0.00000000, EPSILON);

    EXPECT_EQ(zpk_lp.poles.size(), 6);
    EXPECT_NEAR(zpk_lp.poles.at(0).real(), +0.15000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(0).imag(), +1.99436707, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(1).real(), +0.05000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(1).imag(), +1.99937490, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(2).real(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(2).imag(), -2.50000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(3).real(), +0.15000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(3).imag(), -1.99436707, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(4).real(), +0.05000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(4).imag(), -1.99937490, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(5).real(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(5).imag(), +1.60000000, EPSILON);

    EXPECT_NEAR(zpk_lp.gain, 1.0, EPSILON);
}

TEST(filter_design_test, lp2bs)
{
    // Reference values gernerated with
    // signal.lp2bs_zpk(np.array([.1+1j,.2,]), np.array([.3+0j,.1,0.0-0.9j]), 1, 2, 1)

    filter_design::zpk zpk;

    zpk.zeros = std::vector<std::complex<double>>{std::complex<double>(0.1, 1.0),
                                                  std::complex<double>(0.2, 0.0)};
    zpk.poles = std::vector<std::complex<double>>{std::complex<double>(0.3, 0.0),
                                                  std::complex<double>(0.1, 0.0),
                                                  std::complex<double>(0.0, -0.9)};
    double stopband_center = 2;
    double stopband_width = 1;

    filter_design::zpk zpk_lp{filter_design::lp2bs(zpk, stopband_center, stopband_width)};
    const double EPSILON = 1.0e-8;
    EXPECT_EQ(zpk_lp.zeros.size(), 6);
    EXPECT_NEAR(zpk_lp.zeros.at(0).real(), +0.06140292, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(0).imag(), -2.55484678, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(1).real(), +4.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(1).imag(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(2).real(), +0.03760698, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(2).imag(), +1.56474777, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(3).real(), +1.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(3).imag(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(4).real(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(4).imag(), +2.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(5).real(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(5).imag(), -2.00000000, EPSILON);

    EXPECT_EQ(zpk_lp.poles.size(), 6);
    EXPECT_NEAR(zpk_lp.poles.at(0).real(), +1.66666667, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(0).imag(), +1.10554160, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(1).real(), +9.58257569, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(1).imag(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(2).real(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(2).imag(), -1.52017130, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(3).real(), +1.66666667, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(3).imag(), -1.10554160, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(4).real(), +0.41742431, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(4).imag(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(5).real(), -0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(5).imag(), +2.63128241, EPSILON);

    EXPECT_NEAR(zpk_lp.gain, 7.407407407407408, EPSILON);
}

TEST(filter_design_test, bilinear_transform)
{
    // Reference values gernerated with
    // signal.bilinear_zpk(np.array([.1+1j,.2,]), np.array([.3+0j,.1,0.0-0.9j]), 1, 2)

    filter_design::zpk zpk;

    zpk.zeros = std::vector<std::complex<double>>{std::complex<double>(0.1, 1.0),
                                                  std::complex<double>(0.2, 0.0)};
    zpk.poles = std::vector<std::complex<double>>{std::complex<double>(0.3, 0.0),
                                                  std::complex<double>(0.1, 0.0),
                                                  std::complex<double>(0.0, -0.9)};
    double sampling_frequency = 2;

    filter_design::zpk zpk_lp{filter_design::bilinear_transform(zpk, sampling_frequency)};
    const double EPSILON = 1.0e-8;
    EXPECT_EQ(zpk_lp.zeros.size(), 3);
    EXPECT_NEAR(zpk_lp.zeros.at(0).real(), +0.92473782, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(0).imag(), +0.49352252, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(1).real(), +1.10526316, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(1).imag(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(2).real(), -1.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.zeros.at(2).imag(), +0.00000000, EPSILON);

    EXPECT_EQ(zpk_lp.poles.size(), 3);
    EXPECT_NEAR(zpk_lp.poles.at(0).real(), +1.16216216, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(0).imag(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(1).real(), +1.05128205, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(1).imag(), +0.00000000, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(2).real(), +0.90362879, EPSILON);
    EXPECT_NEAR(zpk_lp.poles.at(2).imag(), -0.42831648, EPSILON);

    EXPECT_NEAR(zpk_lp.gain, 0.2302856556277139, EPSILON);
}

TEST(filter_design_test, cplxpair)
{
    std::vector<std::complex<double>> z{std::complex<double>(4, 0.0),
                                        std::complex<double>(3, 0.0),
                                        std::complex<double>(1, 0.0),
                                        std::complex<double>(2, -2.0),
                                        std::complex<double>(1, 1.0),
                                        std::complex<double>(2, -1.0),
                                        std::complex<double>(2, 1.0),
                                        std::complex<double>(1, -1.0),
                                        std::complex<double>(2, 2.0)};
    auto result(filter_design::cplxpair(z));

    const double EPSILON = 1.0e-8;

    EXPECT_EQ(result.first.size(), 3);
    EXPECT_NEAR(result.first.at(0).real(), 1, EPSILON);
    EXPECT_NEAR(result.first.at(1).real(), 3, EPSILON);
    EXPECT_NEAR(result.first.at(2).real(), 4, EPSILON);
    EXPECT_NEAR(result.first.at(0).imag(), 0, EPSILON);
    EXPECT_NEAR(result.first.at(1).imag(), 0, EPSILON);
    EXPECT_NEAR(result.first.at(2).imag(), 0, EPSILON);

    EXPECT_EQ(result.second.size(), 3);
    EXPECT_NEAR(result.second.at(0).real(), 1, EPSILON);
    EXPECT_NEAR(result.second.at(0).imag(), 1, EPSILON);
    EXPECT_NEAR(result.second.at(1).real(), 2, EPSILON);
    EXPECT_NEAR(result.second.at(1).imag(), 1, EPSILON);
    EXPECT_NEAR(result.second.at(2).real(), 2, EPSILON);
    EXPECT_NEAR(result.second.at(2).imag(), 2, EPSILON);
}

TEST(filter_design_test, zpk2tf)
{
    // Reference values generated with
    // signal.zpk2tf(z=np.array([0.5-0.6j, 0.5+0.6j]), p=np.array([0.1+0.2j, 0.1-0.2j]), k=1.0)

    std::vector<std::complex<double>> zeros{std::complex<double>(0.5, -0.6),
                                            std::complex<double>(0.5, 0.6)};
    std::vector<std::complex<double>> poles{std::complex<double>(0.1, 0.2),
                                            std::complex<double>(0.1, -0.2)};
    filter_design::zpk zpk{zeros, poles, 1.0};
    biquad result(filter_design::zpk2tf(zpk));

    const double EPSILON = 1.0e-8;

    std::vector<double> coefficients(result.get_coefficients());

    EXPECT_NEAR(coefficients.at(0), +1.00000000, EPSILON);
    EXPECT_NEAR(coefficients.at(1), -1.00000000, EPSILON);
    EXPECT_NEAR(coefficients.at(2), +0.61000000, EPSILON);
    EXPECT_NEAR(coefficients.at(3), -0.20000000, EPSILON);
    EXPECT_NEAR(coefficients.at(4), +0.05000000, EPSILON);
}

TEST(filter_design_test, zpk2sos)
{
    // Reference values generated with
    // signal.zpk2sos(z=np.array([-1, -0.5-0.5j, -0.5+0.5j]),
    //                p=np.array([0.75, 0.8+0.1j, 0.8-0.1j]),
    //                k=1.0)

    std::vector<std::complex<double>> zeros{std::complex<double>(-1.0, 0.0),
                                            std::complex<double>(-0.5, -0.5),
                                            std::complex<double>(-0.5, 0.5)};
    std::vector<std::complex<double>> poles{std::complex<double>(0.75, 0.0),
                                            std::complex<double>(0.8, 0.1),
                                            std::complex<double>(0.8, -0.1)};
    filter_design::zpk zpk{zeros, poles, 1.0};
    std::vector<biquad> result(filter_design::zpk2sos(zpk));

    const double EPSILON = 1.0e-8;

    std::vector<double> coefficients0(result.at(0).get_coefficients());
    EXPECT_NEAR(coefficients0.at(0), +1.00000000, EPSILON);
    EXPECT_NEAR(coefficients0.at(1), +1.00000000, EPSILON);
    EXPECT_NEAR(coefficients0.at(2), +0.50000000, EPSILON);
    EXPECT_NEAR(coefficients0.at(3), -0.75000000, EPSILON);
    EXPECT_NEAR(coefficients0.at(4), +0.00000000, EPSILON);

    std::vector<double> coefficients1(result.at(1).get_coefficients());
    EXPECT_NEAR(coefficients1.at(0), +1.00000000, EPSILON);
    EXPECT_NEAR(coefficients1.at(1), +1.00000000, EPSILON);
    EXPECT_NEAR(coefficients1.at(2), +0.00000000, EPSILON);
    EXPECT_NEAR(coefficients1.at(3), -1.60000000, EPSILON);
    EXPECT_NEAR(coefficients1.at(4), +0.65000000, EPSILON);
}
