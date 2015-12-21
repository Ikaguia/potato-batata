#include "../batata.h"

typedef struct{
	int tam;
	int dif;
} config_;


typedef struct{
	int val; //qual barco esta na posição
	int val2;//se o inimigo ja atacou essa posição
	int val3;//id do barco
}coord;

typedef coord** mapa;


int barcoDestruido(mapa map,config_ config,int id){
	for(int x=0;x<config.tam;x++){
		for(int y=0;y<config.tam;y++){
			if(map[x][y].val3==id && !map[x][y].val2)return 0;
		}
	}
	return 1;
}

int perdeu(mapa map,config_ config){
	if(config.tam==10)for(int i=1;i<=11;i++)if(!barcoDestruido(map,config,i))return 0;
	if(config.tam==15)for(int i=1;i<=15;i++)if(!barcoDestruido(map,config,i))return 0;
	return 1;
}

int posFree(config_ config,mapa map,int x,int y){
	//printf("checking x%d y%d\n",x,y);
	fflush(stdout);
	if(x<0 || x>=config.tam || y<0 || y>=config.tam)return 0;
	for(int i=-1;i<=1;i++){
		for(int j=-1;j<=1;j++){
			if(x+i>=0 && x+i<config.tam && y+j>=0 && y+j<config.tam && map[x+i][y+j].val)return 0;
		}
	}
	return 1;
}

int colocaBarco(config_ config,mapa map,int id,int coords[4]){
	if(coords[3]==-3){
		if(coords[2]==0 && posFree(config,map,coords[0],coords[1]) && posFree(config,map,coords[0]+1,coords[1]-1) && posFree(config,map,coords[0]+2,coords[1])){
			if(id>=0){//se id<0 apenas retorna se é possivel colocar naquela posição
				map[coords[0]]  [coords[1]]  .val=-3;
				map[coords[0]+1][coords[1]-1].val=-3;
				map[coords[0]+2][coords[1]]  .val=-3;
				map[coords[0]]  [coords[1]]  .val3=id;
				map[coords[0]+1][coords[1]-1].val3=id;
				map[coords[0]+2][coords[1]]  .val3=id;
			}
		}
		else if(coords[2]==1 && posFree(config,map,coords[0],coords[1]) && posFree(config,map,coords[0]+1,coords[1]+1) && posFree(config,map,coords[0],coords[1]+2)){
			if(id>=0){
				map[coords[0]]  [coords[1]]  .val=-3;
				map[coords[0]+1][coords[1]+1].val=-3;
				map[coords[0]]  [coords[1]+2].val=-3;
				map[coords[0]]  [coords[1]]  .val3=id;
				map[coords[0]+1][coords[1]+1].val3=id;
				map[coords[0]]  [coords[1]+2].val3=id;
			}
		}
		else if(coords[2]==2 && posFree(config,map,coords[0],coords[1]) && posFree(config,map,coords[0]-1,coords[1]+1) && posFree(config,map,coords[0]-2,coords[1])){
			if(id>=0){
				map[coords[0]]  [coords[1]]  .val=-3;
				map[coords[0]-1][coords[1]+1].val=-3;
				map[coords[0]-2][coords[1]]  .val=-3;
				map[coords[0]]  [coords[1]]  .val3=id;
				map[coords[0]-1][coords[1]+1].val3=id;
				map[coords[0]-2][coords[1]]  .val3=id;
			}
		}
		else if(coords[2]==3 && posFree(config,map,coords[0],coords[1]) && posFree(config,map,coords[0]-1,coords[1]-1) && posFree(config,map,coords[0],coords[1]-2)){
			if(id>=0){
				map[coords[0]]  [coords[1]]  .val=-3;
				map[coords[0]-1][coords[1]-1].val=-3;
				map[coords[0]]  [coords[1]-2].val=-3;
				map[coords[0]]  [coords[1]]  .val3=id;
				map[coords[0]-1][coords[1]-1].val3=id;
				map[coords[0]]  [coords[1]-2].val3=id;
			}
		}
		else return 0;
	}
	else{
		if     (coords[2]==0){for(int j=0;j<coords[3];j++)if(!posFree(config,map,coords[0]+j,coords[1]))return 0;}
		else if(coords[2]==1){for(int j=0;j<coords[3];j++)if(!posFree(config,map,coords[0],coords[1]+j))return 0;}
		else if(coords[2]==2){for(int j=0;j<coords[3];j++)if(!posFree(config,map,coords[0]-j,coords[1]))return 0;}
		else if(coords[2]==3){for(int j=0;j<coords[3];j++)if(!posFree(config,map,coords[0],coords[1]-j))return 0;}
		if(id>=0){
			if(coords[2]==0)for(int j=0;j<coords[3];j++){
				map[coords[0]+j][coords[1]].val=coords[3];
				map[coords[0]+j][coords[1]].val3=id;
			}
			else if(coords[2]==1)for(int j=0;j<coords[3];j++){
				map[coords[0]][coords[1]+j].val=coords[3];
				map[coords[0]][coords[1]+j].val3=id;
			}
			else if(coords[2]==2)for(int j=0;j<coords[3];j++){
				map[coords[0]-j][coords[1]].val=coords[3];
				map[coords[0]-j][coords[1]].val3=id;
			}
			else if(coords[2]==3)for(int j=0;j<coords[3];j++){
				map[coords[0]][coords[1]-j].val=coords[3];
				map[coords[0]][coords[1]-j].val3=id;
			}
		}
	}
	return 1;
}

