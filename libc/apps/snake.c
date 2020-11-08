// ============================================================
// Ported to JTMOS by Jari Tuominen(jari@vunet.org)
// Original snake game written by Shmoove.
// (C) 2002-2003 by Jari Tuominen.
// ============================================================
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <time.h>


#ifndef DJGPP
#ifndef JTMOS
// Assume Borland C++ compiler.
#define BORLAND
//#error "haloo"
#endif
#endif

#ifdef BORLAND
//
#define random rand
#include <windows.h>
//
#define delay(x) Sleep(x*5);
#endif

//
typedef struct
{
	int key_up;
	int key_down;
	int key_left;
	int key_right;
}SNAKE;
SNAKE snake;

//
#define fixN /16

//
#define BOOL unsigned char
#define FALSE 0
#define TRUE 1

// enums
#define RESULT char
#define DIE 0
#define LIVE 1
#define GROW 2

//
#define GAMEACTION char
#define DIRECTION char
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define END 4
#define PAUSE 5

//
#define CELLVALUE char
#define EMPTY ' '
#define SNAKE 'O'
#define DEAD_SNAKE 'X'
#define FOOD '$'

//
#define LEVEL short int
#define EASY 350 fixN
#define MEDIUM 200 fixN
#define HARD 150 fixN

// constants
#define H 20
#define W 40

//
#define BOARDX 15
#define BOARDY 3
//
#define SCOREX 45
#define SCOREY 1
//
#define MESSAGEX 22
#define MESSAGEY 12
//
#define TITLE_DELAY 70 fixN

// macroes
#define VALID(p)	if (!(p)) \
					{\
						printf("Allocation failed!");\
						exit(1);\
					}

// cell structure
typedef struct {
	CELLVALUE 	value;			// value of cell
	DIRECTION 	dir; 			// direction (in case value==SNAKE)
} CELL;

// board structure
typedef struct {
	short int 		head[2];		// coordinates of snakes head
	short int 		tail[2];		// coordinates of snakes tail
	CELL 		board[W][H];	// board matrix
} BOARD;

// game functions
void InitBoard(BOARD *board,DIRECTION dir); // initialize board
void SetHead(BOARD *board,short int x,short int y); // set the snake's head
void SetTail(BOARD *board,short int x,short int y); // set the snake's tail
RESULT Move(DIRECTION dir,BOARD *board); // move snake
void PutFood(BOARD *board); // put new food on board

// file functions
void LoadHighScore(short int *high_scores,char *path);
void SaveHighScore(short int *high_scores,char *path);
void SetHighScore(short int *high_scores,short int score);
char *GetPath1(char *filename);

// interface functions
GAMEACTION GetInput(DIRECTION current_dir,LEVEL level); // get user input
void SetCell(CELLVALUE output,
		short int x,short int y,
		int color); 
// paint cell in board
void SetScore(short int score); // paint score
void PrintBoard(BOARD board); // print entire board
void PrintFrame(short int *high_scores); // print board frame
void PauseGame(); // pause the game
BOOL EndGame(short int score,short int high_score); // end the game
LEVEL GetLevel(char *choice); // get level choice
void FlushInput(void); // empty keyboard buffer
void Title(); // title screen
//
int tikki=0;

#ifndef BORLAND
//
void randomize(void)
{
	//
}
#endif

//
void gameInit(void)
{
	//
#ifdef JTMOS
	snake.key_up = JKEY_CURSOR_UP;
	snake.key_down = JKEY_CURSOR_DOWN;
	snake.key_left = JKEY_CURSOR_LEFT;
	snake.key_right = JKEY_CURSOR_RIGHT;
#endif

	//
#ifdef MSDOS
	snake.key_up = 'q';
	snake.key_down = 'a';
	snake.key_left = 'o';
	snake.key_right = 'p';
#endif

	//
#ifdef BORLAND
	snake.key_up = 'q';
	snake.key_down = 'a';
	snake.key_left = 'o';
	snake.key_right = 'p';
#endif

	//
	randomize();
}

