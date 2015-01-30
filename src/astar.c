#include "astar.h"

noeud* creerNoeud(int x, int y, int poids, noeud *last){
  noeud *ret = malloc(sizeof(noeud));
  ret->x = x;
  ret->y = y;
  ret->poids = poids;
  ret->last = last;
  return ret;
}

tasNoeud* addNoeud(tasNoeud *tas, noeud *n){

}

tasNoeud* removeNoeud(tasNoeud *tas, noeud *n){

}

void deleteNoeud(noeud *n){
  if(n->last!=NULL){
    removeNoeud(n->last);
  }
  free(n);
}

void deleteTas(tasNoeud *tas){
  
}
