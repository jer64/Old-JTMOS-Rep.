//-----------------------------------------------------------------------
// Filer - Squirrel File Explorer.
// (C) 2003 by Jari Tuominen.
//-----------------------------------------------------------------------
#include <stdio.h>
#include <conio.h>

// Max. amount of files to remember.
#define N_MAX_FILES		4000

// Path to editor executable.
#define EDIT_CMD_PATH		"/hda/bin/edit.bin"
// Path to editor help page text file.
#define FILER_HLP_PATH		"/hda/bin/filer.hlp"

// Directory entry.
typedef struct
{
	// File name.
	char name[48];
	// File size.
	int size;
	// File type.
	// 2: directory, 1: file, other: nothing(?)
	int type;
	// File is selected for operation.
	// (e.g. copy(F5), move(F6), delete(F8))
	char selected;
}DE;

// Main Filer structure.
typedef struct
{
	// File informations.
	DE file[N_MAX_FILES];
	// Amount of files in memory
	int n_files;
	// Number of selected files.
	int n_selected;
	// Total bytes in selected files on the directory.
	int sz_selected;
	// Total bytes in all files on the directory.
	int sz_all;

	// Y-scroll position.
	int ypos;

	// Y-position for cursor.
	int y;
}FILER;
FILER *filer;

//-----------------------------------------------------------------------
// Initialize Filer
//
int initFiler(void)
{
	//
	filer = malloc(sizeof(FILER));
	memset(filer, 0, sizeof(FILER));

	//
	textcolor(7); textbackground(BLUE);
	clrscr();

	//
	return 0;
}

//-----------------------------------------------------------------------
// Filer reads directory
//
int filerReadDir(void)
{
	FFBLK ff;
	int fd,i,i2,i3,i4,retry;
	DE *d;
	DE d1,d2;

	//
	retry = 2;

tryagain:
	//---------------------------------------------------------------------------
	//
	filer->n_files = 0;

	//
	fd = findfirst("*", &ff);
	if(fd<=0)
		// Error
		return 1;

	//---------------------------------------------------------------------------
	// Reset database
	//
	memset(&filer->file, 0, sizeof(filer->file));

	//
	filer->n_selected = 0;

	//---------------------------------------------------------------------------
	// Receive all files
	//
	for(i=0,filer->sz_all=0; ; )
	{
		//-----------------------------------------------
		// Get PTR.
		d = &filer->file[i];

		//-----------------------------------------------
		// Get file type.
		d->type = fexist(ff.ff_name);
		// Allow a directory and a file, ignore others.
		if( d->type!=1 && d->type!=2 )
			// This entry is probably deleted?
			goto next;

		//-----------------------------------------------
		// Store name.
		strcpy(d->name, ff.ff_name);

		// Store size.
		d->size = ff.ff_fsize;

		// Increase total size.
		filer->sz_all += d->size;

		// Next entry.
		i++;

		//---------------------------------------
next:
		// Receive next.
		if( findnext(fd, &ff) )
			// End of directory.
			break;
	}

	//
	filer->n_files = i;

	//---------------------------------------------------------------------------
	// Sort files. Move directories on top of the list.
	//
	for(i=0; i<filer->n_files; i++)
	{
		//
		for(i2=0; i2<(filer->n_files-1); i2++)
		{
			//
			if(filer->file[i2+1].type==2
				&& filer->file[i2+0].type!=2)
			{
				// Is directory, move up.
				memcpy(&d1, &filer->file[i2+0], sizeof(DE));
				memcpy(&d2, &filer->file[i2+1], sizeof(DE));
				//
				memcpy(&filer->file[i2+1], &d1, sizeof(DE));
				memcpy(&filer->file[i2+0], &d2, sizeof(DE));
			}
		}
	}

	//
	if(filer->n_files==0)
	{
		// Aha, no files found, so let's just fix this easily
		// by going to root directory and reloading
		chdir("/");
		if( (retry--)==0 )
			return 1;
		goto tryagain;
	}

	// OK
	return 0;
}

