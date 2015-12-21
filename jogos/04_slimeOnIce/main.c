#include "../batata.h"

#define FILEPATH "04_slimeOnIce/"
#define SOI_ALT (13+2)
#define SOI_LAR (15+2)
#define SOI_PLAYER 1
#define SOI_OBJETIVO 2
#define SOI_BLOCO 3
#define SOI_UP 4
#define SOI_LEFT 5
#define SOI_DOWN 6
#define SOI_RIGHT 7
#define SOI_ESC 8
#define SEGUNDO 1000000

typedef struct {
	int jog_obj[4];
	int* field;
} level;

typedef struct {
	char name[21];
	int config[9];
	int score;
	int fases;
} playerT;

typedef struct {
	char name[21];
	int score;
	int fases;
} playerRank;



int readPlayerName(char* player){
	char str[31];
	int id=0,cur=1,ch;
	menu_opt *menu=NULL;
	screen *scr = screenAlloc(300,38),*scr2=NULL;
	menu = insereMenuOpt(menu,menuOptionButton("Sim",&id),0);
	menu = insereMenuOpt(menu,menuOptionButton("Não", &id),0);
	sprintf(str,"Confirma o nome '%s'?",player);
	printStrToScreenPos(scr,str,1,1);
	screenMove(scr,0,3);
	while(1){
		scr2 = menuToScreen(scr,scr2,menu,1,3);
		clear();
		printScreen(scr2);
		if(strcmp(player,"Default"))ch = readKey();
		else{
			cur=2;
			ch='\n';
		}
		printf("ch = %d",ch);
		menuMove(menu,scr,ch,&cur);
		if(ch=='\n'){
			if(cur==1)return 0;
			if(cur==2){
				resetScreen(scr);
				printStrToScreenPos(scr,"Digite um nome de até 20 caracteres.",1,1);
				do{
					clear();
					printScreen(scr);
					scanf("%20[^\n]",player);
					CLEARBUFFER;
					printStrToScreenPos(scr,"Digite um nome VALIDO de até 20 caracteres.",1,1);
				} while(!strcmp(player,"Default"));
				resetScreen(scr);
				sprintf(str,"Confirma o nome '%s'?",player);
				printStrToScreenPos(scr,str,1,1);
				screenMove(scr,0,3);
				cur=1;
			}
		}
	}
	freeMenu(menu);
	closeScreen(scr);
	closeScreen(scr2);
	return 0;
}

char getNewKey(int n,playerT player){
	char c;
	while(1){
		c = toLowerCase(getch());
		if(!(c==player.config[1]||c==player.config[2]||c==player.config[3]||c==player.config[4]||c==player.config[5]||c==player.config[6]||c==player.config[7]) && c>0 && c<256 && c!='q')return c;
		printf("Digite um caracter válido não ja utilizado (%c,%c,%c,%c,%c,%c,%c,%c)\n",player.config[1],player.config[2],player.config[3],player.config[4],player.config[5],player.config[6],player.config[7],player.config[8]);
	}
	return 0;
}