// Game main function.
void main(short int argc,char *argv[])
{
	BOARD board; // main playing surface
	short int score=0;
	short int high_scores[5]={0};
	LEVEL level;
	GAMEACTION action;
	RESULT res;
	BOOL end;
	DIRECTION current_dir; // current snake direction
	char choice=0,*path; // initial level = EASY, path to executable

	//_setcursortype(_NOCURSOR);
//	path=GetPath1(argv[0]);

	//LoadHighScore(high_scores,path);
	gameInit();
	clrscr(); Title();

	// main loop
	do
	{
		clrscr();
		gotoxy(BOARDX,BOARDY);
		//printf("Last Score : %d    High Score : %d  ",
		//	score,high_scores[0]);
		level=GetLevel(&choice);
		// initialize
		score=0;
		current_dir=RIGHT;
		InitBoard(&board,current_dir);
		// print empty board
		PrintFrame(high_scores);
		PrintBoard(board);
		SetScore(0);
		// game loop
		do
		{
			res=Move(current_dir,&board); // move the snake in current direction
			if (res==GROW)
				// we just ate so increase score
				SetScore(++score);
			// get user input
			action = GetInput(current_dir, level);

			//
			switch(action)
			{
				// arrow pressed -> change current direction
				case UP:
				case DOWN:
				case LEFT:
				case RIGHT:
					current_dir=action;
					break;
				// [P] pressed -> pause game
				case PAUSE:
					PauseGame();
					// print board again
					//PrintFrame(high_scores);
					PrintBoard(board);
					break;
			}
		}
		// loop until [Q] is pressed or snake crashed
		while (action!=END&&res!=DIE);
		if (score>high_scores[4])
		{
			//SetHighScore(high_scores,score);
			//SaveHighScore(high_scores,path);
		}
		FlushInput();
		end=EndGame(score,high_scores[0]); // play another game?
	}
	// loop until user picks not to start a new game
	while (!end);
	clrscr();
	printf("\n\n\n\n\n\n");
	printf("                Thanks for playing GREEDY SNAKE.\n\n");
	printf("    Originally written by Shmoove.\n");
	printf("    Ported to JTMOS and Windows + improvements by Jari Tuominen.\n\n\n");
	printf("For more info : shmoove@mail.com\n\n\n\n\n\n");
	//_setcursortype(_NORMALCURSOR);
}

// Initialize playing board,
// dir is the initial direction.
void InitBoard(BOARD *board,DIRECTION dir)
{
	short int i,j;
	// empty all cells
	for (i=0;i<W;i++)
		for (j=0;j<H;j++)
			board->board[i][j].value=EMPTY;
	// set initial snake
	SetHead(board,7,0);
	SetTail(board,0,0);
	for (i=0;i<7;i++)
	{
		board->board[i][0].value=SNAKE;
		board->board[i][0].dir=dir;
	}
	// put first food on board
	PutFood(board);
}

// Set head of snake at (x,y).
void SetHead(BOARD *board,short int x,short int y)
{
	board->head[0]=x;
	board->head[1]=y;
	board->board[x][y].value=SNAKE;
}

// Set tail of snake as (x,y).
void SetTail(BOARD *board,short int x,short int y)
{
	board->tail[0]=x;
	board->tail[1]=y;
	board->board[x][y].value=SNAKE;
}

