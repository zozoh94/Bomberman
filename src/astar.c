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
	tas = malloc(sizeof(tasNoeud));
	tas->noeuds = malloc(sizeof(noeud*)*TTABLE);
	for(i = 0; i < TTABLE; i++){
	    tas->noeuds[i] = NULL;
	}
	tas->last = -1;
    }
    tas->last += 1;
    if(tas->last >= 0 && tas->last < TTABLE){
	tas->noeuds[tas->last] = n;
	remonter(tas,tas->last);
    }else{
	fprintf(stderr,"erreur out of tas %d\n", tas->last);
    }
    return tas;
}

noeud* getNoeud(tasNoeud *tas, int p){
    if(tas->last >= p){
	noeud *ret = tas->noeuds[p];
	removeNoeud(tas, p);
	return ret;
    }else{
	fprintf(stderr,"Le noeud %d n'existe pas!\n",p);
	return NULL;
    }
}

int remonter(tasNoeud *tas, int position){
    if(position == 0){ // on est au sommet de l'arbre, ne peut pas remonter
	return 0;
    }
    if(position < 0){
	fprintf(stderr,"POSITION NEGATIF ERREUR!!!!\n");
    }
    noeud *parent = (tas->noeuds[(position-1)/2]);
    noeud *actuel = (tas->noeuds[position]);
    if(parent->poids > actuel->poids){
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
	    pos = ((position+1)*2);
	}
    }else if(enfantD == NULL){
	enfantF = enfantG;
	pos =  ((position+1)*2-1);
    }else{
	if(enfantG->poids < enfantD->poids){
	    enfantF = enfantG;
	    pos = ((position+1)*2-1);
	}else{
	    enfantF = enfantD;
	    pos = (position+1)*2;
	}
    }
    if(enfantF->poids < actuel->poids){
	tas->noeuds[position] = enfantF;
	tas->noeuds[pos] = actuel;
	return descendre(tas, pos);
    }else{
	return position;
    }
}

void removeNoeud(tasNoeud *tas, int p){
    if(tas->last == 0){
	if(p != 0){
	    fprintf(stderr,"ERREUR REMOVENOEUD PAS ZERO");
	}
	tas->last--;
	tas->noeuds[0] = NULL;
    }else if(tas->last == p){
	tas->noeuds[p] = NULL;
	tas->last--;
    }else{
	tas->noeuds[p] = tas->noeuds[tas->last];
	tas->noeuds[tas->last--] = NULL;
	if(tas->last!=-1){
	    descendre(tas, p);
	}
    }
}

void deleteTas(tasNoeud *tas){
    free(tas->noeuds);
    free(tas);
}
