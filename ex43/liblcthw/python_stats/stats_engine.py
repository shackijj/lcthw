#!/usr/bin/python

from stats import Stats
from sys import argv

def one_column(filename):

    try:
        with open(filename) as file:
    
            stats = Stats()
    
            for line in file:
                try:
                   stats.sample(float(line.strip()))
                except ValueError as e:
                   print "File should contains number only, got: %s" % line.strip()
                   exit(3)
    
            stats.dump()
    except IOError as e:
        print str(e)
        exit(2)

def multicolumn(filename):

    try:
        with open(filename) as file:
            headers = file.readline().split()
            counter = 1
            stats_pool = []

            for h in headers:
                stats_pool.append(Stats(h))
                
            sp_len = len(stats_pool)

            for line in file:
                counter += 1
                data = line.strip().split()
                                    
                if len(data) != sp_len: 
                    print "Column missing at %d" % counter
                    exit(4)
              
                for i in range(0, sp_len):
                    stats_pool[i].sample(data[i])

            for i in range(0, sp_len):
                stats_pool[i].dump()

    except IOError as e:
        print str(e)
        exit(2)
     
argv_len = len(argv)
if argv_len < 2:
    print "Usage ./stats_engine.py [-h] [filename]"
    exit(1)
elif argv_len == 2:
    one_column(argv[1])
elif argv_len == 3 and argv[1] == "-h":
    multicolumn(argv[2])
