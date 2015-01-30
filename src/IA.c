#include "IA.h"


int* AllerVers(int xA, int yA, int xDest, int yDest, map* m, int** bombes){
  int *ret = malloc(sizeof(int)*3);
  int surBombe = 0; // A mettre à 1 sir on est sur un emplacement dangereux.
  if(bombes[xA][yA] == 1){
    surBombe = 1;
  }
  //Astar de xA yA à xDest yDest en passant par map
  //Recuperer le tout premier mouvement
  //Le mettre dans ret
  ret[0]=-1;
  ret[1]=-1;
  ret[2] = -1;

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
    bomb b = l->data;
    int x = b->x;
    int y = b->y;
    ret[x][y] = 1;
    for (i=0;i<b->explosion;i++)
      {
	if(x+i<0 || x+i >= map->width || y < 0 || y >= map->height){
	  break;
	}else if(m->grid[x+i][y] == UNDESTRUCTIBLE_BLOCK || m->grid[x+i][y] == DESTRUCTIBLE_BLOCK){
	  break;
	}else{
	  ret[x+i][y] = 1;
	}
      }
    for (i=0;i<bomb->explosion;i++)
      {
	if(x-i<0 || x-i >= map->width || y < 0 || y >= map->height){
	  break;
	}else if(m->grid[x-i][y] == UNDESTRUCTIBLE_BLOCK || m->grid[x-i][y] == DESTRUCTIBLE_BLOCK){
	  break;
	}else{
	  ret[x-i][y] = 1;
	}
      }
    for (i=0;i<bomb->explosion;i++)
      {
	if(x<0 || x >= map->width || y+i < 0 || y+i >= map->height){
	  break;
	}else if(m->grid[x][y+i] == UNDESTRUCTIBLE_BLOCK || m->grid[x][y+i] == DESTRUCTIBLE_BLOCK){
	  break;
	}else{
	  ret[x][y+i] = 1;
	}
      }
    for (i=0;i<bomb->explosion;i++)
      {
	if(x<0 || x >= map->width || y-i < 0 || y-i >= map->height){
	  break;
	}else if(m->grid[x][y-i] == UNDESTRUCTIBLE_BLOCK || m->grid[x][y-i] == DESTRUCTIBLE_BLOCK){
	  break;
	}else{
	  ret[x][y-i] = 1;
	}
      }
  }
  return ret;
}

int* TrouverProche(int x, int y, map* m, int** tab){
  int i,j;
  int *ret = malloc(sizeof(int)*2);
  ret[0] = -1;
  ret[1] = -1;
  int prochest = 999;
  for(i=0; i<m->width;i++){
    for(j=0; j<m->height;j++){
      if(tab[i][j]=0){
	int* temp = allerVers(x,y,i,j,m);
	if( temp[2] < prochest){
	  prochest = temp[2];
	  ret[0] = i;
	  ret[1] = j;
	}
      }
    }
  }
  return ret;
}

int** ChercheDest(map* m){
  int i,j,k;
  int **ret = malloc(sizeof(int*)*m->width);
  for(i=0; i <m->width;i++){
    ret[i] = malloc(sizeof(int)*m->height);
    for(j = 0; j < m->height; j++){
      int nbrDestr = 0;
      if(m->grid[i][j]!=UNDESTRUCTIBLE_BLOCK && m->grid[i][j]!=DESTRUCTIBLE_BLOCK && m->grid[i][j]!=BOMB_BLOCK){
	if(m->grid[i][j]==EMPTY_BLOCK){
	  if(m->grid[i+1][j] == DESTRUCTIBLE_BLOCK){
	    nbrDestr++;
	  }
	  if(m->grid[i-1][j] == DESTRUCTIBLE_BLOCK){
	    nbrDestr++;
	  }
	  if(m->grid[i][j+1] == DESTRUCTIBLE_BLOCK){
	    nbrDestr++;
	  }
	  if(m->grid[i][j-1] == DESTRUCTIBLE_BLOCK){
	    nbrDestr++;
	  }
	}else{
	  nbrDestr = 5;
	}
      }
      ret[i][j] = nbrDestr;
    }
  }
  return ret;
}

int Fuite(int x, int y, map* m, int** bombes){
  
}