int printField(level fase, playerT player,int lvl,int movimentos){
	int x, y;
	char str[11];
	printf("   Jogador: \"%s\"\n",player.name);
	printf("      Fase:  %d\n",lvl+1);
	printf("Movimentos:  %d\n",movimentos);
	printf("             ");
	for(x=0;x<SOI_LAR;x++){printf("%c",player.config[SOI_BLOCO]);}printf("%c%c\n",player.config[SOI_BLOCO],player.config[SOI_BLOCO]);
	for(y=0;y<SOI_ALT;y++){
		printf("             %c",player.config[SOI_BLOCO]);
		for(x=0;x<SOI_LAR;x++){
			if(fase.field[x+(SOI_LAR*y)]==SOI_BLOCO){
				printf("%c",player.config[SOI_BLOCO]);
			}
			else{
				if(x==fase.jog_obj[0] && y==fase.jog_obj[1]){
					printf("%c",player.config[SOI_PLAYER]);
				}
				else{
					if(x==fase.jog_obj[2] && y==fase.jog_obj[3]){
						printf("%c",player.config[SOI_OBJETIVO]);
					}
					else{
						printf(" ");
					}
				}
			}
		}
		printf("%c",player.config[SOI_BLOCO]);
		if(y==3)printf("          Mover para Cima = %s",keyToStr(player.config[SOI_UP],str));
		if(y==4)printf("    Mover para a Esquerda = %s",keyToStr(player.config[SOI_LEFT],str));
		if(y==5)printf("         Mover para Baixo = %s",keyToStr(player.config[SOI_DOWN],str));
		if(y==6)printf("     Mover para a Direita = %s",keyToStr(player.config[SOI_RIGHT],str));
		if(y==8)printf("                  Jogador = %s",keyToStr(player.config[SOI_PLAYER],str));
		if(y==9)printf("                 Objetivo = %s",keyToStr(player.config[SOI_OBJETIVO],str));
		if(y==10)printf("                    Bloco = %s",keyToStr(player.config[SOI_BLOCO],str));
		if(y==11)printf(" Voltar ao menu principal = %s",keyToStr(player.config[SOI_ESC],str));
		printf("\n");
	}
	printf("             ");
	for(x=0;x<SOI_LAR;x++){printf("%c",player.config[SOI_BLOCO]);}printf("%c%c\n",player.config[SOI_BLOCO],player.config[SOI_BLOCO]);
	return 0;
}

int coordss(int coord[4]){
	if(coord[0]==coord[2] && coord[1]==coord[3])return 1;
	return 0;
}

int configDefault(char* file){
	FILE *fp;
	fp = fopen(file,"w");
	if(fp){
		fprintf(fp,"48 64 35\n500 501 502 503 27");
		fclose(fp);
		return 0;
	}
	return 1;
}

int move(int moveDir,level *fase,int* movimentos,playerT player,int lvl){
	int xDiff=0,yDiff=0,moved=0;
	if(moveDir==SOI_UP)   yDiff--;
	if(moveDir==SOI_LEFT) xDiff--;
	if(moveDir==SOI_DOWN) yDiff++;
	if(moveDir==SOI_RIGHT)xDiff++;
	while(
			!((*fase).jog_obj[0]==(*fase).jog_obj[2] && (*fase).jog_obj[1]==(*fase).jog_obj[3])
			&&
			!((*fase).field[((*fase).jog_obj[0]+xDiff+(SOI_LAR*((*fase).jog_obj[1]+yDiff)))]==SOI_BLOCO)
			&&
			!(((*fase).jog_obj[0]+xDiff)<0 || ((*fase).jog_obj[0]+xDiff)>=SOI_LAR || ((*fase).jog_obj[1]+yDiff)<0 || ((*fase).jog_obj[1]+yDiff)>=SOI_ALT)
		) {
		if(!moved)(*movimentos)++;
		moved=1;
		(*fase).jog_obj[0] += xDiff;
		(*fase).jog_obj[1] += yDiff;
		if(moveDir==SOI_UP || moveDir==SOI_DOWN)usleep(SEGUNDO/30.0);
		usleep(SEGUNDO/20.0);
		clear();
		printField((*fase),player,lvl,*movimentos);
	}
	return 0;
}

int getMove(level fase,int movimentos,playerT player,int lvl){
	int c,i;
	char str[11];
	do{
		c=toLowerCase(readKey());
		for(i=SOI_UP;i<=SOI_RIGHT;i++){
			if(c==player.config[i])return i;
		}
		if(c==player.config[SOI_ESC])return -1;
		clear();
		printField(fase,player,lvl,movimentos);
		printf("'%c' %d\n",c,c);
		printf("Digite um movimento válido. (%s, ",keyToStr(player.config[SOI_UP],str));
		printf("%s, ",keyToStr(player.config[SOI_LEFT],str));
		printf("%s, ",keyToStr(player.config[SOI_DOWN],str));
		printf("%s, ",keyToStr(player.config[SOI_RIGHT],str));
		printf("%s)\n",keyToStr(player.config[SOI_ESC],str));
	}while(1);
}