void colorScreenCoord(screen* scr,config_ config,mapa map,int x,int y,int baseX,int baseY,int color){
	if(x>=0 && x<config.tam && y>=0 && y<config.tam){
		if(!posFree(config,map,x,y))scr->cur.FGcolor=COLOR_RED;
		else scr->cur.FGcolor=color;
		if(config.tam==10)printCharToScreenPos(scr,"▓",baseX+(x*5)+2,baseY+(y*2)+1);
		if(config.tam==15)printCharToScreenPos(scr,"▓",baseX+(x*3)+1,baseY+(y*2)+1);
	}
}

void printMapToScreen(screen* scr,config_ config,mapa map,int hide,int baseX,int baseY,int* coords){
	char str[10];
	int color;
	for(int x=0;x<config.tam;x++){
		sprintf(str,"%d",x+1);
		if(config.tam==10)printStrToScreenPos(scr,str,baseX+(5*x)+2,baseY);
		if(config.tam==15)printStrToScreenPos(scr,str,baseX+(3*x)+1,baseY);
	}
	baseY++;
	for(int y=0;y<config.tam;y++){
		screenMove(scr,baseX,baseY+(2*y));
		scr->cur.BGcolor=COLOR_BLUE;
		for(int x=0;x<config.tam;x++){
			if(map[x][y].val && (hide!=1 || map[x][y].val2)){
				if(config.tam==10)printCharToScreen(scr,"⧼");
				printCharToScreen(scr,"⧼");
				if(y-1>=0 && map[x][y-1].val3 == map[x][y].val3){
					scr->cur.FGcolor=COLOR_GREEN;
					printCharToScreen(scr,"▓");
					scr->cur.FGcolor=COLOR_DEF;
				}
				else printCharToScreen(scr,"⧼");
				printCharToScreen(scr,"⧼");
				if(config.tam==10)printCharToScreen(scr,"⧼");
			}
			else{
				if(config.tam==10)printCharToScreen(scr,"⧼");
				printCharToScreen(scr,"⧼");
				printCharToScreen(scr,"⧼");
				printCharToScreen(scr,"⧼");
				if(config.tam==10)printCharToScreen(scr,"⧼");
			}
		}
		screenMove(scr,baseX,baseY+(2*y)+1);
		for(int x=0;x<config.tam;x++){
			if(map[x][y].val && (hide!=1 || map[x][y].val2)){
				if(x-1>=0 && map[x-1][y].val3 == map[x][y].val3){
					scr->cur.FGcolor=COLOR_GREEN;
					if(config.tam==10)printCharToScreen(scr,"▓");
					printCharToScreen(scr,"▓");
				}
				else{
					if(config.tam==10)printCharToScreen(scr,"⧼");
					printCharToScreen(scr,"⧼");
					scr->cur.FGcolor=COLOR_GREEN;
				}
				scr->cur.bold=-1;
				printCharToScreen(scr,"▓");
				//sprintf(str,"%d",map[x][y].val3);
				//printStrToScreen(scr,str);
				scr->cur.bold=0;
				if(x+1<config.tam && map[x+1][y].val3 == map[x][y].val3){
					printCharToScreen(scr,"▓");
					if(config.tam==10)printCharToScreen(scr,"▓");
					scr->cur.FGcolor=COLOR_DEF;
				}
				else{
					scr->cur.FGcolor=COLOR_DEF;
					printCharToScreen(scr,"⧼");
					if(config.tam==10)printCharToScreen(scr,"⧼");
				}
			}
			// if(map[x][y].val){
			// 	sprintf(str,"  %2d ",map[x][y].val3);
			// 	printStrToScreen(scr,str);
			// }
			else{
				if(config.tam==10)printCharToScreen(scr,"⧼");
				printCharToScreen(scr,"⧼");
				printCharToScreen(scr," ");
				printCharToScreen(scr,"⧼");
				if(config.tam==10)printCharToScreen(scr,"⧼");
			}
		}
		scr->cur.BGcolor=COLOR_DEF;
		sprintf(str," %2d",y+1);
		printStrToScreen(scr,str);
	}
	if(coords){//colocabarcopl
		if(!colocaBarco(config,map,-1,coords))color=COLOR_BRED;
		else color=COLOR_BYELLOW;
		if(coords[3]==-3){
			if(coords[2]==0){
				colorScreenCoord(scr,config,map,coords[0]  ,coords[1]  ,baseX,baseY,color);
				colorScreenCoord(scr,config,map,coords[0]+1,coords[1]-1,baseX,baseY,color);
				colorScreenCoord(scr,config,map,coords[0]+2,coords[1]  ,baseX,baseY,color);
				scr->cur.FGcolor=COLOR_DEF;
			}
			if(coords[2]==1){
				colorScreenCoord(scr,config,map,coords[0]  ,coords[1]  ,baseX,baseY,color);
				colorScreenCoord(scr,config,map,coords[0]+1,coords[1]+1,baseX,baseY,color);
				colorScreenCoord(scr,config,map,coords[0]  ,coords[1]+2,baseX,baseY,color);
				scr->cur.FGcolor=COLOR_DEF;
			}
			if(coords[2]==2){
				colorScreenCoord(scr,config,map,coords[0]  ,coords[1]  ,baseX,baseY,color);
				colorScreenCoord(scr,config,map,coords[0]-1,coords[1]+1,baseX,baseY,color);
				colorScreenCoord(scr,config,map,coords[0]-2,coords[1]  ,baseX,baseY,color);
				scr->cur.FGcolor=COLOR_DEF;
			}
			if(coords[2]==3){
				colorScreenCoord(scr,config,map,coords[0]  ,coords[1]  ,baseX,baseY,color);
				colorScreenCoord(scr,config,map,coords[0]-1,coords[1]-1,baseX,baseY,color);
				colorScreenCoord(scr,config,map,coords[0]  ,coords[1]-2,baseX,baseY,color);
				scr->cur.FGcolor=COLOR_DEF;
			}
		}
		else{
			if(coords[2]==0)for(int j=0;j<coords[3];j++)colorScreenCoord(scr,config,map,coords[0]+j,coords[1],baseX,baseY,color);
			if(coords[2]==1)for(int j=0;j<coords[3];j++)colorScreenCoord(scr,config,map,coords[0],coords[1]+j,baseX,baseY,color);
			if(coords[2]==2)for(int j=0;j<coords[3];j++)colorScreenCoord(scr,config,map,coords[0]-j,coords[1],baseX,baseY,color);
			if(coords[2]==3)for(int j=0;j<coords[3];j++)colorScreenCoord(scr,config,map,coords[0],coords[1]-j,baseX,baseY,color);
			scr->cur.FGcolor=COLOR_DEF;
		}
		if(config.tam==10)screenMove(scr,baseX+(coords[0]*5)+2,baseY+(coords[1]*2)+1);
		if(config.tam==15)screenMove(scr,baseX+(coords[0]*3)+1,baseY+(coords[1]*2)+1);
	}
};

