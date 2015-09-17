#!/bin/bash

set -e

./struct1 db.dat c 512 10

./struct1 db.dat s 0 hello

./struct1 db.dat s 1 goodbuy

./struct1 db.dat l

./struct1 db.dat g 0

./struct1 db.dat d 1

./struct1 db.dat f hell
