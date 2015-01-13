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
//#include <json/json.h>
#include "player.h"
#include "bomb.h"

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
    char* fileName; //nom du fichier .map (identifiant unique de la map)
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
 * @fn map** ListMap()
 * @brief Initialise toutes les cartes du dossier 'maps', parse la map, le nom de la map et son auteur
 * @return Un tableau de map
 */
map** ListMap();

/**
 * @fn map* InitMap(char* f).
 * @brief Initialise les listes, génère aléatoirement les positions des bloc destructibles
 * @param f Le nom du fichier .map qui contient la carte
 * @return Un pointeur vers la map générée
 */
map* InitMap(char* f);

/**
 * @fn void AddPlayers(player** playerList)
 * @brief Ajoute les joueurs à la carte et les place à leurs positions initiales
 * @param player** playerList: La liste des joueurs
 */
void AddPlayers(player** playerList);

#endif
