#!/bin/bash
make tools ; cp sq2.raw n1 ; nzip n1 ; nzip -d n1.nz
ncftpput 192.168.0.10 -t 1 -P 21 ./ n1

