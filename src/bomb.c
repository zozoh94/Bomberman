#include "bomb.h"




bomb* InitBomb(int x, int y, int rayon, int timer, player* myPlayer, btype type){
  bomb *b = malloc (sizeof(bomb));
  b->x = x;
  b->y = y;
  b->explosion = rayon;
  b->timer = timer;
  b->myPlayer = myPlayer;
  b->type = type;
  Sprite *bombS=malloc(sizeof(Sprite));
  b->sprite = bombS;
  if(ChargeBomb (b->sprite, "bomb.png") == 0){
    fprintf(stderr,"erreur chargement image bombe\n");
  }
  b->flamme = IMG_Load("flame.png");
  if(b->flamme == NULL){
    fprintf(stderr,"erreur chargement image flammes\n");
  }

  int **explZ=malloc(sizeof(int*)*myPlayer->map->width);
  b->explozone = explZ;
  int i,j;
  for(i = 0; i < myPlayer->map->width; i++){
    b->explozone[i] = malloc(sizeof(int)*myPlayer->map->height);
    for(j = 0; j < myPlayer->map->height; j++){
      b->explozone[i][j] = 0;
    }
  }
  return b;
}

bomb* CreateBomb(player* p){
  //Changer rayon, timer et type en fonction du joueur
  return InitBomb(p->x, p->y, p->bombR, TIMERBOMB, p, p->bombT);
}

int DoExplode(int x, int y, map* map, bomb* bomb){
  int i;
  if (map->grid[x][y]==UNDESTRUCTIBLE_BLOCK) //bloc indestructible : fin
    {
      return 1;
    }
  else if (map->grid[bomb->x][bomb->y]==DESTRUCTIBLE_BLOCK) //bloc destructible : destruction du bloc + score du joueur + fin
    {
      map->grid[x][y]=0;
      bomb->myPlayer->score = bomb->myPlayer->score +2;
      return 1;
    }
  else if (map->grid[x][y]==BOMB_BLOCK) //bombe : explosion de la bombe + continue
    {
      GetBomb (map,bomb->x,bomb->y)->timer = 0;
      return 0;
    }
  else //rien ou bonus : continue
    {
      bomb->explozone[x][y]=1;
      //gestion des morts
      for(i=0; i < map->nbrPlayers; i++){
	if ((map->players[i]->x=x) && (map->players[i]->y=y))
	  {
	    map->players[i]->x=map->startingBlocks[i][0];
	    map->players[i]->y=map->startingBlocks[i][1];
	    if(map->players[i] != bomb->myPlayer){
	      bomb->myPlayer->score += 5;
	    }
	  }
      }
      return 0;
    }
}


void Explode(map* map, bomb* bomb){
  int i,x,y;
  x = bomb->x ;
  y = bomb->y ;
  
  bomb->explozone[x][y]=1;
  map->grid[x][y] = 0;
  DoExplode(x,y,map,bomb);
  for (i=0;i<bomb->explosion;i++)
    {
      if(DoExplode(x+i,y,map,bomb) == 1){
	break;
      }
    }
  for (i=0;i<bomb->explosion;i++)
    {
      if(DoExplode(x-i,y,map,bomb) == 1){
	break;
      }
    }
  for (i=0;i<bomb->explosion;i++)
    {
      if(DoExplode(x,y+i,map,bomb) == 1){
	break;
      }
    }
  for (i=0;i<bomb->explosion;i++)
    {
      if(DoExplode(x,y-i,map,bomb) == 1){
	break;
      }
    }
}

bomb* GetBomb (map* map, int x, int y)
{
  bombList* bombList = map->bombs;
  while (((bombList->data)->x != x) && ((bombList->data)->y != y)) //parcoure la liste
    {
      bombList = bombList->next;
    }
  return bombList->data; //retourne celle demandée
} 

bombList* NewBombList()
{
  return NULL;
}
	
bombList *AddBombList(bombList* l, bomb* b)
{
  fprintf(stderr,"ADDBOMBLIST\n");
  bombList *bl = malloc (sizeof(bomb));
  bl->data = b;
  bl->next = l;
  return bl;
}

bombList* RemoveBombList(bombList* l, bomb* b)
{
  fprintf(stderr,"REMOVE BOMBLIST\n");
  bombList *previous = l;
  bombList *temp;
  if (previous->data == b) //cas du premier élément
    {
      l = previous->next;
      deleteSprite(previous->data->sprite);
      free(previous);
    }
  else //parcoure la liste en deux temps, avec temp et avec previous qui a l'ancienne valeur de temp
    {
      temp = previous->next;
      while ((temp!=NULL) || (temp->data==b))
	{
	  previous = temp;
	  temp = temp->next;
	}
      if (temp->data==b) //supprime la bombe en faisant ponté previous vers le suivant de temp
	{
	  previous->next = temp->next;
	  free(temp);
	}
    }
  return l;
}