int fFase(int lvl, playerT player,FILE* fp){
		level fase;
		int vetor[SOI_LAR*SOI_ALT],i=0,n,x,y,movimentos=0,movement;
		for(y=0;y<SOI_ALT;y++){
			for(x=0;x<SOI_LAR;x++){
				vetor[x+(SOI_LAR*y)]=0;
			}
		}
		fscanf(fp,"%d,%d",&fase.jog_obj[0],&fase.jog_obj[1]);
		fgetc(fp);
		fscanf(fp,"%d,%d",&fase.jog_obj[2],&fase.jog_obj[3]);
		fgetc(fp);
		fscanf(fp,"%d",&n);
		fgetc(fp);
		while(i<n){
			fscanf(fp,"%d,%d",&x,&y);
			fgetc(fp);
			vetor[x+(SOI_LAR*y)]=SOI_BLOCO;
			i++;
		}
		fase.field = vetor;
		do{
			clear();
			printField(fase,player,lvl,movimentos);
			movement = getMove(fase,movimentos,player,lvl);
			if(movement==-1){
				printf("Você tem certeza que deseja voltar ao menu principal? Aperte '1' para confirmar ou qualquer outra tecla para rejeitar.\n");
				if(getch()=='1')return -1;
			}
			else{
				move(movement,&fase,&movimentos,player,lvl);
			}
		} while(!coordss(fase.jog_obj));
		return movimentos;
}

int addScore(playerT player){
	int i,found=-10,smallScore=0;
	FILE *fp;
	playerRank curPlayer,defaultPlayer,rank[10];
	strcpy(curPlayer.name,player.name);
	curPlayer.fases=player.fases;
	curPlayer.score=player.score;
	for(i=0;i<10;i++)rank[i].fases=0;
	if(!fCheck(FILEPATH"ranking.bin",1)){
		printf("Falha ao carregar Ranking, criando novo arquivo a partir do Default.\n");
		fp = fopen(FILEPATH"ranking.bin","wb");
		if(!fp){
			printf("Falha ao criar novo arquivo de ranking, seu score não será salvo.\n");
			usleep(SEGUNDO);
			return 1;
		}
		strcpy(defaultPlayer.name,"Default");
		defaultPlayer.fases=0;
		defaultPlayer.score=1000;
		for(i=0;i<10;i++)fwrite(&defaultPlayer,sizeof(playerRank),1,fp);
		rewind(fp);
		fwrite(&curPlayer,sizeof(playerRank),1,fp);
		fclose(fp);
	}
	else{
		fp = fopen(FILEPATH"ranking.bin","rb");
		for(i=0;i<10;i++)fread(&rank[i],sizeof(playerRank),1,fp);
		fclose(fp);
		for(i=0;i<10&&found==-10;i++){
			if(strcmp(rank[i].name,curPlayer.name)==0){
				found =-1;
				if(rank[i].fases<curPlayer.fases)found = i;
				if(rank[i].fases==curPlayer.fases && rank[i].score>curPlayer.score)found = i;
			}
		}
		for(i=0;i<10&&found==-10;i++){
			if(strcmp(rank[i].name,"Default")==0)found=i;
		}
		for(i=1;i<10&&found==-10;i++){
			if(rank[i].fases<rank[smallScore].fases)smallScore=i;
			if(rank[i].fases==rank[smallScore].fases && rank[i].score>rank[smallScore].score)smallScore=i;
		}
		if(found==-10){
			if(rank[smallScore].fases<curPlayer.fases || (rank[smallScore].fases==curPlayer.fases && rank[smallScore].score>curPlayer.score))found=smallScore;
		}
		//printf("found = %d\n",found);
		if(found>=0){
			strcpy(rank[found].name,curPlayer.name);
			rank[found].fases=curPlayer.fases;
			rank[found].score=curPlayer.score;
		}
		fp = fopen(FILEPATH"ranking.bin","wb");
		for(i=0;i<10;i++)fwrite(&rank[i],sizeof(playerRank),1,fp);
		fclose(fp);
	}
	return 0;
}

