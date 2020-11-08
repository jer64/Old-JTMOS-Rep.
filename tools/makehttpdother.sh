#!/bin/bash
echo GETTING UPDATED FILES FROM THE SERVER
echo -------------------------------------
./updmir.sh
./sqarc other_www > web2.img
ncftpput 192.168.0.10 -P 21 ./ web2.img
