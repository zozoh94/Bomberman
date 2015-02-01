#ifndef IA_H
#define IA_H

#include "struct.h"
#include "player.h"
#include "map.h"
#include "bomb.h"
#include "astar.h"

/**
   prend les coordonnées où on est, où on veut aller, et le tableau de bombes à esquiver.
   renvoie les coordonnées où il faut aller pour se déplacer d'une case.
   int[0] = x, int [1] = y, int[2] = distance
   utiliser algorithme A*? :D
*/
int* AllerVers(int xA, int yA, int xDest, int yDest, map* m, int **bombes);

/**
   Cherche les bloc menaçants à cause de bombes
   renvoie ça dans un tableau
   int[x][y] = 0 si libre, 1 si danger
*/
int** ChercheBombes(map* m);

/**
   Renvoie la case la plus proche de x/y parmis les cases pas en 0 du tableau et pas meancées
*/
int* TrouverProche(int x, int y, map* m, int** tab, int** bombes);

/**
   Cherche les zones contigues à des bloc destructibles (propices au posage de bombe), et les zones avec des bonus. assigne une priorite aux cases
   
   0 si bombe pas posable, x qui indique combien de bloc contigues à coté, ou 5 si c'est un bonus.
*/
int** ChercheDest(map* m, player *p, int** bombes);


#endif
