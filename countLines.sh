#!/bin/bash
find . -type f -name \*.c -o -name \
\*.h -o -name \*.asm|xargs cat|wc -l
