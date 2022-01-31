#include <vector>

#include <libbutterworth/biquad.h>
#include <libbutterworth/filter_design.h>
#include <libbutterworth/butterworth.h>

#include <gtest/gtest.h>

// Lowpass filter coefficients, Verify coefficients for a lowpass filter
TEST(butterworth_test, lowpass)
{
    //******************************************************************************
    // scipy coefficients generated with the following code:
    //
    // sos = signal.butter(8, 15, 'lp', fs=50, output='sos')
    //
    //******************************************************************************

    int filter_order = 8;
    std::vector<double> freq{15};
    double sampling_frequency = 50;
    butterworth butterworth{filter_order, freq, filter_design::filter_type::lowpass, sampling_frequency};
    std::vector<biquad> sections(butterworth.get_sections());

    const double EPSILON = 1.0e-4;

    //******************************************************************************
    // scipy coefficients: first section
    //******************************************************************************

    double b0 = 0.02926102;
    double b1 = 0.05852203;
    double b2 = 0.02926102;
    // double a0 = 1.0;
    double a1 = 0.3197639;
    double a2 = 0.03477772;

    std::vector<double> coefficients = sections[0].get_coefficients();
    EXPECT_NEAR(b0, coefficients[0], EPSILON);
    EXPECT_NEAR(b1, coefficients[1], EPSILON);
    EXPECT_NEAR(b2, coefficients[2], EPSILON);
    EXPECT_NEAR(a1, coefficients[3], EPSILON);
    EXPECT_NEAR(a2, coefficients[4], EPSILON);

    //******************************************************************************
    //  scipy coefficients: second section
    //******************************************************************************

    b0 = 1.0;
    b1 = 2.0;
    b2 = 1.0;
    // a0 = 1.0;
    a1 = 0.34512104;
    a2 = 0.11683514;

    coefficients = sections[1].get_coefficients();
    EXPECT_NEAR(b0, coefficients[0], EPSILON);
    EXPECT_NEAR(b1, coefficients[1], EPSILON);
    EXPECT_NEAR(b2, coefficients[2], EPSILON);
    EXPECT_NEAR(a1, coefficients[3], EPSILON);
    EXPECT_NEAR(a2, coefficients[4], EPSILON);

    //******************************************************************************
    //  scipy coefficients: third section
    //******************************************************************************

    b0 = 1.0;
    b1 = 2.0;
    b2 = 1.0;
    // a0 = 1.0;
    a1 = 0.40437229;
    a2 = 0.30857621;

    coefficients = sections[2].get_coefficients();
    EXPECT_NEAR(b0, coefficients[0], EPSILON);
    EXPECT_NEAR(b1, coefficients[1], EPSILON);
    EXPECT_NEAR(b2, coefficients[2], EPSILON);
    EXPECT_NEAR(a1, coefficients[3], EPSILON);
    EXPECT_NEAR(a2, coefficients[4], EPSILON);

    //******************************************************************************
    //  scipy coefficients: fourth section
    //******************************************************************************

    b0 = 1.0;
    b1 = 2.0;
    b2 = 1.0;
    // a0 = 1.0;
    a1 = 0.52130927;
    a2 = 0.68699222;
    coefficients = sections[3].get_coefficients();
    EXPECT_NEAR(b0, coefficients[0], EPSILON);
    EXPECT_NEAR(b1, coefficients[1], EPSILON);
    EXPECT_NEAR(b2, coefficients[2], EPSILON);
    EXPECT_NEAR(a1, coefficients[3], EPSILON);
    EXPECT_NEAR(a2, coefficients[4], EPSILON);
}

