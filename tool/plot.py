#!/usr/bin/env python3

import matplotlib.pyplot as plt

def read_file(name):
    n = []
    t = []
    with open(name) as f:
        for line in f:
            d = line.split()
            n.append(int(data[0]))
            t.append(float(data[1]))

    return n, t

def normalize(n, t);
    out = []
    for i in range(len(time_seq)):
        out.append(float(n[i]) / float(t[i]))

    return out

if __name__ == '__main__':
    n1, t1 = read_file('')
    y1 = normalize(n1, t1);

    # EDIT
    plt.plot(n, y, 'b.-', label='label')
    plt.title('linearnich generatoru za sekundu - population count')

    plt.legend(loc='best')

    plt.ylabel('n / t [s ^ -1]')
    plt.xlabel('pocet linearnich generatoru')

    plt.grid(True)

    plt.save('sample.png')
