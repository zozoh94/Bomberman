#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "map.h" 
#include "game.h"

int main()
{
  SDL_Event event;
  SDL_Surface* ecran;
  
  
  int continuer = 0;
  
  
  ecran = InitSDL();
  map** listMaps;
  int nbrMap = ListMaps(&listMaps);
  fprintf(stderr,"%d cartes chargées\n",nbrMap);
  TTF_Font *titre = TTF_OpenFont("Bomberman.ttf",56);
  TTF_Font *fontmenu = TTF_OpenFont("Bomberman.ttf", 24);
  SDL_Color white = {255, 255, 255, 0}; 
  SDL_Color gray = {150, 150, 150, 0};

  int menu = 0; // 0 = mode, 1 = joueurs, 2 = cartes
  int smenu = 0; // dépend du menu
  int mmax = 2; // menumax

  nbrP nbrPlayers = 0;
  vCond cond = 0;
  
      while (SDL_WaitEvent(&event) && continuer!=1)
	{
	  //Partie affichage du menu
	  ecran =  ScaleSurface(ecran,640,460);
	  SDL_FillRect(ecran, NULL, 0);
	  printText(ecran, titre, white, 8, 64, "PACBOMBERBRIQUE");
	  switch(menu){
	    //pour la position: 320 (moitié de l'écran) - (nombre de lettre * taille des lettres)/2
	    //              et: 128 (sous le titre) + 32 par ligne.
	    // Choix du mode de jeu
	  case(0):
	    mmax = 2;
	    printText(ecran, fontmenu, gray, 320-(10*24)/2, 128, "Mode point");
	    printText(ecran, fontmenu, gray, 320-(11*24)/2, 160, "Mode versus");
	    switch(smenu){
	    case(0):
	      printText(ecran, fontmenu, white, 320-(10*24)/2, 128, "Mode point");

	      break;
	    case(1):
	      printText(ecran, fontmenu, white, 320-(11*24)/2, 160, "Mode versus");
	      break;
	    }
	    break;
	    // Choix du nombre de joueurs
	  case(1):
	    mmax = 4;
	    printText(ecran, fontmenu, gray, 320-(10*24)/2, 128, "J1 vs 1 IA");
	    printText(ecran, fontmenu, gray, 320-(10*24)/2, 160, "J1 vs 3 IA");
	    printText(ecran, fontmenu, gray, 320-(8*24)/2, 192, "J1 vs J2");
	    printText(ecran, fontmenu, gray, 320-(16*24)/2, 224, "J1 vs J2 vs 2 IA");
	    switch(smenu){
	    case(0):
	      printText(ecran, fontmenu, white, 320-(10*24)/2, 128, "J1 vs 1 IA");
	      break;
	    case(1):
	      printText(ecran, fontmenu, white, 320-(10*24)/2, 160, "J1 vs 3 IA");
	      break;
	    case(2):
	      printText(ecran, fontmenu, white, 320-(8*24)/2, 192, "J1 vs J2");
	      break;
	    case(3):
	      printText(ecran, fontmenu, white, 320-(16*24)/2, 224, "J1 vs J2 vs 2 IA");
	      break;
	    }
	    break;
	  // Choix de la carte
	  case(2):
	    mmax = nbrMap;
	    printText(ecran, fontmenu, white, 320-(1*24)/2, 256, "A");
	    printText(ecran, fontmenu, white, 320-(strlen(listMaps[smenu]->name)*24)/2, 288, listMaps[smenu]->name);
	    printText(ecran, fontmenu, white, 320-(1*24)/2, 320, "V");
	    break;
	  }
	  SDL_Flip(ecran);
	  
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
		    fprintf(stderr,"%d nbrPlayers\n",nbrPlayers);
		    smenu = 0;
		    menu = 1;
		    break;
		  case(1):
		    nbrPlayers = smenu;
		    fprintf(stderr,"%d nbrPlayers\n",nbrPlayers);
		    smenu = 0;
		    menu = 2;
		    break;
		  case(2):
		    ParseMap(listMaps[smenu]);
		    fprintf(stderr,"charge la map %s: %d\n",listMaps[smenu]->name,listMaps[smenu]->error);
		    fprintf(stderr,"%d nbrPlayers\n",nbrPlayers);
		    StartGame(listMaps[smenu], nbrPlayers, cond, ecran);
		    smenu = 0;
		    menu = 0;
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
	  SDL_Flip(ecran);
	}
  
  //DELETE SPRITE
  FreeMaps(listMaps, nbrMap);
  TTF_CloseFont(fontmenu);

  SDL_Quit();
  TTF_Quit();
  return EXIT_SUCCESS;
}
