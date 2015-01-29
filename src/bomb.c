#include "bomb.h"


int ChargeBomb( Sprite *sprite, const char *image ){
  sprite->image = IMG_Load( image );
  if ( sprite->image==NULL )
  {
    fprintf(stderr, "Echec de chargement du fichier %s : %s.\n", image, SDL_GetError());
    return 0;
  }
		
  // on fixe la cle de transparance ici blanc
  SDL_SetColorKey( sprite->image, SDL_SRCCOLORKEY, SDL_MapRGB( sprite->image->format, 255, 255, 255 ));

  // le sprite n'est pas animé par defaut
  sprite->anim = 0;

  // on commence par la première animation
  sprite->current_anim =0;

  // le sprite dispose de deux animations
  sprite->total_anims = 2;

  // temps d'affichage pour une animation
  sprite->time_anim = 15;

  // Le temps qu'il reste à afficher l'animation courante
  sprite->time_current_anim = 0;

  // On definit ensuite les dimentions du sprite.
  sprite->width = 24;
  sprite->height = 24;
	
  sprite->source.x = sprite->current_anim * sprite->width;
  sprite->source.y = 0;
  sprite->source.w = sprite->width;
  sprite->source.h = sprite->height;

  return 1;
}

bomb* InitBomb(int x, int y, int rayon, int timer, player* myPlayer, btype type){
  bomb *b = malloc (sizeof(bomb));
  b->x = x;
  b->y = y;
  b->explosion = rayon;
  b->timer = timer;
  b->myPlayer = myPlayer;
  b->type = type;
  Sprite *flammeS=malloc(sizeof(Sprite));
  Sprite *bombS=malloc(sizeof(Sprite));
  b->sprite = bombS;
  b->flamme=flammeS;
  if(ChargeBomb (b->sprite, "bomb.png") == 0){
    fprintf(stderr,"erreur chargement image bombe\n");
  }
  if(ChargeBomb (b->flamme, "flame.png") == 0){
    fprintf(stderr,"erreur chargement image flammes\n");
  }

  int **explZ=malloc(sizeof(int*)*rayon);
  b->explozone = explZ;
  int i,j;
  for(i = 0; i < rayon; i++){
    b->explozone[i] = malloc(sizeof(int)*rayon);
    for(j = 0; j < rayon; j++){
      b->explozone[i][j] = 0;
    }
  }
  return b;
}

bomb* CreateBomb(player* p){
  //Changer rayon, timer et type en fonction du joueur
  return InitBomb(p->x, p->y, p->bombR, TIMERBOMB, p, p->bombT);
}


