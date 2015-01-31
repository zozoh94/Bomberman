#ifndef MAP_H
#define MAP_H

#define MAX_LEN_LINE 100
#define MAX_LEN 1000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/json-c/json.h"
#include <SDL/SDL.h>
#include <stdbool.h>
#include "struct.h"
#include "player.h"
#include "bomb.h"


/**
 * @fn int ListMap(map*** ptrListMap)
 * @brief Initialise toutes les cartes du dossier 'maps'
 * @param ptrListMap adresse de la list de Map
 * @return Retourne le nombre de maps si tout s'est bien passé, un map_Error sinon.
 */
int ListMaps(map*** ptrListMap);

/**
 * @fn int ParseMap(map *map)
 * @brief Initialise une carte, parse la map
 * @param map Pointeur sur la map que l'on souhaite utilisé
 * @return Retourne le nombre de maps si tout s'est bien passé, un map_Error sinon.
 */
int ParseMap(map *map);

/**
 * @fn int InitMap(map* map)
 * @brief Initialise les listes, génère aléatoirement les positions des bloc destructibles
 * @param map Pointeur sur la map que l'on souhaite utilisé
 * @param nbrPlayers: le nombre de joueurs
 * @param playerList: La liste des joueurs
 * @return Un code d'erreur map_Error, 0 si cela c'est bien passé
 */
int InitMap(map* map,  int nbrPlayers, player** listPlayer);

/**
 * @fn FreeMaps(map** ListMap)
 * @brief Detruit la mémoire allouée à la structure map
 * @param listMap Un tableau de map
 * @param nbrMaps le nombre de maps
 */
void FreeMaps(map** listMap, int nbrMaps);


#endif
