#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#ifndef MAP_H
#define MAP_H

/**
  Structure map
  Contient la grille qui compose la map, et la liste des joueurs.
  
  Enregistre la carte sous forme de grille composé de:
  0 = Espace vide
  1 = Bloc indestructible
  2 = Emplacement de départ des joueurs
  3 = Bloc destructible
  4 = Bombe
  2x = Bonus
 */
typedef struct s_map map;
struct s_map{
  char* name; //Le nom de la map (provient du fichier
  int** grid; //La grille de jeu
  player* players; //Liste des joueurs
  bomb* bombs; //Liste des bombes
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
   Initialise la carte.
   Initialise les listes à 0, parse la map et le nom de la map, génère aléatoirement les positions des bloc destructibles
   @param char* f: Le nom du fichier .map qui contient la carte
   @return Un pointeur vers la map générée
 */
map* InitMap(char* f);

/**
   Ajoute les joueurs à la carte et les place à leurs positions initiales
   @param player** playerList: La liste des joueurs
 */
void AddPlayers(player** playerList);

#endif
