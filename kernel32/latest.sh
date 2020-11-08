#!/bin/bash
find $1 -name '*.c' -type f -exec stat --format '%Y :%y %n' "{}" \; | sort -nr | cut -d: -f2- | head -n 100 > latest.txt
less latest.txt
