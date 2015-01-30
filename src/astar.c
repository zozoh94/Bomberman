#include "astar.h"

noeud* creerNoeud(int x, int y, int poids, noeud *last){
  noeud *ret = malloc(sizeof(noeud));
  ret->poids = poids;
  ret->x = x;
  ret->y = y;
  ret->last = last;
  return ret;
}

tasNoeud* addNoeud(tasNoeud *tas, noeud *n){
  int i;
  if(tas == NULL){
    tas = malloc(sizeof(tas));
    tas->noeuds = malloc(sizeof(noeud*)*TTABLE);
    for(i = 0; i < TTABLE; i++){
      tas->noeuds[i] = NULL;
    }
    tas->last = -1;
  }
  tas->noeuds[++tas->last] = n;
  remonter(tas,tas->last);
  return tas;
}

noeud* getNoeud(tasNoeud *tas, int p){
  noeud *ret = tas->noeuds[p];
  removeNoeud(tas, p);
  return ret;
}

int remonter(tasNoeud *tas, int position){
  if(position == 0){ // on est au sommet de l'arbre, ne peut pas remonter
    return 0;
  }
  noeud *parent = (tas->noeuds[(position-1)/2]);
  noeud *actuel = (tas->noeuds[position]);
  if(parent->poids < actuel->poids){
    tas->noeuds[position] = parent;
    tas->noeuds[(position-1)/2] = actuel;
    return remonter(tas, (position-1)/2);
  }else{
    return position;
  }
}

int descendre(tasNoeud *tas, int position){
  int pos=0;
  if(position == tas->last){ // on est en bas de l'arbre, ne peut pas descendre
    return tas->last;
  }
  noeud *enfantG = (tas->noeuds[((position+1)*2-1)]);
  noeud *enfantD = (tas->noeuds[(position+1)*2]);
  noeud *enfantF;
  noeud *actuel = tas->noeuds[position];
  if(enfantG == NULL){
    if(enfantD == NULL){
      return position;
    }else{
      enfantF = enfantD;
    }
  }else{
    if(enfantG->poids < enfantD->poids){
      enfantF = enfantG;
      pos = ((position+1)*2-1);
    }else{
      enfantF = enfantD;
      pos = (position+1)*2;
    }
  }
  if(enfantF->poids > actuel->poids){
    tas->noeuds[position] = enfantF;
    tas->noeuds[pos] = actuel;
    return descendre(tas, pos);
  }else{
    return position;
  }
}

void removeNoeud(tasNoeud *tas, int p){
  if(tas->last == 0){
    tas->last--;
    tas->noeuds[0] = NULL;
  }else{
    tas->noeuds[p] = tas->noeuds[tas->last];
    tas->noeuds[tas->last--] = NULL;
    descendre(tas, p);
  }
}

void deleteNoeud(noeud *n){
  if(n->last!=NULL){
    deleteNoeud(n->last);
  }
  free(n);
}

void deleteTas(tasNoeud *tas){
  free(tas->noeuds);
  free(tas);
}
