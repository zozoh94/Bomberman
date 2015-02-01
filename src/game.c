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

#define FPS 100 //frame par seconde
#define TIMEFRAME 1000/FPS //durée d'une frame en milliseconde

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
  fprintf(stderr,"Map statut: %d\n",InitMap(m, nbrjoueurs, tab));
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
  int win, i;
  int winner=9; //ça ne marche pas très bien.
  TTF_Font *font = TTF_OpenFont("Bomberman.ttf", 24);
  SDL_Color white = {255, 255, 255, 0}; 
  int scoreY = ((m->height)*32)+4;
  int scoreM = (m->width/2)*32;
  //Tableau des appuis à tester, 0 = relevé, 1 = appuyé
  int* inputTab = malloc(sizeof(int)*KEYNUMBER);
  for(win = 0; win < KEYNUMBER; win++){
    inputTab[win] = 0;
  }
  win = 0;
  int ticksA = 0;
  int ticksB = 0;
  while(win == 0){
    ticksA = SDL_GetTicks();
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
	  win = 2;
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
      sprintf(score, "J%d: %d",(i+1),m->players[i]->score);
      printText(dest, font, white, (16+(i%2)*scoreM), (scoreY+(i/2)*32), score);
    }
    MapLoop(m, dest);
    BombLoop(m, dest);
    PlayerLoop(m, inputTab ,dest);
    if(win == 0){
      win = TestWin(m, cond, &winner);
    }
    SDL_Flip(dest);
    ticksB = SDL_GetTicks();
    if((ticksB-ticksA)<TIMEFRAME){
      SDL_Delay(TIMEFRAME-(ticksB-ticksA));
    }
  }
  if(win == 1){
    char *txt;
    switch(winner){
    case IA:
      txt="IA";
      break;
    case J1:
      txt="J1";
      break;
    case J2:
      txt="J2";
      break;
    default:
      txt="GG!";
      break;
    }
    victory_screen(dest, txt);
  }

  //LIBEREZ
  for(i = 0; i < m->nbrPlayers; i++){
    FreePlayer(m->players[i]);
  }
  TTF_CloseFont(font);
  free(inputTab);
}

void BombLoop(map* map, SDL_Surface *dest){
  bombList* l = map->bombs;
  while(l!=NULL){
    l->data->timer--;
    if(l->data->timer==0){ // Explosion de la bombe
      Explode(map, l->data);
      Mix_PlayChannel(-1, RandomBomb(), 0);
    }
    if(l->data->timer>=0){
      bombSprite->pos.x = (l->data->x)*32+4;
      bombSprite->pos.y = (l->data->y)*32+4;
      dessinerSprite(bombSprite, dest);
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
	      SDL_BlitSurface(flammeD, NULL, dest, &position);
	      break;
	    case 2: //bas
	      SDL_BlitSurface(flammeB, NULL, dest, &position);
	      break;
	    case 3: //gauche
	      SDL_BlitSurface(flammeG, NULL, dest, &position);
	      break;
	    case 4: //haut
	      SDL_BlitSurface(flammeH, NULL, dest, &position);
	      break;
	    default: //centre
	      SDL_BlitSurface(flammeC, NULL, dest, &position);
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
    p = map->players[i];
    if(p->x!=-1){ // Si le joueur n'est pas KO, sinon on l'oublie tout simplement 
      int modX = 0;
      int modY = 0;
      if(p->invulnerability>0){
	p->invulnerability--;
      }
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
	  Mix_PlayChannel(-1, bonusSound, 0);
	  if(p->bombR<15){
	    p->bombR+=1;
	  }
	  map->grid[p->x][p->y]=0;
	  break;
	case BONUS_BOMB_LIMIT_BLOCK:
	  if(p->bombMax<15){
	    p->bombMax+=1;
	  }
	  map->grid[p->x][p->y]=0;
	  Mix_PlayChannel(-1, bonusSound, 0);
	  break;	
	case BONUS_SPEED_BLOCK:
	  if(p->speed>15){
	    p->speed-=3;
	  }
	  map->grid[p->x][p->y]=0;
	  Mix_PlayChannel(-1, bonusSound, 0);
	  break;
	case BONUS_INVINCIBILITY_BLOCK:
	  p->invulnerability=1800;
	  map->grid[p->x][p->y]=0;
	  Mix_PlayChannel(-1, bonusSound, 0);
	  break;
	default:
	  break;
	}
      }
      //IA
      if(p->moveTimer == -1 && p->type == IA){
	int **bombes = ChercheBombes(p->map);
	int **possible = ChercheDest(p->map, p, bombes);
	int *dest = TrouverProche(p->x, p->y, p->map, possible, bombes);
	if(dest[0]!=-1){//on a une destination
	  int *va = AllerVers(p->x,p->y,dest[0],dest[1],p->map,bombes);
	  //Si on est à portée de bombe du joueur, que c'est un joueur et qu'on est pas menacé
	  if(va[2]<=(p->bombR) && dest[2]==6 && bombes[p->x][p->y] == 0){
	    PlaceBomb(p);
	    //Si on est sur une case bien, et qu'on est pas menacé
	  }else if(va[2] == 0 && dest[2]>0 && bombes[p->x][p->y] == 0){//poser bombe
	    PlaceBomb(p);
	  }else{//déplacer
	    TryMove(p, va[0], va[1]);
	  }
	}else{//On bouge au pif
	  TryMove(p, p->x+1, p->y);
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
      if(p->invulnerability > 1000 && p->invulnerability%FPS < (FPS/2)){
	dessinerSprite(p->sprite, dest);
      }else if(p->invulnerability > 360 && p->invulnerability%(FPS/2) < (FPS/4)){
	dessinerSprite(p->sprite, dest);
      }else if(p->invulnerability%(FPS/4) < (FPS/8)){
	dessinerSprite(p->sprite, dest);
      }
    }
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
	      SDL_BlitSurface( bonusRadius, NULL, dest, &position);
	      break;
	    case BONUS_BOMB_LIMIT_BLOCK :
	      SDL_BlitSurface( map->floor, NULL, dest, &position);
	      SDL_BlitSurface( bonusBombLimit, NULL, dest, &position);
	      break;
	    case BONUS_SPEED_BLOCK :
	      SDL_BlitSurface( map->floor, NULL, dest, &position);
	      SDL_BlitSurface( bonusSpeed, NULL, dest, &position);
	      break;
	    case BONUS_INVINCIBILITY_BLOCK :
	      SDL_BlitSurface( map->floor, NULL, dest, &position);
	      SDL_BlitSurface( bonusInvincibility, NULL, dest, &position);
	      break;
	    default :
	      SDL_BlitSurface( map->floor, NULL, dest, &position);
	      break;
	    }
	}
    }
}

