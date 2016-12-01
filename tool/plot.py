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
            if int(d[0]) < 40000000:
                n.append(int(d[0]))
                l1.append(int(d[1]))
                cdm.append(int(d[2]))
                cda.append(int(d[3]))
                t.append(float(d[4]))

    return n, l1, cdm, cda, t

def normalize(x, y):
    out = []
    for i in range(len(x)):
        out.append(float(x[i]) / float(y[i]))

    return out

if __name__ == '__main__':
    filenames = [
        'opt/and.sh.e86215',
        'opt/cache-72.sh.o85666'
        ]

    plt.title('casova slozitost - logical &')
    n, l1, dcm, dca, t = read_file(filenames[0])
    plt.plot(n, normalize(n, t), '.-', label='wiht logical &')
    n, l1, dcm, dca, t = read_file(filenames[1])
    plt.plot(n, normalize(n, t), '.-', label='bf = 72')

    plt.xlabel('n - pocet linearnich generatoru')
    plt.ylabel('n / t [s ^ -1]')
    plt.legend(loc='best')
    plt.grid(True)

    #plt.show()
    plt.savefig('sample.svg', bbox_inches='tight')
