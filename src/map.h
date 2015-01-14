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
#include <json/json.h>
#include <stdbool.h>
#include "struct.h"
#include "player.h"
#include "bomb.h"

#define FOREACH_map_JSON_Key(map_JSON_Key) \
    map_JSON_Key(name)			   \
    map_JSON_Key(grid)			   \
    map_JSON_Key(author)			   \
    map_JSON_Key(auto_remove)			   \
    

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,
#define GENERATE_ERROR(ENUM) MAP_FORMAT_ ## ENUM,

enum map_JSON_Key {
    FOREACH_map_JSON_Key(GENERATE_ENUM)
    JSON_KEY_LEN
};

static const char *map_JSON_Key_Str[] = {
    FOREACH_map_JSON_Key(GENERATE_STRING)
};

/**
 * @fn int ListMap(map*** ptrListMap)
 * @brief Initialise toutes les cartes du dossier 'maps', parse la map, le nom de la map et son auteur
 * @param ptrListMap adresse de la list de Map
 * @return Retourne 0 si tout s'est bien passé, un map_Error sinon.
 */
int ListMap(map*** ptrListMap);

/**
 * @fn int InitMap(map* map)
 * @brief Initialise les listes, génère aléatoirement les positions des bloc destructibles
 * @param map Pointeur sur la map que l'on souhaite utilisé
 * @return Un code d'erreur
 */
int InitMap(map* map);

/**
 * @fn void AddPlayers(player** playerList)
 * @brief Ajoute les joueurs à la carte et les place à leurs positions initiales
 * @param map: la map sur laquel on souhaite ajouté un joueur
 * @param playerList: La liste des joueurs
 * @return Un code d'erreur
 */
int AddPlayers(map* map, player** playerList);


#endif