// Highpass filter coefficients, Verify coefficients for a highpass filter
TEST(butterworth_test, highpass)
{
    //******************************************************************************
    // scipy coefficients generated with the following code:
    //
    // sos = signal.butter(8, 15, 'hp', fs=50, output='sos')
    //
    //******************************************************************************

    int filter_order = 8;
    std::vector<double> freq{15};
    double sampling_frequency = 50;
    butterworth butterworth{filter_order, freq, filter_design::filter_type::highpass, sampling_frequency};
    std::vector<biquad> sections(butterworth.get_sections());

    const double EPSILON = 1.0e-4;

    //******************************************************************************
    // scipy coefficients: first section
    //******************************************************************************

    double b0 = 0.00227184;
    double b1 = -0.00454368;
    double b2 = 0.00227184;
    // double a0 = 1.0;
    double a1 = 0.3197639;
    double a2 = 0.03477772;

    std::vector<double> coefficients = sections[0].get_coefficients();
    EXPECT_NEAR(b0, coefficients[0], EPSILON);
    EXPECT_NEAR(b1, coefficients[1], EPSILON);
    EXPECT_NEAR(b2, coefficients[2], EPSILON);
    EXPECT_NEAR(a1, coefficients[3], EPSILON);
    EXPECT_NEAR(a2, coefficients[4], EPSILON);

    //******************************************************************************
    //  scipy coefficients: second section
    //******************************************************************************

    b0 = 1.0;
    b1 = -2.0;
    b2 = 1.0;
    // a0 = 1.0;
    a1 = 0.34512104;
    a2 = 0.11683514;

    coefficients = sections[1].get_coefficients();
    EXPECT_NEAR(b0, coefficients[0], EPSILON);
    EXPECT_NEAR(b1, coefficients[1], EPSILON);
    EXPECT_NEAR(b2, coefficients[2], EPSILON);
    EXPECT_NEAR(a1, coefficients[3], EPSILON);
    EXPECT_NEAR(a2, coefficients[4], EPSILON);

    //******************************************************************************
    //  scipy coefficients: third section
    //******************************************************************************

    b0 = 1.0;
    b1 = -2.0;
    b2 = 1.0;
    // a0 = 1.0;
    a1 = 0.40437229;
    a2 = 0.30857621;

    coefficients = sections[2].get_coefficients();
    EXPECT_NEAR(b0, coefficients[0], EPSILON);
    EXPECT_NEAR(b1, coefficients[1], EPSILON);
    EXPECT_NEAR(b2, coefficients[2], EPSILON);
    EXPECT_NEAR(a1, coefficients[3], EPSILON);
    EXPECT_NEAR(a2, coefficients[4], EPSILON);

    //******************************************************************************
    //  scipy coefficients: fourth section
    //******************************************************************************

    b0 = 1.0;
    b1 = -2.0;
    b2 = 1.0;
    // a0 = 1.0;
    a1 = 0.52130927;
    a2 = 0.68699222;
    coefficients = sections[3].get_coefficients();
    EXPECT_NEAR(b0, coefficients[0], EPSILON);
    EXPECT_NEAR(b1, coefficients[1], EPSILON);
    EXPECT_NEAR(b2, coefficients[2], EPSILON);
    EXPECT_NEAR(a1, coefficients[3], EPSILON);
    EXPECT_NEAR(a2, coefficients[4], EPSILON);
}

