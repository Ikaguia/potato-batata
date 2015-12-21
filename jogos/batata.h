#ifndef BATATA_HEADER
#define BATATA_HEADER

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>


#define CLEARBUFFER while(getchar()!='\n')
#define CLEARBUFFER2 while(kbhit())getch()
#define INIT_RAND srand(time(NULL))
#define RAND(x) (rand()%x)
#define TRUE 1
#define FALSE 0

int usleep(int);/* To avoid warnings on compilation */

#define STR(A) #A
#define STR_(A) STR(A)
#define STR2(A,B) STR_(A) STR_(B)
#define STR2_(A,B) STR2(A,B)

#define COLOR_BLACK    0
#define COLOR_RED      1
#define COLOR_GREEN    2
#define COLOR_YELLOW   3
#define COLOR_BLUE     4
#define COLOR_MAGENTA  5
#define COLOR_CYAN     6
#define COLOR_WHITE    7
#define COLOR_DEF      9
#define COLOR_BBLACK   60
#define COLOR_BRED     61
#define COLOR_BGREEN   62
#define COLOR_BYELLOW  63
#define COLOR_BBLUE    64
#define COLOR_BMAGENTA 65
#define COLOR_BCYAN    66
#define COLOR_BWHITE   67
#define COLOR_DEFAULT -1
#define _M m
#define SUM(x,y) y##x


#define ESCC_SGR_(x)    "\x1b["STR(x)"m"
#define ESCC_SGR(x)     ESCC_SGR_(x)
#define ESCC_DEF        ESCC_SGR(0)          //default
#define ESCC_BOLD       ESCC_SGR(1)          //bold
#define ESCC_UNDER_     ESCC_SGR(4)          //underline
#define ESCC_BLINK      ESCC_SGR(5)          //blink
#define ESCC_INVERTED   ESCC_SGR(7)          //inverted
#define ESCC_N_INVERTED ESCC_SGR(27)         //inverted
#define ESCC_FONT(x)    ESCC_SGR(SUM(x,1))   //change font     ( 10- 19 default=10)
#define ESCC_FG_C(x)    ESCC_SGR(SUM(x,3))   //fg color        ( 30- 37 default=39)
#define ESCC_BG_C(x)    ESCC_SGR(SUM(x,4))   //bg color        ( 40- 47 default=49)
#define ESCC_FG_CB(x)   ESCC_SGR(SUM(x,9))   //fg color bright ( 90- 97 default=39)
#define ESCC_BG_CB(x)   ESCC_SGR(SUM(x,10))  //bg color bright (100-107 default=49)

#define ESCC_HIDE_C     "\x1b[?25l"          //hide cursor
#define ESCC_SHOW_C     "\x1b[?25h"          //show cursor


#define SCREEN_DEF_X    100
#define SCREEN_DEF_Y    100

#define KEY_TAB			9	//'\t'
#define KEY_ENTER		10	//'\n'
#define KEY_ESC         27	//'\x1b['
#define KEY_BCKSP       127
#define KEY_UP          500
#define KEY_LEFT        501
#define KEY_DOWN        502
#define KEY_RIGHT       503
#define KEY_DEL         504
#define KEY_F1          511
#define KEY_F2          512
#define KEY_F3          513
#define KEY_F4          514
#define KEY_F5          515
#define KEY_F6          516
#define KEY_F7          517
#define KEY_F8          518
#define KEY_F9          519
#define KEY_F10         520
#define KEY_F11         521
#define KEY_F12         522

#define KEYSTR_BCKSP	"\x1b["
#define KEYSTR_UP       "\x1b[A"
#define KEYSTR_LEFT     "\x1b[D"
#define KEYSTR_DOWN     "\x1b[B"
#define KEYSTR_RIGHT    "\x1b[C"
#define KEYSTR_DEL      "\x1b[3~"
#define KEYSTR_F1       "\x1bOQ"
#define KEYSTR_F2       "\x1bOQ"
#define KEYSTR_F3       "\x1bOR"
#define KEYSTR_F4       "\x1bOS"
#define KEYSTR_F5       "\x1b[15"
#define KEYSTR_F6       "\x1b[17"
#define KEYSTR_F7       "\x1b[18"
#define KEYSTR_F8       "\x1b[19"
#define KEYSTR_F9       "\x1b[20"
#define KEYSTR_F10      "\x1b[21"
#define KEYSTR_F11      "\x1b[21"
#define KEYSTR_F12      "\x1b[24"

