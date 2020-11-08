#!/bin/bash
echo "Archiving JTMOS($(date +%d/%m/%y)) with tar + gzip..."
cp jtm32.img ~
rm ~/jtmos-$(date +%d%m%y).tar.gz
cd ..
tar -c jtm | gzip -9 > ~/jtmos-$(date +%d%m%y).tar.gz
cd jtm
ls -la ~/jtmos-$(date +%d%m%y).tar.gz
#ncftpput 192.168.0.10 -u code -p code -P 21 ./ \
#ncftpput 192.168.0.10 -P 21 ./ ~/jtm32-$(date +%d%m%y).tar.bz2
