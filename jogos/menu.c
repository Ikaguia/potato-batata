#include "batata.h"

int compEroda(char* str){
	char string[101];
	sprintf(string,"gcc -Wall -lm -o blablabla %s/main.c batata.c -std=c99",str);
	if(!system(string))system("./blablabla");
	else getch();
	return 0;
}

int buttonPress(int menu,int ex){
	char str[101];
	printf(ESCC_SHOW_C);
	clear();
	if(menu==ex)return 1;
	else if(menu<1 || menu>ex)return 0;
	FILE* fp = fopen("jogos.txt","r");
	if(!fp){
		printf("Erro ao abrir arquivo de jogos...\n");
		sleep(3);
		return 1;
	}
	for(int i=0;i<menu;i++){
		fscanf(fp," %s",str);//ignora a primeira parte
		fscanf(fp," %s",str);
	}
	compEroda(str);
	return 0;
}



int mainMenu(){
	int id=0,ch,cur=1;
	char str[101];
	menu_opt *menu=NULL;
	screen *scr = screenAlloc(300,38),*scr2=NULL;
	printStrToScreenPos(scr,"Compilação de jogos em C rodando no terminal",1,1);
	screenMove(scr,0,3);
	FILE* fp = fopen("jogos.txt","r");
	if(!fp){
		printf("Erro ao abrir arquivo de jogos...\n");
		sleep(3);
		return 1;
	}
	while(fscanf(fp," %s",str)!=EOF){
		underLineToSpace(str);
		menu = insereMenuOpt(menu,menuOptionButton(str,&id),0);
		fscanf(fp," %s",str);//ignora a segunda parte
	}
	fclose(fp);
	menu = insereMenuOpt(menu,menuOptionText("",&id),0);
	menu = insereMenuOpt(menu,menuOptionButton("Sair",&id),0);
	while(id){//if there is nothing in the file, this will be 0
		printf(ESCC_HIDE_C);//hide cursor
		scr2 = menuToScreen(scr,scr2,menu,1,3);
		clear();
		printScreen(scr2);
		while(!(ch = readKey()));
		if(!menuMove(menu,scr,ch,&cur) && ch=='\n'){
			if(buttonPress(cur,id)){
				closeScreen(scr);
				closeScreen(scr2);
				return 0;
			}
		}
	}
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
