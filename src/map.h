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
#include "player.h"
#include "bomb.h"

#define FOREACH_map_JSON_Key(map_JSON_Key) \
    map_JSON_Key(name)			   \
    map_JSON_Key(grid)			   \
    map_JSON_Key(author)			   \
    map_JSON_Key(auto_remove)			   \
    

#define GENERATE_ENUM(ENUM) KEY_ ## ENUM,
#define GENERATE_STRING(STRING) #STRING,
#define GENERATE_ERROR(ENUM) MAP_FORMAT_ ## ENUM,

enum map_JSON_Key {
    FOREACH_map_JSON_Key(GENERATE_ENUM)
};

/**
 * @enum map_Error
 * @brief Enumération de erreurs possibles lors de l'initialisation de la structure map
 */
typedef enum
{
    NO_ERROR,
    MAP_FORMAT = 1,
    MAP_ALLOC,
    MAP_FILE,
    FOREACH_map_JSON_Key(GENERATE_ERROR)
}
    map_Error;

/**
 * @struct s_map
 * @brief Contient la grille qui compose la map, et la liste des joueurs.
 * 
 * Enregistre la carte sous forme de grille composé de:
 * 0 = Espace vide
 * 1 = Bloc indestructible
 * 2 = Emplacement de départ des joueurs
 * 3 = Bloc destructible
 * 4 = Bombe
 * 2x = Bonus
 */
typedef struct s_map map;
struct s_map{
    char* name; //Le nom de la map (provient du fichier)
    char* author; //Le nom de l'auteur de la map
    int** grid; //La grille de jeu
    int width; //La largeur de la grille
    int height; //La hauteur de la grille
    player* players; //Liste des joueurs
    bombList* bombs; //Liste des bombes
    bool autoRemove; //Indique si les blocs destructible de la map doivent etre automatiquement supprimés aléatoirement
    map_Error error; //Indique un code d'erreur si l'initialisation de la structure a échoué
    char* filename; //Nom du fichier map
};
/*
  Bonus:
  20 = augmentation du rayon
  21 = augmentation de la limite de bombes
  22 = augmentation de la vitesse
  23 = invincibilité
  
  types de bombes
  24 = bombe SQUARE
  (idées: passe muraille)
*/

/**
 * @fn int ListMap(map*** ptrListMap)
 * @brief Initialise toutes les cartes du dossier 'maps', parse la map, le nom de la map et son auteur
 * @param ptrListMap adresse de la list de Map
 * @return Retourne le nombre de maps si tout s'est bien passé, un map_Error sinon.
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

/**
 * @fn FreeMaps(map** ListMap)
 * @brief Detruit la mémoire allouée à la structure map
 * @param listMap Un tableau de map
 * @param nbrMaps le nombre de maps
 */
void FreeMaps(map** listMap, int nbrMaps);

#endif