#define MENU_TYPE_BOX     1
#define MENU_TYPE_BUTTON  2
#define MENU_TYPE_SLIDER  3
#define MENU_TYPE_NUMBOX  4
#define MENU_TYPE_CHARBOX 5
#define MENU_TYPE_TEXT    6
#define MENU_TYPE_H_LINE  7

struct coords_{
	int x;
	int y;
};

typedef struct coords_ coords;

typedef struct screen_coord_ screen_coord;

typedef struct screen_ screen;

struct screen_coord_{
	char val[10];
	wchar_t uStr[10];
	char unicode;
	char FGcolor;
	char BGcolor;
	char font;
	char bold;
	char blink;
	char underline;
	char inverted;
};

struct screen_{
	int larg;
	int alt;
	coords curPos;
	screen_coord cur;
	screen_coord def;
	screen_coord** vect;
};

typedef struct menu_opt_ menu_opt;

typedef struct menu_ menu;

struct menu_opt_{
	int id;
	int type;
	char str[121];
	int max;
	int min;
	int val;
	struct menu_opt_ *next;
	struct menu_opt_ *prev;
	int hidden;
};

typedef void (*FUNC_PTR)(int, int, int*, char[8]);

int kbhit(void);

int getch(void);

void setEcho(int);

int absolute(int);

int max(int,int);

int min(int,int);

int findXYinArray(int*,int*,int*,int,int,int);
 
int getIntLen(int);
 
void swap(int*,int*);

int toUpperCase(int);

int toLowerCase(int);

void underLineToSpace(char*);

void clear(void);

int readIntInRange(int,int);

int read2IntsInRange(int*,int*,int,int,int,int);

char readCharNotEnter(void);

char readChNotEnter(void);

int readDirArrows(void);

int rand2(int);

screen* screenAlloc(int,int);

screen* resetScreen(screen*);

void resetScreenCoord(screen_coord*);

void printScreen(screen*);

void printStrToScreen(screen*,char*);

void printStrToScreenPos(screen*,char*,int,int);

void printCharToScreen(screen*,char*);

void printCharToScreenPos(screen*,char*,int,int);

//lagura campo,altura campo,largura bloco,altura bloco,  |-  ,  -| , |_  ,  _| ,  |  ,  -  ,  -| , |-  , -|- , _|_ ,  +
void printFieldToScreen(screen*,int,int,int,int,int,int,char*,char*,char*,char*,char*,char*,char*,char*,char*,char*,char*);

void screenMove(screen*,int,int);

int screenMoveDirKey(screen*,int);

void screenCpy(screen*,screen*);

int screenReadIntInRange(screen*,int,int,int,int,int,int);

void closeScreen(screen* scr);

int readKey(void);

char* keyToStr(int,char*);

char isLeft(int x);

char isRight(int x);

char isUp(int x);

char isDown(int x);

menu_opt* insereMenuOpt(menu_opt*,menu_opt*,int);//primeiro menu,menu a inserir,escondido(0,1)

menu_opt* menuOptionBox(char*,int*,int);//texto,id,valor inicial(0,1)

menu_opt* menuOptionButton(char*,int*);//texto,id

menu_opt* menuOptionSlider(char*,int*,int,int,int);//texto,id,valor minimo,valor maximo,valor inicial

menu_opt* menuOptionNumBox(char*,int*,int,int,int);//texto,id,valor minimo,valor maximo,valor inicial

menu_opt* menuOptionCharBox(char*,int*,int);//texto,id,valor inicial

menu_opt* menuOptionText(char*,int*);//texto,id

menu_opt* menuOptionLine(char*,int*);//texto,id


void menuToggleHidden(menu_opt*,int);

char menuMove1(menu_opt*,screen*,int,int*);//menu selecionado,screen,tecla,atual

char menuMove(menu_opt*,screen*,int,int*);//primeiro menu,screen,tecla,atual

screen* menuToScreen(screen*,screen*,menu_opt*,int,int);//screen original,screen destino,primeiro menu,coordenada x,coordenada y

void freeMenu(menu_opt*);

int fCheck(char*,int);//filaname,(0/1) for (.txt/.bin)

#endif