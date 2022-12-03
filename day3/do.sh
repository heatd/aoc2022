#!/bin/sh
set -e

cc day3.c -std=gnu89 -O2 -pedantic -o day3
./day3 < input