//-----------------------------------------------------------------------
// Filer update view
//
void filerUpdateView(void)
{
	int x,y,i,i2,i3,i4,cy,yy;
	DE *de;
	char str[20],str2[20],name[100];

	//--------------------------------------------------------------------------
	// 1) Print instructions.
	//

	// Write program name.
	gotoxy(45+2,2);
	textcolor(14); textbackground(0);
	printf("  Squirrel File Manager 1.0  ");

	// Print window border.
	textcolor(7); textbackground(BLACK);
	DrawAnsiWindow(44,1,80,24,
		1, "", FALSE);

	// Print instructions.
	gotoxy(47,6);
	printf("Cursor up/down - scroll curson.");
	gotoxy(47,7);
	printf("Enter - run file or dir.");
	gotoxy(47,8);
	printf("Escape key - exit program.");
	gotoxy(47,9);
	printf("g - go to root directory /.");
	gotoxy(47,10);
	printf("F1 - view manual.");
	//
	gotoxy(47,12);
	printf("%d items on this directory. \n",
		filer->n_files);
	gotoxy(47,13);
	printf("(%dK)    \n",
		filer->sz_all/1024);
	//
	if(filer->n_selected)
	{
		gotoxy(47,14);
		printf("%d items selected.   \n",
			filer->n_selected);
		gotoxy(47,15);
		printf("(%dK)   \n",
			filer->sz_selected/1024);
	}

	//--------------------------------------------------------------------------
	// 2) Print file view.
	//

	//
	textcolor(7); textbackground(1);

	// Cursor Y
	cy = filer->ypos+filer->y;

	//
	gotoxy(1,1);

	//
	for(y=0; y<24; y++)
	{
		//--------------------------------------------------------
		// Skip non-existant entries
		yy = y+filer->ypos;

		//--------------------------------------------------------
		// Get PTR
		de = &filer->file[yy];

		//--------------------------------------------------------
		if(yy==cy)
		{
			// When under cursor:
			if(de->selected)
			{
				textcolor(RED); textbackground(7);
			}
			else
			{
				textcolor(BLACK); textbackground(7);
			}
		}
		else
		{
			// Not under cursor:

			//
			textcolor(7); textbackground(BLUE);

			// Colorize known file types.

			// Executable
			if( strstr(de->name, ".bin") )
				textcolor(LIGHTRED);
			else
			if( strstr(de->name, ".exe") )
				textcolor(LIGHTRED);
			else
			if( strstr(de->name, ".com") )
				textcolor(LIGHTRED);
			else
			if( strstr(de->name, ".cfg") )
				textcolor(LIGHTGREEN);
			else
			if( strstr(de->name, ".txt") )
				textcolor(YELLOW);

			//
			if(de->selected)
				textbackground(RED);
		}

		//--------------------------------------------------------
		// Wipe line
		gotoxy(1, y+1);
		printf("\r                                          \r");

		// OK, we let it draw empty line, let's skip other stuff if necessary:
		if(yy>=filer->n_files)
			// This is empty entry, let's continue.
			continue;

		//--------------------------------------------------------
		// Check Y-location
		if(yy==cy)
		{
			// Currently selected file
			strcpy(str,  "   "); // >>
			strcpy(str2, "   "); // <<
		}
		else
		{
			// Other file
			strcpy(str,  "   ");
			strcpy(str2, "   ");
		}

		// Construct name string according file type.
		switch(de->type)
		{
			// File:
			case 1:
			sprintf(name, "%s", de->name);
			break;

			// Directory:
			case 2:
			sprintf(name, "[%s]", de->name);
			break;

			// Unknown:
			default:
			sprintf(name, "?%s?", de->name);
			break;
		}

		// Print name
		gotoxy(1, y+1);
		printf("%s%s%s", str, name, str2);

		// Print size
		if(de->type==1)
		{
			//
			gotoxy(1+30, y+1);
			if(de->size>=(1024*1024))
				printf("%dM", de->size/(1024*1024));
			else
			if(de->size>1023)
				printf("%dK", de->size/1024);
			else
				printf("%d", de->size);
		}
	}

	//
	gotoxy(1, filer->y+1);
}

// Move cursor up
void CursorUp(void)
{
	//
	if(filer->y)
		filer->y--;
	else
	if(filer->y==0 && filer->ypos!=0)
		filer->ypos--;
}

// Move cursor down
void CursorDown(void)
{
	//
	if( (filer->y+filer->ypos)<(filer->n_files-1) )
	{
		if(filer->y<23)
			filer->y++;
		else
		if(filer->y>=23)
			filer->ypos++;
	}
}

//
void WipeScreen(void)
{
	//
	textcolor(7); textbackground(0); clrscr();
}

// Reset screen contents.
void ResetScreen(void)
{
	//
	filer->y=0; filer->ypos=0;
	WipeScreen();
}

//
void EditThis(const char *fname)
{
	char str[256];

	//
	sprintf(str, "%s %s", EDIT_CMD_PATH, fname);
	cexec( str,"" );
}

