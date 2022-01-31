#include <libbutterworth/filter_design.h>
#include <libbutterworth/utils.h>

#include <complex>
#include <limits>
#include <algorithm>
#include <exception>
#include <numeric>    // std::accumulate
#include <functional> // std::multiplies
#include <assert.h>

filter_design::zpk filter_design::analog_lowpass(int filter_order)
{
    filter_design::zpk zpk;

    for (int m = -filter_order + 1; m < filter_order; m += 2)
    {
        zpk.poles.push_back(-exp(std::complex<double>(0, PI * m / (2.0 * filter_order))));
    }
    return (zpk);
}

filter_design::zpk filter_design::lp2lp(filter_design::zpk zpk, double cutoff_frequency)
{
    filter_design::zpk zpk_lp;

    int degree = zpk.poles.size() - zpk.zeros.size();

    // Scale all points radially from origin to shift cutoff frequency
    for (auto z : zpk.zeros)
    {
        zpk_lp.zeros.push_back(cutoff_frequency * z);
    }
    for (auto p : zpk.poles)
    {
        zpk_lp.poles.push_back(cutoff_frequency * p);
    }

    // Each shifted pole decreases gain by wo, each shifted zero increases it.
    // Cancel out the net change to keep overall gain the same
    zpk_lp.gain = zpk.gain * pow(cutoff_frequency, degree);

    return (zpk_lp);
}

filter_design::zpk filter_design::lp2hp(filter_design::zpk zpk, double cutoff_frequency)
{
    filter_design::zpk zpk_hp;

    int degree = zpk.poles.size() - zpk.zeros.size();

    // Invert positions radially about unit circle to convert LPF to HPF
    // Scale all points radially from origin to shift cutoff frequency
    for (auto z : zpk.zeros)
    {
        zpk_hp.zeros.push_back(cutoff_frequency / z);
    }
    for (auto p : zpk.poles)
    {
        zpk_hp.poles.push_back(cutoff_frequency / p);
    }

    // If lowpass had zeros at infinity, inverting moves them to origin.
    for (int i = 0; i < degree; i++)
    {
        zpk_hp.zeros.push_back(0);
    }

    // Cancel out gain change caused by inversion
    std::complex<double> prod_z(1.0, 1.0);
    for (auto z : zpk.zeros)
    {
        prod_z *= -z;
    }
    std::complex<double> prod_p(1.0, 1.0);
    for (auto p : zpk.poles)
    {
        prod_p *= -p;
    }
    zpk_hp.gain = zpk.gain * std::real(prod_z / prod_p);

    return (zpk_hp);
}

filter_design::zpk filter_design::lp2bp(filter_design::zpk zpk, double passband_center, double passband_width)
{
    filter_design::zpk zpk_bp;

    int degree = zpk.poles.size() - zpk.zeros.size();

    // Duplicate poles and zeros and shift from baseband to +wo and -wo
    // (order matters: same results as scipy)
    for (auto z : zpk.zeros)
    {
        z *= passband_width / 2;
        zpk_bp.zeros.push_back(z + std::sqrt(std::pow(z, 2) - std::pow(passband_center, 2)));
    }
    for (auto z : zpk.zeros)
    {
        z *= passband_width / 2;
        zpk_bp.zeros.push_back(z - std::sqrt(std::pow(z, 2) - std::pow(passband_center, 2)));
    }
    for (auto p : zpk.poles)
    {
        p *= passband_width / 2;
        zpk_bp.poles.push_back(p + std::sqrt(std::pow(p, 2) - std::pow(passband_center, 2)));
    }
    for (auto p : zpk.poles)
    {
        p *= passband_width / 2;
        zpk_bp.poles.push_back(p - std::sqrt(std::pow(p, 2) - std::pow(passband_center, 2)));
    }

    // Move degree zeros to origin, leaving degree zeros at infinity for BPF
    for (int i = 0; i < degree; i++)
    {
        zpk_bp.zeros.push_back(0);
    }

    // Cancel out gain change from frequency scaling
    zpk_bp.gain = zpk.gain * std::pow(passband_width, degree);

    return (zpk_bp);
}

