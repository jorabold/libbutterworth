"""visualize output of example application (example.cpp)."""

import numpy as np
import matplotlib.pyplot as plt

if __name__ == "__main__":
    original = np.loadtxt("build/original.txt")
    process_batch = np.loadtxt("build/process_batch.txt")
    process_sample = np.loadtxt("build/process_sample.txt")

    fig, axs = plt.subplots(3, figsize=(10, 10))
    fig.suptitle('example application output')
    axs[0].plot(original)
    axs[0].set_title('original')
    axs[1].plot(process_batch)
    axs[1].set_title('batch processed')
    axs[2].plot(process_sample)
    axs[2].set_title('samples processed')
    plt.show()
