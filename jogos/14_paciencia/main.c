#include "../batata.h"

typedef struct{
	int quant;
	char pilhaReserva;
} config_;

typedef struct{
	char mostra;
	char num;//usando char pois o numero maximo é 13
	char naipe;//usando char pois o numero maximo é 4
}carta;

typedef struct fila_{
	struct fila_ *next;
	carta card;
}fila;

typedef struct fila_header_{
	fila* inicio;
	fila* fim;
	int tam;
}fila_header;

fila_header* allocH(){
	fila_header* header = (fila_header*)malloc(sizeof(fila_header));
	if(header)memset(header,0,sizeof(fila_header));
	return header;
}

fila* allocF(char num,char naipe){
	fila *list = (fila*)malloc(sizeof(fila));
	if(!list)return NULL;
	list->card.num=num;
	list->card.naipe=naipe;
	list->card.mostra=1;
	list->next=NULL;
	return list;
}

fila_header* freeF(fila_header* header){
	if(!header)return NULL;
	fila *list,*temp;
	list=header->inicio;
	while(list){
		temp=list;
		list=list->next;
		free(temp);
	}
	free(header);
	return NULL;
}

fila_header* filaRemove(fila_header* header){
	if(!header || !header->inicio)return header;
	fila* temp = header->inicio;
	if(!temp->next)header->fim = NULL;
	header->inicio = temp->next;
	free(temp);
	header->tam--;
	return header;
}

fila_header* filaPop(fila_header* header,carta* card){
	if(!header)return NULL;
	fila* list = header->inicio;
	if(!list)return NULL;
	if(!list->next){
		if(card)*card=list->card;
		free(list);
		header->inicio=NULL;
		header->fim=NULL;
		header->tam--;
		return header;
	}
	for(;list->next->next;list=list->next);
	if(card)*card=list->next->card;
	free(list->next);
	list->next=NULL;
	header->fim = list;
	header->tam--;
	return header;
}

carta* filaTop(fila_header* header){
	if(!header->fim)return NULL;
	return &(header->fim->card);
}

fila_header* filaAdd(fila_header* header,fila* fila){
	if(!header)return NULL;
	if(!header->inicio || !header->fim){
		header->inicio = fila;
		header->fim = fila;
		header->tam++;
		return header;
	}
	header->fim->next=fila;
	header->fim=fila;
	header->tam++;
	return header;
}

fila_header* filaCpy(fila_header* origem,fila_header* destino){
	if(!origem)return destino;
	if(destino)destino = freeF(destino);
	destino = allocH();
	for(fila* list=origem->inicio;list;list=list->next){
		destino = filaAdd(destino,allocF(list->card.num,list->card.naipe));
	}
	return destino;
}

fila* percorreFila(fila_header* header,int n){
	fila* list = header->inicio;
	for(int i=0;i<n;i++)list=list->next;
	return list;
}

fila_header* filaRand(fila_header* header){
	int rnd,rnd2;
	fila_header* temp;
	fila* list;
	for(int i=0;i<header->tam*2;i++){
		temp = allocH();
		rnd=header->tam/3 + rand()%header->tam/3;
		rnd2=rnd+(rand()%(header->tam-rnd));
		list=percorreFila(header,rnd-1);
		temp->inicio = list->next;
		temp->fim=percorreFila(header,rnd2);
		list->next=temp->fim->next;
		temp->fim->next=header->inicio;
		header->inicio=temp->inicio;
		free(temp);
	}
	return header;
}

