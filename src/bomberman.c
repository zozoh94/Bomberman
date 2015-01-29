#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "map.h" 
#include "game.h"

void pauseSDL();

void printText(SDL_Surface *ecr, TTF_Font *font, SDL_Color couleur, int x, int y, char* texte){
  SDL_Rect position;
  SDL_Surface *txt = TTF_RenderText_Blended(font, texte, couleur);
  position.x = x;
  position.y = y;
  SDL_BlitSurface(txt, NULL, ecr, &position);
}

int main(int argc, char **argv)
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
  SDL_Color white = {255, 255, 255}; 
  SDL_Color gray = {150, 150, 150};

  int menu = 0; // 0 = mode, 1 = joueurs, 2 = cartes
  int smenu = 0; // dépend du menu
  int mmax = 3; // menumax

  map *m = listMaps[0];
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
	  case 0:
	    mmax = 2;
	    printText(ecran, fontmenu, white, 320-(10*24)/2, 128, "Mode point");
	    printText(ecran, fontmenu, gray, 320-(11*24)/2, 160, "Mode versus");
	    break;
	    // Choix du nombre de joueurs
	  case 1:
	    mmax = 4;
	    printText(ecran, fontmenu, white, 320-(10*24)/2, 128, "J1 vs 1 IA");
	    printText(ecran, fontmenu, gray, 320-(10*24)/2, 160, "J1 vs 3 IA");
	    printText(ecran, fontmenu, gray, 320-(8*24)/2, 192, "J1 vs J2");
	    printText(ecran, fontmenu, gray, 320-(16*24)/2, 224, "J1 vs J2 vs 2 IA");
	    break;
	  // Choix de la carte
	  case 2:
	    mmax = nbrMap;
	    printText(ecran, fontmenu, white, 320-(1*24)/2, 128, "^");
	    printText(ecran, fontmenu, white, 320-(strlen(listMaps[smenu]->name)*24)/2, 160, listMaps[smenu]->name);
	    printText(ecran, fontmenu, white, 320-(1*24)/2, 192, "v");
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
		  StartGame(m, nbrPlayers, cond, ecran);
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

void pauseSDL()
{
    int continuer = 1;
    SDL_Event event;
 
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
        }
    }
}
