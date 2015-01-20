#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "map.h"
#include "player.h"
#include "bomb.h"

//Dans ce fichier, les initialisations de parties et les boucles de jeux.

/**
   @enum e_vCond
   @brief Conditions de victoires
*/
typedef enum e_vCond vCond;
enum e_vCond{
  SOLO,
  VSIA,
  MULTI
};

/**
   Initialise un jeu en solo
   Map, 1 joueur
 */
void Solo(map *m, player *p, SDL_Surface *dest);

//Boucle de jeu
void GameLoop(map *m, vCond cond, SDL_Surface *dest);

/**
   Contrôle les timer et explosions des bombes.
   Récupère la liste des bombes contenue dans map, et pour chaque bombe, vérifie si elle doit exploser, la fait exploser le cas échéant, sinon décrémente son timer.
   @param map* map: La carte qui contient les bombes
 */
void BombLoop(map* map, SDL_Surface *dest);

/**
   Contrôle les timer des joueurs.
   Récupère la liste des joueurs de la map, et pour chaque joueur, décrémente son timer si besoin.
   @param map* map: La carte qui contient les joueurs
   int* input: le tableau des input à gérer éventuellement
 */
void PlayerLoop(map* map, int* input, SDL_Surface *dest);


/**
   Affiche la map
 */
void MapLoop(map* map, SDL_Surface *dest);

/**
   Test si les conditions de victoire sont remplies
   Met le gagnant (s'il y en a un) dans "winner"
   @param map* map: La carte de jeu
   vCond cond: Les conditions de victoire
   player** winner: Un pointeur qui pointera vers le gagnant s'il y en a un.
   @return 1 si conditions de victoire remplies, sinon 0
 */
int TestWin(map* map, vCond cond, player** winner);

#endif