filter_design::zpk filter_design::lp2bs(filter_design::zpk zpk, double stopband_center, double stopband_width)
{
    int degree = zpk.poles.size() - zpk.zeros.size();

    filter_design::zpk zpk_bs;

    // Invert to a highpass filter with desired bandwidth
    // Duplicate poles and zeros and shift from baseband to +wo and -wo
    for (auto z : zpk.zeros)
    {
        z = (stopband_width / 2) / z;
        zpk_bs.zeros.push_back(z + std::sqrt(std::pow(z, 2) - std::pow(stopband_center, 2)));
    }
    for (auto z : zpk.zeros)
    {
        z = (stopband_width / 2) / z;
        zpk_bs.zeros.push_back(z - std::sqrt(std::pow(z, 2) - std::pow(stopband_center, 2)));
    }
    for (auto p : zpk.poles)
    {
        p = (stopband_width / 2) / p;
        zpk_bs.poles.push_back(p + std::sqrt(std::pow(p, 2) - std::pow(stopband_center, 2)));
    }
    for (auto p : zpk.poles)
    {
        p = (stopband_width / 2) / p;
        zpk_bs.poles.push_back(p - std::sqrt(std::pow(p, 2) - std::pow(stopband_center, 2)));
    }

    // Move any zeros that were at infinity to the center of the stopband
    for (int i = 0; i < degree; i++)
    {
        zpk_bs.zeros.push_back(std::complex<double>(0, stopband_center));
    }
    for (int i = 0; i < degree; i++)
    {
        zpk_bs.zeros.push_back(std::complex<double>(0, -stopband_center));
    }

    // Cancel out gain change caused by inversion
    std::complex<double> prod_z(1.0, 1.0);
    for (auto z : zpk.zeros)
    {
        prod_z *= -z;
    }
    std::complex<double> prod_p(1.0, 1.0);
    for (auto p : zpk.poles)
    {
        prod_p *= -p;
    }
    zpk_bs.gain = zpk.gain * std::real(prod_z / prod_p);

    return (zpk_bs);
}

filter_design::zpk filter_design::bilinear_transform(filter_design::zpk zpk, double sampling_frequency)
{
    int degree = zpk.poles.size() - zpk.zeros.size();

    // Bilinear transform the poles and zeros
    filter_design::zpk zpk_result;

    for (auto z : zpk.zeros)
    {
        zpk_result.zeros.push_back((2.0 * sampling_frequency + z) / (2.0 * sampling_frequency - z));
    }
    for (auto p : zpk.poles)
    {
        zpk_result.poles.push_back((2.0 * sampling_frequency + p) / (2.0 * sampling_frequency - p));
    }

    // Any zeros that were at infinity get moved to the Nyquist frequency
    for (int i = 0; i < degree; i++)
    {
        zpk_result.zeros.push_back(-1);
    }

    // Compensate for gain change
    std::complex<double> prod_z(1.0, 1.0);
    for (auto z : zpk.zeros)
    {
        prod_z *= 2.0 * sampling_frequency - z;
    }
    std::complex<double> prod_p(1.0, 1.0);
    for (auto p : zpk.poles)
    {
        prod_p *= 2.0 * sampling_frequency - p;
    }
    zpk_result.gain = zpk.gain * std::real(prod_z / prod_p);
    return (zpk_result);
}