void moveCursor(screen* scr,int ch,config_ config,int* x,int* y){
	if(isUp(ch) && (*y)>0){
		(*y)--;
		(scr->curPos.y)--;
	}
	if(isDown(ch) && (*y)+1<config.tam){
		(*y)++;
		(scr->curPos.y)++;
	}
	if(isLeft(ch) && (*x)>0){
		(*x)--;
		(scr->curPos.x)--;
	}
	if(isRight(ch) && (*x)+1<config.tam){
		(*x)++;
		(scr->curPos.x)++;
	}
}

mapa alocaMapa(config_ config){
	mapa map = (mapa)malloc(config.tam*sizeof(coord*));
	if(!map)return NULL;
	memset(map,0,config.tam*sizeof(coord*));
	for(int i=0;i<config.tam;i++){
		map[i] = (coord*)malloc(config.tam*sizeof(coord));
		if(!map[i]){
			for(int j=0;j<i;j++){
				free(map[j]);
			}
			printf("Falha ao alocar memoria...\n");
			return NULL;
		}
		memset(map[i],0,config.tam*sizeof(coord));
	}
	return map;
}

void freeMap(config_ config,mapa map){
	for(int i=0;i<config.tam;i++)free(map[i]);
	free(map);
}

int geraMapa(config_ config,mapa map,int func(config_,mapa,int,int,int*)){
	int id=1;
	if(config.tam==10){
		if(func(config,map,5,1,&id) ||
		   func(config,map,4,1,&id) ||
		   func(config,map,3,2,&id) ||
		   func(config,map,2,3,&id) ||
		   func(config,map,1,4,&id))return 1;
	}
	else{
		if(func(config,map, 5,1,&id) ||
		   func(config,map, 4,2,&id) ||
		   func(config,map,-3,5,&id) ||
		   func(config,map, 2,3,&id) ||
		   func(config,map, 1,4,&id))return 1;
	}
	return 0;
}

