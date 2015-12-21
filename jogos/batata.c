#include "batata.h"


int kbhit(void){
	struct termios oldt, newt;
	int ch, oldf;
	tcgetattr(STDIN_FILENO,&oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if(ch != EOF){
		ungetc(ch,stdin);
		return 1;
	}
	return 0;
}

int getch(void){
	int ch;
	struct termios oldt;
	struct termios newt;
	tcgetattr(STDIN_FILENO,&oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}

void setEcho(int x){
	struct termios t;
	tcgetattr(STDIN_FILENO,&t);
	t.c_lflag &= (x)?ECHO:~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void readStrNoEnter(char* str,int maxSize){
	memset(str,0,maxSize*sizeof(char));
	for(int i=0;i<maxSize-1;i++){
		str[i]=getch();
		if(str[i]=='\x1b')i--;
		else if(str[i]!='\n')printf("%c",str[i]);
		else{
			str[i]='\0';
			break;
		}
	}
}

int absolute(int x){
	return (x>0)?x:-x;
}

int max(int x,int y){
	return (x>y)?x:y;
}

int min(int x,int y){
	return (x<y)?x:y;
}

int potency(int x,int y){
	int n=1;
	while(y>0){
		n*=x;
		y--;
	}
	return n;
}

int strToInt(char* str){
	int x=0,y=-1;
	for(int i=0;str[i];i++){
		if(str[i]>='0' && str[i]<='9'){
			y=0;
			x*=10;
			x+=(str[i]-'0');
		}
	}
	return x+y;
}

int findXYinArray(int* x,int* y,int* array,int n,int lar,int alt){
	int i=0;
	while(i<lar*alt){
		if(array[i]==n){
		*x = i%lar;
			*y = i/lar;
			return 1;
		}
		i++;
	}
	return 0;
}

int getIntLen(int x){
	int len=0;
	x = absolute(x);
	while(x>0){
		x /= 10;
		len++;
	}
	return(len)?len:1;
}

void swap(int* a, int* b){
	int aux = *a;
	*a = *b;
	*b = aux;
}

int toUpperCase(int x){
	if(x>='a' && x<= 'z'){
		x -= 'a';
		x += 'A';
	}
	return x;
}

int toLowerCase(int x){
	if(x>='A' && x<= 'Z'){
		x -= 'A';
		x += 'a';
	}
	return x;
}

void underLineToSpace(char* str){
	for(int i=0;str[i];i++)if(str[i]=='_')str[i]=' ';
}

void clear(void){
	printf("\x1b[2J\x1b[H");
	//system("clear || cls");
	/*puts("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");*/
}

int readIntInRange(int a, int b){
	int x=a-1;/*arbitrary number*/
	if(a<b){
		scanf("%d",&x);
		CLEARBUFFER;
		while(x<a || x>b){
			printf("ERRO: Entrada invalida. Digite um numero valido x[%d,%d]\n",a,b);
			printf("» ");
			scanf("%d",&x);
			CLEARBUFFER;
		}
	}
	else{
		CLEARBUFFER;
	}
	return x;
}

int read2IntsInRange(int* x,int* y,int x1,int x2,int y1,int y2){
	int xx=x1-1, yy=y1-1;/*arbitrary numbers*/
	scanf(" %d %d",&xx,&yy);
	CLEARBUFFER;
	while(xx<x1 || xx>x2 || yy<y1 || yy>y2){
		printf("Digite numeros validos x[%d,%d], y[%d,%d]\n",x1,x2,y1,y2);
		scanf(" %d %d",&xx,&yy);
		printf("» ");
		CLEARBUFFER;
	}
	*x = xx;
	*y = yy;
	return 0;
}

char readCharNotEnter(void){
	char c;
	do{
		c = getchar();
	}while(c=='\n');
	CLEARBUFFER;
	return c;
}

char readChNotEnter(void){
	char c;
	do{
		c = getch();
	}while(c=='\n');
	return c;
}

int readDirArrows(void){
	char c = getch();
	if(c==KEY_ESC){
		if(!kbhit()){
			return 5;//only ESC
		}
		c=getch();
		if(c!='['){
			ungetc(c,stdin);
			ungetc(KEY_ESC,stdin);
			return 0;
		}
		c=getch();
		if     (c=='A')return 1;//W
		else if(c=='D')return 2;//A
		else if(c=='B')return 3;//S
		else if(c=='C')return 4;//D
		else{
			ungetc(c,stdin);
			ungetc('[',stdin);
			ungetc(KEY_ESC,stdin);
			return 0;
		}
	}
	else if (c=='\n')return 6;
	ungetc(c,stdin);
	return 0;
};

int rand2(int x){
	int n;
	n = (rand()%6)+1;
	while(n==(7-x)){
		n = (rand()%6)+1;
	}
	return n;
}

screen* screenAlloc(int x,int y){
	if(x==-1)x=SCREEN_DEF_X;
	if(y==-1)y=SCREEN_DEF_Y;
	screen* scr = (screen*) malloc(sizeof(screen));
	if(!scr){
		printf("Erro ao alocar memoria para tela.\n");
		getchar();
		return NULL;
	}
	memset(scr,0,sizeof(screen));
	scr->larg=x;
	scr->alt=y;
	scr->vect=(screen_coord**)malloc(x*sizeof(screen_coord*));
	if(!scr->vect){
		printf("Erro ao alocar memoria para tela.\n");
		free(scr);
		getchar();
		return NULL;
	}
	memset(scr->vect,0,sizeof(x*sizeof(screen_coord**)));
	for(int i=0;i<x;i++){
		scr->vect[i]=(screen_coord*)malloc(y*sizeof(screen_coord));
		if(!scr->vect[i]){
			printf("Erro ao alocar memoria para tela.\n");
			for(int j=0;j<i;j++)free(scr->vect[j]);
			free(scr->vect);
			free(scr);
			getchar();
			return NULL;
		}
	}
	return resetScreen(scr);
};

screen* resetScreen(screen* scr){
	resetScreenCoord(&(scr->cur));
	resetScreenCoord(&(scr->def));
	scr->def.FGcolor=COLOR_DEF;
	scr->def.BGcolor=COLOR_DEF;
	for(int i=0;i<scr->larg;i++){
		for(int j=0;j<scr->alt;j++){
			resetScreenCoord(&(scr->vect[i][j]));
		}
	}
	return scr;
}

void resetScreenCoord(screen_coord *scr_c){
	memset(scr_c,0,sizeof(screen_coord));
	scr_c->val[0]=' ';
	scr_c->FGcolor = COLOR_DEFAULT;
	scr_c->BGcolor = COLOR_DEFAULT;
}

void printScreen(screen* scr){
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	screen_coord cur,cur2;
	printf(ESCC_DEF);
	printf("\x1b[%dm",scr->def.FGcolor+30);
	printf("\x1b[%dm",scr->def.BGcolor+40);
	printf("╔");for(int x=0;x<scr->larg && x<w.ws_col-2;x++)printf("═");printf("╗\n");
	for(int y=0;y<scr->alt && y<w.ws_row-2;y++){
		printf(ESCC_DEF);
		printf("\x1b[%dm",scr->def.FGcolor+30);
		printf("\x1b[%dm",scr->def.BGcolor+40);
		cur2 = scr->def;
		for(int x=0;x<=scr->larg && x<=w.ws_col-2;x++)printf(" ");
		printf("║\r║");
		for(int x=0;x<scr->larg && x<w.ws_col-2;x++){
			cur=scr->vect[x][y];
			if(cur.FGcolor == COLOR_DEFAULT){
				if(cur2.FGcolor!=scr->def.FGcolor){
					cur2.FGcolor = scr->def.FGcolor;
					printf("\x1b[%dm",scr->def.FGcolor+30);
				}
			}
			else if(cur2.FGcolor != cur.FGcolor){
				cur2.FGcolor = cur.FGcolor;
				printf("\x1b[%dm",cur.FGcolor+30);
			}
			if(cur.BGcolor == COLOR_DEFAULT){
				if(cur2.BGcolor!=scr->def.BGcolor){
					cur2.BGcolor = scr->def.BGcolor;
					printf("\x1b[%dm",scr->def.BGcolor+40);
				}
			}
			else if(cur2.BGcolor != cur.BGcolor){
				cur2.BGcolor = cur.BGcolor;
				printf("\x1b[%dm",cur.BGcolor+40);
			}
			if(cur2.font != cur.font){
				cur2.font = cur.font;
				printf("\x1b[%dm",cur.font+10);
			}
			if(cur2.bold && !cur.bold){//normal
				cur2.bold = cur.bold;
				printf(ESCC_SGR(22));
			}
			if(cur2.bold!=1 && cur.bold==1){//bold
				cur2.bold = cur.bold;
				printf(ESCC_BOLD);
			}
			if(cur2.bold!=-1 && cur.bold==-1){//faint
				cur2.bold = cur.bold;
				printf(ESCC_SGR(2));
			}
			if(cur2.blink && !cur.blink){
				cur2.blink = cur.blink;
				printf(ESCC_SGR(25));
			}
			if(!cur2.blink && cur.blink){
				cur2.blink = cur.blink;
				printf(ESCC_BLINK);
			}
			if(cur2.underline && !cur.underline){
				cur2.underline = cur.underline;
				printf(ESCC_SGR(24));
			}
			if(!cur2.underline && cur.underline){
				cur2.underline = cur.underline;
				printf(ESCC_UNDER_);
			}
			if(!cur2.inverted && cur.inverted){
				cur2.inverted = 1;
				printf(ESCC_INVERTED);
			}
			if(cur2.inverted && !cur.inverted){
				cur2.inverted = 0;
				printf(ESCC_N_INVERTED);
			}
			if(scr->curPos.x==x && scr->curPos.y==y){
				if(cur2.inverted)printf(ESCC_N_INVERTED);
				else printf(ESCC_INVERTED);
			}
			printf("%s",cur.val);
			if(scr->curPos.x==x && scr->curPos.y==y){
				if(cur2.inverted)printf(ESCC_INVERTED);
				else printf(ESCC_N_INVERTED);
			}
		}
		printf(ESCC_DEF);
		printf("\n");
	}
	printf("\x1b[%dm",scr->def.FGcolor+30);
	printf("\x1b[%dm",scr->def.BGcolor+40);
	printf("╚");for(int x=0;x<scr->larg && x<w.ws_col-2;x++)printf("═");printf("╝\n");
	printf(ESCC_DEF);
}

void printStrToScreenPos(screen* scr,char* str,int x, int y){
	screenMove(scr,x,y);
	printStrToScreen(scr,str);
}

void printStrToScreen(screen* scr,char* str){
	for (int i=0;str[i];i++){
		if(scr->curPos.x>=scr->larg){
			scr->curPos.x-=scr->larg;
			scr->curPos.y++;
		}
		if(str[0]=='\a');
		else if(str[0]=='\n'){
			if(scr->curPos.y+1<scr->alt){
				scr->curPos.x=0;
				scr->curPos.y++;
			}
		}
		else if(str[0]=='\t'){
			printStrToScreen(scr,"    ");
		}
		else if(str[0]==KEYSTR_BCKSP[0]){//backspace
			if(scr->curPos.x || scr->curPos.y){
				(scr->curPos.x)--;
				if(scr->curPos.x<0){
					(scr->curPos.y)--;
					scr->curPos.x=scr->larg-1;
					while(scr->vect[i][(scr->curPos.y)].val[0]=='\0'){
						(scr->curPos.x)--;
					}
				}
				resetScreenCoord(&(scr->vect[scr->curPos.x][scr->curPos.y]));
			}
		}
		else{
			if(scr->curPos.y>=scr->alt){
				printf("Erro ao salvar mensagem na tela: tamanho limite estourado.\nx=%d larg=%d\ny=%d alt=%d\n",scr->curPos.x,scr->larg,scr->curPos.y,scr->alt);
				CLEARBUFFER;
				scr->curPos.y=scr->alt-1;
				scr->curPos.x=scr->larg-1;
				return;
			}
			scr->vect[scr->curPos.x][scr->curPos.y]=scr->cur;
			scr->vect[scr->curPos.x][scr->curPos.y].val[0]=str[i];
			scr->curPos.x++;
		}
	}
}

void printCharToScreenPos(screen* scr,char* str,int x,int y){
	screenMove(scr,x,y);
	printCharToScreen(scr,str);
}

void printCharToScreen(screen* scr,char* str){
	if(scr->curPos.x>=scr->larg){
		scr->curPos.x=0;
		scr->curPos.y++;
	}
	if(scr->curPos.y>=scr->alt){
		printf("Erro ao salvar mensagem na tela: tamanho limite estourado.\nx=%d larg=%d\ny=%d alt=%d\n",scr->curPos.x,scr->larg,scr->curPos.y,scr->alt);
	}
	if(!strcmp(str,KEYSTR_DEL)){//delete
		resetScreenCoord(&(scr->vect[scr->curPos.x][scr->curPos.y]));
	}
	else{
		scr->vect[scr->curPos.x][scr->curPos.y]=scr->cur;
		strcpy(scr->vect[scr->curPos.x][scr->curPos.y].val,str);
		scr->curPos.x++;
	}
}

void printFieldCoordToScreen(screen* scr,int baseX,int baseY,int sizeX,int sizeY,char* char1,char* char2,char* char3,char* char4,char* char5,char* char6,char* char7,char* char8){
	printStrToScreenPos(scr,char1,baseX,baseY);
	for(int i=0;i<sizeX-2;i++)printStrToScreen(scr,char2);
	printStrToScreen(scr,char3);

	for(int i=1;i<sizeY-1;i++){
		printStrToScreenPos(scr,char4,baseX,baseY+i);
		printStrToScreenPos(scr,char4,baseX+sizeX-1,baseY+i);
	}

	printStrToScreenPos(scr,char6,baseX,baseY+sizeY-1);
	for(int i=0;i<sizeX-2;i++)printStrToScreen(scr,char7);
	printStrToScreen(scr,char8);
}

void printFieldToScreen(screen* scr,int baseX,int baseY,int lar,int alt,int sizeX,int sizeY,char* corner1,char* corner2,char* corner3,char* corner4,char* lineV,char* lineH,char* inLeft,char* inRight,char* inDown,char* inUp,char* cross){
	char str[21];
	int cont = 1,i=alt;
	do{
		cont++;
		i/=10;
	} while(i);
	screenMove(scr,baseX,baseY);
	for(int x=1;x<=lar;x++){
		sprintf(str,"%*d",sizeX-1,x);
		printStrToScreen(scr,str);
	}
	baseY++;
	printFieldCoordToScreen(scr,baseX,baseY,sizeX,sizeY,corner1,lineH,inDown,lineV,lineV,inRight,lineH,cross);
	for(int x=1;x<lar-1;x++){
		printFieldCoordToScreen(scr,baseX+(x*(sizeX-1)),baseY,sizeX,sizeY,inDown,lineH,inDown,lineV,lineV,cross,lineH,cross);
	}
	printFieldCoordToScreen(scr,baseX+((lar-1)*(sizeX-1)),baseY,sizeX,sizeY,inDown,lineH,corner2,lineV,lineV,cross,lineH,inLeft);
	sprintf(str,"%*d",cont,1);
	printStrToScreenPos(scr,str,baseX+(lar*(sizeX-1))+1,baseY+((sizeY-2)/2)+1);

	for(int y=1;y<alt-1;y++){
		printFieldCoordToScreen(scr,baseX,baseY+(y*(sizeY-1)),sizeX,sizeY,inRight,lineH,cross,lineV,lineV,inRight,lineH,cross);
		for(int x=1;x<lar-1;x++){
			printFieldCoordToScreen(scr,baseX+(x*(sizeX-1)),baseY+(y*(sizeY-1)),sizeX,sizeY,cross,lineH,cross,lineV,lineV,cross,lineH,cross);
		}
		printFieldCoordToScreen(scr,baseX+((lar-1)*(sizeX-1)),baseY+(y*(sizeY-1)),sizeX,sizeY,cross,lineH,inLeft,lineV,lineV,cross,lineH,inLeft);
		sprintf(str,"%*d",cont,y+1);
		printStrToScreenPos(scr,str,baseX+(lar*(sizeX-1))+1,baseY+(y*(sizeY-1))+((sizeY-2)/2)+1);
	}

	printFieldCoordToScreen(scr,baseX,baseY+((alt-1)*(sizeY-1)),sizeX,sizeY,inRight,lineH,cross,lineV,lineV,corner3,lineH,inUp);
	for(int x=1;x<lar-1;x++){
		printFieldCoordToScreen(scr,baseX+(x*(sizeX-1)),baseY+((alt-1)*(sizeY-1)),sizeX,sizeY,cross,lineH,cross,lineV,lineV,inUp,lineH,inUp);
	}
	printFieldCoordToScreen(scr,baseX+((lar-1)*(sizeX-1)),baseY+((alt-1)*(sizeY-1)),sizeX,sizeY,cross,lineH,inLeft,lineV,lineV,inUp,lineH,corner4);
	sprintf(str,"%*d",cont,alt);
	printStrToScreenPos(scr,str,baseX+(lar*(sizeX-1))+1,baseY+((alt-1)*(sizeY-1))+((sizeY-2)/2)+1);
}

void screenMove(screen* scr,int x,int y){
	scr->curPos.x=x;
	scr->curPos.y=y;
}

int screenMoveDirKEY(screen* scr,int x){
		if(isUp(x)){
			if(scr->curPos.y>0){
				scr->curPos.y--;
				return 1;
			}
		}
		else if(isLeft(x)){
			if(scr->curPos.x || scr->curPos.y){//not at (0,0)
				scr->curPos.x--;
				if(scr->curPos.x<0){
					scr->curPos.x+=scr->larg;
					scr->curPos.y--;
				}
				return 1;
			}
		}
		else if(isRight(x)){
			scr->curPos.x++;
			if(scr->curPos.x==scr->larg){
				if(scr->curPos.y+1<scr->alt){
					scr->curPos.x-=scr->larg;
					scr->curPos.y++;
					return 1;
				}
				else scr->curPos.x--;
			}
			else return 1;
		}
		else if(isDown(x)){
			if(scr->curPos.y+1<scr->alt){
				scr->curPos.y++;
				return 1;
			}
		}
		return 0;
}

void screenCpy(screen* dest,screen* orig){
	dest->curPos = orig->curPos;
	dest->cur = orig->cur;
	for(int x=0;x<orig->larg;x++){
		for(int y=0;y<orig->alt;y++){
			dest->vect[x][y] = orig->vect[x][y];
		}
	}
}

int screenReadIntInRange(screen* scr,int min,int max,int x1,int y1,int x2,int y2){
	int x=min-1,cont=0;
	char str[101];
	printf("\x1b[%d;%df",y1+2,x1+2);
	readStrNoEnter(str,101);
	x = strToInt(str);
	if(x<min||x>max){
		//printf("ERRO: Entrada invalida. Digite um numero valido x[%d,%d]\n",min,max);
		sprintf(str,"ERRO: Entrada invalida. Digite um numero valido x[%d,%d]",min,max);
		printStrToScreenPos(scr,str,x2,y2);
		while(x<min||x>max){
			screenMove(scr,x1,y1);
			clear();
			printScreen(scr);
			printf("\x1b[%d;%df",y1+2,x1+2);
			readStrNoEnter(str,101);
			x = strToInt(str);
		}
	}
	do{
		cont++;
		min/=10;
	}while(min);
	do{
		cont++;
		max/=10;
	}while(max);
	sprintf(str,"                                                    ");
	for(int i=0;i<cont;i++)sprintf(str,"%s ",str);
	printStrToScreenPos(scr,str,x2,y2);
	return x;
}

void closeScreen(screen* scr){
	for(int x=0;x<scr->larg;x++){
		free(scr->vect[x]);
	}
	free(scr->vect);
	free(scr);
	printf(ESCC_DEF);
}

int readKey(void){
	int c = getch();
	if((c>=' ' && c<='~'+1) || c=='\n' || c=='\t' || c=='\b')return c;
	if(c==KEY_ESC){
		if(!kbhit()){
			return KEY_ESC;//only ESC
		}
		c=getch();
		if(c=='['){
			c=getch();
			if     (c=='A')return KEY_UP;
			else if(c=='D')return KEY_LEFT;
			else if(c=='B')return KEY_DOWN;
			else if(c=='C')return KEY_RIGHT;
			else if(c=='1'){
				c=getch();
				if     (c=='5')return KEY_F5;
				else if(c=='7')return KEY_F6;
				else if(c=='8')return KEY_F7;
				else if(c=='9')return KEY_F8;
				else{
					ungetc(c,stdin);
					ungetc('1',stdin);
					ungetc(KEY_ESC,stdin);
				}
			}
			else if(c=='2'){
				c=getch();
				if     (c=='0')return KEY_F9;
				else if(c=='1')return KEY_F10;
				else if(c=='4')return KEY_F12;
				else{
					ungetc(c,stdin);
					ungetc('2',stdin);
					ungetc(KEY_ESC,stdin);
				}
			}
			else if(c=='3'){
				c=getch();
				if(c=='~')return KEY_DEL;
				else{
					ungetc(c,stdin);
					ungetc('3',stdin);
					ungetc('[',stdin);
					ungetc(KEY_ESC,stdin);
				}
			}
			else{
				ungetc(c,stdin);
				ungetc('[',stdin);
				ungetc(KEY_ESC,stdin);
				return 0;
			}
		}
		else if(c=='O'){
			c=getch();
			if     (c=='Q')return KEY_F2;
			else if(c=='R')return KEY_F3;
			else if(c=='S')return KEY_F4;
			else{
				ungetc(c,stdin);
				ungetc('O',stdin);
				ungetc(KEY_ESC,stdin);
			}
		}
		else{
			ungetc(c,stdin);
			return KEY_ESC;
		}
	}
	return -1;
};

char* keyToStr(int ch,char* str){
	if(str==NULL)return NULL;
	if(ch==KEY_TAB)strcpy(str,"TAB");
	else if(ch==KEY_ENTER)strcpy(str,"ENTER");
	else if(ch==KEY_ESC)strcpy(str,"ESC");
	else if(ch==KEY_BCKSP)strcpy(str,"Backspace");
	else if(ch==KEY_UP)strcpy(str," ↑ ");
	else if(ch==KEY_DOWN)strcpy(str," ↓ ");
	else if(ch==KEY_LEFT)strcpy(str," ← ");
	else if(ch==KEY_RIGHT)strcpy(str," → ");
	else if(ch==KEY_DEL)strcpy(str,"DEL");
	else if(ch>=KEY_F1 && ch<=KEY_F12)sprintf(str,"F%d",ch-KEY_F1+1);
	else sprintf(str,"'%c'",ch);
	return str;
}


char isLeft(int x){
	return (x==KEY_LEFT || x=='A' || x=='a')?1:0;
}

char isRight(int x){
	return (x==KEY_RIGHT || x=='D' || x=='d')?1:0;
}

char isUp(int x){
	return (x==KEY_UP || x=='W' || x=='w')?1:0;
}

char isDown(int x){
	return (x==KEY_DOWN || x=='S' || x=='s')?1:0;
}

menu_opt* insereMenuOpt(menu_opt* menu,menu_opt* opt,int hidden){
	menu_opt* curMenu;
	if(!opt)return menu;
	opt->hidden=hidden;
	if(!menu)return opt;
	for(curMenu=menu;curMenu->next;curMenu=curMenu->next);
	curMenu->next = opt;
	opt->prev=curMenu;
	return menu;
}

menu_opt* menuOptionBox(char *str,int *id,int val){
	menu_opt *opt = (menu_opt*) malloc(sizeof(menu_opt));
	if(!opt)return NULL;
	(*id)++;
	opt->id = *id;
	strcpy(opt->str,str);
	opt->max = 1;
	opt->min = 0;
	opt->val = val;
	opt->type = MENU_TYPE_BOX;
	opt->next=NULL;
	opt->prev=NULL;
	return opt;
}

menu_opt* menuOptionButton(char *str,int* id){
	menu_opt *opt = (menu_opt*) malloc(sizeof(menu_opt));
	if(!opt)return NULL;
	(*id)++;
	opt->id = *id;
	strcpy(opt->str,str);
	opt->max = 0;
	opt->min = 0;
	opt->val = 0;
	opt->type = MENU_TYPE_BUTTON;
	opt->next=NULL;
	opt->prev=NULL;
	return opt;
}

menu_opt* menuOptionSlider(char *str,int* id,int min,int max,int val){
	menu_opt *opt = (menu_opt*) malloc(sizeof(menu_opt));
	if(!opt)return NULL;
	(*id)++;
	opt->id = *id;
	strcpy(opt->str,str);
	opt->max = max;
	opt->min = min;
	opt->val = val;
	opt->type = MENU_TYPE_SLIDER;
	opt->next=NULL;
	opt->prev=NULL;
	return opt;
}

menu_opt* menuOptionNumBox(char *str,int* id,int min,int max,int val){
	menu_opt *opt = (menu_opt*) malloc(sizeof(menu_opt));
	if(!opt)return NULL;
	(*id)++;
	opt->id = *id;
	strcpy(opt->str,str);
	opt->max = max;
	opt->min = min;
	opt->val = val;
	opt->type = MENU_TYPE_NUMBOX;
	opt->next=NULL;
	opt->prev=NULL;
	return opt;
}

menu_opt* menuOptionCharBox(char *str,int* id,int val){
	menu_opt *opt = (menu_opt*) malloc(sizeof(menu_opt));
	if(!opt)return NULL;
	(*id)++;
	opt->id = *id;
	strcpy(opt->str,str);
	opt->max = 0;
	opt->min = 0;
	opt->val = val;
	opt->type = MENU_TYPE_CHARBOX;
	opt->next=NULL;
	opt->prev=NULL;
	return opt;
}

menu_opt* menuOptionText(char *str,int* id){
	menu_opt *opt = (menu_opt*) malloc(sizeof(menu_opt));
	if(!opt)return NULL;
	(*id)++;
	opt->id = *id;
	strcpy(opt->str,str);
	opt->max = 0;
	opt->min = 0;
	opt->val = 0;
	opt->type = MENU_TYPE_TEXT;
	opt->next=NULL;
	opt->prev=NULL;
	return opt;
}

menu_opt* menuOptionLine(char* str,int* id){
	menu_opt *opt = (menu_opt*) malloc(sizeof(menu_opt));
	if(!opt)return NULL;
	(*id)++;
	opt->id = *id;
	strcpy(opt->str,str);
	opt->max = 0;
	opt->min = 0;
	opt->val = 0;
	opt->type = MENU_TYPE_H_LINE;
	opt->next=NULL;
	opt->prev=NULL;
	return opt;
}

void menuToggleHidden(menu_opt* curMenu,int id){
	for(int i=1;curMenu->next && i<id;i++)curMenu=curMenu->next;
	curMenu->hidden = curMenu->hidden%2;
	curMenu->hidden++;
}

char menuMove1(menu_opt* curMenu,screen* scr,int ch,int* cur){
	if(isUp(ch)){
		if(screenMoveDirKEY(scr,KEY_UP))(*cur)--;
		if(!curMenu->prev){
			screenMoveDirKEY(scr,KEY_DOWN);
			(*cur)++;
			return 0;
		}
		else{
			if(curMenu->prev->hidden%2)screenMoveDirKEY(scr,KEY_DOWN);
			if((curMenu->prev->type==MENU_TYPE_TEXT || curMenu->prev->type==MENU_TYPE_H_LINE || curMenu->prev->hidden%2) && !menuMove1(curMenu->prev,scr,KEY_UP,cur)){
				screenMoveDirKEY(scr,KEY_DOWN);
				(*cur)++;
				return 0;
			}
			else if(curMenu->prev->type == MENU_TYPE_SLIDER && !screenMoveDirKEY(scr,KEY_UP)){
				screenMoveDirKEY(scr,KEY_DOWN);
				(*cur)++;
				return 0;
			}
		}
	}
	else {
		if(screenMoveDirKEY(scr,KEY_DOWN))(*cur)++;
		if(!curMenu->next){
			screenMoveDirKEY(scr,KEY_UP);
			(*cur)--;
			return 0;
		}
		else{
			if(curMenu->next->hidden%2)screenMoveDirKEY(scr,KEY_UP);
			if((curMenu->next->type==MENU_TYPE_TEXT || curMenu->next->type==MENU_TYPE_H_LINE || curMenu->next->hidden%2) && !menuMove1(curMenu->next,scr,KEY_DOWN,cur)){
				screenMoveDirKEY(scr,KEY_UP);
				(*cur)--;
				return 0;
			}
			else if(curMenu->type == MENU_TYPE_SLIDER && !screenMoveDirKEY(scr,KEY_DOWN)){
				screenMoveDirKEY(scr,KEY_UP);
				(*cur)--;
				return 0;
			}
		}
	}
	return 1;
}

char menuMove(menu_opt* menu,screen* scr,int ch,int* cur){
	menu_opt* curMenu = menu;
	for(int i=1;i<(*cur);i++){
		if(curMenu->next)curMenu=curMenu->next;
		else{
			printf("Erro ao percorrer opções de Menu...");
			getchar();
			return 0;
		}
	}
	if(isUp(ch)||isDown(ch)){
		return menuMove1(curMenu,scr,ch,cur);
	}
	else if(ch=='\n'){
		if(curMenu->type == MENU_TYPE_BOX){
			curMenu->val = (curMenu->val+1)%2;
			return 1;
		}
		if(curMenu->type == MENU_TYPE_NUMBOX){
			curMenu->val = readIntInRange(curMenu->min,curMenu->max);
			return 1;
		}
		if(curMenu->type == MENU_TYPE_CHARBOX){
			while(!(curMenu->val = readKey()));
			return 1;
		}
	}
	else if(isLeft(ch) || isRight(ch)){
		if(curMenu->type == MENU_TYPE_SLIDER){
			if(isLeft(ch))curMenu->val = max(curMenu->val-1,curMenu->min);
			else curMenu->val = min(curMenu->val+1,curMenu->max);
			return 1;
		}
	}
	return 0;
};

screen* menuToScreen(screen* scr_,screen* scr,menu_opt* menu,int baseX,int baseY){
	if(!scr)scr = screenAlloc(scr_->larg,scr_->alt);
	screenCpy(scr,scr_);
	int curX,curY,X=scr->curPos.x,Y=scr->curPos.y,i,temp;
	char str[101],str2[101];
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	if(baseX > w.ws_row)baseX=0;
	if(baseY > w.ws_col)baseY++;
	curX=baseX;
	curY=baseY;
	while(menu){
		if(!(menu->hidden%2)){
			if(Y==curY || (Y==curY+1 && menu->type==MENU_TYPE_SLIDER))scr->cur.BGcolor=COLOR_BLUE;
			if(menu->type == MENU_TYPE_BOX){
				sprintf(str,"%s - ",menu->str);
				printStrToScreenPos(scr,str,curX,curY);
				if(menu->val==1)strcpy(str,"\u2611");
				else strcpy(str,"\u2610");
				printCharToScreen(scr,str);
			}
			else if(menu->type == MENU_TYPE_BUTTON){
				scr->cur.bold=1;
				sprintf(str,"%s",menu->str);
				printStrToScreenPos(scr,str,curX,curY);
				scr->cur.bold=0;
			}
			else if(menu->type == MENU_TYPE_SLIDER){
				i=0;
				for(int x=menu->max;x;x/=10)i++;
				sprintf(str,"%s - %0*d",menu->str,i,menu->val);
				printStrToScreenPos(scr,str,curX,curY);
				curY++;
				sprintf(str,"<");
				for(i=0;i<11;i++){
					temp = (((menu->val-menu->min)*100)/(menu->max-menu->min));
					if(temp-(10*i)<5 && temp-(10*i)>=-5){
						i++;
						break;
					}
					else sprintf(str,"%s-",str);
				}
				printStrToScreenPos(scr,str,curX,curY);
				strcpy(str,"\u2A15");
				printCharToScreen(scr,str);
				str[0]='\0';
				for(;i<11;i++)sprintf(str,"%s-",str);
				sprintf(str,"%s>",str);
				printStrToScreen(scr,str);
			}
			else if(menu->type == MENU_TYPE_CHARBOX){
				sprintf(str,"%s - %s",menu->str,keyToStr(menu->val,str2));
				printStrToScreenPos(scr,str,curX,curY);
			}
			else if(menu->type == MENU_TYPE_NUMBOX){
				i=0;
				for(int x=menu->max;x;x/=10)i++;
				sprintf(str,"%s - %0*d",menu->str,i,menu->val);
				printStrToScreenPos(scr,str,curX,curY);
			}
			else if(menu->type==MENU_TYPE_TEXT){
				sprintf(str,"%s",menu->str);
				printStrToScreenPos(scr,str,curX,curY);
			}
			else if(menu->type==MENU_TYPE_H_LINE){
				screenMove(scr,curX,curY);
				i=scr->curPos.y;
				while(i==scr->curPos.y)printStrToScreen(scr,menu->str);
				for(int i=0;str[i];i++)printStrToScreen(scr,KEYSTR_BCKSP);
			}
			printStrToScreen(scr," ");
			scr->cur.BGcolor=COLOR_DEF;
			for(i=0;i<20;i++)printStrToScreen(scr," ");
			curX=baseX;
			curY++;
		}
		menu=menu->next;
	}
	scr->curPos.x=X;
	scr->curPos.y=Y;
	return scr;
}

void freeMenu(menu_opt* menu){
	if(menu->next)freeMenu(menu->next);
	free(menu);
}

int fCheck(char* file,int bin){
	FILE* fp;
	if(bin) fp = fopen(file,"rb");
	else    fp = fopen(file,"r");
	if(fp){
		fclose(fp);
		return 1;
	}
	return 0;
}


