#include "map.h"

int ListMaps(map*** ptrListMap)
{
    map** listMaps = NULL;
    map** listMapsRealloc;
    struct dirent *mapFile;
    DIR *mapsDir;
    mapsDir = opendir("." );
    struct stat statFile;
    //On parcourt tous les fichiers du dossier
    int nbrMap = 0;
    while ((mapFile = readdir(mapsDir))) {
	//On teste si le fichier n'est pas un dossier
	if(stat(mapFile->d_name, &statFile) != -1)
	{
	    if(S_ISREG(statFile.st_mode))
	    {
		//On teste si le fichier porte bien l'extension .map
		const char *dot = strrchr(mapFile->d_name, '.');
		if(dot && dot != mapFile->d_name && strcmp("map", dot+1) == 0)
	        {
		    ++nbrMap;
		    if(nbrMap>99)
			return MAP_TOO_MUCH_ERROR;
		    if(nbrMap==1)
		    {
			listMaps = malloc(nbrMap*sizeof(map*));
			if(listMaps == NULL)
			    return MAP_ALLOC_ERROR;
		    }
		    else
		    {
			listMapsRealloc = realloc(listMaps, nbrMap*sizeof(map*));
			if(listMapsRealloc != NULL)
			    listMaps = listMapsRealloc;
			else
			{
			    free(listMaps);
			    *ptrListMap =NULL;
			    return MAP_ALLOC_ERROR;
			}
		    }
		    *ptrListMap = listMaps;
		    listMaps[nbrMap-1] = malloc(sizeof(map));
		    if(listMaps[nbrMap-1] == NULL)
		    {
			free(listMaps);
			*ptrListMap =NULL;
			return MAP_ALLOC_ERROR;
		    }
		    listMaps[nbrMap-1]->error = NO_ERROR;
		    listMaps[nbrMap-1]->filename = malloc(sizeof(mapFile->d_name));
		    strcpy(listMaps[nbrMap-1]->filename, mapFile->d_name);
		    listMaps[nbrMap-1]->nbrPlayers = 0;
		    listMaps[nbrMap-1]->players = NULL;
		    listMaps[nbrMap-1]->bombs = NULL;
		    listMaps[nbrMap-1]->startingBlocks = NULL;
		    listMaps[nbrMap-1]->victory = 0;
		    int result = ParseMap(listMaps[nbrMap-1]);
		    if(result != 0)
			return result;
		}
	    }
	}
    }
    closedir(mapsDir);
    return nbrMap;
}

