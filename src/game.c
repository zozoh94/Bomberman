#include "game.h"
#define EXPLOSIONTIME 10
#define KEYNUMBER 11
#define K_LEFT 0
#define K_UP 1
#define K_RIGHT 2
#define K_DOWN 3
#define K_Q 4
#define K_Z 5
#define K_D 6
#define K_S 7
#define K_ENTER 8
#define K_SPACE 9
#define K_ESCAPE 10
void Solo(map *m, player *p){
  AddPlayers(m, &p);
  GameLoop(m);
}

void GameLoop(map *m, vCond cond){
  int win;
  player* winner;
  //Tableau des appuis à tester, 0 = relevé, 1 = appuyé
  int* inputTab = malloc(sizeof(int)*KEYNUMBER);
  for(win = 0; win < KEYNUMBER; win++){
    inputTab[win] = 0;
  }
  win = 0;
  while(win == 0){
    while( SDL_PollEvent( &event ) ){
      switch( event.type ){
      case SDL_KEYDOWN:
	switch( event.key.keysym.sym ){
	case SDLK_LEFT:
	  inputTab[K_LEFT] = 1;
	  break;
	case SDLK_RIGHT:
	  inputTab[K_RIGHT] = 1;
	  break;
	case SDLK_UP:
	  inputTab[K_UP] = 1;
	  break;
	case SDLK_DOWN:
	  inputTab[K_DOWN] = 1;
	  break;
	case SDLK_Q:
	  inputTab[K_Q] = 1;
	  break;
	case SDLK_Z:
	  inputTab[K_Z] = 1;
	  break;
	case SDLK_D:
	  inputTab[K_D] = 1;
	  break;
	case SDLK_S:
	  inputTab[K_S] = 1;
	  break;
	case SDLK_RETURN:
	  inputTab[K_ENTER] = 1;
	  break;
	case SDLK_SPACE:
	  inputTab[K_SPACE] = 1;
	  break;
	case SDLK_ESCAPE:
	  inputTab[K_ESCAPE] = 1;
	  break;
	default:
	  break;
	}
	break;
      case SDL_KEYUP:
	switch( event.key.keysym.sym ){
	case SDLK_LEFT:
	  inputTab[K_LEFT] = 0;
	  break;
	case SDLK_RIGHT:
	  inputTab[K_RIGHT] = 0;
	  break;
	case SDLK_UP:
	  inputTab[K_UP] = 0;
	  break;
	case SDLK_DOWN:
	  inputTab[K_DOWN] = 0;
	  break;
	case SDLK_Q:
	  inputTab[K_Q] = 0;
	  break;
	case SDLK_Z:
	  inputTab[K_Z] = 0;
	  break;
	case SDLK_D:
	  inputTab[K_D] = 0;
	  break;
	case SDLK_S:
	  inputTab[K_S] = 0;
	  break;
	case SDLK_RETURN:
	  inputTab[K_ENTER] = 0;
	  break;
	case SDLK_SPACE:
	  inputTab[K_SPACE] = 0;
	  break;
	case SDLK_ESCAPE:
	  inputTab[K_ESCAPE] = 0;
	  break;
	default:
	  break;
	}
	break;
      }
    }
    
    BombLoop(m);
    PlayerLoop(m);
    Affichage();
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

void PlayerLoop(map* map, int* input){
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
    
    //IA
    if(p->moveTimer == -1 && p->type == IA){
      //IA ICI
    }
    
    //INPUT DES JOUEURS
    if(p->type == J1){
      if(input[K_LEFT]==1 && p->moveTimer == -1){
	tryMove(p, p->x-1, p->y);
      }
      if(input[K_UP]==1 && p->moveTimer == -1){
	tryMove(p, p->x, p->y-1);
      }
      if(input[K_RIGHT]==1 && p->moveTimer == -1){
	tryMove(p, p->x+1, p->y);
      }
      if(input[K_DOWN]==1 && p->moveTimer == -1){
	tryMove(p, p->x, p->y+1);
      }
    }

    if(p->type == J2){
      if(input[K_Q]==1 && p->moveTimer == -1){
	tryMove(p, p->x-1, p->y);
      }
      if(input[K_Z]==1 && p->moveTimer == -1){
	tryMove(p, p->x, p->y-1);
      }
      if(input[K_D]==1 && p->moveTimer == -1){
	tryMove(p, p->x+1, p->y);
      }
      if(input[K_S]==1 && p->moveTimer == -1){
	tryMove(p, p->x, p->y+1);
      }
  }
}
