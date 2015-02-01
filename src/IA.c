#include "IA.h"

int* AllerVers(int xA, int yA, int xDest, int yDest, map* m, int** bombes){
    int i,j;
    int *ret = malloc(sizeof(int)*3);
    int surBombe = 0; // A mettre à 1 si on est sur un emplacement dangereux.
    if(bombes[xA][yA] == 1){
	surBombe = 1;
    }
    ret[0]=-1;
    ret[1]=-1;
    ret[2] = -1;
    if(xA == xDest && yA == yDest){
	ret[0] = xA;
	ret[1] = yA;
	ret[2] = 0;
    }
    noeud ***noeuds = malloc(sizeof(noeud**)*(m->width));
    for(i = 0; i < m->width; i++){
	noeuds[i] = malloc(sizeof(noeud*)*(m->height));
	for(j = 0; j < m->height; j++){
	    noeuds[i][j] = creerNoeud(i, j, 999, NULL);
	}
    }
    noeuds[xA][yA]->poids = 0;
  
    tasNoeud *tas = addNoeud(NULL, noeuds[xA][yA]);
    noeud *act;
    do{
	act = getNoeud(tas, 0);
	int x = act->x;
	int y = act->y;
	if(x+1 < m->width && m->grid[x+1][y]!=UNDESTRUCTIBLE_BLOCK && m->grid[x+1][y] != DESTRUCTIBLE_BLOCK && m->grid[x+1][y] != BOMB_BLOCK && ((surBombe==0 && bombes[x+1][y]==0) || (surBombe==1))){
	    if((noeuds[x+1][y]->poids > (act->poids)+1) || (noeuds[x+1][y]->poids == 999)){
		noeuds[x+1][y]->last = act;
		noeuds[x+1][y]->poids = (act->poids)+1+ ((bombes[x+1][y]==1 && surBombe==0)? 2:0);
		addNoeud(tas, noeuds[x+1][y]);
	    }
	}
	if(y+1 < m->height && m->grid[x][y+1]!=UNDESTRUCTIBLE_BLOCK && m->grid[x][y+1] != DESTRUCTIBLE_BLOCK && m->grid[x][y+1] != BOMB_BLOCK && ((surBombe==0 && bombes[x][y+1]==0) || (surBombe==1))){
	    if(noeuds[x][y+1]->poids > (act->poids)+1 || (noeuds[x][y+1]->poids == 999)){
		noeuds[x][y+1]->last = act;
		noeuds[x][y+1]->poids = (act->poids)+1+ ((bombes[x][y+1]==1 && surBombe==0)? 2:0);
		addNoeud(tas, noeuds[x][y+1]);
	    }
	}
	if(y-1 >= 0 && m->grid[x][y-1]!=UNDESTRUCTIBLE_BLOCK && m->grid[x][y-1] != DESTRUCTIBLE_BLOCK && m->grid[x][y-1] != BOMB_BLOCK && ((surBombe==0 && bombes[x][y-1]==0) || (surBombe==1))){
	    if(noeuds[x][y-1]->poids > (act->poids)+1 || (noeuds[x][y-1]->poids == 999)){
		noeuds[x][y-1]->last = act;
		noeuds[x][y-1]->poids = (act->poids)+1+ ((bombes[x][y-1]==1 && surBombe==0)? 2:0);
		addNoeud(tas, noeuds[x][y-1]);
	    }
	}
	if(x-1 >= 0 && m->grid[x-1][y]!=UNDESTRUCTIBLE_BLOCK && m->grid[x-1][y] != DESTRUCTIBLE_BLOCK && m->grid[x-1][y] != BOMB_BLOCK && ((surBombe==0 && bombes[x-1][y]==0) || (surBombe==1))){
	    if(noeuds[x-1][y]->poids > (act->poids)+1 || (noeuds[x-1][y]->poids == 999)){
		noeuds[x-1][y]->last = act;
		noeuds[x-1][y]->poids = (act->poids)+1+ ((bombes[x-1][y]==1 && surBombe == 0)? 2:0);
		addNoeud(tas, noeuds[x-1][y]);
	    }
	}
    }while((tas->last) > -1 && (tas->last) < TTABLE && (act->x != xDest || act->y != yDest));
    if(act->x == xDest && act->y == yDest){
	ret[2] = act->poids;
    
	noeud *prelast = act;
	while(act->last!=NULL){
	    prelast = act;
	    act = act->last;
	}
	ret[0]=prelast->x;
	ret[1]=prelast->y;
    }
    for(i = 0; i < m->width; i++){
	for(j = 0; j < m->height; j++){
	    free(noeuds[i][j]);
	}
	free(noeuds[i]);
    }
    free(noeuds);
    deleteTas(tas);
    return ret;
}