int ParseMap(map *map)
{
    static const char *map_JSON_Key_Str[] = {
	FOREACH_map_JSON_Key(GENERATE_STRING)
    };
    char string[MAX_LEN_LINE] = "";
    char stringFile[MAX_LEN] = "";
    json_object *jobj = NULL;
    FILE* file = NULL;
    file = fopen(map->filename, "r");
    if (file!=NULL)
    {
	while (fgets(string, MAX_LEN_LINE, file) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
	{
	    strcat(stringFile, string);
	}
	jobj = json_tokener_parse(stringFile);
	if(jobj != NULL)
	{
	    //On parse
	    json_object_object_foreach(jobj, key, val)
	    {
		if(strcmp(key, map_JSON_Key_Str[KEY_name]) == 0)
		{
		    if (json_object_is_type(val, json_type_string))
		    {
			map->name = malloc(strlen(json_object_get_string(val))+1);
			strcpy(map->name, json_object_get_string(val));
		    }
		    else
		    {
			map->error = MAP_FORMAT_name_ERROR;
		    }
		}
		if(strcmp(key, map_JSON_Key_Str[KEY_author]) == 0)
		{
		    if (json_object_is_type(val, json_type_string))
		    {
			map->author = malloc(strlen(json_object_get_string(val))+1);
			strcpy(map->author, json_object_get_string(val));
		    }
		    else
		    {
			map->error = MAP_FORMAT_author_ERROR;
		    }
		}
		if(strcmp(key, map_JSON_Key_Str[KEY_auto_remove]) == 0)
		{
		    if (json_object_is_type(val, json_type_boolean))
		    {
			map->autoRemove = json_object_get_boolean(val);
		    }
		    else
		    {
			map->error = MAP_FORMAT_auto_remove_ERROR;
		    }
		}
		if(strcmp(key, map_JSON_Key_Str[KEY_theme]) == 0)
		{
		    if (json_object_is_type(val, json_type_string))
		    {
			char chaine[255];
			strcpy(chaine, json_object_get_string(val));
			map->undestructibleBlock = IMG_Load(strcat(chaine, "_undestr.bmp"));
			strcpy(chaine, json_object_get_string(val));
			map->destructibleBlock = IMG_Load(strcat(chaine, "_destr.bmp"));
			strcpy(chaine, json_object_get_string(val));
			map->floor = IMG_Load(strcat(chaine, "_floor.bmp"));
		    }
		    else
		    {
			map->error = MAP_FORMAT_theme_ERROR;
		    }
		}

		if(strcmp(key,  map_JSON_Key_Str[KEY_grid]) == 0)
		{
		    if (json_object_is_type(val, json_type_array))
		    {
			array_list *grid = json_object_get_array(val);
			if(grid->length<=0)
			    map->error = MAP_FORMAT_grid_ERROR;
			map->height = grid->length;
			if(!json_object_is_type(grid->array[0], json_type_string))
			    map->error = MAP_FORMAT_grid_ERROR;
			map->width = strlen(json_object_get_string(grid->array[0]));

			map->grid = malloc(map->width*sizeof(int*));
			for(int i=0; i<map->width; i++)
			{
			    map->grid[i] = malloc(map->height*sizeof(int));
			}
			for(int i=0; i<map->height; i++)
			{
			    if(json_object_is_type(grid->array[i], json_type_string))
			    {
				const char *c = json_object_get_string(grid->array[i]);
				for(int j=0; j<map->width; j++)
				{
				    map->grid[j][i] = c[j] - '0';
				}
			    }
			    else
				map->error = MAP_FORMAT_grid_ERROR;
			}
		    }
		    else
		    {
			map->error = MAP_FORMAT_grid_ERROR;
		    }
		}
	    }
	}
	else
	{
	    return MAP_FORMAT_ERROR;
	}
    }
    else
    {
	map->error = MAP_FILE_ERROR;
    }
    fclose(file);
    return 0;
}

int InitMap(map* map, int nbrPlayers, player** listPlayer)
{
    map->bombs = NULL;
    //On compte le nombre de cases de départ joueurs déjà disponible
    int nbrStartingBlock = 0;
    for(int i=0; i<map->height; i++)
    {
	for(int j=0; j<map->width; j++)
	{
	    if(map->grid[j][i]==STARTING_BLOCK)
		++nbrStartingBlock;
	}
    }
    if(nbrStartingBlock<nbrPlayers){
	return MAP_TOO_MUCH_PLAYER_ERROR;
    }
    int proba = 20;
    map->nbrPlayers = nbrPlayers;
    //On parcourt la grille
    for(int i=0; i<map->height; i++)
    {
    	for(int j=0; j<map->width; j++)
    	{
    	    //On supprime aléatoirement des blocs déstructibles
    	    if(map->grid[j][i]==DESTRUCTIBLE_BLOCK && map->autoRemove==true)
    	    {
    		if(proba > rand() % 100)
    		    map->grid[j][i] = EMPTY_BLOCK;
    	    }
    	}
    }
    //En fonction du nombre de joueur on supprime des cases de départ
    if(map->nbrPlayers==2) //Cas du 1vs1
    {
    	//On supprime les starting block de la partie superieur droite et de la partien inférieur gauche
	for(int i=0; i<(int)(map->height/2); i++)
    	{
	    for(int j=(int)(map->width/2); j<map->width; j++)
    	    {
		if(map->grid[j][i]==STARTING_BLOCK)
		    map->grid[j][i]=EMPTY_BLOCK;
	    }
	}
	for(int i=(int)(map->height/2); i<map->height; i++)
    	{
	    for(int j=0; j<(int)(map->width/2); j++)
    	    {
		if(map->grid[j][i]==STARTING_BLOCK)
		    map->grid[j][i]=EMPTY_BLOCK;
	    }
	}
    	//On laisse un seul starting block dans la partie superieur gauche
	bool startingBlockFound = false;
    	for(int i=0; i<(int)(map->height/2); i++)
    	{
    	    for(int j=0; j<(int)(map->width/2); j++)
    	    {
		if(map->grid[j][i]==STARTING_BLOCK && !startingBlockFound)
		    startingBlockFound = true;
		else if(map->grid[j][i]==STARTING_BLOCK && startingBlockFound)
		    map->grid[j][i]=EMPTY_BLOCK;
    	    }
    	}
	if(!startingBlockFound)
	    return MAP_1VS1_ERROR;
	startingBlockFound = false;
    	//On laisse un seul starting block dans la partie inférieur droite
    	for(int i=map->height-1; i<=(int)(map->height/2); i--)
    	{
    	    for(int j=map->width-1; j<=(int)(map->width/2); j--)
    	    {
		if(map->grid[j][i]==STARTING_BLOCK && !startingBlockFound)
		    startingBlockFound = true;
		else if(map->grid[j][i]==STARTING_BLOCK && startingBlockFound)
		    map->grid[j][i]=EMPTY_BLOCK;
    	    }
    	}
    }
    else
    {
	if(nbrPlayers<nbrStartingBlock)
	{
	    //On supprime les cases en trop
	    while(nbrStartingBlock != nbrPlayers)
	    {
		for(int i=0; i<map->height; i++)
		{
		    for(int j=0; j<map->width; j++)
		    {
			if(map->grid[j][i]==STARTING_BLOCK && proba > rand() % 100 && nbrStartingBlock != nbrPlayers)
			{
			    map->grid[j][i]=EMPTY_BLOCK;
			    --nbrStartingBlock;
			}
		    }
		}
	    }
	}
    }
    //On enregistre les points de départs
    map->startingBlocks = malloc(nbrPlayers*sizeof(int*));
    for(int i=0; i<nbrPlayers; i++)
    {
    	map->startingBlocks[i] = malloc(2*sizeof(int));
    }
    int k=0;
    for(int i=0; i<map->height; i++)
    {
    	for(int j=0; j<map->width; j++)
    	{
    	    if(map->grid[j][i] == STARTING_BLOCK)
    	    {
    		map->startingBlocks[k][0]=j;
    		map->startingBlocks[k][1]=i;
    		++k;
    		map->grid[j][i] = EMPTY_BLOCK;
    	    }
    	}
    }
    map->players = listPlayer;
    for(int i=0; i<map->nbrPlayers; i++)
    {
    	//On affectes aux joueurs des coordonnées de départ
    	map->players[i]->x=map->startingBlocks[i][0];
    	map->players[i]->y=map->startingBlocks[i][1];
    }
    return 0;
}

void FreeMaps(map** listMap, int nbrMaps)
{
    for(int i=0; i<nbrMaps; i++)
    {
	//On libère la grille
	for(int j=0; j<listMap[i]->height; j++)
	{
	    free(listMap[i]->grid[j]);
	}
	free(listMap[i]->grid);
	//On libère le nom
	free(listMap[i]->name);
	//On libère l'auteur
	free(listMap[i]->author);
	//On libère la mémoire du nom de fichier
	free(listMap[i]->filename);
	//On libère la mémoire de la liste de joueurs
	free(listMap[i]->players);
	//On libère la mémoire des enregistrements de startingBlock
	for(int k=0; k<listMap[i]->nbrPlayers; k++)
	    free(listMap[i]->startingBlocks[k]);
	free(listMap[i]->startingBlocks);
	//On peux liberer la mémoire de la structure
	free(listMap[i]);
    }
    //Et enfin on libère le tableau de map
    free(listMap);
}
