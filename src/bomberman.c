#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "map.h" 

void pauseSDL();

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) // Démarrage de la SDL. Si erreur :
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // Écriture de l'erreur
        exit(EXIT_FAILURE); // On quitte le programme
    }

    SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE); // Ouverture de la fenêtre

    pauseSDL(); // Mise en pause du programme
    
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
