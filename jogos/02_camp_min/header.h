#define LAR_DEF 30
#define ALT_DEF 10
#define BMB_DEF 50
#define LAR_MAX 32
#define ALT_MAX 15

#define BOMB 9
#define FECHADO 0
#define ABERTO 1
#define BANDEIRA 2
#define INTERROGACAO 3

typedef struct config__ config_;
typedef struct coord_ coord;
typedef coord** mapa;

mapa criaMap(config_,int,int,int*);
int abreCoord(config_,coord**,int,int,int*,int*);
int marcaCoord(config_,coord**,int,int,int*);
void printMap(screen*,config_,coord**,int,int,int,int,int);
