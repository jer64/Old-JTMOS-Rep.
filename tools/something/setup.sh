#!/bin/sqsh
cd /
clear
echo "JTMOS INSTALLATION"
echo "=================="
echo ""
echo "Installing operating system to hard disk ..." 
mkfs hda
cd /bin
mkdir /hda/bin
cp --verbose * /hda/bin
echo ""
echo "Installation complete."
echo "----------------------"
echo ""
echo "System should be able to find the system files"
echo "on your hard when you next time boot up system.
echo "Remember that you have to boot using the system"
echo "boot floppy, hard disk booting is not yet supported."
