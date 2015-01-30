#include "game.h"
#define EXPLOSIONTIME 100
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
  fprintf(stderr,"GAME: %d \n",nbrPlayers);
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
  dest = ScaleSurface(dest,(m->width)*32,(m->height)*32+64);
  m->victory = cond;
  if(cond == VERSUS){
    for(int i = 0; i < m->nbrPlayers; i++){
      m->players[i]->score = 1;
    }
  }
  GameLoop(m, cond, dest);  
}

void GameLoop(map *m, vCond cond, SDL_Surface *dest){
  SDL_Event event;
  int win;
  player* winner;
  TTF_Font *font = TTF_OpenFont("Bomberman.ttf", 24);
  SDL_Color white = {255, 255, 255, 0}; 
  int scoreY = m->height-52;
  int scoreM = m->width/2;
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
    //AFFICHAGE DES SCORES
    for(int i = 0; i<m->nbrPlayers;i++){
      char* score = malloc(sizeof(char)*255);
      sprintf(score, "P%d: %d",i,m->players[i]->score);
      printText(dest, font, white, (16+(i%2)*scoreM), (scoreY+(i/2)*32), score);
    }
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
    l->data->timer--;
    if(l->data->timer==0){ // Explosion de la bombe
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
	  if(l->data->explozone[i][j] != 0){
	    position.x=i*32;
	    position.y=j*32;
	    switch(l->data->explozone[i][j]){
	    case 1: //droite
	      SDL_BlitSurface( l->data->flammeD, NULL, dest, &position);
	      break;
	    case 2: //bas
	      SDL_BlitSurface( l->data->flammeB, NULL, dest, &position);
	      break;
	    case 3: //gauche
	      SDL_BlitSurface( l->data->flammeG, NULL, dest, &position);
	      break;
	    case 4: //haut
	      SDL_BlitSurface( l->data->flammeH, NULL, dest, &position);
	      break;
	    default: //centre
	      SDL_BlitSurface( l->data->flammeC, NULL, dest, &position);
	      break;
	    }
	  }
	}
      }
    }
    if(l->data->timer==-EXPLOSIONTIME){ // Destruction de la bombe
      l->data->myPlayer->bombs --; //décrémente le nombre de bombes posées par le joueur qui a posé la bombe
      map->bombs = RemoveBombList(map->bombs, l->data);
      l = l->next;
    }else{
      l = l->next;
    }
  }
}

void PlayerLoop(map* map, int* input, SDL_Surface *dest){
  int i;
  player *p=NULL;
  for(i = 0; i < map->nbrPlayers; i++){
    int modX = 0;
    int modY = 0;
    p = map->players[i];
    
    if(p->moveTimer > 0){ // On arrête le décompte à -1
      p->moveTimer --;
    }
    if(p->moveTimer > 0){
      // Déplacement fluide
      switch(p->sprite->orientation){
      case UP :
	modY = -(int)(32 - 32*((double) (p->moveTimer)/(double) (p->speed)));
	break;
      case RIGHT :
	modX = (int)(32 - 32*((double) (p->moveTimer)/(double) (p->speed)));;;
	break;
      case DOWN :
	modY = 	(int)(32 - 32*((double) (p->moveTimer)/(double) (p->speed)));;
	break;
      case LEFT :
	modX = -(int)(32 - 32*((double) (p->moveTimer)/(double) (p->speed)));;;
	break;
      }
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
	p->speed-=3;
	map->grid[p->x][p->y]=0;
	break;
      default:
	break;
      }
    }
    //IA
    if(p->moveTimer == -1 && p->type == IA){
      int **bombes = ChercheBombes(p->map);
      if(bombes[p->x][p->y]==1){ // On est menacé par une bombe
	//fuire
      }else{
	int **possible = ChercheDest(p->map, p);
	int *dest = TrouverProche(p->x, p->y, p->map, possible, bombes);
	if(dest[0]!=-1){//on a une destination
	  int *va = AllerVers(p->x,p->y,dest[0],dest[1],p->map);
	  if(va[2]<=2 && dest[2]==10){ //joueur
	    PlaceBomb(p);
	  }else if(va[2] == 0){//poser bombe
	    PlaceBomb(p);
	  }else{//déplacer
	    TryMove(p, va[0], va[1]);
	  }
	}else{//On bouge au pif
	  TryMove(p, p->x+1, p->y);
	}
      }
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
    p->sprite->pos.x = (p->x)*32+5+modX; //Le sprite fait 22*32 donc on le décale de (32-22)/2 = 10
    p->sprite->pos.y = (p->y)*32+modY;
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
	    case BONUS_RADIUS_BLOCK :
	      SDL_BlitSurface( map->floor, NULL, dest, &position);
	      SDL_BlitSurface( map->bonusRadius, NULL, dest, &position);
	      break;
	    case BONUS_BOMB_LIMIT_BLOCK :
	      SDL_BlitSurface( map->floor, NULL, dest, &position);
	      SDL_BlitSurface( map->bonusBombLimit, NULL, dest, &position);
	      break;
	    case BONUS_SPEED_BLOCK :
	      SDL_BlitSurface( map->floor, NULL, dest, &position);
	      SDL_BlitSurface( map->bonusSpeed, NULL, dest, &position);
	      break;
	    case BONUS_INVINCIBILITY_BLOCK :
	      SDL_BlitSurface( map->floor, NULL, dest, &position);
	      SDL_BlitSurface( map->bonusInvincibility, NULL, dest, &position);
	      break;
	    default :
	      SDL_BlitSurface( map->floor, NULL, dest, &position);
	      break;
	    }
	}
    }
}

int TestWin(map* map, vCond cond, player** winner){
  int restant;
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
      restant = map->nbrPlayers; // nombres de joueurs sur la map
      for(int i = 0;i< map->nbrPlayers;i++){
	if ((map->players[i]->score)<=0) // On retire ceux qui n'ont plus de vie
	  {
	    restant-=1;
	  }else{
	  winner = &map->players[i]; //Si il lui reste de la vie on le met en winner,
	}
	if(restant == 1){
	  return 1; // On ne le renvoie que si il est le seul restant de toute façon.
	}
      }
      break;
      
    default :
      break;
    }
  return 0;
}