int TestWin(map* map, vCond cond, int* winner){
  int restant;
  switch (cond)
    {
    case POINTS :
      for(int i = 0;i< map->nbrPlayers;i++){
	if (map->players[i]->score>=30)
	  {
	    *winner = map->players[i]->type;
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
	  *winner = map->players[i]->type; //Si il lui reste de la vie on le met en winner,
	}
	if(restant <= 1){
	  return 1; // On ne le renvoie que si il est le seul restant de toute façon.
	}
      }
      break;
      
    default :
      break;
    }
  return 0;
}

void victory_screen(SDL_Surface *ecran, char *winner)
{
	SDL_Surface *victory = NULL;
	SDL_Surface *texte = NULL;
	SDL_Rect positionImage,positionTexte;
	TTF_Font *police = NULL;

	SDL_Color whiteColor = {255, 255, 255, 0};
	
	SDL_Event event;
	victory=IMG_Load("victory.png");

	police = TTF_OpenFont("lastninja.ttf", 24);	//On charge la police
	
	texte = TTF_RenderText_Blended(police, winner, whiteColor);	//On charge le texte
	
	ecran = ScaleSurface(ecran,256, 224);
	
	positionImage.x=0;//ecran->w/2-victory->w/2;
	positionImage.y=0;//ecran->w/2-victory->h/2;
	SDL_BlitSurface( victory, NULL, ecran, &positionImage);	//On blit l'image au milieu

	positionTexte.x=ecran->w/2-texte->w/2;
	positionTexte.y=12;
	SDL_BlitSurface(texte, NULL, ecran, &positionTexte);	//On blit le texte


	SDL_Delay(500); //Une demi seconde pour admirer la fin de la game
	
	SDL_Flip(ecran);
	Mix_PlayChannel(-1, winSound, 0);
	
	//1 seconde d'écran de victoire
	SDL_Delay(1000);
	
	int fin = 0;
	while(SDL_WaitEvent(&event) && fin == 0){
	  if (event.type == SDL_KEYDOWN){
	    fin = 1;
	    Mix_PlayMusic( music, -1 );
	  }
	}
	
	TTF_CloseFont(police);

	SDL_FreeSurface(victory);
	SDL_FreeSurface(texte);	
}
