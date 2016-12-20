#!/usr/bin/env python

import matplotlib.pyplot as plt

def read_file(name):
    n = []
    t = []
    with open(name) as f:
        for line in f:
            d = line.split()
            n.append(int(d[0]))
            t.append(float(d[1]))

    return n, t

def normalize(x, y):
    out = []
    for i in range(len(x)):
        out.append(float(x[i]) / float(y[i]))

    return out

if __name__ == '__main__':
    filenames = [
        'par/12-threads.sh.e88962',
        'par/24-threads-n.sh.e88974'
        ]

    plt.title('casova slozitost - paralelizace')
    n, t = read_file(filenames[0])
    plt.plot(n, normalize(n, t), '.-', label='paralelizace')
    n, t = read_file(filenames[1])
    plt.plot(n, normalize(n, t), '.-', label='24 vlaken')

    plt.xlabel('n - pocet linearnich generatoru')
    plt.ylabel('n / t [s ^ -1]')
    plt.legend(loc='best')
    plt.grid(True)

    #plt.show()
    plt.savefig('sample.svg', bbox_inches='tight')