std::pair<std::vector<std::complex<double>>, std::vector<std::complex<double>>> filter_design::cplxpair(std::vector<std::complex<double>> z)
{
    double tol = 100 * std::numeric_limits<double>::epsilon();

    // Sort by real part, magnitude of imaginary part (speed up further sorting)
    // (https://stackoverflow.com/a/28183074)
    auto complex_comparator = [](const std::complex<double> &a, const std::complex<double> &b)
    {
        return (a.real() == b.real() ? a.imag() < b.imag() : a.real() < b.real());
    };

    std::sort(z.begin(), z.end(), complex_comparator);

    std::vector<std::complex<double>> z_r;  // real numbers
    std::vector<std::complex<double>> z_ip; // complex numers with positive imaginary part
    std::vector<std::complex<double>> z_in; // complex numers with negative imaginary part
    for (std::complex<double> number : z)
    {
        if (std::abs(number.imag()) < tol)
        {
            z_r.push_back(number.real());
        }
        else if (number.imag() > 0)
        {
            z_ip.push_back(number);
        }
        else
        {
            z_in.push_back(number);
        }
    }
    if (z_ip.size() != z_in.size())
    {
        throw std::invalid_argument("Array contains complex value with no matching conjugate");
    }

    for (int i = 0; i < z_ip.size(); i++)
    {
        bool found = false;
        for (int j = 0; j < z_in.size(); j++)
        {
            if (std::abs(z_ip.at(i) - std::conj(z_in.at(j))) < tol)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            throw std::invalid_argument("Array contains complex value with no matching conjugate");
        }
    }

    return (std::pair<std::vector<std::complex<double>>, std::vector<std::complex<double>>>{z_r, z_ip});
}

biquad filter_design::zpk2tf(filter_design::zpk zpk_sos)
{
    // calculating poly in-place
    std::vector<std::complex<double>> a{std::complex<double>(1, 0),
                                        (-zpk_sos.poles.at(0) - zpk_sos.poles.at(1)),
                                        zpk_sos.poles.at(0) * zpk_sos.poles.at(1)};
    for (auto ai : a)
    {
        // if complex roots are all complex conjugates, the roots are real. If not, smth went wrong.
        if (!utils::is_real(ai))
            throw std::logic_error("Filter coefficients are complex.");
    }

    std::vector<std::complex<double>> b{zpk_sos.gain * std::complex<double>(1, 0),
                                        zpk_sos.gain * (-zpk_sos.zeros.at(0) - zpk_sos.zeros.at(1)),
                                        zpk_sos.gain * zpk_sos.zeros.at(0) * zpk_sos.zeros.at(1)};
    for (auto bi : b)
    {
        // if complex roots are all complex conjugates, the roots are real. If not, smth went wrong.
        if (!utils::is_real(bi))
            throw std::logic_error("Filter coefficients are complex.");
    }

    return (biquad(std::real(b[0]), std::real(b[1]), std::real(b[2]), std::real(a[1]), std::real(a[2])));
}