int startGame(playerT player){
	char configPath[32];
	int i, nFases, nFase=0,result;
	FILE *fpconfig, *fpfases;
	sprintf(configPath,FILEPATH"%s_config.txt",player.name);
	if(!fCheck(configPath,0)){
		printf("Falha ao carregar configurações, criando novo arquivo a partir do Default.\n");
		if(configDefault(configPath)){
			printf("Falha ao criar novo arquivo de configurações, terminando programa.\n");
			return 1;
		}
	}
	fpconfig = fopen(configPath,"r");
	for(i=1;i<9;i++)fscanf(fpconfig," %d",player.config+i);
	fclose(fpconfig);


	if(!fCheck(FILEPATH"fases.txt",0)){
		printf("Falha ao carregar arquivo de fases, o jogo será encerrado.\n");
		fflush(stdout);
		sleep(3);
		return 1;
	}
	fpfases = fopen(FILEPATH"fases.txt","rt");
	fscanf(fpfases,"%d ",&nFases);

	clear();

	printf("Bem vindo ao Slime on Ice '%s', aperte enter para continuar.\n",player.name);
	player.score = 0;
	CLEARBUFFER;
	do{
		result = fFase(nFase,player,fpfases);
		if(result==-1)return 0;
		player.score += result;
		player.fases = nFase+1;
		addScore(player);
		nFase++;
		printf("Parabéns, voce passou da fase %d!\nDigite enter para continuar.\n",nFase);
		CLEARBUFFER;
	} while(nFase<nFases);
	printf("Parabéns %s, você venceu!\n",player.name);
	printf("Digite enter para voltar ao menu principal.\n");
	CLEARBUFFER;
	fclose(fpfases);
	return 0;
}