// Bandpass filter coefficients, Verify coefficients for a bandpass filter
TEST(butterworth_test, bandpass)
{
    //******************************************************************************
    // scipy coefficients generated with the following code:
    //
    // sos = signal.butter(4, [15,20], 'bp', fs=50, output='sos')
    //
    //******************************************************************************

    int filter_order = 4;
    std::vector<double> freq{15, 20};
    double sampling_frequency = 50;
    butterworth butterworth{filter_order, freq, filter_design::filter_type::bandpass, sampling_frequency};
    std::vector<biquad> sections(butterworth.get_sections());

    const double EPSILON = 1.0e-4;

    //******************************************************************************
    // scipy coefficients: first section
    //******************************************************************************

    double b0 = 0.00482434;
    double b1 = -0.00964869;
    double b2 = 0.00482434;
    // double a0 = 1.0;
    double a1 = 0.75735748;
    double a2 = 0.50881463;

    std::vector<double> coefficients = sections[0].get_coefficients();
    EXPECT_NEAR(b0, coefficients[0], EPSILON);
    EXPECT_NEAR(b1, coefficients[1], EPSILON);
    EXPECT_NEAR(b2, coefficients[2], EPSILON);
    EXPECT_NEAR(a1, coefficients[3], EPSILON);
    EXPECT_NEAR(a2, coefficients[4], EPSILON);

    //******************************************************************************
    //  scipy coefficients: second section
    //******************************************************************************

    b0 = 1.0;
    b1 = -2.0;
    b2 = 1.0;
    // a0 = 1.0;
    a1 = 1.12678067;
    a2 = 0.58202014;

    coefficients = sections[1].get_coefficients();
    EXPECT_NEAR(b0, coefficients[0], EPSILON);
    EXPECT_NEAR(b1, coefficients[1], EPSILON);
    EXPECT_NEAR(b2, coefficients[2], EPSILON);
    EXPECT_NEAR(a1, coefficients[3], EPSILON);
    EXPECT_NEAR(a2, coefficients[4], EPSILON);

    //******************************************************************************
    //  scipy coefficients: third section
    //******************************************************************************

    b0 = 1.0;
    b1 = 2.0;
    b2 = 1.0;
    // a0 = 1.0;
    a1 = 0.58235702;
    a2 = 0.75562373;

    coefficients = sections[2].get_coefficients();
    EXPECT_NEAR(b0, coefficients[0], EPSILON);
    EXPECT_NEAR(b1, coefficients[1], EPSILON);
    EXPECT_NEAR(b2, coefficients[2], EPSILON);
    EXPECT_NEAR(a1, coefficients[3], EPSILON);
    EXPECT_NEAR(a2, coefficients[4], EPSILON);

    //******************************************************************************
    //  scipy coefficients: fourth section
    //******************************************************************************

    b0 = 1.0;
    b1 = 2.0;
    b2 = 1.0;
    // a0 = 1.0;
    a1 = 1.47008035;
    a2 = 0.83737284;
    coefficients = sections[3].get_coefficients();
    EXPECT_NEAR(b0, coefficients[0], EPSILON);
    EXPECT_NEAR(b1, coefficients[1], EPSILON);
    EXPECT_NEAR(b2, coefficients[2], EPSILON);
    EXPECT_NEAR(a1, coefficients[3], EPSILON);
    EXPECT_NEAR(a2, coefficients[4], EPSILON);
}