int colocaBarcoAI(config_ config,mapa map,int tam,int quant,int *idP){
	int coords[3],id;
	coords[3]=tam;
	for(int i=0;i<quant;i++){
		id=*idP;
		(*idP)++;
		//printf("gen tam %d id %d i %d\n",tam,id,i);
		do{
			coords[0]=rand()%config.tam;
			coords[1]=rand()%config.tam;
			coords[2]=rand()%4;
			//printf("x%d y%d rot%d\n",coords[0],coords[1],coords[2]);
		}while(!colocaBarco(config,map,id,coords));
	}
	return 0;
}

int colocaBarcoPL(config_ config,mapa map,int tam,int quant,int *id){
	screen *scr = screenAlloc(300,38);
	char str[101];
	int coords[4],ch;
	memset(coords,0,3*sizeof(int));
	coords[3]=tam;
	printStrToScreenPos(scr,"Batalha Naval",0,0);
	printStrToScreenPos(scr,"Aperte enter para colocar o barco, espaço para rotacionar ou esc para sair.",1,37);
	for(int i=0;i<quant;i++){
		sprintf(str,"Colocando barco de tamanho %d",tam);
		printStrToScreenPos(scr,str,1,1);
		do{
			printMapToScreen(scr,config,map,2,4,3,coords);
			clear();
			printScreen(scr);
			ch=readKey();
			if(ch==' ')coords[2] = ((coords[2])+1)%4;
			if(ch==KEY_ESC)return 1;
			moveCursor(scr,ch,config,&coords[0],&coords[1]);
		}while(ch!='\n' || !colocaBarco(config,map,*id,coords));
		(*id)++;
	}
	return 0;
}

