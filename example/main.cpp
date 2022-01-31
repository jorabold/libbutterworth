#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <iterator>

#include <libbutterworth/butterworth.h>
#include <libbutterworth/utils.h>

void signal2file(std::vector<double> signal, std::string filename)
{
    std::ofstream output_file(filename);
    for (const auto &s : signal)
        output_file << s << "\n";
}

std::vector<double> process_batch(std::vector<double> signal, double sampling_frequency)
{
    int filter_order = 8;
    std::vector<double> freq{10, 20};
    butterworth filter(filter_order, freq, filter_design::filter_type::bandpass, sampling_frequency);
    return filter.process(signal);
}

std::vector<double> process_sample(std::vector<double> signal, double sampling_frequency)
{
    int filter_order = 8;
    std::vector<double> freq{10, 20};
    butterworth filter(filter_order, freq, filter_design::filter_type::bandpass, sampling_frequency);

    std::vector<double> signal_filtered;
    for (double sample : signal)
    {
        signal_filtered.push_back(filter.process(sample));
    }

    return signal_filtered;
}

int main(int argc, char *argv[])
{
    double sampling_frequency = 50; // Hz
    double t_max = 2.0;             // s
    std::vector<double> signal;
    for (int t = 0; t <= t_max * sampling_frequency; t++)
        signal.push_back(sin(2 * PI * 5 * t / sampling_frequency));
    signal2file(signal, "original.txt");

    std::vector<double> signal_filtered(process_batch(signal, sampling_frequency));
    signal2file(signal_filtered, "process_batch.txt");

    signal_filtered = process_sample(signal, sampling_frequency);
    signal2file(signal_filtered, "process_sample.txt");
}