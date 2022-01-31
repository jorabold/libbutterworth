#include <libbutterworth/biquad.h>

#include <gtest/gtest.h>

TEST(biquad_test, coefficients)
{
    const double EPSILON = 1.0e-4;

    double b0{2};
    double b1{3};
    double b2{4};
    double a1{5};
    double a2{6};
    biquad biquad1(b0, b1, b2, a1, a2);
    std::vector<double> coefficients = biquad1.get_coefficients();
    EXPECT_NEAR(b0, coefficients[0], EPSILON);
    EXPECT_NEAR(b1, coefficients[1], EPSILON);
    EXPECT_NEAR(b2, coefficients[2], EPSILON);
    EXPECT_NEAR(a1, coefficients[3], EPSILON);
    EXPECT_NEAR(a2, coefficients[4], EPSILON);

    biquad biquad2;
    coefficients = biquad2.get_coefficients();
    EXPECT_NEAR(0.0, coefficients[0], EPSILON);
    EXPECT_NEAR(0.0, coefficients[1], EPSILON);
    EXPECT_NEAR(0.0, coefficients[2], EPSILON);
    EXPECT_NEAR(0.0, coefficients[3], EPSILON);
    EXPECT_NEAR(0.0, coefficients[4], EPSILON);
}

TEST(biquad_test, process)
{
    double b0{4.0};
    double b1{3.0};
    double b2{2.0};
    double a1{0.0};
    double a2{-1.0};
    {
        biquad biquad(b0, b1, b2, a1, a2);
        std::vector<double> signal{1, 3, 2, 4, 3};
        std::vector<double> result(biquad.process(signal));

        const double EPSILON = 1.0e-4;
        EXPECT_NEAR(4.0, result[0], EPSILON);
        EXPECT_NEAR(15.0, result[1], EPSILON);
        EXPECT_NEAR(23.0, result[2], EPSILON);
        EXPECT_NEAR(43.0, result[3], EPSILON);
        EXPECT_NEAR(51.0, result[4], EPSILON);
    }

    {
        biquad biquad(b0, b1, b2, a1, a2);
        std::vector<double> signal{1, 3, 2, 4, 3};
        std::vector<double> result;

        for (double sample : signal)
        {
            result.push_back(biquad.process(sample));
        }

        const double EPSILON = 1.0e-4;
        EXPECT_NEAR(4.0, result[0], EPSILON);
        EXPECT_NEAR(15.0, result[1], EPSILON);
        EXPECT_NEAR(23.0, result[2], EPSILON);
        EXPECT_NEAR(43.0, result[3], EPSILON);
        EXPECT_NEAR(51.0, result[4], EPSILON);
    }
}
