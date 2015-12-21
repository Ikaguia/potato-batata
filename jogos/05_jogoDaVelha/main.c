#include "../batata.h"

int ganhou(int x[9]){
	//diagonal
	if(x[0]==x[4] && x[4]==x[8])return x[0];
	if(x[2]==x[4] && x[4]==x[6])return x[2];
	//vertical
	if(x[0]==x[3] && x[3]==x[6])return x[0];
	if(x[1]==x[4] && x[4]==x[7])return x[1];
	if(x[2]==x[5] && x[5]==x[8])return x[2];
	//horizontal
	if(x[0]==x[1] && x[1]==x[2])return x[0];
	if(x[3]==x[4] && x[4]==x[5])return x[3];
	if(x[6]==x[7] && x[7]==x[8])return x[6];
	for(int i=0;i<9;i++)if(x[i]==0)return 0;
	return -1;
}

int jogo(){
	int n=0,field[9],ch,cur=0;
	char str[101];
	memset(field,0,9*sizeof(int));
	screen *scr = screenAlloc(300,38);
	scr->cur.BGcolor=COLOR_BWHITE;
	scr->cur.FGcolor=COLOR_BLACK;
	for(int y=0;y<13;y++){
		for(int x=0;x<19;x++){
			if(y<=0 || !x || y==12 || x==18)printCharToScreenPos(scr," ",60+x,10+y);
			else if((y==4 || y==8) && (x==6 || x==12))printCharToScreenPos(scr,"+",60+x,10+y);
			else if(y==4 || y==8)printCharToScreenPos(scr,"─",60+x,10+y);
			else if(x==6 || x==12)printCharToScreenPos(scr,"│",60+x,10+y);
			else printCharToScreenPos(scr," ",60+x,10+y);
		}
	}
	do{
		scr->cur.BGcolor=COLOR_DEF;
		scr->cur.FGcolor=COLOR_DEF;
		if(n)printCharToScreenPos(scr,"©",69,9);
		else printCharToScreenPos(scr,"×",69,9);
		scr->cur.BGcolor=COLOR_BWHITE;
		scr->cur.FGcolor=COLOR_BLACK;
		do{
			screenMove(scr,63+(cur%3)*6,12+(cur/3)*4);
			clear();
			printScreen(scr);
			ch=readKey();
			if(isUp(ch) && cur/3>0)cur-=3;
			if(isDown(ch) && cur/3<2)cur+=3;
			if(isLeft(ch) && cur%3>0)cur--;
			if(isRight(ch) && cur%3<2)cur++;
		}while(ch!='\n' || field[cur]);
		field[cur]=n+1;
		if(n)printCharToScreenPos(scr,"©",63+(cur%3)*6,12+(cur/3)*4);
		else printCharToScreenPos(scr,"×",63+(cur%3)*6,12+(cur/3)*4);
		n=(n+1)%2;
	}while(!ganhou(field));
	if(ganhou(field)>0)sprintf(str,"Parabéns player %d, voce venceu.",ganhou(field));
	else sprintf(str,"Ninguem venceu, deu velha.");
	scr->cur.BGcolor=COLOR_DEF;
	scr->cur.FGcolor=COLOR_DEF;
	printStrToScreenPos(scr,str,50,24);
	printStrToScreenPos(scr,"Aperte enter para voltar ao menu principal.",50,25);
	clear();
	printScreen(scr);
	CLEARBUFFER;
	return 0;
}

int mainMenu(){
	int id=0,cur=1,ch;
	menu_opt *menu=NULL;
	screen *scr = screenAlloc(300,38),*scr2=NULL;
	printStrToScreenPos(scr,"Jogo da Velha",1,1);
	screenMove(scr,0,3);
	menu = insereMenuOpt(menu,menuOptionButton("Iniciar o jogo",&id),0);
	menu = insereMenuOpt(menu,menuOptionText("", &id),0);
	menu = insereMenuOpt(menu,menuOptionButton("Sair",          &id),0);
	while(1){
		scr2 = menuToScreen(scr,scr2,menu,1,3);
		clear();
		printScreen(scr2);
		while(!(ch = readKey()));
		menuMove(menu,scr,ch,&cur);
		if(ch=='\n'){
			if(cur==1)jogo();
			if(cur==id)break;
		}
	}
	freeMenu(menu);
	closeScreen(scr);
	closeScreen(scr2);
	return 0;
}

int main(){
	screen *scr = screenAlloc(300,38);
	printStrToScreenPos(scr,"Bem vindo.",2,2);
	printStrToScreenPos(scr,"Pressione qualquer tecla para continuar.",2,4);
	clear();
	printScreen(scr);
	getch();
	mainMenu();
	clear();
	closeScreen(scr);
	printf(ESCC_DEF);
	return 0;
}
