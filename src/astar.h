#ifndef ASTAR_H
#define ASTAR_H

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
 */
typedef struct s_tasNoeud {

  struct s_tasNoeud *droit;
  struct s_tasNoeud *gauche;
}tasNoeud;

noeud* creerNoeud(int x, int y, int poids, noeud *last);

tasNoeud* addNoeud(tasNoeud *tas, noeud *n);

tasNoeud* removeNoeud(tasNoeud *tas, noeud *n);

void deleteNoeud(noeud *n);

void deleteTas(tasNoeud *tas);

#endif
