#ifndef STRUCT_H
#define STRUCT_H

#include <stdbool.h>

typedef enum e_map_Error map_Error;
typedef enum e_case_Type case_Type;
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
    

#define GENERATE_ENUM(ENUM) KEY_ ## ENUM,
#define GENERATE_STRING(STRING) #STRING,
#define GENERATE_ERROR(ENUM) MAP_FORMAT_ ## ENUM ## _ERROR,

enum map_JSON_Key {
    FOREACH_map_JSON_Key(GENERATE_ENUM)
};

/**
 * @enum map_Error
 * @brief Enumération de erreurs possibles lors de l'initialisation de la structure map
 */
enum e_map_Error;
{
    NO_ERROR,
    MAP_FORMAT_ERROR = 100,
    MAP_ALLOC_ERROR,
    MAP_FILE_ERROR,
    MAP_TOO_MUCH_ERROR,
    MAP_TOO_MUCH_PLAYER,
    FOREACH_map_JSON_Key(GENERATE_ERROR)
};

/**
 * @enum case_Type
 * @brief Enumération des types cases
 * 0 = Espace vide
 * 1 = Bloc indestructible
 * 2 = Emplacement de départ des joueurs
 * 3 = Bloc destructible
 * 4 = Bombe
 * 5 = Bonus ...
 */
enum e_case_Type
{
    EMPTY_BLOCK,
    UNDESTRUCTIBLE_BLOCK,
    STARTING_BLOCK,
    DESTRUCTIBLE_BLOCK,
    BOMB_BLOCK,
    BOMB_SQUAREX_BLOCK,
    BONUS_RADIUS_BLOCK, //augmentation du rayon
    BONUS_BOMB_LIMIT_BLOCK, //augmentation de la limite de bombes
    BONUS_SPEED_BLOCK, //augmentation de la vitesse
    BONUS_INVINCIBILITY_BLOCK, //invincibilité
};
/*
  idées: 
  -passe muraille
  -téléporteur
*/


/**
 * @struct s_map
 * @brief Contient la grille qui compose la map, et la liste des joueurs.
 */
struct s_map{
    char* name; //Le nom de la map (provient du fichier)
    char* author; //Le nom de l'auteur de la map
    int** grid; //La grille de jeu
    int width; //La largeur de la grille
    int height; //La hauteur de la grille
    player* players; //Liste des joueurs
    int nbrPlayers;
    bombList* bombs; //Liste des bombes
    bool autoRemove; //Indique si les blocs destructible de la map doivent etre automatiquement supprimés aléatoirement
    map_Error error; //Indique un code d'erreur si l'initialisation de la structure a échoué
    char* filename; //Nom du fichier map
    int** startingBlocks; //Enregistrement des points de départ de la map
};

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