#!/bin/sh
set -e

cc day4.S -std=gnu89 -O2 -pedantic -o day4
./day4 < input
