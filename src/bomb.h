#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "map.h"
#ifndef BOMB_H
#define BOMB_H

/*
  Structure bomb
  Contient les coordonnées de la bombe, son type, son rayon d'explosion.
  
 */
typedef struct s_bomb bomb;
typedef enum e_btype btype;
struct s_bomb{
  int x; //Coordonnée X
  int y; //Coordonnée Y
  int explosion; //Rayon d'explosion
  int timer; //Temps avant explosion
  player* myPlayer; //Joueur ayant posé la bombe
  btype type; //Type de la bombe
};

/*
  Type possible des bombes
 */
enum e_btype{
  NORMAL, //Bombe normale, explose en croix
  SQUARE // Bombe qui explose en carré
};

/**
   Initialise une bombe
   @param int x: coordonnée X de départ
   int y: coordonnée Y de départ
   int explosion: rayon d'explosion
   int timer: temps avant explosion (explose à 0)
   player* myPlayer: le joueur qui a posé la bombe
   btype type: type de la bombe
   
   @return La bombe
*/
bomb* InitBomb(int x, int y, int rayon, int timer, player* myPlayer, btype type);

/**
   Explose la bombe.
   Crée les flammes de la bombe et interagit avec tout les bloc touchés,
   décrémente le nombre de bombes posées par le joueur de 1.
 */
void Explode(map* map);

#endif
