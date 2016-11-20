#!/usr/bin/env python3

import matplotlib.pyplot as plt

def read_file(name):
    cdm = []
    cda = []
    l1 = []
    n = []
    t = []
    with open(name) as f:
        for line in f:
            d = line.split()
            n.append(int(d[0]))
            l1.append(int(d[1]))
            cdm.append(int(d[2]))
            cda.append(int(d[3]))
            t.append(float(d[4]))

    return cdm, cda, n, l1, t

def normalize(x, y):
    out = []
    for i in range(len(x)):
        out.append(float(x[i]) / float(y[i]))

    return out

if __name__ == '__main__':
    cdm, cda, n, l1, t = read_file('cache.sh.o85062')
    ratio = normalize(cdm, cda)

    opt_cdm, opt_cda, opt_n, opt_l1, opt_t = read_file('cache-bf-1000.sh.o85600')
    opt_ratio = normalize(opt_cdm, opt_cda)

    plt.subplot(211)
    plt.plot(opt_n, opt_ratio, '.-', label='loop tiling')
    plt.xlabel('n - pocet linearnich generatoru')
    plt.ylabel('L2 cache misses ratio')
    plt.legend(loc='best')
    plt.grid(True)

    plt.subplot(212)
    plt.plot(opt_n, normalize(opt_t, n), 'g.-', label='loop tiling')
    plt.xlabel('n - pocet linearnich generatoru')
    plt.ylabel('n / t [s ^ -1]')
    plt.legend(loc='best')
    plt.grid(True)

    #plt.show()
    plt.savefig('sample.svg', bbox_inches='tight')