int openConfig(playerT player){
	char configPath[32];
	FILE *fpconfig;
	sprintf(configPath,FILEPATH"%s_config.txt",player.name);
	if(!fCheck(configPath,0)){
		printf("Falha ao carregar configurações, criando novo arquivo a partir do Default.\n");
		if(configDefault(configPath)){
			printf("Falha ao criar novo arquivo de configurações, terminando programa.\n");
			return 1;
		}
	}
	fpconfig = fopen(configPath,"r");
	for(int i=1;i<9;i++)fscanf(fpconfig," %d",player.config+i);
	int id=0,cur=1,ch;
	menu_opt *menu=NULL,*curMenu;
	screen *scr = screenAlloc(300,38),*scr2=NULL;
	printStrToScreenPos(scr,"Configurações",1,1);
	screenMove(scr,0,3);
	menu = insereMenuOpt(menu,menuOptionCharBox("Personagem",&id,player.config[SOI_PLAYER]),0);
	menu = insereMenuOpt(menu,menuOptionCharBox("  Objetivo",&id,player.config[SOI_OBJETIVO]),0);
	menu = insereMenuOpt(menu,menuOptionCharBox("     Bloco",&id,player.config[SOI_BLOCO]),0);
	menu = insereMenuOpt(menu,menuOptionCharBox("      Cima",&id,player.config[SOI_UP]),0);
	menu = insereMenuOpt(menu,menuOptionCharBox("  Esquerda",&id,player.config[SOI_LEFT]),0);
	menu = insereMenuOpt(menu,menuOptionCharBox("     Baixo",&id,player.config[SOI_DOWN]),0);
	menu = insereMenuOpt(menu,menuOptionCharBox("   Direita",&id,player.config[SOI_RIGHT]),0);
	menu = insereMenuOpt(menu,menuOptionCharBox("      Sair",&id,player.config[SOI_ESC]),0);
	menu = insereMenuOpt(menu,menuOptionText("", &id),0);
	menu = insereMenuOpt(menu,menuOptionButton("Sair",          &id),0);
	while(1){
		scr2 = menuToScreen(scr,scr2,menu,1,3);
		clear();
		printScreen(scr2);
		while(!(ch = readKey()));
		menuMove(menu,scr,ch,&cur);
		if(ch=='\n'){
			if(cur==id)break;
			curMenu=menu;
			for(int i=1;i<cur;i++)curMenu=curMenu->next;
			while(cur>3 && (
				curMenu->val==player.config[4] ||
				curMenu->val==player.config[5] ||
				curMenu->val==player.config[6] ||
				curMenu->val==player.config[7] ||
				curMenu->val==player.config[8] )){
				curMenu->val = readKey();
			}
			player.config[cur]=curMenu->val;
		}
	}
	fclose(fpconfig);
	fpconfig = fopen(configPath,"w");
	fprintf(fpconfig,"%d %d %d\n%d %d %d %d %d",player.config[1],player.config[2],player.config[3],player.config[4],player.config[5],player.config[6],player.config[7],player.config[8]);
	fclose(fpconfig);
	freeMenu(menu);
	closeScreen(scr);
	closeScreen(scr2);
	return 0;
	// clear();
	// char configPath[32];
	// int i,n;
	// FILE *fpconfig;
	// sprintf(configPath,FILEPATH"%s_config.txt",player.name);
	// if(!fCheck(configPath,0)){
	// 	printf("Falha ao carregar configurações, criando novo arquivo a partir do Default.\n");
	// 	if(configDefault(configPath)){
	// 		printf("Falha ao criar novo arquivo de configurações, terminando programa.\n");
	// 		return 1;
	// 	}
	// }
	// fpconfig = fopen(configPath,"rt");
	// for(i=1;i<9;i++){
	// 	player.config[i] = fgetc(fpconfig);
	// 	fgetc(fpconfig);
	// }
	// do{
	// 	clear();
	// 	printf("Configurações:\n\n\n");
	// 	printf("1- Personagem = '%c'\n",player.config[SOI_PLAYER]);
	// 	printf("2-   Objetivo = '%c'\n",player.config[SOI_OBJETIVO]);
	// 	printf("3-      Bloco = '%c'\n",player.config[SOI_BLOCO]);
	// 	printf("4-       Cima = '%c'\n",player.config[SOI_UP]);
	// 	printf("5-   Esquerda = '%c'\n",player.config[SOI_LEFT]);
	// 	printf("6-      Baixo = '%c'\n",player.config[SOI_DOWN]);
	// 	printf("7-    Direita = '%c'\n",player.config[SOI_RIGHT]);
	// 	printf("8-       Sair = 'q' (Não é possivel mudar esta tecla)\n");
	// 	printf("\nDigite o que deseja mudar: [1,7] ou 0 para voltar ao menu principal.\n");
	// 	n = readIntInRange(0,7);
	// 	if(n){
	// 		printf("Agora digite a tecla desejada.\n");
	// 		player.config[n]=getNewKey(n,player);
	// 	}
	// } while(n);
	// fclose(fpconfig);
	// fpconfig = fopen(configPath,"w");
	// fprintf(fpconfig,"%d,%d,%d\n%d,%d,%d,%d,%d",player.config[1],player.config[2],player.config[3],player.config[4],player.config[5],player.config[6],player.config[7],player.config[8]);
	// fclose(fpconfig);
	// return 0;
}

