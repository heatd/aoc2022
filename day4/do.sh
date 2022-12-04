#!/bin/sh
set -e

cc day4.c -std=gnu89 -O2 -pedantic -o day4
./day4 < input
