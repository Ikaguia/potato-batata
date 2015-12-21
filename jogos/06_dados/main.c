#include "../batata.h"

typedef struct{
	int quant;
	int forca;
} config_;


int randDado(int x){
	int n;
	do{
		n = (rand()%6)+1;
	}while(n==7-x);
	return n;
}

void printDadoToScreenPos(screen* scr,int dado,int x,int y){
	if(dado==1)printCharToScreenPos(scr,"⚀",x,y);
	else if(dado==2)printCharToScreenPos(scr,"⚁",x,y);
	else if(dado==3)printCharToScreenPos(scr,"⚂",x,y);
	else if(dado==4)printCharToScreenPos(scr,"⚃",x,y);
	else if(dado==5)printCharToScreenPos(scr,"⚄",x,y);
	else if(dado==6)printCharToScreenPos(scr,"⚅",x,y);
}

int jogo(config_ config,screen* scrOrig){
	screen *scr = screenAlloc(300,38);
	screenCpy(scr,scrOrig);
	int dados[config.quant][3],cont;
	for(int i=0;i<config.quant;i++){
		dados[i][0]=(((rand()%30)+15)*config.forca)/100;
		dados[i][1]=1+rand()%6;
		dados[i][2]=1;
		printDadoToScreenPos(scr,dados[i][1],1,1+i);
	}
	clear();
	screenMove(scr,scrOrig->curPos.x,scrOrig->curPos.y);
	printScreen(scr);
	screenCpy(scr,scrOrig);
	usleep((rand()%50000)+25000);
	do{
		for(int i=0;i<config.quant;i++){
			if(dados[i][0]){
				dados[i][1]=randDado(dados[i][1]);
				if(rand()%10){
					dados[i][2]++;
					dados[i][0]--;
				}
			}
			printDadoToScreenPos(scr,dados[i][1],dados[i][2],1+i);
		}
		clear();
		screenMove(scr,scrOrig->curPos.x,scrOrig->curPos.y);
		printScreen(scr);
		screenCpy(scr,scrOrig);
		usleep((rand()%50000)+25000);
		cont=0;
		for(int i=0;i<config.quant;i++)if(dados[i][0])cont++;
	}while(cont);
	CLEARBUFFER;
	return 0;
}

int configura(config_ *config){
	int id=0,cur=1,ch;
	menu_opt *curMenu,*menu=NULL;
	screen *scr = screenAlloc(300,38),*scr2=NULL;
	printStrToScreenPos(scr,"Configurações:",1,1);
	screenMove(scr,0,3);
	menu = insereMenuOpt(menu,menuOptionSlider("Quantidade de dados",&id,0,100,config->quant),0);
	menu = insereMenuOpt(menu,menuOptionSlider("Força",&id,0,200,config->forca),0);
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
		if(cur==1)config->quant = curMenu->val;
		if(cur==2)config->forca = curMenu->val;
		if(ch=='\n'){
			if(cur==id)break;//sair
		}
	}
	freeMenu(menu);
	closeScreen(scr);
	closeScreen(scr2);
	return 0;
}

int mainMenu(){
	config_ config;
	config.quant=2;
	config.forca=100;
	int id=0,cur=1,ch;
	menu_opt *menu=NULL;
	screen *scr = screenAlloc(300,38),*scr2=NULL;
	printStrToScreenPos(scr,"Dados",1,1);//////////////////////////////////////////////////////////////////
	screenMove(scr,0,3);
	menu = insereMenuOpt(menu,menuOptionButton("Jogar dados",&id),0);
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
			if(cur==1)jogo(config,scr2);
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
