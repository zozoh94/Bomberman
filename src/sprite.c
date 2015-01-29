#include "sprite.h"


int chargerBombermanSprite( Sprite *sprite, const char *image )
{
	sprite->image = IMG_Load(image);
	if ( sprite->image==NULL )
	{
		fprintf(stderr, "Echec de chargement du fichier %s : %s.\n", image, SDL_GetError());
		return 0;
	}else{
	  fprintf(stderr, "Chargement du fichier %s\n", image);
	}
		
	// on fixe la cle de transparance ici blanc
	SDL_SetColorKey( sprite->image, SDL_SRCCOLORKEY, SDL_MapRGB( sprite->image->format, 255, 255, 255 ) );

	
	
	// le sprite n'est pas animé par defaut
	sprite->anim = 0;

	// on commence par la première animation
	sprite->current_anim =0;

	// le sprite dispose de cinq animations
	sprite->total_anims = 5;

	// par défaut, le sprite est tourné vers le bas
	sprite->orientation = DOWN;

	// temps d'affichage pour une animation
	sprite->time_anim = 128;

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

int ChargeFlame( Sprite *sprite, const char *image ){
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
	
		// on met la première animation
		sprite->current_anim = 0;
		sprite->time_current_anim = 0;
	}
		
	// mise à jour de l'image à copier
	sprite->source.y = sprite->orientation * sprite->height;
	sprite->source.x = sprite->current_anim * sprite->width;
	
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
			sprite->current_anim++;
			// si on était à la dernière animation, on repasse à la première
			if (sprite->current_anim >= sprite->total_anims)	sprite->current_anim = 0;
				
			// on regle la source à copier
			sprite->source.x = sprite->width * sprite->current_anim;
			sprite->time_current_anim = sprite->time_anim;
		}
	}
	
	// enfin, on dessine le sprite à l'ecran
	SDL_BlitSurface( sprite->image, &sprite->source, destination, &sprite->pos );

}

SDL_Surface* InitSDL()
{
	SDL_Surface *ecran;

	/* initialisation de SDL_Video */
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	  {
	    fprintf(stderr, "Echec d'initialisation de SDL.\n");
	    return NULL;
	  }
	printf("SDL initialisé avec succès.\n");
	
	if(TTF_Init() == -1)
	  {
	    fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
	    return NULL;
	  }
	printf("ttf initialisé avec succès.\n");

	/* On fixe le mode d'affichage à 384*384 */
	ecran = SDL_SetVideoMode(640, 460, 32, SDL_SWSURFACE| SDL_DOUBLEBUF);
	if ( ecran == NULL )
	{
		fprintf(stderr, "Echec de changement du mode video : %s.\n", SDL_GetError());
		return NULL;
	}
	return ecran;
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
