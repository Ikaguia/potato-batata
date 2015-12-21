#include "../batata.h"

typedef struct{
	int dif;
} config_;

int jogo(config_ config){
	coords save;
	int tentativas = 2,respostaCerta;
	int id=0,cur=5,ch;
	menu_opt *menu=NULL;
	screen *scr = screenAlloc(300,38),*scr2=NULL;
	if(config.dif==1){
		respostaCerta = 1;
		menu = insereMenuOpt(menu,menuOptionText("Dificuldade = Fácil", &id),0);
		menu = insereMenuOpt(menu,menuOptionLine("-", &id),0);
		menu = insereMenuOpt(menu,menuOptionText("O Cebolinha vai ao cinema com dois pirulitos, qual o nome do filme?", &id),0);
		menu = insereMenuOpt(menu,menuOptionText(" ", &id),0);
		menu = insereMenuOpt(menu,menuOptionButton("Lambo 2",&id),0);
		menu = insereMenuOpt(menu,menuOptionButton("Lesident Evil",&id),0);
		menu = insereMenuOpt(menu,menuOptionButton("Lesgate",&id),0);
		menu = insereMenuOpt(menu,menuOptionButton("O Lei Leão",&id),0);
	}
	if(config.dif==2){
		respostaCerta = 4;
		menu = insereMenuOpt(menu,menuOptionText("Dificuldade = Média", &id),0);
		menu = insereMenuOpt(menu,menuOptionLine("-", &id),0);
		menu = insereMenuOpt(menu,menuOptionText("Uma mulher está no cinema, quando um homem com uma garrafa de vodka senta", &id),0);
		menu = insereMenuOpt(menu,menuOptionText("na cadeira imediatamente anterior à dela, qual o nome do filme?", &id),0);
		menu = insereMenuOpt(menu,menuOptionButton("O homem de aço",&id),0);
		menu = insereMenuOpt(menu,menuOptionButton("O homem que desafiou o diabo",&id),0);
		menu = insereMenuOpt(menu,menuOptionButton("O homem que copiava",&id),0);
		menu = insereMenuOpt(menu,menuOptionButton("O homem de Alcatraz",&id),0);
	}
	if(config.dif==3){
		respostaCerta = 3;
		menu = insereMenuOpt(menu,menuOptionText("Dificuldade = Difícil", &id),0);
		menu = insereMenuOpt(menu,menuOptionLine("-", &id),0);
		menu = insereMenuOpt(menu,menuOptionText("Um “amigo” foi lhe visitar e levou duas de suas taças favoritas, qual o nome do filme?", &id),0);
		menu = insereMenuOpt(menu,menuOptionText(" ", &id),0);
		menu = insereMenuOpt(menu,menuOptionButton("Exterminador do Futuro",&id),0);
		menu = insereMenuOpt(menu,menuOptionButton("O roubo do Banco Central",&id),0);
		menu = insereMenuOpt(menu,menuOptionButton("RoboCop 2",&id),0);
		menu = insereMenuOpt(menu,menuOptionButton("Roubo a Máfia",&id),0);
	}
	menu = insereMenuOpt(menu,menuOptionText(" ",&id),0);
	menu = insereMenuOpt(menu,menuOptionButton("Sair",&id),0);
	screenMove(scr,0,5);
	while(1){
		scr2 = menuToScreen(scr,scr2,menu,1,1);
		clear();
		printScreen(scr2);
		while(!(ch = readKey()));
		menuMove(menu,scr,ch,&cur);
		if(ch=='\n'){
			tentativas--;
			save = scr->curPos;
			if(cur==10){
				break;
			}
			else if(cur-4==respostaCerta){
				printStrToScreenPos(scr2,"Você acertou, parabéns!",5,11);
				printStrToScreenPos(scr2,"Pressione qualquer tecla para voltar ao Menu Principal",5,12);
				clear();
				printScreen(scr2);
				getch();
				break;
			}
			else if(tentativas){
				printStrToScreenPos(scr,"Você errou, tente outra vez.",1,11);
			}
			else{
				printStrToScreenPos(scr,"Você errou duas vezes e perdeu o jogo.",5,11);
				printStrToScreenPos(scr,"Tente novamente",5,12);
				printStrToScreenPos(scr,"Pressione qualquer tecla para voltar ao Menu Principal",5	,13);
				clear();
				printScreen(scr);
				getch();
				break;
			}
			scr->curPos = save;
		}
	}
	freeMenu(menu);
	closeScreen(scr);
	closeScreen(scr2);
	return 0;
}

int configura(config_ *config){
	int id=0,cur=1,ch;
	menu_opt *menu=NULL;
	screen *scr = screenAlloc(300,38),*scr2=NULL;
	printStrToScreenPos(scr,"Configurações:",1,1);
	screenMove(scr,0,3);
	menu = insereMenuOpt(menu,menuOptionButton("Dificuldade",&id),0);
	menu = insereMenuOpt(menu,menuOptionBox("Fácil",&id,(config->dif==1)?1:0),1);
	menu = insereMenuOpt(menu,menuOptionBox("Média",&id,(config->dif==2)?1:0),1);
	menu = insereMenuOpt(menu,menuOptionBox("Difícil",&id,(config->dif==3)?1:0),1);
	menu = insereMenuOpt(menu,menuOptionText(" ",&id),0);
	menu = insereMenuOpt(menu,menuOptionButton("Sair",&id),0);
	while(1){
		scr2 = menuToScreen(scr,scr2,menu,1,3);
		clear();
		printScreen(scr2);
		while(!(ch = readKey()));
		menuMove(menu,scr,ch,&cur);
		if(ch=='\n'){
			if(cur==1){
				menu->next->hidden=(menu->next->hidden+1)%2;
				menu->next->next->hidden=menu->next->hidden;
				menu->next->next->next->hidden=menu->next->hidden;
			}
			if(cur==2){
				config->dif=1;
				menu->next->val=1;
				menu->next->next->val=0;
				menu->next->next->next->val=0;
			};
			if(cur==3){
				config->dif=2;
				menu->next->val=0;
				menu->next->next->val=1;
				menu->next->next->next->val=0;
			};
			if(cur==4){
				config->dif=3;
				menu->next->val=0;
				menu->next->next->val=0;
				menu->next->next->next->val=1;
			};
			if(cur==6){
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
	config.dif=2;
	int id=0,cur=1,ch;
	menu_opt *menu=NULL;
	screen *scr = screenAlloc(300,38),*scr2=NULL;
	printStrToScreenPos(scr,"Charadas",1,1);
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
	printStrToScreenPos(scr,"Bem Vindo ao jogo: \"Charadas!\"",2,2);
	printStrToScreenPos(scr,"Pressione qualquer tecla para continuar.",2,4);
	clear();
	printScreen(scr);
	getch();
	mainMenu();
	clear();
	printf(ESCC_DEF);
	closeScreen(scr);
	return 0;
}
