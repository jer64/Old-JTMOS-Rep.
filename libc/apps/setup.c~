//==============================================================
// JTMOS Setup Program
// (C) 2003 by Jari Tuominen
//==============================================================
#include <stdio.h>
#include <conio.h>

int main(int argc, char **argv)
{
	textcolor(7); textbackground(BLUE);
	clrscr();

	// Report
	printf("\n");
	textbackground(RED);
	printf("  Welcome to JTMOS installation  \n");
	textbackground(BLUE);
	printf("\n");
	printf("Note: installation will erase hard disk's contents\n");
	printf("press [i] to format HD & install JTMOS\n");
	printf("press [ESC] to cancel installation\n");
	if(getch()!='i')
	{
		printf("> Cancelling installation.\n");
		sleep(2);
		exit(0);
	}
	printf("> Installing ");
	sleep(2);

	// Install
	clrscr();
	system("mkfs hda");
	clrscr();
	printf("\n  Installing JTMOS core files\n");
	printf("  ===========================\n\n");
	printf("\n");
	printf("  This will take a moment please wait awhile ... \n\n");

	//
	printf("- directory structure creation\n");
	chdir("/hda");
	mkdir("bin");
	mkdir("etc");
	mkdir("home");
	mkdir("root");
	mkdir("usr");
	mkdir("tmp");

	// Copy files (I)
	printf("- copying files\n");

	// Install install.img & unz
	chdir("/hda/bin");
	system(":install -q -o /hda/bin");
	printf("\n");

	// Unpack install.img.nz
	// Extract install.img archive
	chdir("/hda/bin");
	system("unz");

	printf("Installation completed.\n"); sleep(1);
	return 0;
}

