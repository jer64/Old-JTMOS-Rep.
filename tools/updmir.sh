#!/bin/bash
cd oravahttpd_files
rm -f index.html
wget http://www.saunalahti.fi/ehc50/oravahttpd/index.html
rm -f oravahttpd.jpg
wget http://www.saunalahti.fi/ehc50/oravahttpd/oravahttpd.jpg
cd ../