void printCardToScreen(screen* scr,carta* card_,int baseX,int baseY){
	char str[10];
	scr->cur.FGcolor=COLOR_DEFAULT;
	scr->cur.BGcolor=COLOR_DEFAULT;
	if(card_ && card_->num==0){
		scr->cur.BGcolor=COLOR_BBLACK;
	 	for(int y=0;y<6;y++)if(baseY+y<scr->alt)printStrToScreenPos(scr,"        ",baseX,baseY+y);
	}
	else{
	 	for(int y=0;y<6;y++)if(baseY+y<scr->alt)if(!scr->vect[baseX+6][baseY+y].val[0])printStrToScreenPos(scr," ",baseX+8,baseY+y);
		scr->cur.FGcolor=COLOR_BLACK;
		scr->cur.BGcolor=COLOR_BWHITE;
	 	for(int y=0;y<6;y++)if(baseY+y<scr->alt)printStrToScreenPos(scr,"        ",baseX,baseY+y);
		if(baseY<scr->alt)printCharToScreenPos(scr,"╔",baseX,baseY);
		if(baseY<scr->alt)for(int x=1;x<=6;x++)printCharToScreenPos(scr,"═",baseX+x,baseY);
		if(baseY<scr->alt)printCharToScreenPos(scr,"╗",baseX+7,baseY);
		for(int y=1;y<=4;y++){
			if(baseY+y<scr->alt)printCharToScreenPos(scr,"║",baseX,baseY+y);
			if(baseY+y<scr->alt)printCharToScreenPos(scr,"║",baseX+7,baseY+y);
		}
		if(baseY+5<scr->alt)printCharToScreenPos(scr,"╚",baseX,baseY+5);
		if(baseY+5<scr->alt)for(int x=1;x<=6;x++)printCharToScreenPos(scr,"═",baseX+x,baseY+5);
		if(baseY+5<scr->alt)printCharToScreenPos(scr,"╝",baseX+7,baseY+5);

		if(card_ && card_->mostra){
			carta card=*card_;
			if(baseY+1<scr->alt){
				if     (card.num==11)printStrToScreenPos(scr,"J",baseX+3,baseY+1);
				else if(card.num==12)printStrToScreenPos(scr,"Q",baseX+3,baseY+1);
				else if(card.num==13)printStrToScreenPos(scr,"K",baseX+3,baseY+1);
				else if(card.num==1) printStrToScreenPos(scr,"A",baseX+3,baseY+1);
				else if(card.num){
					sprintf(str,"%d",card.num);
					printStrToScreenPos(scr,str,baseX+3,baseY+1);
				}

				if(card.naipe%2)scr->cur.FGcolor=COLOR_RED;
				if(card.naipe==1)printCharToScreenPos(scr,"♥",baseX+1,baseY+1);
				if(card.naipe==2)printCharToScreenPos(scr,"♠",baseX+1,baseY+1);
				if(card.naipe==3)printCharToScreenPos(scr,"♦",baseX+1,baseY+1);
				if(card.naipe==4)printCharToScreenPos(scr,"♣",baseX+1,baseY+1);
			}
		}
		else{
			//scr->cur.BGcolor=COLOR_BBLUE;
		 	for(int y=1;y<=4;y++){
		 		if(baseY+y<scr->alt){
					scr->cur.FGcolor=COLOR_RED;
			 		printCharToScreenPos(scr,"⟪",baseX+2,baseY+y);
			 		printCharToScreenPos(scr,"⟪",baseX+4,baseY+y);
			 		printCharToScreenPos(scr,"⟪",baseX+6,baseY+y);
					scr->cur.FGcolor=COLOR_BLACK;
			 		printCharToScreenPos(scr,"⟪",baseX+1,baseY+y);
			 		printCharToScreenPos(scr,"⟪",baseX+3,baseY+y);
			 		printCharToScreenPos(scr,"⟪",baseX+5,baseY+y);
		 		}
		 	}
		}
	}
	scr->cur.FGcolor=COLOR_DEFAULT;
	scr->cur.BGcolor=COLOR_DEFAULT;
	screenMove(scr,-1,-1);

}