int bomb(screen* scr,config_ config,mapa map,int x,int y,int baseX,int baseY){
	if(x<0 || x>=config.tam || y<0 || y>=config.tam || map[x][y].val2)return 0;
	map[x][y].val2=1;
	//do the screen
	if(!map[x][y].val){
		scr->cur.BGcolor=COLOR_BLUE;
		if(config.tam==10)printCharToScreenPos(scr,"⧽",baseX+(x*5)+2,baseY+(y*2)+2);
		if(config.tam==15)printCharToScreenPos(scr,"⧽",baseX+(x*3)+1,baseY+(y*2)+2);
		scr->cur.BGcolor=COLOR_DEF;
	}
	else{
		if(barcoDestruido(map,config,map[x][y].val3)){
			for(int i=0;i<config.tam;i++){
				for(int j=0;j<config.tam;j++){
					if(map[i][j].val3==map[x][y].val3){
						scr->cur.FGcolor=COLOR_RED;
						if(config.tam==10)printCharToScreenPos(scr,"▓",baseX+(i*5)+2,baseY+(j*2)+2);
						if(config.tam==15)printCharToScreenPos(scr,"▓",baseX+(i*3)+1,baseY+(j*2)+2);
						scr->cur.FGcolor=COLOR_BRED;
						if(i-1>=0 && map[i-1][j].val3==map[i][j].val3){
							if(config.tam==10){
								printCharToScreenPos(scr,"▓",baseX+(i*5)+1,baseY+(j*2)+2);
								printCharToScreenPos(scr,"▓",baseX+(i*5),baseY+(j*2)+2);
							}
							if(config.tam==15)printCharToScreenPos(scr,"▓",baseX+(i*3),baseY+(j*2)+2);
						}
						if(i+1<config.tam && map[i+1][j].val3==map[i][j].val3){
							if(config.tam==10){
								printCharToScreenPos(scr,"▓",baseX+(i*5)+3,baseY+(j*2)+2);
								printCharToScreenPos(scr,"▓",baseX+(i*5)+4,baseY+(j*2)+2);
							}
							if(config.tam==15)printCharToScreenPos(scr,"▓",baseX+(i*3)+2,baseY+(j*2)+2);
						}
						if(j-1>=0 && map[i][j-1].val3==map[i][j].val3){
							if(config.tam==10)printCharToScreenPos(scr,"▓",baseX+(i*5)+2,baseY+(j*2)+1);
							if(config.tam==15)printCharToScreenPos(scr,"▓",baseX+(i*3)+1,baseY+(j*2)+1);
						}
						scr->cur.FGcolor=COLOR_DEF;
					}
				}

			}
		}
		else{
			scr->cur.FGcolor=COLOR_YELLOW;
			if(config.tam==10)printCharToScreenPos(scr,"▓",baseX+(x*5)+2,baseY+(y*2)+2);
			if(config.tam==15)printCharToScreenPos(scr,"▓",baseX+(x*3)+1,baseY+(y*2)+2);
			scr->cur.FGcolor=COLOR_BYELLOW;
			if(x-1>=0 && map[x-1][y].val3==map[x][y].val3){
				if(config.tam==10){
					printCharToScreenPos(scr,"▓",baseX+(x*5)+1,baseY+(y*2)+2);
					printCharToScreenPos(scr,"▓",baseX+(x*5),baseY+(y*2)+2);
				}
				if(config.tam==15)printCharToScreenPos(scr,"▓",baseX+(x*3),baseY+(y*2)+2);
			}
			if(x+1<config.tam && map[x+1][y].val3==map[x][y].val3){
				if(config.tam==10){
					printCharToScreenPos(scr,"▓",baseX+(x*5)+3,baseY+(y*2)+2);
					printCharToScreenPos(scr,"▓",baseX+(x*5)+4,baseY+(y*2)+2);
				}
				if(config.tam==15)printCharToScreenPos(scr,"▓",baseX+(x*3)+2,baseY+(y*2)+2);
			}
			if(y-1>=0 && map[x][y-1].val3==map[x][y].val3){
				if(config.tam==10)printCharToScreenPos(scr,"▓",baseX+(x*5)+2,baseY+(y*2)+1);
				if(config.tam==15)printCharToScreenPos(scr,"▓",baseX+(x*3)+1,baseY+(y*2)+1);
			}
			if(y+1<config.tam && map[x][y+1].val3==map[x][y].val3){
				if(config.tam==10)printCharToScreenPos(scr,"▓",baseX+(x*5)+2,baseY+(y*2)+3);
				if(config.tam==15)printCharToScreenPos(scr,"▓",baseX+(x*3)+1,baseY+(y*2)+3);
			}
			scr->cur.FGcolor=COLOR_DEF;
		}
	}
	return 1;
}

