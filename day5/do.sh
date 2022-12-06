#!/bin/sh
set -e

cc day5.c -std=gnu89 -O2-pedantic -o day5
./day5 < input
