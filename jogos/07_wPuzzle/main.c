#include "../batata.h"

typedef struct{
	int lar;
	int alt;
} config_;


void move(int* field,int ch,config_ config){
	if((isUp(ch) || ch==0) && field[0]>config.lar){
		field[field[0]]=field[field[0]-config.lar];
		field[field[0]-config.lar]=0;
		field[0]-=config.lar;
	}
	if((isDown(ch) || ch==1) && field[0]<=(config.alt-1)*config.lar){
		field[field[0]]=field[field[0]+config.lar];
		field[field[0]+config.lar]=0;
		field[0]+=config.lar;
	}
	if((isLeft(ch) || ch==2) && (field[0]-1)%config.lar>0){
		field[field[0]]=field[field[0]-1];
		field[field[0]-1]=0;
		field[0]--;
	}
	if((isRight(ch) || ch==3) && (field[0]-1)%config.lar<config.lar-1){
		field[field[0]]=field[field[0]+1];
		field[field[0]+1]=0;
		field[0]++;
	}
}

int jogo(config_ config){
	int field[1+config.lar*config.alt],cont;
	char str[101];
	screen *scr = screenAlloc(300,38);
	printStrToScreenPos(scr,"wPuzzle",1,1);
	scr->cur.BGcolor=COLOR_YELLOW;
	scr->cur.FGcolor=COLOR_WHITE;
	printFieldToScreen(scr,3,4,config.lar,config.alt,6,4,"#","#","#","#","|","-","+","+","+","+","+");
	scr->cur.BGcolor=COLOR_BWHITE;
	scr->cur.FGcolor=COLOR_BBLACK;
	for(int x=0;x<config.lar;x++){
		for(int y=0;y<config.alt;y++){
			printStrToScreenPos(scr,"    ",4+5*x,6+3*y);
			printStrToScreenPos(scr,"    ",4+5*x,7+3*y);
			field[1+x+(config.lar*y)]=1+x+(config.lar*y);
		}
	}
	field[config.lar*config.alt]=0;
	field[0]=config.lar*config.alt;
	for(int x=0;x<config.lar;x++){
		for(int y=0;y<config.alt;y++){
			if(field[1+x+(config.lar*y)])sprintf(str,"%2d",field[1+x+(config.lar*y)]);
			else sprintf(str,"  ");
			printStrToScreenPos(scr,str,5+5*x,6+3*y);
		}
	}
	screenMove(scr,-1,-1);
	clear();
	printScreen(scr);
	usleep(1000000);
	for(int i=0;i<(config.lar*config.alt)*10000;i++){
		move(field,rand()%4,config);
		if((i%1000)==0){
			for(int x=0;x<config.lar;x++){
				for(int y=0;y<config.alt;y++){
					if(field[1+x+(config.lar*y)])sprintf(str,"%2d",field[1+x+(config.lar*y)]);
					else sprintf(str,"  ");
					printStrToScreenPos(scr,str,5+5*x,6+3*y);
				}
			}
			screenMove(scr,-1,-1);
			clear();
			printScreen(scr);
			usleep(20000);
		}
	}
	do{
		for(int x=0;x<config.lar;x++){
			for(int y=0;y<config.alt;y++){
				if(field[1+x+(config.lar*y)])sprintf(str,"%2d",field[1+x+(config.lar*y)]);
				else sprintf(str,"  ");
				printStrToScreenPos(scr,str,5+5*x,6+3*y);
			}
		}
		screenMove(scr,-1,-1);
		clear();
		printScreen(scr);
		move(field,readKey(),config);
		cont=0;
		for(int i=1;i<=config.lar*config.alt;i++)if(field[i]!=i)cont++;
	}while(cont);
	// clear();
	// printScreen(scr);
	// while(n!=config.a){
	// 	sprintf(str,"Blablabla %d %d",config.a,n);
	// 	printStrToScreenPos(scr,str,1,1);
	// 	printStrToScreenPos(scr,">> ",1,2);
	// 	clear();
	// 	printScreen(scr);
	// 	n = screenReadIntInRange(scr,0,100,4,2,1,3);
	// }
	// resetScreen(scr);
	// printStrToScreenPos(scr,"Parabéns, voce venceu.",1,1);
	// printStrToScreenPos(scr,"Aperte enter para voltar ao menu principal.",1,2);
	// printStrToScreenPos(scr,">> ",1,3);
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
	menu = insereMenuOpt(menu,menuOptionSlider("Largura",&id,3,10,config->lar),0);
	menu = insereMenuOpt(menu,menuOptionSlider("Altura",&id,3,10,config->alt),0);
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
		if(cur==1)config->lar = menu->val;
		if(cur==2)config->alt = menu->val;
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
	config.lar=5;
	config.alt=5;
	int id=0,cur=1,ch;
	menu_opt *menu=NULL;
	screen *scr = screenAlloc(300,38),*scr2=NULL;
	printStrToScreenPos(scr,"wPuzzle",1,1);
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