void doAIturn(config_ config,mapa map,int* px,int* py){
	int x,y;

	switch(config.dif){
		//AAAAAA
		//case 3:
		//	break;
		//case 2:
		//	break;
		default:
			do{
				x=rand()%config.tam;
				y=rand()%config.tam;
			}while(map[x][y].val2);
	}

	*px=x;
	*py=y;
}

int jogo(config_ config){
	screen *scr = screenAlloc(300,38);
	int x=0,y=0,x2,y2,ch;
	mapa map[2];
	map[0] = alocaMapa(config);
	if(!map[0])return 1;
	map[1] = alocaMapa(config);
	if(!map[1]){
		freeMap(config,map[0]);
		return 2;
	}
	if(geraMapa(config,map[0],colocaBarcoPL))return 0;
	//if(geraMapa(config,map[0],colocaBarcoAI))return 0;
	if(geraMapa(config,map[1],colocaBarcoAI))return 0;
	printStrToScreenPos(scr,"Batalha Naval",0,0);
	scr->cur.FGcolor=COLOR_BLUE;
	printStrToScreenPos(scr,"Barcos Aliados",4,2);
	scr->cur.FGcolor=COLOR_RED;
	printStrToScreenPos(scr,"Barcos Inimigos",42+(2*config.tam),2);
	scr->cur.FGcolor=COLOR_DEF;
	printStrToScreenPos(scr,"Selecione as coordenadas e aperte: Enter para bombardear ou esc para sair.",1,37);
	printMapToScreen(scr,config,map[0],0,4,3,NULL);
	printMapToScreen(scr,config,map[1],1,42+(2*config.tam),3,NULL);
	//printMapToScreen(scr,config,map[1],0,42+(2*config.tam),3,NULL);
	do{
		if(config.tam==10)screenMove(scr,44+(2*config.tam)+(x*5),5+(y*2));
		if(config.tam==15)screenMove(scr,43+(2*config.tam)+(x*3),5+(y*2));
		do{
			clear();
			printScreen(scr);
			ch=readKey();
			if(ch==KEY_ESC)return 1;
			if(isUp(ch) && y>0){
				y--;
				(scr->curPos.y)--;
				(scr->curPos.y)--;
			}
			if(isDown(ch) && y+1<config.tam){
				y++;
				(scr->curPos.y)++;
				(scr->curPos.y)++;
			}
			if(isLeft(ch) && x>0){
				x--;
				if(config.tam==10)(scr->curPos.x)-=5;
				if(config.tam==15)(scr->curPos.x)-=3;
			}
			if(isRight(ch) && x+1<config.tam){
				x++;
				if(config.tam==10)(scr->curPos.x)+=5;
				if(config.tam==15)(scr->curPos.x)+=3;
			}
		}while(ch!='\n' || !bomb(scr,config,map[1],x,y,42+(2*config.tam),3));
		doAIturn(config,map[0],&x2,&y2);
		bomb(scr,config,map[0],x2,y2,4,3);
	}while(!perdeu(map[0],config) && !perdeu(map[1],config));
	if(!perdeu(map[0],config))printStrToScreenPos(scr,"Parabéns, voce venceu!",1,36);
	else printStrToScreenPos(scr,"Voce perdeu, que noob.",1,36);
	printStrToScreenPos(scr,"Aperte qualquer botão para voltar ao menu principal.                      ",1,37);
	clear();
	printScreen(scr);
	getch();
	closeScreen(scr);
	freeMap(config,map[0]);
	freeMap(config,map[1]);
	return 0;
}

