#!/usr/bin/env python3

import matplotlib.pyplot as plt

def read_file(name):
    cdm = []
    cda = []
    l1 = []
    n = []
    with open(name) as f:
        for line in f:
            d = line.split()
            n.append(int(d[0]))
            l1.append(int(d[1]))
            cdm.append(int(d[2]))
            cda.append(int(d[3]))

    return cdm, cda, n, l1

def normalize(x, y):
    out = []
    for i in range(len(x)):
        out.append(float(x[i]) / float(y[i]))

    return out

if __name__ == '__main__':
    cdm, cda, n, l1 = read_file('multiple-long-job.sh.o85062')
    ratio = normalize(cdm, cda)

    plt.subplot(211)
    plt.plot(n, ratio, '.-', label='TODO')
    plt.xlabel('n - pocet linearnich generatoru')
    plt.ylabel('L2 cache misses ratio')
    plt.grid(True)

    plt.subplot(212)
    plt.plot(n, l1, 'r.-', label='L1 cache misses')
    plt.xlabel('n - pocet linearnich generatoru')
    plt.ylabel('L1 cache misses')
    plt.grid(True)

    #plt.show()
    plt.savefig('sample.svg', bbox_inches='tight')
