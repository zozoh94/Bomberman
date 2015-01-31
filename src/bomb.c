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
  b->flammeC = IMG_Load("flameC.png");
  if(b->flammeC == NULL){
    fprintf(stderr,"erreur chargement image flammes centre\n");
  }
  b->flammeD = IMG_Load("flameD.png");
  if(b->flammeD == NULL){
    fprintf(stderr,"erreur chargement image flammes droite\n");
  }
  b->flammeB = IMG_Load("flameB.png");
  if(b->flammeB == NULL){
    fprintf(stderr,"erreur chargement image flammes bas\n");
  }
  b->flammeG = IMG_Load("flameG.png");
  if(b->flammeG == NULL){
    fprintf(stderr,"erreur chargement image flammes gauche\n");
  }
  b->flammeH = IMG_Load("flameH.png");
  if(b->flammeH == NULL){
    fprintf(stderr,"erreur chargement image flammes haut\n");
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

int DoExplode(int x, int y, map* map, bomb* bomb, int dir){
  int i, random;
  if(x<0 || x >= map->width || y < 0 || y >= map->height){
    return 1;
  }
  if (map->grid[x][y]==UNDESTRUCTIBLE_BLOCK) //bloc indestructible : fin
    {
      return 1;
    }
  else if (map->grid[x][y]==DESTRUCTIBLE_BLOCK) //bloc destructible : destruction du bloc + score du joueur + fin
    {
      bomb->explozone[x][y]=dir;
      if (map->victory == POINTS)
	  {
	    bomb->myPlayer->score = bomb->myPlayer->score +1;
	  }
      random = rand()%15;
      switch (random)
	{
	case 0 :
	  map->grid[x][y]=BONUS_RADIUS_BLOCK;
	  break;
	case 1 :
	  map->grid[x][y]=BONUS_BOMB_LIMIT_BLOCK;
	  break;
	case 2 :
	  map->grid[x][y]=BONUS_SPEED_BLOCK;
	  break;
	case 3 :
	  map->grid[x][y]=BONUS_INVULNERABILITY_BLOCK;
	  break;
	default :
	  map->grid[x][y]=0;
	  break;
	}
      return 1;
    }
  else if (map->grid[x][y]==BOMB_BLOCK) //bombe : explosion de la bombe + continue
    {
      bomb->explozone[x][y]=dir;
      GetBomb (map,bomb->x,bomb->y)->timer = 1;
      return 0;
    }
  else //rien ou bonus : continue
    {
      map->grid[x][y]=0; //Détruit les bonus si il y en as.
      bomb->explozone[x][y]=dir;
      //gestion des morts
      for(i=0; i < map->nbrPlayers; i++){
	if ((map->players[i]->x==x) && (map->players[i]->y==y) && (map->players[i]->invulnerability==0))
	  {
	    if (map->victory == POINTS)
	      {
		map->players[i]->x=map->startingBlocks[i][0];
		map->players[i]->y=map->startingBlocks[i][1];
		map->players[i]->destX=map->startingBlocks[i][0];
		map->players[i]->destY=map->startingBlocks[i][1];
		map->players[i]->invulnerability = 32;
		if(map->players[i] != bomb->myPlayer){
		  bomb->myPlayer->score += 5;
		}
	      }
	    else if (map->victory == VERSUS)
	      {
		if (map->players[i]->score>1)
		  {
		    map->players[i]->x=map->startingBlocks[i][0];
		    map->players[i]->y=map->startingBlocks[i][1];
		    map->players[i]->destX=map->startingBlocks[i][0];
		    map->players[i]->destY=map->startingBlocks[i][1];
		    map->players[i]->invulnerability = 32;
		    map->players[i]->score--;
		  }
		else
		  {
		    map->players[i]->x=-1;
		    map->players[i]->y=-1;
		    map->players[i]->destX=-1;
		    map->players[i]->destY=-1;
		    map->players[i]->score--;
		  }
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
  DoExplode(x,y,map,bomb,5);
  for (i=0;i<bomb->explosion;i++)
    {
      if(DoExplode(x+i,y,map,bomb,1) == 1){
	break;
      }
    }
  for (i=0;i<bomb->explosion;i++)
    {
      if(DoExplode(x-i,y,map,bomb,3) == 1){
	break;
      }
    }
  for (i=0;i<bomb->explosion;i++)
    {
      if(DoExplode(x,y+i,map,bomb,2) == 1){
	break;
      }
    }
  for (i=0;i<bomb->explosion;i++)
    {
      if(DoExplode(x,y-i,map,bomb,4) == 1){
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
  bombList *bl = malloc (sizeof(bomb));
  bl->data = b;
  bl->next = l;
  return bl;
}

bombList* RemoveBombList(bombList* l, bomb* b)
{
  bombList *previous = l;
  bombList *temp;
  if (previous->data == b) //cas du premier élément
    {
      if(previous->next != NULL){
	l = previous->next;
      }else{
	l = NULL;
      }
      deleteSprite(previous->data->sprite);
      free(previous);
    }
  else //parcoure la liste en deux temps, avec temp et avec previous qui a l'ancienne valeur de temp
    {
      temp = previous->next;
      while ((temp!=NULL) && (temp->data!=b))
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
