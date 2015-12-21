# jogos

-"batata.c" e "batata.h" são os arquivos da 'biblioteca' comum a todos os jogos

-"jogos.txt" é um arquivo com o nome de exibição e o nome da pasta de cada jogo

-"menu.c" é o arquivo usado para acessar todos os jogos

	cada uma das pastas contém um arquivo "main.c" ,que é chamado por "menu.c", e todos os outros arquivos que são usados apenas por aquele jogo

-"blablabla" é apenas o executavel temporario gerado ao compilar menu.c


meteodo de compilação:

	"gcc -Wall -o blablabla menu.c batata.c -lm -std=c99"

	"time ./blablabla"