#!/usr/bin/perl
require "$ENV{'HOME'}/jtm/kernel32/tools.pl";

# Load symbols list.
@lst = LoadList("kernel32/kernel32.map");

#
print("extern void *xml_functions_to_call[]=(\n");

#
for($i=0; $i<($#lst+1); $i++) {
	@sp = split(/\s+/, $lst[$i]);
	if($sp[0] =~ /^0x00000000/ && $sp[1] ne "" && $sp[2]=~/^\s*$/) {
		# found mapping
		print "\""  . $sp[1] . "\"," . $sp[1] .",\n";
	}
}

#
print(");\n");
