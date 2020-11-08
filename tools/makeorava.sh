#!/bin/bash
echo GETTING UPDATED FILES FROM THE SERVER
echo -------------------------------------
./updmir.sh
./sqarc oravahttpd_files2 > oravasite.img
ncftpput 192.168.0.10 -P 21 ./ oravasite.img
