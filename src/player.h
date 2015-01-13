#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include "map.h"


typedef enum e_typeP typeP;
typedef struct s_player player;
/*
  Structure player
  Contient les coordonnées des joueurs, leur score, leurs bonus, leur nombre de bombes.
  
 */
struct s_player{
  int x; //Coordonnée X
  int y; //Coordonnée Y
  int destX; //Coordonnée X d'arrivée en cas de déplacement
  int destY; //Coordonnée Y d'arrivée en cas de déplacement
  int score; //Score
  int bombs; //Nombre de bombes posées
  int bombMax; //Nombre de bombes posables max simultanéments
  int speed; //Le temps de déplacement (plus petit = plus rapide)
  int moveTimer; //Le timer du déplacement
  typeP type; //Type de player
  int *map; //Pointeur vers la map
  
  //Stats des bombes
  int bombR; //rayon des bombes
  btype bombT; //type des bombes
};

/*
  Enumeration typeP
  Le type de joueur (J1, J2, IA)
*/
enum e_typeP{
  J1,
  J2,
  IA
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
player* InitPlayer(int x, int y, int score, int bombMax, int speed, int bombR, type bombT, typeP type, Map *map);

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

#endif
