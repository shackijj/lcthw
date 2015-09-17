#!/usr/bin/python2.7

class Stack:

    def __init__(self):
        self.stack = []

    def push(self, i):
        self.stack.append(i)
     
    def pop(self):
        return self.stack.pop()

    def last(self):
        return self.stack[0]

if __name__ == "__main__":
    s = Stack()
    s.push(1)
    s.push(2)
    print s.pop()
    print len(s.stack)