//-----------------------------------------------------------------------
// onEnter
//
void onEnter(DE *de)
{
	char str[256],anykey,refresh;

	//
	anykey = FALSE;
	refresh = FALSE;

	//
	switch( fexist(de->name) )
	{
		//----------------------------------------------------------------
		// File: Is it executable? => Run it.
		//
		case 1:

		// Executable, run.
		if( strstr(de->name,".bin") )
		{
			//
			WipeScreen();
			cexec( de->name,"" );
			anykey = TRUE;
			refresh = TRUE;
		}

		// Text file, edit.
		if( strstr(de->name,".txt")
			|| strstr(de->name,".c")
			|| strstr(de->name,".h")
			|| strstr(de->name,".cpp")
			|| strstr(de->name,".doc")
			|| strstr(de->name,".nfo")
			|| strstr(de->name,".diz") )
		{
			//
			WipeScreen();
			textcolor(7); textbackground(BLACK); clrscr();
			EditThis(de->name);
			refresh = TRUE;
		}

		//
		if(anykey)
		{
			printf("Press any key to continue . . ");
			getch();
			printf("\n");
		}
		if(refresh)
		{
			filerReadDir();
			WipeScreen();
		}

		//
		break;

		//----------------------------------------------------------------
		// Directory, chdir to it & reload directory.
		//
		case 2:
		chdir( de->name );
		filerReadDir();
		ResetScreen();
		break;

		//----------------------------------------------------------------
		// Unknown: Do nothing.
		default:
		break;
	}
}

//
void DeleteSelectedFiles(void)
{
	int i;
	DE *d;

	//
	for(i=0; i<filer->n_files; i++)
	{
		//
		d = &filer->file[i];

		//
		if(d->selected)
			remove(d->name);
	}
}

// Asks user a question with Yes or No as only valid answers.
int Question(const char *msgi)
{
	static int key;

	// Print window border.
	textcolor(7); textbackground(0);
	DrawAnsiWindow(44,1,80,24,
		1, "", TRUE);
	gotoxy(45,2);
	printf("%s", msgi);

	//
	key = getch();

	//
	return key;
}

//-----------------------------------------------------------------------
// Run Filer
//
int runFiler(void)
{
	int key,ekey,i,i2,i3,i4;
	DE *de;
	char *txt_del_confirm="Delete files: Are you sure (y/n)?";

	// Read directory
	filerReadDir();

	// Loop
	while(1)
	{
		//-------------------------------------------
		// Assign currently selected file.
		//
		de = &filer->file[filer->y+filer->ypos];

		//-------------------------------------------
		// Update Filer view
		//
		filerUpdateView();

		//-------------------------------------------
		// Get key
		//
		key = getch();

		//-------------------------------------------
		// ESC hit?
		//
		if(key==27)
			// Exit program
			break;

		//-------------------------------------------
		// Insert: Select file.
		//
		if(key==JKEY_INSERT)
		{
			// Select file.
			de->selected ^= 1;
			if(de->selected)
			{
				// SELECT FILE
				filer->n_selected++;
				filer->sz_selected += de->size;
			}
			else
			{
				// DESELECT FILE
				filer->n_selected--;
				filer->sz_selected -= de->size;
			}
			// Move cursor down.
			CursorDown();
			//
			if(filer->n_selected==0)
				WipeScreen();
			continue;
		}

		//-------------------------------------------
		// F1: View help page.
		//
		if(key==JKEY_F1)
		{
			ResetScreen();
			EditThis(FILER_HLP_PATH);
			filerReadDir();
			ResetScreen();
			continue;
		}

		//-------------------------------------------
		// F8: Delete selected files.
		//
		if(key==JKEY_F8 && filer->n_selected)
		{
			// Ask user.
			if( Question(txt_del_confirm)=='y' )
			{
				DeleteSelectedFiles();
				filerReadDir();
			}
			ResetScreen();
			continue;
		}

		//-------------------------------------------
		// Go to root directory 'g'.
		//
		if(key=='g')
		{
			//
			chdir("/");
			filerReadDir();
			ResetScreen();
			continue;
		}

		//-------------------------------------------
		// Enter hit?
		//
		if(key=='\n')
		{
			//
			onEnter(de);
			continue;
		}

		//-------------------------------------------
                // Get extra keys
		//
                if(key>=0xFF00)
			ekey = key&255;
		else
			ekey = 0;

		//-------------------------------------------
		if(ekey==72)
			CursorUp();
		if(ekey==80)
			CursorDown();
		//
		if(key==JKEY_PAGE_UP)
		{
			for(i=0; i<8; i++)
				CursorUp();
		}
		//
		if(key==JKEY_PAGE_DOWN)
		{
			for(i=0; i<8; i++)
				CursorDown();
		}
	}

	//
	return 0;
}

//-----------------------------------------------------------------------
// End Filer
//
int endFiler(void)
{
	//
	textcolor(7); textbackground(BLACK);
	clrscr();
	return 0;
}

//-----------------------------------------------------------------------
// Main function
//
int main(int argc, char **argv)
{
	//
	initFiler();
	ResetScreen();
	runFiler();
	endFiler();
	return 0;
}
