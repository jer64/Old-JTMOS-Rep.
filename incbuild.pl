#!/usr/bin/perl
main();

sub main()
{
 #
 local $i,$str,$str2;

 #
 print stderr "incbuild.pl: increasing build number";

 #
 open f,"<kernel32/init/buildnr.h";
 @lines = <f>;
 close f;

 #
 for($i=0; $i<$#lines; $i++) { chomp $lines[$i]; }

 # grab build number indentifier
 $str = $lines[3];
 @str2 = split /0x/,$str;
 $buildnr = $str2[1];

 # increase build number
 print stderr "Inc build $buildnr to ";
 $buildnr++;
 print stderr "$buildnr\n";

 #
 system "./telldate.sh > date.tmp";
 open(FILE,"date.tmp");
 $aika=<FILE>;
 chomp $aika;
 close(FILE);

 #
 $timex = localtime;
 $lines[3] = sprintf "\#define BUILD_NR 0x%08u",$buildnr;
 $lines[4] = sprintf "\#define BUILD_DATE \"%s\"",$aika;

 # and finally store
 # the changes at the file
 open f2,">kernel32/init/buildnr.h";
 for($i=0; $i<$#lines+1; $i++) {
    print f2 "$lines[$i]\n";
 }
 close f2;
}
