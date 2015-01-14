#ifndef STRUCT_H
#define STRUCT_H

#include <stdbool.h>

typedef enum e_map_Error map_Error;
typedef struct s_map map;
typedef enum e_typeP typeP;
typedef struct s_player player;
typedef enum e_btype btype;
typedef struct s_bomb bomb;
typedef struct s_bombList bombList;

#define FOREACH_map_JSON_Key(map_JSON_Key) \
    map_JSON_Key(name)			   \
    map_JSON_Key(grid)			   \
    map_JSON_Key(author)			   \
    map_JSON_Key(auto_remove)			   \
    

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,
#define GENERATE_ERROR(ENUM) MAP_FORMAT_ ## ENUM,

/**
 * @enum map_Error
 * @brief Enumération de erreurs possibles lors de l'initialisation de la structure map
 */
enum e_map_Error{
  MAP_FORMAT,
  MAP_ALLOC,
  MAP_FILE,
  FOREACH_map_JSON_Key(GENERATE_ERROR)
};

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
struct s_map{
    char* name; //Le nom de la map (provient du fichier)
    char* author; //Le nom de l'auteur de la map
    int** grid; //La grille de jeu
    int width; //La largeur de la grille
    int height; //La hauteur de la grille
  int nbrPlayer; //nombre de joueurs
    player** players; //Liste des joueurs
    bombList* bombs; //Liste des bombes
    char* fileName; //nom du fichier .map (identifiant unique de la map)
    bool autoRemove;
    map_Error error;
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

/*
  Enumeration typeP
  Le type de joueur (J1, J2, IA)
*/
enum e_typeP{
  J1,
  J2,
  IA
};

/*
  Type possible des bombes
 */
enum e_btype{
  NORMAL,
  SQUARE
};

/*
  Structure player
  Contient les coordonnées des joueurs, leur score, leurs bonus, leur nombre de bombes.
  
 */
struct s_player{
  int x; //Coordonnée X
  int y; //Coordonnée Y
  int destX; //Coordonnée X d'arrivée en cas de déplacement
  int destY; //Coordonnée Y d'arrivée en cas de déplacement
  int score; //Score
  int bombs; //Nombre de bombes posées
  int bombMax; //Nombre de bombes posables max simultanéments
  int speed; //Le temps de déplacement (plus petit = plus rapide)
  int moveTimer; //Le timer du déplacement
  typeP type; //Type de player
  map *map; //Pointeur vers la map
  
  //Stats des bombes
  int bombR; //rayon des bombes
  btype bombT; //type des bombes
};


/*
  Structure bomb
  Contient les coordonnées de la bombe, son type, son rayon d'explosion.
  
 */
struct s_bomb{
  int x; //Coordonnée X
  int y; //Coordonnée Y
  int explosion; //Rayon d'explosion
  int timer; //Temps avant explosion
  player* myPlayer; //Joueur ayant posé la bombe
  btype type; //Type de la bombe
};


/*
  Liste chainée de bombes
 */
struct s_bombList{
  bomb* data;
  struct s_bombList* next;
};


#endif