// Move snake
// Dir is the direction to move the snake.
RESULT Move(DIRECTION dir,BOARD *board)
{
	CELLVALUE new_head;
	RESULT result=LIVE; // default result
	// get old head and tail coordinates
	short int		old_head_x=board->head[0],
			old_head_y=board->head[1],
			old_tail_x=board->tail[0],
			old_tail_y=board->tail[1];
	short int 	new_head_x=old_head_x,
			new_head_y=old_head_y,
			new_tail_x=old_tail_x,
			new_tail_y=old_tail_y;

	// calculate new head values
	switch (dir)
	{
	case UP:
		new_head_y--;
		break;
	case DOWN:
		new_head_y++;
		break;
	case LEFT:
		new_head_x--;
		break;
	case RIGHT:
		new_head_x++;
	}

	// calculate new tail values
	switch (board->board[old_tail_x][old_tail_y].dir)
	{
	case UP:
		new_tail_y--;
		break;
	case DOWN:
		new_tail_y++;
		break;
	case LEFT:
		new_tail_x--;
		break;
	case RIGHT:
		new_tail_x++;
		break;
	}

	// check for crashes
	new_head=board->board[new_head_x][new_head_y].value;
	if (new_head_x<0||new_head_x>W-1||
		new_head_y<0||new_head_y>H-1 // out of bounds
		||new_head==SNAKE) // crashed with itself
		result=DIE; // game over

	// check for food
	if (new_head==FOOD) // found food
		result=GROW;

	// set new head
	SetHead(board,new_head_x,new_head_y);
	// set direction for old head
	board->board[old_head_x][old_head_y].dir=dir;
	// output new head to screen
	if (result!=DIE)
		SetCell(SNAKE,new_head_x,new_head_y, 7);
	else
		SetCell(DEAD_SNAKE,new_head_x,new_head_y, BLUE);
	if (result!=GROW) // clear old tail  and set new one
	{
		board->board[old_tail_x][old_tail_y].value=EMPTY;
		SetCell(EMPTY,old_tail_x,old_tail_y, 7);
		SetTail(board,new_tail_x,new_tail_y);
	}
	else // put new food on board
		PutFood(board);
	return result;
}

// Puts new food in random location of board.
void PutFood(BOARD *board)
{
	short int x,y;

	// get random empty location
	do
	{
		x=random()%W;
		y=random()%H;
	}
	while (!(board->board[x][y].value==EMPTY));
	// set food in board matrix
	board->board[x][y].value=FOOD;
	// print the food on the screen
	SetCell(FOOD,x,y, GREEN);
}

//
#ifdef DJGPP
int getch1(void)
{
	if(bioskey(1)==0)
		return 0;
	else
		return getch();
}
#endif

#ifdef BORLAND
int getch1(void)
{
	if(kbhit())
		return getch();
	else
		return 0;
}
#endif

// Gets user input.
GAMEACTION GetInput(DIRECTION current_dir,LEVEL level)
{
	int input,dir_input;
	int key;

	//fflush(stdin); // clear input buffer
	//FlushInput();
#ifdef JTMOS
	delay(level); // wait
#endif

#ifdef DJGPP
	delay(level*10000); // wait
#endif

#ifdef BORLAND
	delay(level); // wait
#endif
	if ( (key=getch1())<=0 )
		// no keys have been pressed so return current direction
		return current_dir;
	// else get key pressed
	input=key;
	if(input==snake.key_up)
		return UP;
	if(input==snake.key_down)
		return DOWN;
	if(input==snake.key_left)
		return LEFT;
	if(input==snake.key_right)
		return RIGHT;
	switch(input)
	{
		case ' ':
			return PAUSE;
		case 27:
			return END;
		default:
			break;
	}
	// invalid input -> return current direction
	return current_dir;
}

// Sets on-screen cell to value in output at (x,y).
void SetCell(CELLVALUE output,short int x,short int y, int color)
{
	//textcolor(color);
	gotoxy(BOARDX+x,BOARDY+y);
	putch(output);
}

// Prints score.
void SetScore(short int score)
{
	gotoxy(SCOREX,SCOREY);
	printf("Score - %d",score);
}

// Prints entire board (in case it has been hidden, like after pause).
void PrintBoard(BOARD board)
{
	short int x,y;
	for (x=0;x<W;x++)
		for (y=0;y<H;y++)
		{
			gotoxy(BOARDX+x,BOARDY+y);
			putch(board.board[x][y].value);
		}
}

