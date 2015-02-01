#include "sprite.h"

Sprite *bombSprite = NULL;
SDL_Surface* flammeC = NULL; //Image des flammes au centre
SDL_Surface* flammeD = NULL; //Image des flammes a droite
SDL_Surface* flammeB = NULL; //Image des flammes en bas
SDL_Surface* flammeG = NULL; //Image des flammes a gauche
SDL_Surface* flammeH = NULL; //Image des flammes en haut
SDL_Surface* bonusRadius = NULL;
SDL_Surface* bonusBombLimit = NULL;
SDL_Surface* bonusSpeed = NULL;
SDL_Surface* bonusInvincibility = NULL;

int chargerBombermanSprite( Sprite *sprite, const char *image )
{
    sprite->image = IMG_Load(image);
    if ( sprite->image==NULL )
    {
	fprintf(stderr, "Echec de chargement du fichier %s : %s.\n", image, SDL_GetError());
	return 0;
    }
  
    // on fixe la cle de transparance ici blanc
    SDL_SetColorKey( sprite->image, SDL_SRCCOLORKEY, SDL_MapRGB( sprite->image->format, 255, 255, 255 ) );
  
    sprite->pos_bbm.x = -1;
    sprite->pos_bbm.y = 3;
  
    // le sprite n'est pas animé par defaut
    sprite->anim = 0;
    sprite->animDir = 1;

    // on commence par la première animation (qui est la 3eme)
    sprite->current_anim =0;
  
    // le sprite dispose de cinq animations, on part de 0 donc jusqu'à 4.
    sprite->total_anims = 4;
  
    // par défaut, le sprite est tourné vers le bas
    sprite->orientation = DOWN;
  
    // temps d'affichage pour une animation
    sprite->time_anim = 10;
  
    // Le temps qu'il reste à afficher l'animation courante
    sprite->time_current_anim = 0;
  
    // On definit ensuite les dimentions du sprite.
    sprite->width = 22;
    sprite->height = 32;
  
    sprite->source.x = sprite->current_anim * sprite->width;
    sprite->source.y = sprite->orientation * sprite->height;
    sprite->source.w = sprite->width;
    sprite->source.h = sprite->height;
  
    sprite->pos.x = 0;
    sprite->pos.y = 0;
  
    // on definit enfin la speed et la direction du sprite
    sprite->speed = 2;
    sprite->direction = NO_DIRECTION;
  
    return 1;
}

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

    // le sprite dispose de trois animations
    sprite->total_anims = 0;

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


void deleteSprite( Sprite *sprite )
{
    SDL_FreeSurface( sprite->image );
}
/**
   Direction de bomberman

*/
void fixDirectionSprite( Sprite *sprite, int direction )
{
    // On affecte la direction au sprite
    sprite->direction = direction;
  
    // on affecte l'animation correcpondant à la direction au sprite
    if (direction != NO_DIRECTION)
    {
	sprite->anim = 1;
	switch (direction)
	{
	case UP:
	    sprite->orientation = UP;
	    break;
	  
	case RIGHT:
	    sprite->orientation = RIGHT;
	    break;
	  
	case DOWN:
	    sprite->orientation = DOWN;
	    break;
	  
	case LEFT:
	    sprite->orientation = LEFT;
	    break;
	  
	default:
	    ;
	}
    }
    else
    {
	// si le sprite est areté, on ne l'anime pas
	sprite->anim = 0;
      
	// on met la première animation (qui est la 3eme)
	sprite->current_anim = 3;
	sprite->time_current_anim = 0;
    }
  
    // mise à jour de l'image à copier
    sprite->source.y = sprite->orientation * sprite->height + sprite->pos_bbm.y;
    sprite->source.x = sprite->current_anim * sprite->width + sprite->pos_bbm.x;
  
}

/**
   
 */
