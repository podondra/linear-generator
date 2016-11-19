#!/usr/bin/env python3

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

def normalize(n, t):
    out = []
    for i in range(len(t)):
        out.append(float(n[i]) / float(t[i]))

    return out

if __name__ == '__main__':
    n2, t2 = read_file('opt-measure/fast-math.sh.o84785')
    y2 = normalize(n2, t2);

    n3, t3 = read_file('opt-measure/profile.sh.o84973')
    y3 = normalize(n3, t3);

    plt.title('tripruchodova optimalizace')
    plt.plot(n2, y2, 'm.-', label='-ffast-math')
    plt.plot(n3, y3, 'k.-', label='-fprofile-use')

    plt.legend(loc='best')

    plt.ylabel('n / t [s ^ -1]')
    plt.xlabel('n - pocet linearnich generatoru')

    plt.grid(True)

    plt.savefig('sample.svg', bbox_inches='tight')
