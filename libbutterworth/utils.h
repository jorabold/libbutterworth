#ifndef __UTILS__H__
#define __UTILS__H__

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <complex>
#include <limits>

const double PI = 3.141592653589793238463;

#define DEBUG

#ifdef DEBUG
#define DEBUG_STREAM(stream)                             \
    {                                                    \
        std::stringstream ss;                            \
        ss << stream;                                    \
        std::cout << "Debug: " << ss.str() << std::endl; \
    }
#else
#define DEBUG_STREAM(stream)
#endif //DEBUG

#define INFO_STREAM(stream)                             \
    {                                                   \
        std::stringstream ss;                           \
        ss << stream;                                   \
        std::cout << "Info: " << ss.str() << std::endl; \
    }

#define WARN_STREAM(stream)                                \
    {                                                      \
        std::stringstream ss;                              \
        ss << stream;                                      \
        std::cout << "Warning: " << ss.str() << std::endl; \
    }

#define ERROR_STREAM(stream)                             \
    {                                                    \
        std::stringstream ss;                            \
        ss << stream;                                    \
        std::cout << "Error: " << ss.str() << std::endl; \
    }

namespace utils
{
    /** Test whether z is real or has a complex part.
     *
     * @param z number with or without complex part
     * @return true if z has a complex part
     */
    template <typename T>
    bool is_real(T z)
    {
        return (std::abs(std::imag(z)) < 100 * std::numeric_limits<double>::epsilon());
    }

    /** Test for every number in a vector whether it is real or has a complex part.
     *
     * @param zv vector containing numbers with or without complex part
     * @return vector of true/false: true if number has no complex part
     */
    template <typename T>
    std::vector<bool> is_real(std::vector<T> zv)
    {
        std::vector<bool> result;

        for (auto z : zv)
        {
            result.push_back(is_real(z));
        }
        return (result);
    }

    /** Return real or complex element of for closest to to and remove it from fro.
     *
     * Assumes fro is sorted or sorts it.
     *
     * @param fro Grab to from this vector
     * @param to Return number out of fro closes to this number
     * @param real set to true is returned number should be real
     * @return number from fro closest to to
     */

    std::complex<double> pop_nearest_real_complex(std::vector<std::complex<double>> &fro, std::complex<double> to, bool real);
} // namespace utils

#endif //!__UTILS__H__