// Prints board frame and instructions.
void PrintFrame(short int *high_scores)
{
	short int i;
	gotoxy(BOARDX,SCOREY);
	printf("GREEDY SNAKE");
	gotoxy(BOARDX-1,BOARDY-1);
	putch(201);
	gotoxy(BOARDX+W,BOARDY-1);
	putch(187);
	gotoxy(BOARDX-1,BOARDY+H);
	putch(200);
	gotoxy(BOARDX+W,BOARDY+H);
	putch(188);
	for (i=0;i<W;i++)
	{
		gotoxy(BOARDX+i,BOARDY-1);
		putch(205);
		gotoxy(BOARDX+i,BOARDY+H);
		putch(205);
	}
	for (i=0;i<H;i++)
	{
		gotoxy(BOARDX-1,BOARDY+i);
		putch(186);
		gotoxy(BOARDX+W,BOARDY+i);
		putch(186);
	}
	gotoxy(BOARDX+W+2,BOARDY);
	printf("A,Q, O,P  to move");
	gotoxy(BOARDX+W+2,BOARDY+2);
	printf("[Space] to pause");
	gotoxy(BOARDX+W+2,BOARDY+4);
	printf("[ESC] to quit");
	gotoxy(BOARDX+W+2,BOARDY+6);
	/*printf("HIGH SCORES :");
	for (i=0;i<5;i++)
	{
		gotoxy(BOARDX+W+2,BOARDY+6+(i+1)*2);
		printf("%8d",high_scores[i]);
	}*/
}

// Pauses the game.
void PauseGame()
{
	// print pause message
	gotoxy(MESSAGEX,MESSAGEY);
	printf("Game paused. Press any key...");
	while (!getch()); // wait for any key
	// erase pause message
	gotoxy(MESSAGEX,MESSAGEY);
	printf("                             ");
	fflush(stdin); // flush input buffer
}

// Ends the game and asks the user if he wants to play another.
// (!!returns TRUE if no!!)
BOOL EndGame(short int score,short int high_score)
{
	char answer;
	while (TRUE)
	{
		gotoxy(MESSAGEX,MESSAGEY);
		printf("Game over. Play again(y/n)?");
		gotoxy(MESSAGEX-3,MESSAGEY+2);
		/*printf("Score : %5d   High Score : %5d",
			score,high_score);*/
		answer=getch();
		if (answer=='n'||answer=='N')
			return TRUE;
		else if (answer=='y'||answer=='Y')
			return FALSE;
	}
}

// Gets level
// Parameter choice is the choice in the last game (default choice).
LEVEL GetLevel(char *choice)
{
	int key;

	//
	textcolor(YELLOW); textbackground(BLACK);
	gotoxy(MESSAGEX,MESSAGEY-5);
	printf("G R E E D Y   S N A K E");
	gotoxy(MESSAGEX,MESSAGEY-3);
#ifdef DJGPP
	printf("Use keys q/a,o/p, and press space.");
#endif

#ifdef BORLAND
	printf("Use keys q/a,o/p, and press space.");
#endif

#ifdef JTMOS
	printf("Use cursor keys, and press space.");
#endif
	textcolor(7); textbackground(BLACK);
	gotoxy(MESSAGEX,MESSAGEY);
	printf("Choose a level : (ESC to quit)");
	gotoxy(MESSAGEX+8,MESSAGEY+2);
	printf("Slow");
	gotoxy(MESSAGEX+8,MESSAGEY+3);
	printf("Medium");
	gotoxy(MESSAGEX+8,MESSAGEY+4);
	printf("Fast");

	//
	do
	{
		// print arrow according to choice
		gotoxy(MESSAGEX+4,MESSAGEY+2+(*choice));
		printf("->");
		key = getch(); // get input

		// erase old arrow
		gotoxy(MESSAGEX+4,MESSAGEY+2+(*choice));
		printf("  ");
		// update choice according to arrow pressed
		if(key==27)
		{
			clrscr();
			exit(0);
		}
		if (key==snake.key_up) // up arrow
			(*choice)--;
		if (key==snake.key_down) // down arrow
			(*choice)++;
		// normalize choice to (0-2) range
		if ((*choice)==-1)
			(*choice)=2;
		if ((*choice)==3)
			(*choice)=0;

	}while (key!=32); // loop until [SPACE] is pressed

	// return appropiate value according to choice
	switch (*choice)
	{
	case 0:
		return EASY;
	case 1:
		return MEDIUM;
	case 2:
		return HARD;
	}
	return MEDIUM;
}

