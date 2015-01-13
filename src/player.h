#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include "map.h"

/*
  Structure player
  Contient les coordonnées des joueurs, leur score, leurs bonus, leur nombre de bombes.
  
 */
typedef struct s_player player;
struct s_player{
  int x; //Coordonnée X
  int y; //Coordonnée Y
  int score; //Score
  int bombs; //Nombre de bombes posées
  int bombMax; //Nombre de bombes posables max simultanéments
  int speed; //Le temps de déplacement (plus petit = plus rapide)
  int moveTimer; //Le timer du déplacement
};


/**
   Initialise un joueur.
   @param int x: coordonnée X de départ
   int y: coordonnée Y de départ
   int score: score de départ
   int bombMax: nombre de bombes posables maximum au départ
   int speed: temps de déplacement au départ
   @return Un joueur
 */
player* InitPlayer(int x, int y, int score, int bombMax, int speed);


/**
   Pose une bombe aux coordonnées du joueur si c'est possible:
   - Si il n'y en a pas déjà
   - Si le moveTimer du joueur est prêt (=-1) (on ne pose pas de bombe en se déplaçant)
 */
void PlaceBomb();

/**
   Test:
   - Si le joueur peut aller en X/Y
   - Si le moveTimer du joueur est prêt (=-1)
   Si oui, alors met moveTimer à speed, bloque les input du joueur et commence l'animation de déplacement du joueur vers X/Y.
   @param int X: x d'arrivée
   int Y: y d'arrivée
 */
void TryMove(int X, int Y);

/**
   Quand le timer arrive à 0, change les coordonnées du joueur en X/Y
   @param int X: x d'arrivée
   int Y: y d'arrivée
 */
void Move(int X, int Y);

#endif
