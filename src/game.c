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

void StartGame(map *m, nbrP nbrPlayers, vCond cond, SDL_Surface *dest){
  player **tab;
  int nbrjoueurs;
  player *one;
  player *two;
  player *three;
  player *four;
  switch(nbrPlayers){
  case(VSIA):
    nbrjoueurs = 4;
    tab = malloc(sizeof(player*)*nbrjoueurs);
    one = AutoInit(m,J1,"p1.bmp");
    two = AutoInit(m,J2,"p2.bmp");
    three = AutoInit(m,IA,"p3.bmp");
    four = AutoInit(m,IA,"p4.bmp");
    tab[0] = one;
    tab[1] = two;
    tab[2] = three;
    tab[3] = four;
    break;
    
  case(PVP):
    nbrjoueurs = 2;
    tab = malloc(sizeof(player*)*nbrjoueurs);
    one = AutoInit(m,J1,"p1.bmp");
    two = AutoInit(m,J2,"p2.bmp");
    tab[0] = one;
    tab[1] = two;
    break;
    
  case(SOLOVSBCPIA):
    nbrjoueurs = 4;
    tab = malloc(sizeof(player*)*nbrjoueurs);
    one = AutoInit(m,J1,"p1.bmp");
    two = AutoInit(m,IA,"p2.bmp");
    three = AutoInit(m,IA,"p3.bmp");
    four = AutoInit(m,IA,"p4.bmp");
    tab[0] = one;
    tab[1] = two;
    tab[2] = three;
    tab[3] = four;
    break;
    
  default:
    nbrjoueurs = 2;
    tab = malloc(sizeof(player*)*nbrjoueurs);
    one = AutoInit(m,J1,"p1.bmp");
    two = AutoInit(m,IA,"p2.bmp");
    tab[0] = one;
    tab[1] = two;
    break;
  }
  fprintf(stderr,"%d\n",InitMap(m, nbrjoueurs, tab));
  dest = ScaleSurface(dest,(m->width)*32,(m->height)*32);
  GameLoop(m, cond, dest);  
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
  while(l!=NULL){
    fprintf(stderr,"Bombe en %d %d \n", l->data->x, l->data->y);
    l->data->timer--;
    if(l->data->timer==-EXPLOSIONTIME){ // Destruction de la bombe
      l->data->myPlayer->bombs --; //décrémente le nombre de bombes posées par le joueur qui a posé la bombe
      map->bombs = RemoveBombList(map->bombs, l->data);
    }
    if(l->data->timer<=0){ // Explosion de la bombe
      Explode(map, l->data);
    }
    if(l->data->timer>=0){
      l->data->sprite->pos.x = (l->data->x)*32+4;
      l->data->sprite->pos.y = (l->data->y)*32+4;
      dessinerSprite(l->data->sprite, dest);
    }
    if(l->data->timer<=0){
      SDL_Rect position;
      for(int i = 0; i < map->width; i++){
	for(int j = 0; j < map->height; j++){
	  if(l->data->explozone[i][j] == 1){
	    position.x=i*32+4;
	    position.y=j*32+4;
	    SDL_BlitSurface( l->data->flamme, NULL, dest, &position);
	  }
	}
      }
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
      /* Déplacement fluide, pas fonctionnel
      switch(p->sprite->orientation){
      case 0 :
	p->sprite->pos.y=(p->y)*32;
	break;
      case 1 :
	p->sprite->pos.x=(p->x)*32;
	break;
      case 2 :
	p->sprite->pos.y=(p->y)*32;
	break;
      case 3 :
	p->sprite->pos.x=(p->x)*32;
	break;
      */
    }
    if(p->moveTimer == 0){
      Move(p);
      switch(map->grid[p->x][p->y]){
      case BONUS_RADIUS_BLOCK:
	p->bombR+=1;
	map->grid[p->x][p->y]=0;
	break;
      case BONUS_BOMB_LIMIT_BLOCK:
	p->bombMax+=1;
	map->grid[p->x][p->y]=0;
	break;	
      case BONUS_SPEED_BLOCK:
	p->speed-=1;
	map->grid[p->x][p->y]=0;
	break;
      default:
	break;
      }
    }
    //IA
    if(p->moveTimer == -1 && p->type == IA){
      //IA ICI
      // Regarde si il y a des bombes qui risques de le tuer
      // Si oui
      //  fuit, FIN
      // Sinon:
      //  Cherche les joueurs
      //  Regarde quels joueurs sont atteignables
      //  Si == 0
      //   
      //  Sinon
      //   Si peut poser bombe (et safe), pose bombe puis se déplace vers le joueur

      //vérifier l'état du jeu
      //chercher un objectif
      //chercher comment aller là bas ou si il faut poser une bombe
      // donner l'ordre "trymove(X,Y)" ou poserBomb


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
      if(input[K_ENTER]==1 && p->moveTimer == -1){
	PlaceBomb(p);
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
      if(input[K_SPACE]==1 && p->moveTimer == -1){
	PlaceBomb(p);
      }
    }
    p->sprite->pos.x = (p->x)*32;
    p->sprite->pos.y = (p->y)*32;
    dessinerSprite(p->sprite, dest);
  }
}

void MapLoop(map* map, SDL_Surface *dest){
  SDL_Rect position;
  int i,j;
  for(i=0;i<map->width;i++)
    {
      for(j=0;j<map->height;j++)
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
  switch (cond)
    {
    case POINTS :
      for(int i = 0;i< map->nbrPlayers;i++){
	if (map->players[i]->score==30)
	  {
	    winner = &map->players[i];
	    return 1;
	  }
      }
      break;
    case VERSUS :
      for(int i = 0;i< map->nbrPlayers;i++){
	if (map->players[i]->score==30)
	  {
	    winner = &map->players[i];
	    return 1;
	  }
      }
      break;
    default :
      break;
    }
  return 0;
}