int configura(config_ *config){
	int id=0,cur=1,ch;
	menu_opt *curMenu,*menu=NULL;
	screen *scr = screenAlloc(300,38),*scr2=NULL;
	printStrToScreenPos(scr,"Configurações:",1,1);
	screenMove(scr,0,3);
	menu = insereMenuOpt(menu,menuOptionButton("Tamanho do tabuleiro",&id),0);
	menu = insereMenuOpt(menu,menuOptionBox("10x10",&id,(config->tam==10)?1:0),1);
	menu = insereMenuOpt(menu,menuOptionBox("15x15",&id,(config->tam==15)?1:0),1);
	menu = insereMenuOpt(menu,menuOptionButton("Dificuldade",&id),0);
	menu = insereMenuOpt(menu,menuOptionBox("Fácil",&id,(config->dif==1)?1:0),1);
	menu = insereMenuOpt(menu,menuOptionBox("Média(WIP)",&id,(config->dif==2)?1:0),1);
	menu = insereMenuOpt(menu,menuOptionBox("Difícil(WIP)",&id,(config->dif==3)?1:0),1);
	menu = insereMenuOpt(menu,menuOptionText(" ",&id),0);
	menu = insereMenuOpt(menu,menuOptionButton("Sair",&id),0);
	while(1){
		scr2 = menuToScreen(scr,scr2,menu,1,3);
		clear();
		printScreen(scr2);
		//printf("cur = %d\n",cur);
		while(!(ch = readKey()));
		menuMove(menu,scr,ch,&cur);
		curMenu=menu;
		for(int i=1;curMenu->next && i<cur;i++)curMenu=curMenu->next;
		if(ch=='\n'){
			if(cur==1){
				menuToggleHidden(menu,2);
				menuToggleHidden(menu,3);
			}
			if(cur==2){
				config->tam=10;
				curMenu->val=1;
				curMenu->next->val=0;
			}
			if(cur==3){
				config->tam=15;
				curMenu->val=1;
				curMenu->prev->val=0;
			}
			if(cur==4){
				menuToggleHidden(menu,5);
				menuToggleHidden(menu,6);
				menuToggleHidden(menu,7);
			}
			if(cur==5){
				config->dif=1;
				curMenu->val=1;
				curMenu->next->val=0;
				curMenu->next->next->val=0;
			}
			if(cur==6){
				config->dif=2;
				curMenu->prev->val=0;
				curMenu->val=1;
				curMenu->next->val=0;
			}
			if(cur==7){
				config->dif=3;
				curMenu->prev->prev->val=0;
				curMenu->prev->val=0;
				curMenu->val=1;
			}
			if(cur==9){
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
	config.tam=10;
	config.dif=1;
	srand(time(NULL));
	int id=0,cur=1,ch;
	menu_opt *menu=NULL;
	screen *scr = screenAlloc(300,38),*scr2=NULL;
	printStrToScreenPos(scr,"Batalha naval",1,1);
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
	printStrToScreenPos(scr,"Bem vindo à batalha naval.",2,2);
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
