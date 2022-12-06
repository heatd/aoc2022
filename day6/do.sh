#!/bin/sh
set -e

cc day6.c -std=gnu89 -O2 -pedantic -o day6
./day6 < input
