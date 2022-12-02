#!/bin/sh
set -e

cc day2.c -std=gnu89 -O2 -pedantic -o day2
./day2 < input
