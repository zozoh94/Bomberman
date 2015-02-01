#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <libgen.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include "map.h"
#include "game.h"
#include "audio.h"

SDL_Surface* InitSDL();

int main(int argc, char* argv[])
{
    chdir(dirname(argv[0]));
    chdir("ressources");

    SDL_Event event;
    SDL_Surface* ecran;
    srand(time(NULL));

    int continuer = 0;

    ecran = InitSDL();
    map** listMaps;
    int nbrMap = ListMaps(&listMaps);

    fprintf(stderr,"%d cartes chargées\n",nbrMap);
    TTF_Font *titre = TTF_OpenFont("Bomberman.ttf",56);
    TTF_Font *fontmenu = TTF_OpenFont("Bomberman.ttf", 24);
    SDL_Color white = {255, 255, 255, 0};
    SDL_Color gray = {150, 150, 150, 0};
    SDL_Surface* menuScreen = IMG_Load("Menu.bmp");
    SDL_Rect menuPos;
    menuPos.x = 0;
    menuPos.y = 0;

    int menu = 0; // 0 = mode, 1 = joueurs, 2 = cartes
    int smenu = 0; // dépend du menu
    int mmax = 2; // menumax

    nbrP nbrPlayers = 0;
    vCond cond = 0;

    if(LoadSprite() == 1)
	return EXIT_FAILURE;

    if(InitSounds() == 1)
	return EXIT_FAILURE;

    Mix_PlayMusic( music, -1 );

    while (continuer == 2 || (SDL_WaitEvent(&event) && continuer!=1))
    {

	//Partie affichage du menu
	ecran =  ScaleSurface(ecran,640,460);
	SDL_FillRect(ecran, NULL, 0);
	SDL_BlitSurface(menuScreen, NULL, ecran, &menuPos);
	printText(ecran, titre, white, 8, 64, "PACBOMBERBRIQUE");
	switch(menu){
	    //pour la position: 320 (moitié de l'écran) - (nombre de lettre * taille des lettres)/2
	    //              et: 128 (sous le titre) + 32 par ligne.
	    // Choix du mode de jeu
	case(0):
	    mmax = 2;
	    printText(ecran, fontmenu, gray, 320-(10*24)/3, 128, "Mode point");
	    printText(ecran, fontmenu, gray, 320-(11*24)/3, 160, "Mode versus");
	    switch(smenu){
	    case(0):
		printText(ecran, fontmenu, white, 320-(10*24)/3, 128, "Mode point");

		break;
	    case(1):
		printText(ecran, fontmenu, white, 320-(11*24)/3, 160, "Mode versus");
		break;
	    }
	    break;
	    // Choix du nombre de joueurs
	case(1):
	    mmax = 5;
	    printText(ecran, fontmenu, gray, 320-(10*24)/3, 128, "J1 vs 1 IA");
	    printText(ecran, fontmenu, gray, 320-(10*24)/3, 160, "J1 vs 3 IA");
	    printText(ecran, fontmenu, gray, 320-(8*24)/3, 192, "J1 vs J2");
	    printText(ecran, fontmenu, gray, 320-(20*24)/3, 224, "J1 vs J2 vs IA vs IA");
	    printText(ecran, fontmenu, gray, 320-(4*24)/3, 256, "SHOW");
	    switch(smenu){
	    case(0):
		printText(ecran, fontmenu, white, 320-(10*24)/3, 128, "J1 vs 1 IA");
		break;
	    case(1):
		printText(ecran, fontmenu, white, 320-(10*24)/3, 160, "J1 vs 3 IA");
		break;
	    case(2):
		printText(ecran, fontmenu, white, 320-(8*24)/3, 192, "J1 vs J2");
		break;
	    case(3):
		printText(ecran, fontmenu, white, 320-(20*24)/3, 224, "J1 vs J2 vs IA vs IA");
		break;
	    case(4):
		printText(ecran, fontmenu, white, 320-(4*24)/3, 256, "SHOW");
		break;
	    }
	    break;
	    // Choix de la carte
	case(2):
	    mmax = nbrMap;
	    printText(ecran, fontmenu, white, 320-(1*24)/3, 256, "A");
	    printText(ecran, fontmenu, white, 320-(strlen(listMaps[smenu]->name)*24)/3, 288, listMaps[smenu]->name);
	    printText(ecran, fontmenu, white, 320-(1*24)/3, 320, "V");
	    break;
	}
	SDL_Flip(ecran);

	if(continuer == 2){
	    continuer = 0;
	}else{
	    //Partie input du menu
	    switch (event.type)
	    {
	      
	    case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
		    continuer = 1;
		    break;
		case SDLK_RETURN:
		    switch(menu){
		    case(0):
			cond = smenu;
			smenu = 0;
			menu = 1;
			break;
		    case(1):
			nbrPlayers = smenu;
			smenu = 0;
			menu = 2;
			break;
		    case(2):
			ParseMap(listMaps[smenu]);
			StartGame(listMaps[smenu], nbrPlayers, cond, ecran);
			smenu = 0;
			menu = 0;
			continuer = 2;
			break;
		    default:
			break;
		    }
		    break;
		case SDLK_BACKSPACE:
		    if(menu>0)
			menu--;
		    break;
		case SDLK_UP:
		    smenu=(smenu-1+mmax)%mmax;
		    break;
		case SDLK_DOWN:
		    smenu=(smenu+1)%mmax;
		    break;
		  
		default:
		    break;
		}
		break;
	      
	    case SDL_QUIT:
		continuer = 1;
		break;
	    }
	}
	SDL_Flip(ecran);
    }

    //DELETE SPRITE
    FreeSounds();
    FreeSprite();
    FreeMaps(listMaps, nbrMap);
    TTF_CloseFont(fontmenu);
    SDL_FreeSurface(menuScreen);

    SDL_Quit();
    TTF_Quit();
    return EXIT_SUCCESS;
}


SDL_Surface* InitSDL()
{
    SDL_Surface *ecran;

    /* initialisation de SDL_Video */
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
	fprintf(stderr, "Echec d'initialisation de SDL.\n");
	return NULL;
    }
    printf("SDL initialisé avec succès.\n");

    if(TTF_Init() == -1)
    {
	fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
	return NULL;
    }
    printf("ttf initialisé avec succès.\n");

    /* On fixe le mode d'affichage à 384*384 */
    ecran = SDL_SetVideoMode(640, 460, 32, SDL_SWSURFACE| SDL_DOUBLEBUF);
    if ( ecran == NULL )
    {
	fprintf(stderr, "Echec de changement du mode video : %s.\n", SDL_GetError());
	return NULL;
    }

    //On initialise le mixer de la SDL
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
	return false;
    }

    //Mise en place de la barre caption
    SDL_WM_SetCaption( "PacBomberBrique", NULL );

    return ecran;
}
