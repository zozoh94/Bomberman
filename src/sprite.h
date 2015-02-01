#ifndef SPRITE_H
#define SPRITE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#define NO_DIRECTION	9
#define DOWN	0
#define RIGHT	1
#define UP	2
#define LEFT	3

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
  int animDir;

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

extern Sprite *bombSprite;
extern SDL_Surface* flammeC;
extern SDL_Surface* flammeD;
extern SDL_Surface* flammeB;
extern SDL_Surface* flammeG;
extern SDL_Surface* flammeH;
extern SDL_Surface* bonusRadius;
extern SDL_Surface* bonusBombLimit;
extern SDL_Surface* bonusSpeed;
extern SDL_Surface* bonusInvincibility;

int chargerBombermanSprite( Sprite *sprite, const char *image );

int ChargeBomb( Sprite *sprite, const char *image );

int ChargeFlame( Sprite *sprite, const char *image );

void deleteSprite( Sprite *sprite );

void fixDirectionSprite( Sprite *sprite, int direction );

void dessinerSprite( Sprite *sprite, SDL_Surface *destination );

SDL_Surface *ScaleSurface(SDL_Surface *Surface, int width, int height);

void printText(SDL_Surface *ecr, TTF_Font *font, SDL_Color couleur, int x, int y, char* texte);

void FreeSprite();

int LoadSprite();

#endif
