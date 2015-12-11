#!/usr/bin/python

class Stats(object):

    def __init__(self, name="Stats"):
        self.recreate(0.0, 0.0, 0L, 0.0, 0.0)
        self.name = name

    def recreate(self, sum, sumsq, n, min, max):

        self.sum = float(sum)
        self.sumsq = float(sumsq)
        self.n = long(n)
        self.min = float(min)
        self.max = float(max)

    def name(self, name):
        self.name = name

    def mean(self):
        return (self.sum / self.n)

    def stdev(self):
        return ((self.sumsq - ( self.sum * self.sum / self.n)) / (self.n - 1)) ** 0.5

    def sample(self, s):
        s = float(s)
        self.sum += s
        self.sumsq += s * s

        if self.n == 0:
            self.min = s
            self.max = s
        else:
            if s < self.min: self.min = s
            if s > self.max: self.max = s

        self.n += 1

    def dump(self):
        print "%s\nsum: %f, sumsq: %f, n: %ld, min: %f, max: %f, mean: %f, stdev: %f" % (
            self.name, self.sum, self.sumsq, self.n, self.min, self.max,
            self.mean(), self.stdev())