void printCardsToScreen(screen* scr,config_ config,fila_header* pilhaReserva,fila_header* pilhaDescarte,fila_header** pilhas,int objetivo[4],carta* extra,int baseX,int baseY){
	carta card;
	card.num=0;
	card.mostra=1;
	if(pilhaReserva->tam)printCardToScreen(scr,NULL,baseX,baseY);
	else printCardToScreen(scr,&card,baseX,baseY);

	for(int i=0;i<config.quant;i++)if(extra[i].num)printCardToScreen(scr,&(extra[i]),baseX+9+(6*i),baseY);

	for(int i=0;i<7;i++){
		for(int j=0;j<pilhas[i]->tam;j++){
			printCardToScreen(scr,&(percorreFila(pilhas[i],j)->card),baseX+(14*i)+j,baseY+7+(2*j));
		}
	}

	for(int i=0;i<4;i++){
		card.naipe=i+1;
		card.num=objetivo[i];
		printCardToScreen(scr,&card,baseX+(14*(i+3)),baseY);
	}

	//printCardToScreen(scr,NULL,baseX+1,baseY+2);
	//printCardToScreen(scr,NULL,baseX+2,baseY+4);
	//printCardToScreen(scr,NULL,baseX+3,baseY+6);
	//printCardToScreen(scr,&pilhaReserva->inicio->next->next->card,baseX+4,baseY+8);
 }

void encheFila(fila_header* header,int cont){
	for(int i=0;i<cont;i++){
		for(int naipe=1;naipe<=4;naipe++){
			for(int num=1;num<=13;num++){
				header = filaAdd(header,allocF(num,naipe));
			}
		}
	}
}

void moveCursor(screen* scr,config_ config,int cursor[2],fila_header* pilhaReserva,fila_header* pilhaDescarte,fila_header** pilhas,int objetivo[4],carta* extra,int baseX,int baseY,int ch,int* selected){
	if(isUp(ch)){
		if(cursor[0]>5 && pilhas[cursor[0]-6]->tam-1>cursor[1]){
			cursor[1]++;
		}
	}
	if(isDown(ch)){
		if(cursor[1]>0){
			cursor[1]--;
		}
	}
	if(isLeft(ch)){
		cursor[0]--;
		cursor[1]=0;
		if(cursor[0]==1 && !extra[0].num)cursor[0]--;
		if(cursor[0]==5)cursor[0]=12;
		if(cursor[0]==5)cursor[0]=12;
		if(cursor[0]<0)cursor[0]=5;
	}
	if(isRight(ch)){
		cursor[0]++;
		cursor[1]=0;
		if(cursor[0]==1 && !extra[0].num)cursor[0]++;
		if(cursor[0]==6)cursor[0]=0;
		if(cursor[0]>12)cursor[0]=6;
	}
	if(ch==' '){
		if(cursor[0]<6)cursor[0]=6;
		else cursor[0]=0;
	}
	if(selected[0]!=-1){
		if     (selected[0]==0)scr->vect[1+baseX][baseY].inverted=1;
		else if(selected[0]==1){
			for(int i=config.quant-1;i>=0;i--){
				if(extra[i].num){
					scr->vect[1+baseX+9+(6*i)][baseY].inverted=1;
					break;
				}
			}
		}
		else if(selected[0]<=5)scr->vect[1+baseX+(14*((selected[0]-2)+3))][baseY].inverted=1;
		else if(selected[0]<13)scr->vect[1+baseX+(14*(selected[0]-6))+pilhas[selected[0]-6]->tam-selected[1]-1][baseY+7+(2*(max(1,pilhas[selected[0]-6]->tam)-selected[1]-1))].inverted=1;
	}
	if     (cursor[0]==0)screenMove(scr,baseX,baseY);
	else if(cursor[0]==1){
		for(int i=config.quant-1;i>=0;i--){
			if(extra[i].num){
				screenMove(scr,baseX+9+(6*i),baseY);
				break;
			}
		}
	}
	else if(cursor[0]<=5)screenMove(scr,baseX+(14*((cursor[0]-2)+3)),baseY);
	else if(cursor[0]<13)screenMove(scr,baseX+(14*(cursor[0]-6))+max(1,pilhas[cursor[0]-6]->tam)-cursor[1]-1,baseY+7+(2*(max(1,pilhas[cursor[0]-6]->tam)-cursor[1]-1)));
}

int canMove(fila* list){
	if(!list || !list->card.mostra)return 0;
	if(!list->next)return 1;
	if(list->card.naipe%2 != list->next->card.naipe%2 && list->card.num-1 == list->next->card.num)return canMove(list->next);
	return 0;
}

