#!/usr/bin/python

from stats import Stats
import unittest

def EQ(a, b, n):

    return round(a, n) == round(b, n)

class StatsTests(unittest.TestCase):

    def setUp(self):
        self.st = Stats()
        
        self.expect = Stats()
        self.expect.sumsq = 425.1641
        self.expect.sum = 55.84602
        self.expect.min = 0.333
        self.expect.max = 9.678
        self.expect.n = 10

    def test_operations(self):
        
        samples = [
            6.1061334, 9.6783204, 1.2747090, 8.2395131, 0.3333483,
            6.9755066, 1.0626275, 7.6587523, 4.9382973, 9.5788115
        ]


        for i in samples: self.st.sample(i)

        self.st.dump()

        self.assertEqual(EQ(self.st.sumsq, self.expect.sumsq, 3), True)
        self.assertEqual(EQ(self.st.sum, self.expect.sum, 3), True)
        self.assertEqual(EQ(self.st.min, self.expect.min, 3), True)
        self.assertEqual(EQ(self.st.max, self.expect.max, 3), True)
        self.assertEqual(EQ(self.st.n, self.expect.n, 3), True)
        self.assertEqual(EQ(self.expect.mean(), self.st.mean(), 3), True)
        self.assertEqual(EQ(self.expect.stdev(), self.st.stdev(), 3), True)

    def test_recreate(self):
        self.st.recreate( self.expect.sum, self.expect.sumsq, self.expect.n,
            self.expect.min, self.expect.max
        )
        
        self.assertEqual(self.expect.sum == self.st.sum, True)
        self.assertEqual(self.expect.sumsq == self.st.sumsq, True)
        self.assertEqual(self.expect.min == self.st.min, True)
        self.assertEqual(self.expect.max == self.st.max, True)
        self.assertEqual(self.expect.n == self.st.n, True)
        self.assertEqual(EQ(self.expect.mean(), self.st.mean(), 3),  True)
        self.assertEqual(EQ(self.expect.stdev(), self.st.stdev(), 3), True)
  
        
if __name__ == "__main__":
    unittest.main()