int orderRanking(playerRank *rank){
	int mudou=1,i;
	playerRank aux;
	while(mudou){
		mudou=0;
		for(i=0;i<9;i++){
			if(rank[i].fases<rank[i+1].fases || (rank[i].fases==rank[i+1].fases && rank[i].score>rank[i+1].score) || (rank[i].fases==rank[i+1].fases && rank[i].score==rank[i+1].score && strcmp(rank[i].name,rank[i+1].name)>0)){
				strcpy(aux.name,rank[i].name);
				aux.fases=rank[i].fases;
				aux.score=rank[i].score;
				strcpy(rank[i].name,rank[i+1].name);
				rank[i].fases=rank[i+1].fases;
				rank[i].score=rank[i+1].score;
				strcpy(rank[i+1].name,aux.name);
				rank[i+1].fases=aux.fases;
				rank[i+1].score=aux.score;
				mudou=1;
			}
		}
	}
	return 0;
}

int openRanking(char* player){
	int i;
	FILE *fp;
	playerRank defaultPlayer,rank[10];
	for(i=0;i<10;i++)rank[i].fases=0;
	if(!fCheck(FILEPATH"ranking.bin",1)){
		printf("Falha ao carregar Ranking, criando novo arquivo a partir do Default.\n");
		fp = fopen(FILEPATH"ranking.bin","wb");
		if(!fp){
			printf("Falha ao criar novo arquivo de ranking, retornando ao menu principal.\n");
			usleep(SEGUNDO);
			return 1;
		}
		strcpy(defaultPlayer.name,"Default");
		defaultPlayer.fases=0;
		defaultPlayer.score=1000;
		for(i=0;i<10;i++)fwrite(&defaultPlayer,sizeof(playerRank),1,fp);
		fclose(fp);
	}
	fp = fopen(FILEPATH"ranking.bin","rb");
	for(i=0;i<10;i++)fread(&rank[i],sizeof(playerRank),1,fp);
	fclose(fp);
	orderRanking(rank);
	clear();
	printf("              Ranking             \n");
	printf("----------------------------------\n");
	printf(" # Usuário            Fases Pontos\n");
	printf("----------------------------------\n");
	for(i=0;i<10;i++){
		if(strcmp("Default",rank[i].name)){
			printf("                       %2d %8d",rank[i].fases,rank[i].score);
			printf("\r%2d ",i+1);
			if(!strcmp(player,rank[i].name))printf(ESCC_INVERTED"%s"ESCC_DEF,rank[i].name);
			else printf("%s",rank[i].name);
			printf("\n");
		}
	}
	printf("\nAperte Enter para voltar ao menu principal.\n");
	CLEARBUFFER;
	return 0;
}

int mainMenu(){
	playerT player;
	strcpy(player.name,"Default");
	int id=0,cur=1,ch;
	menu_opt *menu=NULL;
	screen *scr = screenAlloc(300,38),*scr2=NULL;
	printStrToScreenPos(scr,"Slime On Ice",1,1);
	screenMove(scr,0,3);
	menu = insereMenuOpt(menu,menuOptionButton("Jogar",&id),0);
	menu = insereMenuOpt(menu,menuOptionButton("Configurações", &id),0);
	menu = insereMenuOpt(menu,menuOptionButton("Ranking", &id),0);
	menu = insereMenuOpt(menu,menuOptionText("", &id),0);
	menu = insereMenuOpt(menu,menuOptionButton("Sair",          &id),0);
	while(1){
		scr2 = menuToScreen(scr,scr2,menu,1,3);
		clear();
		printScreen(scr2);
		while(!(ch = readKey()));
		if(!menuMove(menu,scr,ch,&cur) && ch=='\n'){
			if(cur!=5)readPlayerName(player.name);
			if(cur==1)if(startGame(player))return 1;
			if(cur==2)openConfig(player);
			if(cur==3)openRanking(player.name);
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

int main(){
	setEcho(1);
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