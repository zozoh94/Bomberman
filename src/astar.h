#ifndef ASTAR_H
#define ASTAR_H

#include <stdio.h>
#include <stdlib.h>

#define TTABLE 63
// En fait c'est Dijkstra, pas A*

/**
   noeud pour A*
 */
typedef struct s_noeud {
  int x;
  int y;
  int poids;
  struct s_noeud *last;
}noeud;

/**
   TAS noeud pour A*
   parent de X = (X-1)/2
   enfant de X = (X+1)*2 et (X+1)*2 -1
 */
typedef struct s_tasNoeud {
  noeud **noeuds; //tableau de tableaux de noeuds
  int last;
}tasNoeud;

noeud* creerNoeud(int x, int y, int poids, noeud *last);

tasNoeud* addNoeud(tasNoeud *tas, noeud *n);

noeud* getNoeud(tasNoeud *tas, int p);

int remonter(tasNoeud *tas, int position);

int descendre(tasNoeud *tas, int position);

void removeNoeud(tasNoeud *tas, int p);

void deleteNoeud(noeud *n);

void deleteTas(tasNoeud *tas);

#endif
