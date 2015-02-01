#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "map.h"
#include "bomb.h"
#include "sprite.h"

/**
   Initialise un joueur.
   @param int x: coordonnée X de départ
   int y: coordonnée Y de départ
   int score: score de départ
   int bombMax: nombre de bombes posables maximum au départ
   int speed: temps de déplacement au départ
   @return Un joueur
 */
player *InitPlayer(int x, int y, int score, int bombMax, int speed, int bombR, btype bombT, typeP type, map *map, Sprite *sprite, const char *image);

/**
   Initialise avec les valeurs par défaut un joueur
 */
player *AutoInit(map *map, typeP type, const char *image);

/**
   Pose une bombe aux coordonnées du joueur p si c'est possible:
   - Si il n'y en a pas déjà
   - Si le moveTimer du joueur est prêt (=-1) (on ne pose pas de bombe en se déplaçant)
 */
void PlaceBomb(player* p);

/**
   Test:
   - Si le joueur peut aller en X/Y
   - Si le moveTimer du joueur est prêt (=-1)
   Si oui, alors met moveTimer à speed, bloque les input du joueur et commence l'animation de déplacement du joueur vers X/Y.
   @param player *p: joueur à déplacer
   int X: x d'arrivée
   int Y: y d'arrivée
 */
void TryMove(player *p, int X, int Y);

/**
   Quand le moveTimer arrive à 0, déplace le joueur
   @param int X: x d'arrivée
   int Y: y d'arrivée
 */
void Move(player* p);

void FreePlayer(player *p);

#endif
