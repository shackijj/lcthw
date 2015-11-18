#!/usr/bin/python
from subprocess import Popen, PIPE
import re

timer = 0
regexp = re.compile(r'^\[INFO\] \(tests/radixmap_tests\.c\:\d\d\) RadixMap lyfecycle Time elapsed\: (.*)$')

for i in range(0, 30):
    p = Popen(['./radixmap_tests'], stdin=PIPE, stdout=PIPE, stderr=PIPE)
    out,err = p.communicate()
    
    for line in err.splitlines():
        
        res = regexp.match(line)
        if res:
            timer += float(res.group(1))

print "Average: %f" % (timer / 30)        
