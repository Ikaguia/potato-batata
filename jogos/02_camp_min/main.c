#include "../batata.h"
#include "header.h"

struct config__{
	int larg;
	int alt;
	int nBomb;
};

struct coord_{
	int val;
	int val2;
};


void moveCursor(screen* scr,int ch,config_ config,int* x,int* y){
	if(isUp(ch) && (*y)>0){
		(*y)--;
		(scr->curPos.y)--;
	}
	if(isDown(ch) && (*y)+1<config.alt){
		(*y)++;
		(scr->curPos.y)++;
	}
	if(isLeft(ch) && (*x)>0){
		(*x)--;
		(scr->curPos.x)--;
	}
	if(isRight(ch) && (*x)+1<config.larg){
		(*x)++;
		(scr->curPos.x)++;
	}
}

void printMap(screen* scr, config_ config,coord** map,int espRest,int bombRest,int cliques,int curX,int curY){
	char str[120];
	sprintf(str,"Espaços Restantes = %d/%d",absolute(espRest),config.larg * config.alt);
	printStrToScreenPos(scr,str,3,1);
	sprintf(str,"Largura = %d  Altura = %d  Nº de Bombas = %d Nº de Cliques = %d",config.larg,config.alt,bombRest,cliques);
	printStrToScreenPos(scr,str,3,2);
	printFieldToScreen(scr,3,4,config.larg,config.alt,5,3,"#","#","#","#","|","-","+","+","+","+","+");
	if(map){
		for(int y=0;y<config.alt;y++){
			for(int x=0;x<config.larg;x++){
				if(map[x][y].val2==BANDEIRA)	printCharToScreenPos(scr,"⛳",5+(x*4),6+(y*2));
				else if(map[x][y].val2==INTERROGACAO)printCharToScreenPos(scr,"❔",5+(x*4),6+(y*2));
				else if(map[x][y].val2==ABERTO){
					if(map[x][y].val==BOMB){
						if(curX==x && curY==y)scr->cur.FGcolor=COLOR_RED;
						printCharToScreenPos(scr,"✵",5+(x*4),6+(y*2));
						if(curX==x && curY==y)scr->cur.FGcolor=COLOR_DEF;
					}
					else if(map[x][y].val==0){
						scr->cur.inverted=1;
						printStrToScreenPos(scr,"___",4+(x*4),6+(y*2));
						scr->cur.inverted=0;
					}
					else{
						sprintf(str," %d ",map[x][y].val);
						scr->cur.FGcolor=map[x][y].val;
						printStrToScreenPos(scr,str,4+(x*4),6+(y*2));
						scr->cur.FGcolor=COLOR_DEF;
					}
				}
				else{
						printStrToScreenPos(scr,"   ",4+(x*4),6+(y*2));
					}
			}
		}
	}
	if(curX!=-1)screenMove(scr,4+(curX*4),6+(curY*2));
	else screenMove(scr,-1,-1);
	clear();
	printScreen(scr);
}

int abreCoord(config_ config,coord** map,int a,int b,int* espRest,int* bombRest){
	if(a>=0 && a<config.larg && b>=0 && b<config.alt){
		if(map[a][b].val2==BANDEIRA || map[a][b].val2==INTERROGACAO){
			map[a][b].val2=FECHADO;
			if(bombRest)(*bombRest)++;
			return 0;
		}
		map[a][b].val2=ABERTO;
		(*espRest)--;
		if(!map[a][b].val){
			for(int x=a-1;x<a+2;x++)
				for(int y=b-1;y<b+2;y++)
					if(x>=0 && x<config.larg && y>=0 && y<config.alt && map[x][y].val2!=ABERTO)
						abreCoord(config,map,x,y,espRest,bombRest);
		}
		else if(map[a][b].val==BOMB){
			(*espRest)*=-1;
		}
	}
	else{
		printf("Função abreCoord chamada com coordenadas invalidas.\n");
		return 0;
	}
	return 1;
};

int marcaCoord(config_ config,coord** map,int a,int b,int* bombRest){
	if(a>=0 && a<config.larg && b>=0 && b<config.alt){
		if(map[a][b].val2==FECHADO){
			map[a][b].val2=BANDEIRA;
			(*bombRest)--;
		}
		else map[a][b].val2++;
		if(map[a][b].val2>INTERROGACAO){
			map[a][b].val2=FECHADO;
			(*bombRest)++;
		}
	}
	else{
		printf("Função abreCoord chamada com coordenadas invalidas.\n");
	}
	return 0;
};

