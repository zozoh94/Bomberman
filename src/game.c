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
void Solo(map *m, SDL_Surface *dest){
  Sprite *spr=malloc(sizeof(Sprite));
  player **tab = malloc(sizeof(player*)*2);
  player *p = InitPlayer(1, 1, 0, 1, 150, 5, 0, J1, m, spr, "p1.bmp");
  player *ia = InitPlayer(1, 1, 0, 1, 150, 5, 0, IA, m, spr, "p2.bmp");
  tab[0] = p;
  tab[1] = ia;
  fprintf(stderr,"%d, map(%d,%d):\n",InitMap(m, 1, tab),m->width, m->height);
  int i,j;
  for(i = 0; i < m->width; i++){
    for(j = 0; j < m->height; j++){
      fprintf(stderr,"%d",m->grid[j][i]);
    }
    fprintf(stderr,"\n");
  }
  GameLoop(m, SOLO, dest);  
}

void GameLoop(map *m, vCond cond, SDL_Surface *dest){
  SDL_Event event;
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
	case SDLK_q:
	  inputTab[K_Q] = 1;
	  break;
	case SDLK_z:
	  inputTab[K_Z] = 1;
	  break;
	case SDLK_d:
	  inputTab[K_D] = 1;
	  break;
	case SDLK_s:
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
	  win = 1;
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
	case SDLK_q:
	  inputTab[K_Q] = 0;
	  break;
	case SDLK_z:
	  inputTab[K_Z] = 0;
	  break;
	case SDLK_d:
	  inputTab[K_D] = 0;
	  break;
	case SDLK_s:
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
    SDL_FillRect(dest, NULL, 0);
    MapLoop(m, dest);
    BombLoop(m, dest);
    PlayerLoop(m, inputTab ,dest);
    TestWin(m, cond, &winner);
    SDL_Flip(dest);
  }
}

void BombLoop(map* map, SDL_Surface *dest){
  bombList* l = map->bombs;
  while(l!=NULL && l->next != NULL){
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

void PlayerLoop(map* map, int* input, SDL_Surface *dest){
  int i;
  player *p=NULL;
  for(i = 0; i < map->nbrPlayers; i++){
    p = map->players[i];
    if(p->moveTimer > 0){ // On arrête le décompte à -1
      p->moveTimer --;
      switch(p->sprite->orientation){
      case 0 :
	p->sprite->source.y=(p->y)*32/* + 32/(p->moveTimer - p->speed)*/;
	break;
      case 1 :
	p->sprite->source.x=(p->x)*32/* + 32/(p->moveTimer - p->speed)*/;
	break;
      case 2 :
	p->sprite->source.y=(p->y)*32/* - 32/(p->moveTimer - p->speed)*/;
	break;
      case 3 :
	p->sprite->source.x=(p->x)*32/* - 32/(p->moveTimer - p->speed)*/;
	break;
      }
    }
    p->sprite->source.x=(p->x)*32;
    p->sprite->source.y=(p->y)*32;
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
	TryMove(p, p->x-1, p->y);
      }
      if(input[K_UP]==1 && p->moveTimer == -1){
	TryMove(p, p->x, p->y-1);
      }
      if(input[K_RIGHT]==1 && p->moveTimer == -1){
	TryMove(p, p->x+1, p->y);
      }
      if(input[K_DOWN]==1 && p->moveTimer == -1){
	TryMove(p, p->x, p->y+1);
      }
    }
    
    if(p->type == J2){
      if(input[K_Q]==1 && p->moveTimer == -1){
	TryMove(p, p->x-1, p->y);
      }
      if(input[K_Z]==1 && p->moveTimer == -1){
	TryMove(p, p->x, p->y-1);
      }
      if(input[K_D]==1 && p->moveTimer == -1){
	TryMove(p, p->x+1, p->y);
      }
      if(input[K_S]==1 && p->moveTimer == -1){
	TryMove(p, p->x, p->y+1);
      }
    }
    fprintf(stderr,"bom: %d/%d spr: %d/%d (%d/%d)\n", p->x, p->y, p->sprite->source.x, p->sprite->source.y, (p->x)/32, (p->y)/32);
    dessinerSprite(p->sprite, dest);
  }
}

void MapLoop(map* map, SDL_Surface *dest){
  SDL_Rect position;
  int i,j;
  for(i=0;i<map->height;i++)
    {
      for(j=0;j<map->width;j++)
	{
	  position.x=i*32;
	  position.y=j*32;
	  switch(map->grid[i][j])
	    {
	    case UNDESTRUCTIBLE_BLOCK :
	      SDL_BlitSurface( map->undestructibleBlock, NULL, dest, &position);
	      break;
	    case DESTRUCTIBLE_BLOCK :
	      SDL_BlitSurface( map->destructibleBlock, NULL, dest, &position);
	      break;
	    default :
	      SDL_BlitSurface( map->floor, NULL, dest, &position);
	      break;
	    }
	}
    }
}

int TestWin(map* map, vCond cond, player** winner){
  return 0;
}
