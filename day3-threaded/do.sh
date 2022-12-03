#!/bin/sh
set -e

cc day3.c -std=gnu99 -O2 -o day3
./day3 < input