// Bandstop filter coefficients, Verify coefficients for a bandstop filter
TEST(butterworth_test, bandstop)
{
    //******************************************************************************
    // scipy coefficients generated with the following code:
    //
    // sos = signal.butter(4, [15,20], 'bs', fs=50, output='sos')
    //
    //******************************************************************************

    int filter_order = 4;
    std::vector<double> freq{15, 20};
    double sampling_frequency = 50;
    butterworth butterworth{filter_order, freq, filter_design::filter_type::bandstop, sampling_frequency};
    std::vector<biquad> sections(butterworth.get_sections());

    const double EPSILON = 1.0e-4;

    //******************************************************************************
    // scipy coefficients: first section
    //******************************************************************************

    double b0 = 0.43284664;
    double b1 = 0.53502788;
    double b2 = 0.43284664;
    // double a0 = 1.0;
    double a1 = 0.75735748;
    double a2 = 0.50881463;

    std::vector<double> coefficients = sections[0].get_coefficients();
    EXPECT_NEAR(b0, coefficients[0], EPSILON);
    EXPECT_NEAR(b1, coefficients[1], EPSILON);
    EXPECT_NEAR(b2, coefficients[2], EPSILON);
    EXPECT_NEAR(a1, coefficients[3], EPSILON);
    EXPECT_NEAR(a2, coefficients[4], EPSILON);

    //******************************************************************************
    //  scipy coefficients: second section
    //******************************************************************************

    b0 = 1.0;
    b1 = 1.23606798;
    b2 = 1.0;
    // a0 = 1.0;
    a1 = 1.12678067;
    a2 = 0.58202014;

    coefficients = sections[1].get_coefficients();
    EXPECT_NEAR(b0, coefficients[0], EPSILON);
    EXPECT_NEAR(b1, coefficients[1], EPSILON);
    EXPECT_NEAR(b2, coefficients[2], EPSILON);
    EXPECT_NEAR(a1, coefficients[3], EPSILON);
    EXPECT_NEAR(a2, coefficients[4], EPSILON);

    //******************************************************************************
    //  scipy coefficients: third section
    //******************************************************************************

    b0 = 1.0;
    b1 = 1.23606798;
    b2 = 1.0;
    // a0 = 1.0;
    a1 = 0.58235702;
    a2 = 0.75562373;

    coefficients = sections[2].get_coefficients();
    EXPECT_NEAR(b0, coefficients[0], EPSILON);
    EXPECT_NEAR(b1, coefficients[1], EPSILON);
    EXPECT_NEAR(b2, coefficients[2], EPSILON);
    EXPECT_NEAR(a1, coefficients[3], EPSILON);
    EXPECT_NEAR(a2, coefficients[4], EPSILON);

    //******************************************************************************
    //  scipy coefficients: fourth section
    //******************************************************************************

    b0 = 1.0;
    b1 = 1.23606798;
    b2 = 1.0;
    // a0 = 1.0;
    a1 = 1.47008035;
    a2 = 0.83737284;
    coefficients = sections[3].get_coefficients();
    EXPECT_NEAR(b0, coefficients[0], EPSILON);
    EXPECT_NEAR(b1, coefficients[1], EPSILON);
    EXPECT_NEAR(b2, coefficients[2], EPSILON);
    EXPECT_NEAR(a1, coefficients[3], EPSILON);
    EXPECT_NEAR(a2, coefficients[4], EPSILON);
}

TEST(butterworth_test, process)
{
    //******************************************************************************
    // scipy signal generated with the following code:
    //
    // sos = signal.butter(4, [15,20], 'bs', fs=50, output='sos')
    // y_filt = signal.sosfilt(sos, np.array([1,3,2,4,3]))
    //
    //******************************************************************************
    {
        int filter_order = 4;
        std::vector<double> freq{15, 20};
        double sampling_frequency = 50;
        butterworth butterworth{filter_order, freq, filter_design::filter_type::bandstop, sampling_frequency};
        std::vector<double> signal{1, 3, 2, 4, 3};
        std::vector<double> result(butterworth.process(signal));

        const double EPSILON = 1.0e-4;
        EXPECT_NEAR(0.4328, result[0], EPSILON);
        EXPECT_NEAR(1.7347, result[1], EPSILON);
        EXPECT_NEAR(2.5811, result[2], EPSILON);
        EXPECT_NEAR(3.4544, result[3], EPSILON);
        EXPECT_NEAR(2.8257, result[4], EPSILON);
    }

    {
        int filter_order = 4;
        std::vector<double> freq{15, 20};
        double sampling_frequency = 50;
        butterworth butterworth{filter_order, freq, filter_design::filter_type::bandstop, sampling_frequency};
        std::vector<double> signal{1, 3, 2, 4, 3};
        std::vector<double> result;

        for (double sample : signal)
        {
            result.push_back(butterworth.process(sample));
        }

        const double EPSILON = 1.0e-4;
        EXPECT_NEAR(0.4328, result[0], EPSILON);
        EXPECT_NEAR(1.7347, result[1], EPSILON);
        EXPECT_NEAR(2.5811, result[2], EPSILON);
        EXPECT_NEAR(3.4544, result[3], EPSILON);
        EXPECT_NEAR(2.8257, result[4], EPSILON);
    }
}
