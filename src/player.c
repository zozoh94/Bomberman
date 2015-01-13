#include "player.h"

player *InitPlayer(int x, int y, int score, int bombMax, int speed, Map *map){
  player *p = malloc(sizeof(player));
  p->x = x;
  p->y = y;
  p->destX = x;
  p->destY = y;
  p->score = score;
  p->bombs = 0;
  p->bombMax = bombMax;
  p->speed = speed;
  p->moveTimer = 0;
  p->map = map;
  return p;
}

bomb* CreateBomb(player* p){
  //Changer rayon, timer et type en fonction du joueur
  return InitBomb(p->x, p->y, 3, 1000, p, NORMAL);
}

void PlaceBomb(player *p){
  //On suppose que le joueur est à un endroit où il peut marcher (espace vide)
  if(p->bombs < p->bombMax && p->moveTimer == -1){
    if(IsBomb(p->map, p->x, p->y) == 0){
      bombs++;
      AddBombList(p->map->bombs, CreateBomb(p));
      p->map->grid[p->x][p->y] = 4;
    }
  }
}

void TryMove(player *p, int X, int Y){
  if(p->moveTimer == -1){
    if(p->map->grid->[X][Y]==0 || p->map->grid->[X][Y]==2 || p->map->grid->[X][Y]>10){
      p->moveTimer = p->speed;
      p->destX = X;
      p->destY = Y;
    }
  }
}

void Move(player* p){
  p->x = p->destX;
  p->y = p->destY;
  moveTimer = -1;
}
