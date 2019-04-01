#!usr/bin/env python
#-*- coding: utf-8 -*-

import math
import theano
import theano.tensor as T

def main():
    x1 = T.matrix()
    x2 = T.matrix()
    y1 = x1 * x2
    y2 = T.dot(x1, x2)
    F1 = theano.function([x1, x2], y1)
    F2 = theano.function([x1, x2], y2)
    A = [[1, 2], [3, 4]]
    B = [[2, 4], [6, 8]]
    C = [[1, 2], [3, 4], [5, 6]]

    print F1(A, B)
    print F2(C, B)
    pass

if __name__ == '__main__':
    main()   
