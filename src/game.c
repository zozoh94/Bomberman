#include "game.h"

#define EXPLOSIONTIME 10

void Solo(map *m, player *p){
  AddPlayers(m, &p);
  GameLoop(m);
}

void GameLoop(map *m, vCond cond){
  int win = 0;
  player* winner;
  while(win == 0){
    
    BombLoop(m);
    PlayerLoop(m);
    TestWin(m, cond, &winner);
  }
}

void BombLoop(map* map){
  bombList* l = map->bombs;
  while(l->next != NULL){
    l->data->timer--;
    if(l->data->timer==-EXPLOSIONTIME){ // Destruction de la bombe
      map->bombs = RemoveBombList(map->bombs, l->data);
    }
    if(l->data->timer<=0){ // Explosion de la bombe
      Explode(map, l->data);
    }
    l = l->next;
  }
}

void PlayerLoop(map* map){
  int i;
  player* p;
  for(i = 0; i < map->nbrPlayer; i++){
    p = map->players[i];
    if(p->moveTimer > 0){ // On arrête le décompte à -1
      p->moveTimer --;
    }
    if(p->moveTimer == 0){
      Move(p);
    }
  }
}
