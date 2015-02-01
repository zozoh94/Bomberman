#ifndef AUDIO_H
#define AUDIO_H

#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#define NBR_BOMB_SOUND 3

extern Mix_Music *music;
extern Mix_Chunk *bombSound[NBR_BOMB_SOUND];
extern Mix_Chunk *bonusSound;
extern Mix_Chunk *winSound;

/**
 * @fn int InitSounds()
 * @brief Initialise touts les sons du jeu
 * @return Retourne 0 si tout s'est bien passé, 1 sinon
 */
int InitSounds();

/**
 * @fn void FreeSounds()
 * @brief Libère la mémoire des sons
 */
void FreeSounds();

/**
 * @fn Mix_Chunk* RandomBomb()
 * @brief Renvoies un son de bombe au hasard
 * @return Un pointeur sur un son de bombe
 */
Mix_Chunk* RandomBomb();

#endif
