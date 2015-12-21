#include "batata.h"

int buttonPress(menu_opt* menu,screen* scr){
	if(menu->id==6){
		closeScreen(scr);
		return 0;
	}
	return 1;
}

int main(){
	int ch=1,cur=0,temp,id=0;
	screen *scr = screenAlloc(120,30);
	menu_opt *curMenu,*menu=NULL;
	menu = insereMenuOpt(menu,menuOptionBox("Batata", &id,1));
	menu = insereMenuOpt(menu,menuOptionBox("Batata2",&id,0));
	menu = insereMenuOpt(menu,menuOptionSlider("slider",&id,0,100,50));
	menu = insereMenuOpt(menu,menuOptionNumBox("numero",&id,0,100,50));
	menu = insereMenuOpt(menu,menuOptionLine("\u2500",&id));
	menu = insereMenuOpt(menu,menuOptionButton("sair",&id));
	screenMove(scr,0,0);
	while(1){
		menuToScreen(scr,menu,1,1);
		clear();
		printScreen(scr);
		printf("%d\n",cur);
		while(!(ch = readKey()));
		//printf("ch = %d\n",ch);
		//while(getchar()!='\n');
		temp=scr->curPos.y;
		if(ch==KEY_UP || ch==KEY_DOWN){
			screenMoveDirKey(scr,ch);
			if(temp!=scr->curPos.y){
				printf("temp %d curposy %d\n",temp,scr->curPos.y);
				if(ch==KEY_UP)cur--;
				else cur++;
				curMenu=menu;
				for(temp=2;curMenu->next && temp<min(id,cur);temp++)curMenu=curMenu->next;
				if(curMenu->next){
					if(curMenu->type==MENU_TYPE_SLIDER && ch==KEY_DOWN)screenMoveDirKey(scr,KEY_DOWN);
					if(curMenu->next->type==MENU_TYPE_SLIDER && ch==KEY_UP)screenMoveDirKey(scr,KEY_UP);
					if(curMenu->next->type==MENU_TYPE_H_LINE || curMenu->next->type==MENU_TYPE_TEXT){
						screenMoveDirKey(scr,ch);
						if(ch==KEY_UP)cur--;
						else cur++;
					}
				}
			}
		}
		else{
			curMenu=menu;
			for(int i=1;curMenu->next && i<cur;i++)curMenu=curMenu->next;
			if(curMenu->type==MENU_TYPE_BOX)if(ch=='\n')curMenu->val=(curMenu->val+1)%2;
			if(curMenu->type==MENU_TYPE_BUTTON)if(ch=='\n')if(!buttonPress(curMenu,scr))return 0;
			if(curMenu->type==MENU_TYPE_SLIDER){
				if(ch==KEY_LEFT)curMenu->val=max(curMenu->min,curMenu->val-1);
				if(ch==KEY_RIGHT)curMenu->val=min(curMenu->max,curMenu->val+1);
			}
			if(curMenu->type==MENU_TYPE_NUMBOX)if(ch=='\n')curMenu->val=readIntInRange(curMenu->min,curMenu->max);
		}
	}
	return 0;
}