void Explode(map* map, bomb* bomb){
  int i,x,y;
  x = bomb->myPlayer->x ;
  y = bomb->myPlayer->y ;
  RemoveBombList(map->bombs, bomb); //retire la bombe de la liste des bombes de la map
  bomb->myPlayer->bombs --; //décrémente le nombre de bombes posées par le joueur qui a posé la bombe
  
  if ((map->players[0]->x=x) && (map->players[0]->y=y))
  {
    map->players[0]->x=map->startingBlocks[0][0];
    map->players[0]->y=map->startingBlocks[0][1];
    map->players[1]->score = map->players[1]->score +5;
  }
  if ((map->players[1]->x=x) && (map->players[1]->y=y))
  {
    map->players[1]->x=map->startingBlocks[1][0];
    map->players[1]->y=map->startingBlocks[1][1];
    map->players[0]->score = map->players[0]->score +5;
  }
  bomb->explozone[x][y]=1;
  for (i=0;i<bomb->explosion;i++)
    {
      if (map->grid[bomb->x+i][bomb->y]==UNDESTRUCTIBLE_BLOCK) //bloc indestructible : fin
	{
	  break;
	}
      else if (map->grid[bomb->x+i][bomb->y]==DESTRUCTIBLE_BLOCK) //bloc destructible : destruction du bloc + score du joueur + fin
	{
	  map->grid[bomb->x+i][bomb->y]=0;
	  bomb->myPlayer->score = bomb->myPlayer->score +2;
	  break;
	}
      else if (map->grid[bomb->x+i][bomb->y]==BOMB_BLOCK) //bombe : explosion de la bombe + continue
	{
	  GetBomb (map,bomb->x+i,bomb->y)->timer = 0;
	}
      else //rien ou bonus : continue
	{
	  bomb->explozone[x+i][y]=1;
	  //gestion des morts
	  if ((map->players[0]->x=x+i) && (map->players[0]->y=y))
	  {
	    map->players[0]->x=map->startingBlocks[0][0];
    	    map->players[0]->y=map->startingBlocks[0][1];
	    map->players[1]->score = map->players[1]->score +5;
	  }
	  if ((map->players[1]->x=x+i) && (map->players[1]->y=y))
	  {
	    map->players[1]->x=map->startingBlocks[1][0];
    	    map->players[1]->y=map->startingBlocks[1][1];
	    map->players[0]->score = map->players[0]->score +5;
	  }
	}
    }
  for (i=0;i<bomb->explosion;i++)
    {
      if (map->grid[bomb->x-i][bomb->y]==UNDESTRUCTIBLE_BLOCK)
	{
	  break;
	}
      else if (map->grid[bomb->x-i][bomb->y]==DESTRUCTIBLE_BLOCK)
	{
	  map->grid[bomb->x-i][bomb->y]=0;
	  bomb->myPlayer->score = bomb->myPlayer->score +2;
	  break;
	}
      else if (map->grid[bomb->x-i][bomb->y]==BOMB_BLOCK)
	{
	  GetBomb (map,bomb->x+i,bomb->y)->timer = 0;
	  /* bomb* bombTouch = GetBomb (map,bomb->x-i,bomb->y);
	     bombTouch->timer = 0;*/
	}
      else
	{
	  bomb->explozone[x-i][y]=1;
	  //gestion des morts
	  if ((map->players[0]->x=x-i) && (map->players[0]->y=y))
	  {
	    map->players[0]->x=map->startingBlocks[0][0];
    	    map->players[0]->y=map->startingBlocks[0][1];
	    map->players[1]->score = map->players[1]->score +5;
	  }
	  if ((map->players[1]->x=x-i) && (map->players[1]->y=y))
	  {
	    map->players[1]->x=map->startingBlocks[1][0];
    	    map->players[1]->y=map->startingBlocks[1][1];
	    map->players[0]->score = map->players[0]->score +5;
	  }
	}
    }
  for (i=0;i<bomb->explosion;i++)
    {
      if (map->grid[bomb->x][bomb->y+i]==UNDESTRUCTIBLE_BLOCK)
	{
	  break;
	}
      else if (map->grid[bomb->x][bomb->y+i]==DESTRUCTIBLE_BLOCK)
	{
	  map->grid[bomb->x][bomb->y+i]=0;
	  bomb->myPlayer->score = bomb->myPlayer->score +2;
	  break;
	}
      else if (map->grid[bomb->x][bomb->y+i]==BOMB_BLOCK)
	{
	  GetBomb (map,bomb->x+i,bomb->y)->timer = 0;
	  /*	  bomb* bombTouch = GetBomb (map,bomb->x,bomb->y+i);
		  bombTouch->timer = 0;*/
	}
      else
	{
	  bomb->explozone[x][y+i]=1;
	  //gestion des morts
	  if ((map->players[0]->x=x) && (map->players[0]->y=y+1))
	  {
	    map->players[0]->x=map->startingBlocks[0][0];
    	    map->players[0]->y=map->startingBlocks[0][1];
	    map->players[1]->score = map->players[1]->score +5;
	  }
	  if ((map->players[1]->x=x) && (map->players[1]->y=y+1))
	  {
	    map->players[1]->x=map->startingBlocks[1][0];
    	    map->players[1]->y=map->startingBlocks[1][1];
	    map->players[0]->score = map->players[0]->score +5;
	  }
	}
    }
  for (i=0;i<bomb->explosion;i++)
    {
      if (map->grid[bomb->x][bomb->y-i]==UNDESTRUCTIBLE_BLOCK)
	{
	  break;
	}
      else if (map->grid[bomb->x][bomb->y-i]==DESTRUCTIBLE_BLOCK)
	{
	  map->grid[bomb->x][bomb->y-i]=0;
	  bomb->myPlayer->score = bomb->myPlayer->score +2;
	  break;
	}
      else if (map->grid[bomb->x][bomb->y-i]==BOMB_BLOCK)
	{
	  GetBomb (map,bomb->x+i,bomb->y)->timer = 0;
	  /*	  bomb* bombTouch = GetBomb (map,bomb->x,bomb->y-i);
		  bombTouch->timer = 0;*/
	}
      else
	{
	  bomb->explozone[x][y-i]=1;
	  //gestion des morts
	  if ((map->players[0]->x=x) && (map->players[0]->y=y-i))
	  {
	    map->players[0]->x=map->startingBlocks[0][0];
    	    map->players[0]->y=map->startingBlocks[0][1];
	    map->players[1]->score = map->players[1]->score +5;
	  }
	  if ((map->players[1]->x=x) && (map->players[1]->y=y-i))
	  {
	    map->players[1]->x=map->startingBlocks[1][0];
    	    map->players[1]->y=map->startingBlocks[1][1];
	    map->players[0]->score = map->players[0]->score +5;
	  }
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
