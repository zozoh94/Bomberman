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
   décrémente le nombre de bombes posées par le joueur de 1,
   retire la bombe de la liste des bombes de la map.
 */
void Explode(map* map);

/*
  Liste chainée de bombes
 */
typedef struct s_bombList bombList{
  bomb* data;
  bombList* next;
};

/**
   Initialise une nouvelle bombList vide
   @return une nouvelle liste de bombes vide.
 */
bombList* NewBombList();

/**
   Ajoute la bombe b à la liste l
   @param bombList* l: la liste à laquelle ajouter la bombe
   bomb* b: la bombe à ajouter
*/
void AddBombList(bombList* l, bomb* b);

/**
   Retire la bombe b de la liste l
   @param bombList* l: la liste
   bomb* b: la bombe à retirer
   @return 0 si tout s'est bien passé, 1 si la bombe n'était pas dans la liste
 */
int RemoveBombList(bombList* l, bomb* b);

#endif