std::vector<biquad> filter_design::zpk2sos(filter_design::zpk zpk)
{
    // ensure we have the same number of poles and zeros
    for (int i = 0; i < zpk.zeros.size() - zpk.poles.size(); i++)
    {
        zpk.poles.push_back(0);
    }
    for (int i = 0; i < zpk.poles.size() - zpk.zeros.size(); i++)
    {
        zpk.zeros.push_back(0);
    }
    assert(zpk.poles.size() == zpk.zeros.size());
    int n_sections = (zpk.zeros.size() + 1) / 2;

    if (zpk.zeros.size() % 2 == 1)
    {
        zpk.poles.push_back(0);
        zpk.zeros.push_back(0);
    }

    // Ensure we have complex conjugate pairs
    // (note that cplxpair only gives us the element of each complex pair with
    // positive imaginary part):
    auto poles_r_i = cplxpair(zpk.poles);
    std::vector<std::complex<double>> poles(poles_r_i.first);
    for (std::complex<double> p : poles_r_i.second)
    {
        poles.push_back(p);
    }
    auto zeros_r_i = cplxpair(zpk.zeros);
    std::vector<std::complex<double>> zeros(zeros_r_i.first);
    for (std::complex<double> z : zeros_r_i.second)
    {
        zeros.push_back(z);
    }

    // sort poles by how close they are to the unit circle
    sort(poles.begin(), poles.end(), [](std::complex<double> p1, std::complex<double> p2) -> double
         { return (std::abs(1.0 - std::abs(p1)) < std::abs(1.0 - std::abs(p2))); });

    std::vector<std::vector<std::complex<double>>> poles_sos;
    std::vector<std::vector<std::complex<double>>> zeros_sos;
    for (int s = 0; s < n_sections; s++)
    {
        std::complex<double> p1, p2, z1, z2;

        // Select the next "worst" pole (closes to unit cycle)
        p1 = poles.front();
        poles.erase(poles.begin());

        // Pair that pole with a zero

        std::vector<bool> poles_real(utils::is_real(poles));
        if (utils::is_real(p1) && std::accumulate(poles_real.begin(), poles_real.end(), 0) == 0)
        {
            // Special case to set a first-order section
            z1 = utils::pop_nearest_real_complex(zeros, p1, true);
            p2 = std::complex<double>(0, 0);
            z2 = std::complex<double>(0, 0);
        }
        else
        {
            if (!utils::is_real(p1) && std::accumulate(poles_real.begin(), poles_real.end(), 0) == 1)
            {
                // Special case to ensure we choose a complex zero to pair
                // with so later (setting up a first-order section)
                z1 = utils::pop_nearest_real_complex(zeros, p1, false);
                assert(!utils::is_real(z1));
            }
            else
            {
                // Pair the pole with the closest zero (real or complex)
                int z1_idx = std::distance(
                    zeros.begin(),
                    std::min_element(zeros.begin(),
                                     zeros.end(),
                                     [p1](std::complex<double> zi, std::complex<double> zj)
                                     {
                                         return (std::abs(p1 - zi) < std::abs(p1 - zj));
                                     }));
                z1 = zeros.at(z1_idx);
                zeros.erase(zeros.begin() + z1_idx);
            }

            // Now that we have p1 and z1, figure out what p2 and z2 need to be
            if (!utils::is_real(p1))
            {
                if (!utils::is_real(z1))
                {
                    // complex pole, complex zero
                    p2 = std::conj(p1);
                    z2 = std::conj(z1);
                }
                else
                {
                    // complex pole, real zero
                    p2 = std::conj(p1);
                    z2 = utils::pop_nearest_real_complex(zeros, p1, true);
                    assert(utils::is_real(z2));
                }
            }
            else
            {
                if (!utils::is_real(z1))
                {
                    // real pole, complex zero
                    z2 = std::conj(z1);
                    p2 = utils::pop_nearest_real_complex(poles, z1, true);
                    assert(utils::is_real(p2));
                }
                else
                {
                    // real pole, real zero
                    // pick the next "worst" pole to use
                    auto p2_idx = std::find_if(poles.begin(), poles.end(), [](std::complex<double> pi)
                                               { return (utils::is_real(pi)); });
                    p2 = *p2_idx;
                    assert(utils::is_real(p2));
                    poles.erase(p2_idx);
                    // find a real zero to match the added pole
                    z2 = utils::pop_nearest_real_complex(zeros, p2, true);
                    assert(utils::is_real(z2));
                }
            }
        }
        poles_sos.push_back(std::vector<std::complex<double>>{p1, p2});
        zeros_sos.push_back(std::vector<std::complex<double>>{z1, z2});
    }
    assert(poles.size() == 0 && zeros.size() == 0);

    // Construct the system, reversing order so the "worst" are last
    std::vector<biquad> sos;
    for (int s = n_sections - 1; s >= 0; s--)
    {
        double gain = (s == n_sections - 1) ? zpk.gain : 1;
        sos.push_back(zpk2tf(filter_design::zpk{zeros_sos.at(s), poles_sos.at(s), gain}));
    }

    return (sos);
}
