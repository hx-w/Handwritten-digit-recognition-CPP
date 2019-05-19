# D:/PY2/python 2.7.15
# -*- coding: utf-8 -*-

import numpy as np
import os

def main():
    filename = './mnist_train.csv'
    container = []
    with open(filename, 'r') as inFile:
        for line in inFile:
            container.append(line)
    
    np.random.shuffle(container)
    os.remove(filename)
    with open(filename, 'w') as outFile:
        for line in container:
            outFile.write(line)

if __name__ == '__main__':
    main()
