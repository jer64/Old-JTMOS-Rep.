#!/bin/bash
perl xmlparse_createsymlist.pl > xml_funccalls.c
gcc -w -o xmlparse xmlparse.c
