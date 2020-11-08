#!/usr/bin/perl
#
require "./tools.pl";

#
main();

#
sub CheckDir
{
	my ($i,$i2,@lst);

	#
	@lst = LoadList("find $_[0] -maxdepth 1 -type f -name '*.c'|");
	if($#lst<0) { return; }

	#
	print "\n";
	for($i=0; $i<($#lst+1); $i++)
	{
		#
		print "$lst[$i]\n";
	}
}

#
sub main
{
	my ($i,$i2,@lst);

	#
	@lst = LoadList("find . -type d|");

	#
	for($i=0; $i<($#lst+1); $i++)
	{
		if($lst[$i] eq ".") { goto skip; }
		if($lst[$i] eq "..") { goto skip; }
		CheckDir($lst[$i]);
skip:
	}
}

