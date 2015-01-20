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



int chargerSprite( Sprite *sprite, const char *image )
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
/** pour la dir de bomberman
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
	
}*/

/** Change la position, pas besoin
void miseAJourSprite( Sprite *sprite )
{
	// le sprite vas vers le haut
	if (sprite->direction==UP)
	{
		// on monte et on verifie si on sort de l'ecran
		sprite->pos.y = sprite->pos.y-sprite->speed;
		if (sprite->pos.y < 0)	sprite->pos.y = 0;
			
	}
	else if (sprite->direction==RIGHT)
	{
		// on va à droite et on verifie si on sort de l'ecran
		sprite->pos.x = sprite->pos.x + sprite->speed;	
		if (sprite->pos.x + sprite->width > width_ECRAN)	sprite->pos.x = width_ECRAN - sprite->width;
	}
	else if (sprite->direction==DOWN)
	{
		// on descend et on verifie si on sort de l'ecran
		sprite->pos.y = sprite->pos.y + sprite->speed;	
		if (sprite->pos.y + sprite->height > height_ECRAN)	sprite->pos.y = height_ECRAN - sprite->height;
	}
			
	else if (sprite->direction==LEFT)
	{
		// on va à gauche et on verifie si on sort de l'ecran
		sprite->pos.x = sprite->pos.x - sprite->speed;	
		if (sprite->pos.x < 0)	sprite->pos.x = 0;
	}
}
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

/**
   Initialise la map dja fait
void init_map(int map[12][12])
{

	int i,j;

	for(i=0;i<12;i++)
	{
		for(j=0;j<12;j++)
		{
			map[i][j]=rand()%2;
		}
	}

}
*/


/** Pour afficher la map
void blitWall(int map[12][12], SDL_Surface *ecran)
{
	SDL_Surface *mur, *caisse;
	SDL_Rect position;

	caisse=IMG_Load("caisse.jpg");		
	mur=IMG_Load("mur.jpg");

	int i,j;
	for(i=0;i<12;i++)
	{
		for(j=0;j<12;j++)
		{
			position.x=i*34;
			position.y=j*34;
	
			switch(map[i][j])
			{
				case 0 :
					SDL_BlitSurface( mur, NULL, ecran, &position);		
					break;
				
				/*case 1 :
					SDL_BlitSurface( caisse, NULL, ecran, &position);		
					break;
			*/
			}

		}

	}

}
*/

int main()
{
	SDL_Surface *ecran;
	SDL_Event event;
	int continuer = 0;
	int current_time, prochain_rendu = 0;
	
	Sprite bomberman;
	int map[12][12]={0};
	init_map(map);
	

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
	
	/* On charge le sprite controlable avec l'image bbm*/
	if (!chargerSprite( &bomberman, "bm.bmp"))
	{
		SDL_Quit();
		return 1;
	}

	// on place le sprite que l'on controle au milieu
	bomberman.pos.x = width_ECRAN/2;
	bomberman.pos.y = height_ECRAN/2;
	fixDirectionSprite( &bomberman, DOWN);
	fixDirectionSprite( &bomberman, NO_DIRECTION);

	while (continuer!=1)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						continuer = 1;
						break;
					
					// on deplace le sprite au clavier
					case SDLK_UP:
						fixDirectionSprite(&bomberman, UP);
						break;
				
					case SDLK_RIGHT:
						fixDirectionSprite(&bomberman, RIGHT);
						break;
				
					case SDLK_DOWN:
						fixDirectionSprite(&bomberman, DOWN);
						break;
				
					case SDLK_LEFT:
						fixDirectionSprite(&bomberman, LEFT);
						break;
				
					default:
						printf("Une touche à été pressée.\n");
				}
				break;
			
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
					// Si on relache une touche, plus de deplacement 
					case SDLK_UP:
						if (bomberman.direction == UP)
							fixDirectionSprite( &bomberman, NO_DIRECTION );
						break;
				
					case SDLK_RIGHT:
						if (bomberman.direction == RIGHT)
							fixDirectionSprite( &bomberman, NO_DIRECTION );
						break;
				
					case SDLK_DOWN:
						if (bomberman.direction == DOWN)
							fixDirectionSprite( &bomberman, NO_DIRECTION );
						break;
				
					case SDLK_LEFT:
						if (bomberman.direction == LEFT)
							fixDirectionSprite( &bomberman, NO_DIRECTION );
						break;
					
					default:
						;
				}
				break;

			case SDL_QUIT:
				continuer = 1;
				break;
			
			default:
				;
			}
		}
		
		/* On recupère le temps écoulé en mili-secondes */
		current_time = SDL_GetTicks();
		
		/* On regarde s'il est temps d'effectuer le prochain rendu */
		if (current_time > prochain_rendu)
		{
			/* un rendu toutes les 20 milli-secondes = 50 images par secondes */
			prochain_rendu = current_time + 20;
			
			
			// puis les positions des sprites
			miseAJourSprite(&bomberman);
			
		
			/* On dessine la scene */
			
			/* On efface l'écran */
			SDL_FillRect(ecran, NULL, 0);

			/* On dessine les sprites à l'écran */
			dessinerSprite(&bomberman, ecran);
						
			/* On met à jour de la zone d'affichage de la fenetre */
			
		}
		blitWall(map, ecran);
		SDL_Flip( ecran );
		
	}

	
	deleteSprite(&bomberman);
	SDL_Quit();
	

	return 0;
}
