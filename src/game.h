#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "player.h"
#include "bomb.h"
#ifndef GAME_H
#define GAME_H
//Dans ce fichier, les initialisations de parties et les boucles de jeux.


//Initialise un jeu solo (param à ajouter plus tard), puis lance le loop
void Solo();
//Boucle de jeu
void GameLoop();


/**
   Contrôle les timer et explosions des bombes.
   Récupère la liste des bombes contenue dans map, et pour chaque bombe, vérifie si elle doit exploser, la fait exploser le cas échéant, sinon décrémente son timer.
   @param map* map: La carte qui contient les bombes
 */
void BombLoop(map* map);

/**
   Contrôle les timer des joueurs.
   Récupère la liste des joueurs de la map, et pour chaque joueur, décrémente son timer si besoin.
   @param map* map: La carte qui contient les joueurs
 */
void PlayerLoop(map* map);

#endif
