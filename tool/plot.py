#!/usr/bin/env python3

import matplotlib.pyplot as plt

def read_file(name):
    x = []
    y = []
    with open(name) as f:
        for line in f:
            d = line.split()
            x.append(int(d[0]))
            y.append(float(d[1]))

    return x, y

def normalize(x, y):
    out = []
    for i in range(len(x)):
        out.append(float(x[i]) / float(y[i]))

    return out

if __name__ == '__main__':
    n, c = read_file('multiple-job.sh.o85040')
    c = normalize(c, n)

    plt.title('L1 cache misses')
    plt.plot(n, c, '.-', label='papi')

    plt.legend(loc='best')

    plt.xlabel('n - pocet linearnich generatoru')
    plt.ylabel('L1 cache misses')

    plt.grid(True)

    plt.show()
    #plt.savefig('sample.svg', bbox_inches='tight')