coord** criaMap(config_ config,int a,int b,int *espRest){
	coord** map = (coord**)malloc(config.larg * sizeof(coord*));
	if(!map){
		printf("Erro 1 ao alocar memoria para mapa.\n");
		return NULL;
	}
	memset(map,0,config.larg * sizeof(coord*));
	for(int i=0;i<config.larg;i++){
		map[i] = (coord*)malloc(config.alt * sizeof(coord));
		if(!map[i]){
			printf("Erro 2 ao alocar memoria para mapa.\n");
			for(int j=0;j<i;j++){
				free(map[j]);
			}
			free(map);
			return NULL;
		}
		memset(map[i],0,config.alt * sizeof(coord));
	}

	int bombs=config.nBomb,x,y;
	while(bombs){
		//printf("BOMB\n");getch();
		do{
			x=rand()%config.larg;
			y=rand()%config.alt;
		}while(map[x][y].val==BOMB || (x==a && y==b));
		map[x][y].val=BOMB;
		for(int i=-1;i<2;i++)
			for(int j=-1;j<2;j++)
				if(x+i>=0 && x+i<config.larg && y+j>=0 && y+j<config.alt && map[x+i][y+j].val!=BOMB)
					map[x+i][y+j].val++;
		bombs--;
	}
	abreCoord(config,map,a,b,espRest,NULL);
	return map;
}

int jogo(config_ config){
	coord **map;
	screen *scr = screenAlloc(300,38),*scr2=screenAlloc(300,38);
	int espRest=(config.larg * config.alt) - config.nBomb,bombRest=config.nBomb,x=0,y=0,cliques=0,ch;
	printStrToScreenPos(scr,"Selecione as coordenadas iniciais e aperte enter.",1,2*config.alt + 7);
	do{
		screenCpy(scr2,scr);
		printMap(scr2,config,NULL,espRest,bombRest,cliques,x,y);
		ch=readKey();
		moveCursor(scr,ch,config,&x,&y);
	}while(ch!='\n');
	map = criaMap(config,x,y,&espRest);
	cliques++;
	if(!map)return 1;
	do{
		printStrToScreenPos(scr,"Selecione as coordenadas e aperte: Enter para abrir, Espaço para Marcar ou Esc para sair.",1,2*config.alt + 7);
		do{
			screenCpy(scr2,scr);
			printMap(scr2,config,map,espRest,bombRest,cliques,x,y);
			ch=readKey();
			if(ch==KEY_ESC)return 0;
			if(ch==' ')marcaCoord(config,map,x,y,&bombRest);
			moveCursor(scr,ch,config,&x,&y);
		}while(ch!='\n');
		if(abreCoord(config,map,x,y,&espRest,&bombRest))cliques++;
	}while(espRest>0);
	printMap(scr,config,map,espRest,bombRest,cliques,x,y);
	if(!espRest)printStrToScreenPos(scr,"Parabéns, você venceu!",1,2*config.alt + 7);
	else printStrToScreenPos(scr,"Voce perdeu, que noob.",1,2*config.alt + 7);
	printStrToScreen(scr,"Aperte qualquer tecla para voltar ao menu principal.                ");
	screenCpy(scr2,scr);
	printMap(scr2,config,map,espRest,bombRest,cliques,x,y);
	getch();
	return 0;
}

int configura(config_ *config){
	int id=0,cur=1,ch;
	menu_opt *menu=NULL;
	screen *scr = screenAlloc(300,38),*scr2=NULL;
	printStrToScreenPos(scr,"Configurações",1,1);
	screenMove(scr,0,3);
	menu = insereMenuOpt(menu,menuOptionSlider("Largura",&id,5,LAR_MAX,config->larg),0);
	menu = insereMenuOpt(menu,menuOptionSlider("Altura",&id,5,ALT_MAX,config->alt),0);
	menu = insereMenuOpt(menu,menuOptionSlider("Bombas",&id,1,(config->alt*config->larg)-1,config->nBomb),0);
	menu = insereMenuOpt(menu,menuOptionText("", &id),0);
	menu = insereMenuOpt(menu,menuOptionButton("Sair",&id),0);
	while(1){
		scr2 = menuToScreen(scr,scr2,menu,1,3);
		clear();
		printScreen(scr2);
		while(!(ch = readKey()));
		menuMove(menu,scr,ch,&cur);
		if(isLeft(ch) || isRight(ch) || ch=='\n'){
			if(cur==1){
				config->larg=menu->val;
				menu->next->next->max=(config->alt*config->larg)-1;
				config->nBomb = min(config->nBomb,(config->alt*config->larg)-1);
			}
			if(cur==2){
				config->alt=menu->next->val;
				menu->next->next->max=(config->alt*config->larg)-1;
				config->nBomb = min(config->nBomb,(config->alt*config->larg)-1);
			}
			if(cur==3)config->nBomb=menu->next->next->val;
			if(cur==5){
				break;
			}
		}
	}
	freeMenu(menu);
	closeScreen(scr);
	closeScreen(scr2);
	return 0;
}

int mainMenu(){
	config_ config;
	config.larg =LAR_DEF;
	config.alt  =ALT_DEF;
	config.nBomb=BMB_DEF;
	srand(time(NULL));
	int id=0,cur=1,ch;
	menu_opt *menu=NULL;
	screen *scr = screenAlloc(300,38),*scr2=NULL;
	printStrToScreenPos(scr,"Campo Minado",1,1);
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
		if(!menuMove(menu,scr,ch,&cur) && ch=='\n'){
			if(cur==1)jogo(config);
			if(cur==2)configura(&config);
			if(cur==4){
				break;
			}
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
