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
    n1, t1 = read_file('opt-measure/opt-popcount.sh.o84757')
    y1 = normalize(n1, t1);

    n2, t2 = read_file('opt-measure/opt-interchange.sh.o84762')
    y2 = normalize(n2, t2);

    # EDIT
    plt.title('loop interchange')
    plt.plot(n1, y1, 'g.-', label='population count')
    plt.plot(n2, y2, 'r.-', label='loop interchange')

    plt.legend(loc='best')

    plt.ylabel('n / t [s ^ -1]')
    plt.xlabel('n - pocet linearnich generatoru')

    plt.grid(True)

    plt.savefig('sample.svg', bbox_inches='tight')
