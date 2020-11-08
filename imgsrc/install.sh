#!/bin/sh
echo "    -=- COMPILATION PROGRESS -=- "
echo "******* COMPILING"
gcc -o img img.c -O2
echo "******* BINARY :"
ls -la img
echo "******* INSTALLING(Requires you to be a root user! -- try command 'su')"
chmod a+rx img
su -c 'cp img /bin ; chmod a+rx /bin/img'
echo You can also manually copy the binary executable to your executable files directory...
echo "* Done."