// Empties keyboard buffer.
void FlushInput(void)
{
	// as long as there are keyboard hits get them with getch()
#ifdef JTMOS
	while( getch1()>0 ) { SwitchToThread(); }
#endif
#ifdef DJGPP
	while(getch1());
#endif
#ifdef BORLAND
	while(getch1());
#endif
}

// Loads high score from file and returns it.
void LoadHighScore(short int *high_scores,char *path)
{
	FILE *fscore;
	char *filename;
	short int l;
	l=strlen(path)+13;
	filename=(char *)malloc(sizeof(char)*l);
	VALID (filename);
	strcpy(filename,path);
	strcat(filename,"\\snkhiscr.dat");
	// get high score from file if it exists
	fscore=fopen(filename,"rb");
	if (fscore)
	{
		fread(high_scores,sizeof(short int),5,fscore);
		fclose(fscore);
	}
}

// Saves high score to file.
void SaveHighScore(short int *high_scores,char *path)
{
	FILE *fscore;
	char *filename;
	short int l;
	l=strlen(path)+13;
	filename=(char *)malloc(sizeof(char)*l);
	VALID (filename);
	strcpy(filename,path);
	strcat(filename,"\\snkhiscr.dat");
	fscore=fopen(filename,"wb");
	if (fscore)
	{
		fwrite(high_scores,sizeof(short int),5,fscore);
		fclose(fscore);
	}
}

// title screen
void Title()
{
	short int i,j;
	char title[]="G R E E D Y   S N A K E";
	j=strlen(title)-1;
	for (i=80;i>80-strlen(title);i--)
	{
		gotoxy(i,MESSAGEY-5);
		putch(SNAKE);
		delay(TITLE_DELAY);
	}
	for (i=80-strlen(title);i>0;i--)
	{
		gotoxy(i,MESSAGEY-5);
		putch(SNAKE);
		gotoxy(i+strlen(title),MESSAGEY-5);
		if (j>0&&i<MESSAGEX)
			putch(title[j--]);
		else
			putch(EMPTY);
		delay(TITLE_DELAY);
	}
	for (i=strlen(title);i>0;i--)
	{
		gotoxy(i,MESSAGEY-5);
		if (j>=0)
			putch(title[j--]);
		else
			putch(EMPTY);
		delay(TITLE_DELAY);
	}
}

#ifndef BORLAND
char *GetPath1(char *filename)
{
	short int i;
	char *path;
	for (i=strlen(filename)-1;filename[i]!='\\'&&i>0;i--);
	path=(char *)malloc(sizeof(char)*i);
	VALID(path);
	path[i--]=NULL;
	for (;i>=0;i--)
		path[i]=filename[i];
	return path;
}
#endif

void SetHighScore(short int *high_scores,short int score)
{
	short int i=0,j=0,new_scores[5];
	while (high_scores[i]>score&&i<5)
		new_scores[j++]=high_scores[i++];
	new_scores[j++]=score;
	while (j<5)
		new_scores[j++]=high_scores[i++];
	for (i=0;i<5;i++)
		high_scores[i]=new_scores[i];
}