int** ChercheBombes(map* m){
    int i,j;
    int **ret = malloc(sizeof(int*)*m->width);
    for(i=0; i <m->width;i++){
	ret[i] = malloc(sizeof(int)*m->height);
	for(j = 0; j < m->height; j++){
	    ret[i][j] = 0;
	}
    }
    bombList* l = m->bombs;
    while(l != NULL){
	bomb *b = l->data;
	int x = b->x;
	int y = b->y;
	ret[x][y] = 1;
	for (i=0;i<b->explosion;i++)
	{
	    if(x+i<0 || x+i >= m->width || y < 0 || y >= m->height){
		break;
	    }else if(m->grid[x+i][y] == UNDESTRUCTIBLE_BLOCK || m->grid[x+i][y] == DESTRUCTIBLE_BLOCK){
		break;
	    }else{
		ret[x+i][y] = 1;
	    }
	}
	for (i=0;i<b->explosion;i++)
	{
	    if(x-i<0 || x-i >= m->width || y < 0 || y >= m->height){
		break;
	    }else if(m->grid[x-i][y] == UNDESTRUCTIBLE_BLOCK || m->grid[x-i][y] == DESTRUCTIBLE_BLOCK){
		break;
	    }else{
		ret[x-i][y] = 1;
	    }
	}
	for (i=0;i<b->explosion;i++)
	{
	    if(x<0 || x >= m->width || y+i < 0 || y+i >= m->height){
		break;
	    }else if(m->grid[x][y+i] == UNDESTRUCTIBLE_BLOCK || m->grid[x][y+i] == DESTRUCTIBLE_BLOCK){
		break;
	    }else{
		ret[x][y+i] = 1;
	    }
	}
	for (i=0;i<b->explosion;i++)
	{
	    if(x<0 || x >= m->width || y-i < 0 || y-i >= m->height){
		break;
	    }else if(m->grid[x][y-i] == UNDESTRUCTIBLE_BLOCK || m->grid[x][y-i] == DESTRUCTIBLE_BLOCK){
		break;
	    }else{
		ret[x][y-i] = 1;
	    }
	}
	l = l->next;
    }
    return ret;
}

int* TrouverProche(int x, int y, map* m, int** tab, int** bombes){
    int i,j;
    int *ret = malloc(sizeof(int)*3);
    ret[0] = x;
    ret[1] = y;
    ret[2] = 0;
    int prochest = 999;
    for(i=0; i<m->width;i++){
	for(j=0; j<m->height;j++){
	    if((tab[i][j]>0 || (tab[i][j] == 0 && ret[2] == 0)) && bombes[i][j] != 1){
		int *temp = AllerVers(x,y,i,j,m,bombes); //La distance et la position
		//On retire l'importance de la case à sa distance, pour prioriser les bonus et l'explosion du joueur
		if((temp[2]-(tab[i][j])*2) <= prochest && temp[0] != -1){
		    prochest = temp[2]-(tab[i][j]*2);
		    ret[0] = i;
		    ret[1] = j;
		    ret[2] = tab[i][j];
		}
	    }
	}
    }
    return ret;
}

int** ChercheDest(map* m, player *p, int** bombes){
    int i,j;
    int **ret = malloc(sizeof(int*)*m->width);
    for(i=0; i <m->width;i++){
	ret[i] = malloc(sizeof(int)*m->height);
	for(j = 0; j < m->height; j++){
	    int nbrDestr = 0;
	    if(m->grid[i][j]!=UNDESTRUCTIBLE_BLOCK && m->grid[i][j]!=DESTRUCTIBLE_BLOCK && m->grid[i][j]!=BOMB_BLOCK && bombes[i][j]==0){
		if(m->grid[i][j]==EMPTY_BLOCK){
		    if(i+1<m->width && m->grid[i+1][j] == DESTRUCTIBLE_BLOCK){
			nbrDestr++;
		    }
	  
		    if(i-1 >= 0 && m->grid[i-1][j] == DESTRUCTIBLE_BLOCK){
			nbrDestr++;
		    }
		    if(j+1 < m->height && m->grid[i][j+1] == DESTRUCTIBLE_BLOCK){
			nbrDestr++;
		    }
		    if(j-1 >= 0 && m->grid[i][j-1] == DESTRUCTIBLE_BLOCK){
			nbrDestr++;
		    }
		}else{
		    nbrDestr = 5;
		}
	    }else if(bombes[i][j] == 1){
		nbrDestr = -5;
	    }
      
	    ret[i][j] = nbrDestr;
	}
    }
    for(i=0; i<m->nbrPlayers;i++){
	if(m->players[i]->x!=-1 && m->players[i] != p){
	    if(bombes[m->players[i]->x][m->players[i]->y]== 0){
		ret[m->players[i]->x][m->players[i]->y] = 6;
	    }
	}
    }
    return ret;
}
