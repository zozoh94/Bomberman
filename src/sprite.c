#ifndef SPRITE_C
#define SPRITE_C

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL/SDL.h>
#include<SDL/SDL_image.h>

#define NO_DIRECTION	9
#define DOWN	0
#define RIGHT	1
#define UP	2
#define LEFT	3



#define width_ECRAN 408
#define height_ECRAN 408

typedef struct
{
	// pour l'affichage
	
	SDL_Surface *image;
	SDL_Rect source;
	SDL_Rect pos;
	SDL_Rect pos_bbm;

	
	// taille du sprite

	int width;
	int height;
	
	// pour le déplacement
	
	// direction dans laquelle se deplace le sprite
	int direction;
	
	// speed du sprite
	int speed;
	
	// pour la gestion des animations
	
	// si le sprite est anime
	int anim;
	
	// direction dans laquelle est orienté le sprite
	int orientation;

	// animation courante du sprite
	int current_anim;

	// le temps que dure une animation
	int time_anim;

	// le temps de l'animation courante
	int time_current_anim;

	// nombre d'animations du sprite
	int total_anims;

}Sprite;



int chargerBombermanSprite( Sprite *sprite, const char *image )
{
	sprite->image = IMG_Load( image );
	if ( sprite->image==NULL )
	{
		fprintf(stderr, "Echec de chargement du fichier %s : %s.\n", image, SDL_GetError());
		return 0;
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
	sprite->time_anim = 8;

	// Le temps qu'il reste à afficher l'animation courante
	sprite->time_current_anim = 0;

	// On definit ensuite les dimentions du sprite.
	sprite->width = 22;
	sprite->height = 32;
	
	sprite->source.x = sprite->current_anim * sprite->width;
	sprite->source.y = sprite->orientation * sprite->height;
	sprite->source.w = sprite->width;
	sprite->source.h = sprite->height;

	//sprite->pos.x = 0;
	//sprite->pos.y = 0;
	
	// on definit enfin la speed et la direction du sprite
	sprite->speed = 2;
	sprite->direction = NO_DIRECTION;
	
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

int InitSDL()
{
	SDL_Surface *ecran;

	/* initialisation de SDL_Video */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
	{
		fprintf(stderr, "Echec d'initialisation de SDL.\n");
		return 1;
	}
	printf("SDL initialisé avec succès.\n");
	
	
	/* On fixe le mode d'affichage à 384*384 */
	ecran = SDL_SetVideoMode(width_ECRAN, height_ECRAN, 32, SDL_SWSURFACE| SDL_DOUBLEBUF);
	if ( ecran == NULL )
	{
		fprintf(stderr, "Echec de changement du mode video : %s.\n", SDL_GetError());
		return 1;
	}
	
}

#endif
