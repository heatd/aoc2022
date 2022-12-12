#!/bin/sh
set -e

cc day7.c -std=gnu89 -O2 -pedantic -o day7
./day7 < input
