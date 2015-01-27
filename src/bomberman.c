#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "map.h" 
#include "game.h"

void pauseSDL();

int main(int argc, char **argv)
{
  SDL_Event event;
  SDL_Surface* ecran;


  int continuer = 0;


  ecran = InitSDL();
  map** listMaps;
  int nbrMap = ListMaps(&listMaps);
  while (continuer!=1)
    {
      while (SDL_WaitEvent(&event) && continuer!=1)
	{
	  switch (event.type)
	  {
	
	    	case SDL_KEYDOWN:
			  	switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
			  		continuer = 1;
			  		break;
	
					case SDLK_RETURN:
			  		Solo(listMaps[0], ecran);
			  		break;
					default:
			  		;
				}

			
			case SDL_QUIT: 
		  	continuer = 1;
		  	break;
	   }
	  
	}
      
      
      
    }
  
  //DELETE SPRITE
  FreeMaps(listMaps, nbrMap);

  SDL_Quit();
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
