# Butterworth Filter

Port of the [scipy](https://docs.scipy.org/doc//scipy/reference/generated/scipy.signal.butter.html) digital butterworth filter design methods to C++. The filter can be used to filter a signal with second order sections online (push sample through sections individually) or offline (process whole signal).
The project uses [CMake](https://cmake.org/) as build-tool and [googletest](http://google.github.io/googletest/primer.html) as testing-framework.

How to build and run the tests (start from top-level directory)
```sh
cmake -S . -B build
cmake --build build
cmake --build build --target test # to run the tests
cmake --build build --target install # to install the library (run with sudo if required)
```
You can then change into the `build` directory and run the example application
```sh
cd build && ./filter_example
```
The example application will produce `build/original.txt`, `build/process_batch.txt` and `build/process_sample.txt`. You can display their content with
```sh
cd .. # change back to the top-level directory
python3 test_data/vis_example_output.py
```

The `example` directory contains the example application and a CMakeLists.txt file demonstrating how to use the library after it has been installed. After changing into the `example` directory, it can be built exactly like described above:
```sh
cmake -S . -B build
cmake --build build
cd build && ./filter_example
```