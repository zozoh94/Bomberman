#include "player.h"

player *InitPlayer(int x, int y, int score, int bombMax, int speed, int bombR, btype bombT, typeP type, map *map, Sprite *sprite, const char *image){
  player *p = malloc(sizeof(player));
  p->x = x;
  p->y = y;
  p->destX = x;
  p->destY = y;
  p->score = score;
  p->bombs = 0;
  p->bombMax = bombMax;
  p->speed = speed;
  p->moveTimer = -1;
  p->bombR = bombR;
  p->bombT = bombT;
  p->type = type;
  p->map = map;
  p->sprite = sprite;
  chargerBombermanSprite(sprite, image);
  return p;
}

player *AutoInit(map *map, typeP type, const char *image){
  Sprite *spr=malloc(sizeof(Sprite));
  return InitPlayer(0,0,0,1,100,3,0,type,map,spr,image);
}

void PlaceBomb(player *p){
  //On suppose que le joueur est à un endroit où il peut marcher (espace vide)
  if(p->bombs < p->bombMax && p->moveTimer == -1){
    if(p->map->grid[p->x][p->y] != 4){ //si il n'y a pas déjà une bombe
      p->bombs++;
      p->map->bombs = AddBombList(p->map->bombs, CreateBomb(p));
      p->map->grid[p->x][p->y] = 4;
    }
  }
}

void TryMove(player *p, int X, int Y){
  if(p->moveTimer == -1 && X >= 0 && Y >= 0 && X < p->map->width && Y < p->map->height){
    if(p->map->grid[X][Y]==0 || p->map->grid[X][Y]>10){
      p->moveTimer = p->speed;
      p->destX = X;
      p->destY = Y;
      p->sprite->anim = 0;
      switch(X-(p->x)){
      case 0 :
	switch(Y-(p->y)){
	case 1 :
	  fixDirectionSprite(p->sprite,0);
	  break;
	case -1 :
	  fixDirectionSprite(p->sprite,2);
	  break;
	}
      case 1 :
	fixDirectionSprite(p->sprite,1);
	break;
      case -1 :
	fixDirectionSprite(p->sprite,3);
	break;
      }
    }
  }
}

void Move(player* p){
  p->x = p->destX;
  p->y = p->destY;
  p->moveTimer = -1;
  p->sprite->anim = 0;
  p->sprite->pos.x = (p->x)*32;
  p->sprite->pos.y = (p->y)*32;
}
