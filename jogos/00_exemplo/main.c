#include "../batata.h"

typedef struct{
	int a;
} config_;


int jogo(config_ config){
	int n=0;
	char str[101];
	screen *scr = screenAlloc(300,38);
	sprintf(str,"Blablabla %d",config.a);
	printStrToScreenPos(scr,str,1,1);
	printStrToScreenPos(scr,">> ",1,2);
	clear();
	printScreen(scr);
	n = screenReadIntInRange(scr,0,100,4,2,1,3);
	while(n!=config.a){
		sprintf(str,"Blablabla %d %d",config.a,n);
		printStrToScreenPos(scr,str,1,1);
		printStrToScreenPos(scr,">> ",1,2);
		clear();
		printScreen(scr);
		n = screenReadIntInRange(scr,0,100,4,2,1,3);
	}
	resetScreen(scr);
	printStrToScreenPos(scr,"Parabéns, voce venceu.",1,1);
	printStrToScreenPos(scr,"Aperte enter para voltar ao menu principal.",1,2);
	printStrToScreenPos(scr,">> ",1,3);
	clear();
	printScreen(scr);
	CLEARBUFFER;
	return 0;
}

int configura(config_ *config){
	int id=0,cur=1,ch;
	menu_opt *curMenu,*menu=NULL;
	screen *scr = screenAlloc(300,38),*scr2=NULL;
	printStrToScreenPos(scr,"Configurações:",1,1);
	screenMove(scr,0,3);
	menu = insereMenuOpt(menu,menuOptionNumBox("Chave",&id,0,100,config->a),0);
	menu = insereMenuOpt(menu,menuOptionText(" ",&id),0);
	menu = insereMenuOpt(menu,menuOptionButton("Sair",&id),0);
	while(1){
		scr2 = menuToScreen(scr,scr2,menu,1,3);
		clear();
		printScreen(scr2);
		while(!(ch = readKey()));
		menuMove(menu,scr,ch,&cur);
		curMenu=menu;
		for(int i=1;curMenu->next && i<cur;i++)curMenu=curMenu->next;
		if(ch=='\n'){
			if(cur==id)break;//sair
			if(cur==1)config->a = menu->val;
		}
	}
	freeMenu(menu);
	closeScreen(scr);
	closeScreen(scr2);
	return 0;
}

int mainMenu(){
	config_ config;
	config.a=1;
	int id=0,cur=1,ch;
	menu_opt *menu=NULL;
	screen *scr = screenAlloc(300,38),*scr2=NULL;
	printStrToScreenPos(scr,"Exemplo",1,1);//////////////////////////////////////////////////////////////////
	screenMove(scr,0,3);
	menu = insereMenuOpt(menu,menuOptionButton("Iniciar o jogo",&id),0);
	menu = insereMenuOpt(menu,menuOptionButton("Configurações", &id),0);
	menu = insereMenuOpt(menu,menuOptionText("", &id),0);
	menu = insereMenuOpt(menu,menuOptionButton("Sair",          &id),0);
	while(1){
		scr2 = menuToScreen(scr,scr2,menu,1,3);
		clear();
		printScreen(scr2);
		while(!(ch = readKey()));
		menuMove(menu,scr,ch,&cur);
		if(ch=='\n'){
			if(cur==1)jogo(config);
			if(cur==2)configura(&config);
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