void dessinerSprite( Sprite *sprite, SDL_Surface *destination )
{
    /* si le sprite est animé, on gere l'animation */
    if (sprite->anim==1)
    {
	// on decremente le temps restant à l'animation courante
	sprite->time_current_anim--;
	// et on regarde s'il est temps de changer d'animation
	if ( sprite->time_current_anim <= 0 )
	{
	    // s'il faut changer, on passe à l'animation suivante
	    sprite->current_anim += sprite->animDir;
	    if(sprite->current_anim == sprite->total_anims){
		sprite->animDir = -1;
	    }else if(sprite->current_anim == 0){
		sprite->animDir = 1;
	    }
	  
	    // on regle la source à copier
	    sprite->source.x = sprite->width * sprite->current_anim + sprite->pos_bbm.x;
	    sprite->time_current_anim = sprite->time_anim;
	}
    }
  
    // enfin, on dessine le sprite à l'ecran
    SDL_BlitSurface( sprite->image, &sprite->source, destination, &sprite->pos );
}

SDL_Surface *ScaleSurface(SDL_Surface *Surface, int width, int height)
{
    Surface = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE| SDL_DOUBLEBUF | SDL_RESIZABLE);
    return Surface;
}

void printText(SDL_Surface *ecr, TTF_Font *font, SDL_Color couleur, int x, int y, char* texte){
    SDL_Rect position;
    SDL_Surface *txt = TTF_RenderText_Blended(font, texte, couleur);
    position.x = x;
    position.y = y;
    SDL_BlitSurface(txt, NULL, ecr, &position);
}

int LoadSprite(){
    int ret = 0;
    bombSprite = malloc(sizeof(Sprite));  
    if(ChargeBomb (bombSprite, "bomb.png") == 0){
	fprintf(stderr,"erreur chargement image bombe\n");
	ret = 1;
    }
    flammeC = IMG_Load("flameC.png");
    if(flammeC == NULL){
	fprintf(stderr,"erreur chargement image flammes centre\n");
	ret = 1;
    }
    flammeD = IMG_Load("flameD.png");
    if(flammeD == NULL){
	fprintf(stderr,"erreur chargement image flammes droite\n");
	ret = 1;
    }
    flammeB = IMG_Load("flameB.png");
    if(flammeB == NULL){
	fprintf(stderr,"erreur chargement image flammes bas\n");
	ret = 1;
    }
    flammeG = IMG_Load("flameG.png");
    if(flammeG == NULL){
	fprintf(stderr,"erreur chargement image flammes gauche\n");
	ret = 1;
    }
    flammeH = IMG_Load("flameH.png");
    if(flammeH == NULL){
	fprintf(stderr,"erreur chargement image flammes haut\n");
	ret = 1;
    }
    bonusRadius = IMG_Load("bonus_radius.bmp");
    if(bonusRadius == NULL){
	fprintf(stderr,"erreur chargement image bonusRadius\n");
	ret = 1;
    }
    bonusBombLimit = IMG_Load("bonus_bomb_limit.bmp");
    if(bonusBombLimit == NULL){
	fprintf(stderr,"erreur chargement image bonusBombLimit\n");
	ret = 1;
    }
    bonusSpeed = IMG_Load("bonus_speed.bmp");
    if(bonusSpeed == NULL){
	fprintf(stderr,"erreur chargement image bonusSpeed\n");
	ret = 1;
    }
    bonusInvincibility = IMG_Load("bonus_invincibility.bmp");
    if(bonusInvincibility == NULL){
	fprintf(stderr,"erreur chargement image bonusInvincibility\n");
	ret = 1;
    }

    return ret;
}

void FreeSprite(){
    if(bombSprite != NULL){
	deleteSprite(bombSprite);
    }
    if(flammeC != NULL){
	SDL_FreeSurface(flammeC);
    }
    if(flammeD != NULL){
	SDL_FreeSurface(flammeD);
    }
    if(flammeB != NULL){
	SDL_FreeSurface(flammeB);
    }
    if(flammeG != NULL){
	SDL_FreeSurface(flammeG);
    }
    if(flammeH != NULL){
	SDL_FreeSurface(flammeH);
    }
    if(bonusRadius != NULL){
	SDL_FreeSurface(bonusRadius);
    }
    if(bonusSpeed != NULL){
	SDL_FreeSurface(bonusSpeed);
    }
    if(bonusBombLimit != NULL){
	SDL_FreeSurface(bonusBombLimit);
    }
    if(bonusInvincibility != NULL){
	SDL_FreeSurface(bonusInvincibility);
    }
}
