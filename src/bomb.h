#ifndef BOMB_H
#define BOMB_H

#include <stdio.h>
#include <stdlib.h>
#include "sprite.h"
#include "struct.h"
#include "player.h"
#include "map.h"

#define TIMERBOMB 400

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
   Crée une bombe du type de celles que pose le joueur
*/
bomb* CreateBomb(player* p);

/**
   Effets de l'explosion en X Y,
   renvoie 1 si l'explosion continue pas
   sinon renvoie 0.
   met dir dans explozone.
   Directions = 
   1 - droite
   2 - bas
   3 - gauche
   4 - haut
   5 - bombe
*/
int DoExplode(int x, int y, map* map, bomb* bomb, int dir);

/**
   Explose la bombe.
   Crée les flammes de la bombe et interagit avec tout les bloc touchés,
   décrémente le nombre de bombes posées par le joueur de 1,
   retire la bombe de la liste des bombes de la map.
*/
void Explode(map* map, bomb* bomb);

/**
   Trouve dans la liste des bombes de la map laquelle est aux coordonnées (x,y).
   @param map* map: La carte qui contient les bombes
   int x: coordonnée X de la bombe recherchée
   int y: coordonnée Y de le bombe recherchée
   @return la bombe recherchée
*/
bomb* GetBomb (map* map, int x, int y);


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
bombList* AddBombList(bombList* l, bomb* b);

/**
   Retire la bombe b de la liste l
   @param bombList* l: la liste
   bomb* b: la bombe à retirer
   @return 0 si tout s'est bien passé, 1 si la bombe n'était pas dans la liste
*/
bombList* RemoveBombList(bombList* l, bomb* b);

#endif
