#include <libbutterworth/utils.h>

#include <gtest/gtest.h>

#include <complex>

TEST(utils_tests, is_real)
{
    std::complex<double> a(1., -1.);

    EXPECT_TRUE(!utils::is_real(a));
    std::complex<float> b(1., -0.5);
    EXPECT_TRUE(!utils::is_real(b));
    std::complex<int> c(0, -1);
    EXPECT_TRUE(!utils::is_real(c));

    std::complex<double> d(0.5, 0.0);
    EXPECT_TRUE(utils::is_real(d));
    std::complex<float> e(-1., 0.0);
    EXPECT_TRUE(utils::is_real(e));
    std::complex<int> f(1, 0);
    EXPECT_TRUE(utils::is_real(f));

    double g = 0.5;
    EXPECT_TRUE(utils::is_real(g));
    float h = 10.1;
    EXPECT_TRUE(utils::is_real(h));
    int i = 0;
    EXPECT_TRUE(utils::is_real(i));
}

TEST(utils_tests, pop_nearest_real_complex)
{
    const double EPSILON = 1.0e-8;
    std::vector<std::complex<double>> a{std::complex<double>(0.1, 1.0),
                                        std::complex<double>(0.2, 0.0),
                                        std::complex<double>(0.3, -1.0),
                                        std::complex<double>(0.5, 0.0)};

    std::complex<double> a1 = utils::pop_nearest_real_complex(a, std::complex<double>(0.4, 0.0), false);
    EXPECT_NEAR(std::real(a1), 0.3, EPSILON);
    EXPECT_NEAR(std::imag(a1), -1.0, EPSILON);

    std::complex<double> a2 = utils::pop_nearest_real_complex(a, std::complex<double>(0.4, 0.0), true);
    EXPECT_NEAR(std::real(a2), 0.5, EPSILON);
    EXPECT_NEAR(std::imag(a2), 0.0, EPSILON);

    std::complex<double> a3 = utils::pop_nearest_real_complex(a, std::complex<double>(-0.1, 0.0), true);
    EXPECT_NEAR(std::real(a3), 0.2, EPSILON);
    EXPECT_NEAR(std::imag(a3), 0.0, EPSILON);
}
