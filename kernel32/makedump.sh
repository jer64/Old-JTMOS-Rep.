#!/bin/bash
echo Making dump out of 'kernel32.bin' ...
./bin2asc kernel32.bin -a >kernel32.ascdump
ls -la kernel32.ascdump
echo Done.
