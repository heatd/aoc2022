#!/bin/sh
set -e

cc day1.c -std=gnu89 -O2 -pedantic -o day1
./day1 < input
