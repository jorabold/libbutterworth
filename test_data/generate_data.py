"""Generate data for the unit-tests."""

import numpy as np
from scipy import signal
np.set_printoptions(formatter={'float': '{: 0.4f}'.format})

if __name__ == "__main__":
    print(30*"-", "filter_design_test.analog_lowpass ", 30*"-")
    for N in range(8):
        print(N, signal.buttap(N))

    print(30*"-", "filter_design_test.lp2lp", 30*"-")
    print(signal.lp2lp_zpk(np.array([.1+1j, .2, .3]),
                           np.array([.3+0j, .2, .1, 0.0-0.9j]), 1, 2))

    print(30*"-", "filter_design_test.lp2hp", 30*"-")
    print(signal.lp2hp_zpk(np.array([.1+1j, .2, .3]),
                           np.array([.3+0j, .2, .1, 0.0-0.9j]), 1, 2))

    print(30*"-", "filter_design_test.lp2bp", 30*"-")
    print(signal.lp2bp_zpk(np.array([.1+1j, .2, ]),
                           np.array([.3+0j, .1, 0.0-0.9j]), 1, 2, 1))

    print(30*"-", "filter_design_test.lp2lp", 30*"-")
    print(signal.lp2bs_zpk(np.array([.1+1j, .2, ]),
                           np.array([.3+0j, .1, 0.0-0.9j]), 1, 2, 1))

    print(30*"-", "filter_design_test.lp2lp", 30*"-")
    print(signal.bilinear_zpk(
        np.array([.1+1j, .2, ]), np.array([.3+0j, .1, 0.0-0.9j]), 1, 2))

    # can only access that from filter_design.py in scipy?
    # a = [4, 3, 1, 2-2j, 1+1j, 2-1j, 2+1j, 1-1j, 2+2j]
    # print(signal._cplxreal(a))

    print(30*"-", "filter_design_test.zpk2tf", 30*"-")
    print(signal.zpk2tf(z=np.array(
        [0.5-0.6j, 0.5+0.6j]), p=np.array([0.1+0.2j, 0.1-0.2j]), k=1.0))

    print(30*"-", "filter_design_test.zpk2sos", 30*"-")
    print(signal.zpk2sos(z=np.array(
        [-1, -0.5-0.5j, -0.5+0.5j]), p=np.array([0.75, 0.8+0.1j, 0.8-0.1j]), k=1.0))

    print(30*"-", "butterworth_test.lowpass", 30*"-")
    print(signal.butter(8, 15, 'lp', fs=50, output='sos'))

    print(30*"-", "butterworth_test.highpass", 30*"-")
    print(signal.butter(8, 15, 'hp', fs=50, output='sos'))

    print(30*"-", "butterworth_test.bandpass", 30*"-")
    print(signal.butter(4, [15, 20], 'bp', fs=50, output='sos'))

    print(30*"-", "butterworth_test.bandstop", 30*"-")
    print(signal.butter(4, [15, 20], 'bs', fs=50, output='sos'))

    print(30*"-", "butterworth_test.process", 30*"-")
    sos = signal.butter(4, [15, 20], 'bs', fs=50, output='sos')
    print(signal.sosfilt(sos, np.array([1, 3, 2, 4, 3])))
