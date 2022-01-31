#include <libbutterworth/utils.h>

#include <exception>
#include <algorithm>
#include <assert.h>

std::complex<double> utils::pop_nearest_real_complex(std::vector<std::complex<double>> &fro, std::complex<double> to, bool real)
{
    sort(fro.begin(), fro.end(), [to](std::complex<double> f1, std::complex<double> f2) -> double
         { return (std::abs(f1 - to) < std::abs(f2 - to)); });
    auto it = std::find_if(fro.begin(), fro.end(), [real](std::complex<double> f)
                           { return ((is_real(f) && real) || (!is_real(f) && !real)); });
    if (it == fro.end())
    {
        throw std::logic_error("Cannot find real/complex number in array.");
    }
    int idx = std::distance(fro.begin(), it);
    std::complex<double> result(fro.at(idx));
    assert(is_real(result) && real || !is_real(result) && !real);
    fro.erase(fro.begin() + idx);
    return (result);
}