void winScreen(screen* scr,int* objetivo){
	int naipe,rnd;
	carta card;
	card.mostra=1;
	printStrToScreenPos(scr,"Parabens, voce ganhou!",101,4);
	printCharToScreenPos(scr,"é",106,4);
	screenMove(scr,-1,-1);
	clear();
	printScreen(scr);
	usleep(500000);
	for(int i=0;i<4;i++){
		rnd=rand()%2;
		do{
			naipe=1+rand()%4;
		}while(!objetivo[naipe]);
		card.naipe=naipe;
		for(int n=13;n>0;n--){
			card.num=n;
			if(rnd)printCardToScreen(scr,&card,(13-n)+29+(14*naipe),max(0,13-n-7)+max(0,13-n-4)+(13-n)+1);
			else   printCardToScreen(scr,&card,-(13-n)+29+(14*naipe),max(0,13-n-7)+max(0,13-n-4)+(13-n)+1);
			printScreen(scr);fflush(stdout);
			usleep((500000*n)/13);
		}
		objetivo[naipe]=0;
	}
}

int jogo(config_ config){
	//char str[101];
	screen *scr = screenAlloc(300,38);
	fila_header* temp;
	//fila* tempList;

	fila_header* pilhaReserva = allocH();
	if(!pilhaReserva)return 0;
	encheFila(pilhaReserva,1);
	pilhaReserva = filaRand(pilhaReserva);

	fila_header* pilhaDescarte = allocH();
	if(!pilhaDescarte){
		freeF(pilhaReserva);
		return 0;
	}

	fila_header* pilhas[7];
	for(int i=0;i<7;i++){
		pilhas[i] = allocH();
		if(!pilhas[i]){
			for(int j=0;j<i;j++)freeF(pilhas[j]);
			freeF(pilhaReserva);
			freeF(pilhaDescarte);
			return 0;
		}
		for(int j=0;j<=i;j++){
			pilhas[i] = filaAdd(pilhas[i],allocF(pilhaReserva->inicio->card.num,pilhaReserva->inicio->card.naipe));
			pilhas[i]->fim->card.mostra=0;
			filaRemove(pilhaReserva);
		}
		pilhas[i]->fim->card.mostra=1;
	}

	int objetivo[4];
	memset(objetivo,0,4*sizeof(int));
	//for(int i=0;i<4;i++)objetivo[i]=13;
	//for(int i=0;i<4;i++)objetivo[i]=rand()%14;

	carta extra[config.quant];
	for(int i=0;i<config.quant;i++){
		extra[i].num=0;
		//extra[i].num=1+(rand()%13);
		//extra[i].naipe=1+(rand()%4);
		extra[i].mostra=1;
	}

	int cursor[2],selected[2],ch;
	cursor[0]=cursor[1]=0;
	selected[0]=selected[1]=-1;

	resetScreen(scr);
	do{
		clear();
		scr->def.BGcolor=COLOR_GREEN;
		scr->def.FGcolor=COLOR_BLACK;
		printStrToScreenPos(scr,"Aperte enter para (de)selecionar uma carta",100,1);
		printStrToScreenPos(scr,"espaço para subir/descer o cursor",100,2);
		printStrToScreenPos(scr,"ou esc para sair.",100,3);
		printCardsToScreen(scr,config,pilhaReserva,pilhaDescarte,pilhas,objetivo,extra,1,1);
		//sprintf(str,"cursor = {%d,%d}",cursor[0],cursor[1]);
		//printStrToScreenPos(scr,str,1,28);
		//sprintf(str,"selected = {%d,%d}",selected[0],selected[1]);
		//printStrToScreenPos(scr,str,1,29);
		moveCursor(scr,config,cursor,pilhaReserva,pilhaDescarte,pilhas,objetivo,extra,1,1,0,selected);
		printScreen(scr);
		resetScreen(scr);
		ch=readKey();
		if(ch==KEY_ESC)return 0;
		moveCursor(scr,config,cursor,pilhaReserva,pilhaDescarte,pilhas,objetivo,extra,1,1,ch,selected);
		if(ch=='\n'){
			if(cursor[0]==0){//pilha reserva
				if(selected[0]==1)selected[0]=-1;
				//AAAAAA
				if(config.pilhaReserva || 1){//mudar esse para para checar se existem jogadas disponiveis
					if(pilhaReserva->tam || extra[0].naipe){
						for(int i=0;i<config.quant;i++){
							if(extra[i].num)pilhaDescarte = filaAdd(pilhaDescarte,allocF(extra[i].num,extra[i].naipe));
							if(pilhaReserva->tam){
								extra[i].num=pilhaReserva->inicio->card.num;
								extra[i].naipe=pilhaReserva->inicio->card.naipe;
							}
							else{
								extra[i].num=0;
								extra[i].naipe=0;
							}
							filaRemove(pilhaReserva);
						}
					}
					if(!pilhaReserva->tam && !extra[0].naipe){
						temp = pilhaReserva;
						pilhaReserva = pilhaDescarte;
						pilhaDescarte = temp;
					}
				}
			}
			else if(cursor[0]==1){//extra
				for(int i=config.quant-1;i>=0;i--){
					if(extra[i].num){
						if(selected[0]==1)selected[0]=-1;
						else if(selected[0]==-1)selected[0]=1;
						else{
							printStrToScreenPos(scr,"Nao e possivel colocar cartas na pilha extra.",101,4);
							printCharToScreenPos(scr,"ã",102,4);
							printCharToScreenPos(scr,"é",105,4);
						}
						break;
					}
				}
			}
			else if(cursor[0]<=5){//objetivo
				if(selected[0]==cursor[0])selected[0]=-1;
				else if(selected[0]==-1){
					if(objetivo[cursor[0]-2])selected[0]=cursor[0];
					else{
						printStrToScreenPos(scr,"Nao ha nenhuma carta aqui para ser movida.",101,4);
						printCharToScreenPos(scr,"ã",102,4);
						printCharToScreenPos(scr,"á",106,4);
					}
				}
				else{
					if(selected[0]==1){
						for(int i=config.quant-1;i>-2;i--){
							if(i==-1){
								printStrToScreenPos(scr,"ERRO",101,4);
								break;
							}
							if(extra[i].num){
								if(extra[i].num == objetivo[cursor[0]-2]+1 && extra[i].naipe==cursor[0]-1){
									extra[i].num=0;
									objetivo[cursor[0]-2]++;
									selected[0]=-1;
								}
								else{
									printStrToScreenPos(scr,"Nao e possivel colocar essa carta aqui.",101,4);
									printCharToScreenPos(scr,"ã",102,4);
									printCharToScreenPos(scr,"é",105,4);
								}
								break;
							}
						}
					}
					else if(selected[0]>5){
						if(selected[1]){
							printStrToScreenPos(scr,"Nao e possivel mover mais de uma carta por vez para o objetivo.",101,4);
							printCharToScreenPos(scr,"ã",102,4);
							printCharToScreenPos(scr,"é",105,4);
						}
						else{
							if(pilhas[selected[0]-6]->fim->card.num==objetivo[cursor[0]-2]+1 && pilhas[selected[0]-6]->fim->card.naipe==cursor[0]-1){
								filaPop(pilhas[selected[0]-6],NULL);
								objetivo[cursor[0]-2]++;
								selected[0]=-1;
							}
							else{
								printStrToScreenPos(scr,"Nao e possivel colocar essa carta aqui.",101,4);
								printCharToScreenPos(scr,"ã",102,4);
								printCharToScreenPos(scr,"é",105,4);
							}
						}
					}
					else printStrToScreenPos(scr,"ERRO.",101,4);
				}
			}
			else if(cursor[0]<13){//pilhas
				if(selected[0]==cursor[0])selected[0]=-1;
				else if(selected[0]==-1){
					if(pilhas[cursor[0]-6]->tam==0){
						printStrToScreenPos(scr,"Nao ha nenhuma carta aqui para ser movida.",101,4);
						printCharToScreenPos(scr,"ã",102,4);
						printCharToScreenPos(scr,"á",106,4);
					}
					else if(!pilhas[cursor[0]-6]->fim->card.mostra)pilhas[cursor[0]-6]->fim->card.mostra=1;
					else if(canMove(percorreFila(pilhas[cursor[0]-6],pilhas[cursor[0]-6]->tam-cursor[1]-1))){
						selected[0]=cursor[0];
						selected[1]=cursor[1];
					}
					else{
						printStrToScreenPos(scr,"Nao e possivel mover essa carta.",101,4);
						printCharToScreenPos(scr,"ã",102,4);
						printCharToScreenPos(scr,"é",105,4);
					}
				}
				else{
					if(pilhas[cursor[0]-6]->fim && !pilhas[cursor[0]-6]->fim->card.mostra){
						printStrToScreenPos(scr,"Nao e possivel mover cartas para cima de uma virada para baixo.",101,4);
						printCharToScreenPos(scr,"ã",102,4);
						printCharToScreenPos(scr,"é",105,4);
					}
					else if(cursor[1]){
						printStrToScreenPos(scr,"Nao e possivel mover carta(s) para o meio de uma pilha.",101,4);
						printCharToScreenPos(scr,"ã",102,4);
						printCharToScreenPos(scr,"é",105,4);
					}
					else if(selected[0]==1){
						for(int i=config.quant-1;i>-2;i--){
							if(i==-1){
								printStrToScreenPos(scr,"ERRO",101,4);
								break;
							}
							if(extra[i].num){
								if(pilhas[cursor[0]-6]->tam && extra[i].num == pilhas[cursor[0]-6]->fim->card.num-1 && extra[i].naipe%2!=pilhas[cursor[0]-6]->fim->card.naipe%2){
									filaAdd(pilhas[cursor[0]-6],allocF(extra[i].num,extra[i].naipe));
									extra[i].num=0;
									selected[0]=-1;
								}
								else if(extra[i].num == 13 && !pilhas[cursor[0]-6]->tam){
									filaAdd(pilhas[cursor[0]-6],allocF(extra[i].num,extra[i].naipe));
									extra[i].num=0;
									selected[0]=-1;
								}
								else{
									printStrToScreenPos(scr,"Nao e possivel colocar essa carta aqui.",101,4);
									printCharToScreenPos(scr,"ã",102,4);
									printCharToScreenPos(scr,"é",105,4);
								}
								break;
							}
						}
					}
					else if(selected[0]<=5){
						if(pilhas[cursor[0]-6]->tam && objetivo[selected[0]-2] == pilhas[cursor[0]-6]->fim->card.num-1 && selected[0]%2!=pilhas[cursor[0]-6]->fim->card.naipe-1%2){
							filaAdd(pilhas[cursor[0]-6],allocF(objetivo[selected[0]-2],selected[0]-1));
							objetivo[selected[0]-2]--;
							selected[0]=-1;
						}
						else if(objetivo[selected[0]-2] == 13 && !pilhas[cursor[0]-6]->tam){
							filaAdd(pilhas[cursor[0]-6],allocF(objetivo[selected[0]-2],selected[0]-2)+1);
							objetivo[selected[0]-2]--;
							selected[0]=-1;
						}
						else{
							printStrToScreenPos(scr,"Nao e possivel colocar essa carta aqui.",101,4);
							printCharToScreenPos(scr,"ã",102,4);
							printCharToScreenPos(scr,"é",105,4);
						}
					}
					else if(selected[0]<13){
						if(pilhas[cursor[0]-6]->tam && pilhas[selected[0]-6]->tam && percorreFila(pilhas[selected[0]-6],pilhas[selected[0]-6]->tam-selected[1]-1)->card.num == pilhas[cursor[0]-6]->fim->card.num-1 && percorreFila(pilhas[selected[0]-6],pilhas[selected[0]-6]->tam-selected[1]-1)->card.naipe%2!=pilhas[cursor[0]-6]->fim->card.naipe%2){
							selected[1]++;
							pilhas[selected[0]-6]->tam-=selected[1];
							pilhas[cursor[0]-6]->tam+=selected[1];
							if(!pilhas[selected[0]-6]->tam){
								pilhas[cursor[0]-6]->fim->next=pilhas[selected[0]-6]->inicio;
								pilhas[cursor[0]-6]->fim = pilhas[selected[0]-6]->fim;
								pilhas[selected[0]-6]->fim = pilhas[selected[0]-6]->inicio = NULL;
							}
							else{
								pilhas[selected[0]-6]->fim = percorreFila(pilhas[selected[0]-6],pilhas[selected[0]-6]->tam-1);
								pilhas[cursor[0]-6]->fim->next=pilhas[selected[0]-6]->fim->next;
								pilhas[cursor[0]-6]->fim = percorreFila(pilhas[cursor[0]-6],pilhas[cursor[0]-6]->tam-1);
								pilhas[selected[0]-6]->fim->next = NULL;
							}
							selected[0]=-1;
						}
						else if(pilhas[selected[0]-6]->tam && percorreFila(pilhas[selected[0]-6],pilhas[selected[0]-6]->tam-selected[1]-1)->card.num == 13 && !pilhas[cursor[0]-6]->tam){
							selected[1]++;
							if(pilhas[selected[0]-6]->tam == selected[1]){
								temp = pilhas[selected[0]-6];
								pilhas[selected[0]-6] = pilhas[cursor[0]-6];
								pilhas[cursor[0]-6] = temp;
							}
							else{
								pilhas[selected[0]-6]->tam-=selected[1];
								pilhas[cursor[0]-6]->tam=selected[1];
								pilhas[selected[0]-6]->fim = percorreFila(pilhas[selected[0]-6],pilhas[selected[0]-6]->tam-1);
								pilhas[cursor[0]-6]->inicio=pilhas[selected[0]-6]->fim->next;
								pilhas[cursor[0]-6]->fim = percorreFila(pilhas[cursor[0]-6],pilhas[cursor[0]-6]->tam-1);
								pilhas[selected[0]-6]->fim->next = NULL;
							}
							selected[0]=-1;
						}
						else{
							printStrToScreenPos(scr,"Nao e possivel colocar essa carta aqui.",101,4);
							printCharToScreenPos(scr,"ã",102,4);
							printCharToScreenPos(scr,"é",105,4);
						}
					}
					else{
						printStrToScreenPos(scr,"Nao e possivel mover essa(s) carta(s) para ca.",101,4);
						printCharToScreenPos(scr,"ã",102,4);
						printCharToScreenPos(scr,"é",105,4);
					}
				}
			}
			else printStrToScreenPos(scr,"ERRO.",101,4);
		}
	}while(objetivo[0]!=13 || objetivo[1]!=13 || objetivo[2]!=13 || objetivo[3]!=13);
	clear();
	scr->def.BGcolor=COLOR_GREEN;
	scr->def.FGcolor=COLOR_BLACK;
	printCardsToScreen(scr,config,pilhaReserva,pilhaDescarte,pilhas,objetivo,extra,1,1);
	printScreen(scr);
	winScreen(scr,objetivo);
	getch();
	return 0;
}

int configura(config_ *config){
	int id=0,cur=1,ch;
	menu_opt *curMenu,*menu=NULL;
	screen *scr = screenAlloc(300,38),*scr2=NULL;
	printStrToScreenPos(scr,"Configurações:",1,1);
	screenMove(scr,0,3);
	menu = insereMenuOpt(menu,menuOptionSlider("Quantidade de cartas retiradas por vez da pilha reserva",&id,1,3,config->quant),0);
	menu = insereMenuOpt(menu,menuOptionBox("Pode usar a pilha reserva mesmo tendo outras jogadas disponiveis",&id,config->pilhaReserva),0);
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
			if(cur==1)config->quant = menu->val;
			if(cur==2)config->pilhaReserva = menu->val;
			if(cur==4)break;
		}
	}
	freeMenu(menu);
	closeScreen(scr);
	closeScreen(scr2);
	return 0;
}

int mainMenu(){
	config_ config;
	config.quant=3;
	config.pilhaReserva=1;
	int id=0,cur=1,ch;
	menu_opt *menu=NULL;
	screen *scr = screenAlloc(300,38),*scr2=NULL;
	printStrToScreenPos(scr,"Paciência",1,1);
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
	srand(time(NULL));
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
