#!/usr/bin/perl
#########################################################################################

#
$PROGRAM_NAME = "makgen.pl 1.0.2004";

# Some flags...
$JTMOSSTDLIBCLOC="../libc/stlibc.o";
$STDSRC="../lib/stdapp.src";
$_8TO1="../tools/8to1";
$BIN2C="../tools/bin2c";
$CC="gcc";
$CFLAGS="-Wall -w -c -O -march=i486 -DCPU=i586 -I../include";
$AR="ar";
$AS="as";
$ASM="nasm";
$ASMFLAGS="-f elf";
$LD="ld";
$LDFLAGS="-oformat=binary -Map \$(APPNAME).map -T\$(STDSRC) -L../lib -lcjtmos -o \$(APPNAME).bin";

#########################################################################################
#
main();

#########################################################################################
#
sub WriteVars
{
	my $of;

	#
	$of = $_[0];

	#
	print $of ("#
#
# $APPNAME
#
# AUTOMATICALLY GENERATED MAKEFILE
# GENERATED BY $PROGRAM_NAME
#
APPNAME=$APPNAME
JTMOSSTDLIBCLOC=$JTMOSSTDLIBCLOC
STDSRC=$STDSRC
8TO1=$8TO1
BIN2C=$BIN2C
CC=$CC
CFLAGS=$CFLAGS
AR=$AR
AS=$AS
ASM=$ASM
ASMFLAGS=$ASMFLAGS
LD=$LD
LDFLAGS=$LDFLAGS

default: linkit

		");
}

#########################################################################################
#
sub Options
{
	my $of,$i,$i2;

	#
	$of = $_[0];

	#
	print $of ("
linkit: application
	\@echo ---: Linking out binary: $fn.bin 
	\$(LD) \$(JTMOSSTDLIBCLOC) $objfiles \$(LDFLAGS)

application:
	\n");

	#
	for($i=0; $i<($#objs+1); $i++)
	{
		print $of ("	\$(CC) \$(CFLAGS) $objs[$i].c\n");
	}

	#
	print $of ("
clean:
	rm -f $objfiles
	
	\n");
}

#########################################################################################
#
sub Usage
{
	#
	print "$PROGRAM_NAME\n";
	print "Usage: makgen.pl [program name] [object1+object2+...]\n";
	print "Decription: generates make file for specified program\n";
}

#########################################################################################
#
sub main
{
	my $f,$i,$i2;

	#
	$fn = $ARGV[0];
	$objlist = $ARGV[1];
	$objlist =~ s/\+/ /g;
	@objs = split(" ", $objlist);

	#
	for($i=0; $i<($#objs+1); $i++)
	{
		$objfiles = "$objs[$i].o $objfiles";
		$cfiles = "$objs[$i].c $cfiles";
	}

#	print "$objfiles $cfiles\n";

	#
	$APPNAME = $fn;

	#
	if($fn eq "" || $objlist eq "")
	{
		Usage();
		return;
	}

	###################################################################
	#
	print "$fn.mak\n";

	#
	open($f, ">$fn.mak") || die "Can't write $fn.mak.\n";
	#
	WriteVars($f);
	Options($f);
	#
	close($f);
}
