#include "map.h"

int ListMaps(map*** ptrListMap)
{
    static const char *map_JSON_Key_Str[] = {
	FOREACH_map_JSON_Key(GENERATE_STRING)
    };
    map** listMaps;
    map** listMapsRealloc;
    struct dirent *mapFile;
    DIR *mapsDir;
    FILE* file = NULL;
    char string[MAX_LEN_LINE] = "";
    char stringFile[MAX_LEN] = "";
    json_object *jobj = NULL;
    chdir("../maps");
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
			
		    //On peut maintenant parser le fichier .map formaté en JSON
		    file = fopen(mapFile->d_name, "r");
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
					listMaps[nbrMap-1]->name = malloc(sizeof(json_object_get_string(val)));
					strcpy(listMaps[nbrMap-1]->name, json_object_get_string(val));
				    }
				    else
				    {
					listMaps[nbrMap-1]->error = MAP_FORMAT_name_ERROR;
				    }
				}
				if(strcmp(key, map_JSON_Key_Str[KEY_author]) == 0)
				{
				    if (json_object_is_type(val, json_type_string))
				    {
					listMaps[nbrMap-1]->author = malloc(sizeof(json_object_get_string(val)));
					strcpy(listMaps[nbrMap-1]->author, json_object_get_string(val));
				    }
				    else
				    {
					listMaps[nbrMap-1]->error = MAP_FORMAT_author_ERROR;
				    }
				}
				if(strcmp(key, map_JSON_Key_Str[KEY_auto_remove]) == 0)
				{
				    if (json_object_is_type(val, json_type_boolean))
				    {
					listMaps[nbrMap-1]->autoRemove = json_object_get_string(val);
				    }
				    else
				    {
					listMaps[nbrMap-1]->error = MAP_FORMAT_author_ERROR;
				    }
				}
				if(strcmp(key,  map_JSON_Key_Str[KEY_author]) == 0)
				{
				    if (json_object_is_type(val, json_type_array))
				    {
					array_list *grid = json_object_get_array(val);
					if(grid->length<=0)
					    listMaps[nbrMap-1]->error = MAP_FORMAT_grid_ERROR;
					listMaps[nbrMap-1]->height = grid->length;
					if(!json_object_is_type(grid->array[0], json_type_string))
					    listMaps[nbrMap-1]->error = MAP_FORMAT_grid_ERROR;
					listMaps[nbrMap-1]->width = strlen(json_object_get_string(grid->array[0]));
					//On initialise la matrice de la grille
					listMaps[nbrMap-1]->grid = malloc(grid->length*sizeof(int*));
					for(int i=0; i<grid->length; i++)
					{
					    listMaps[nbrMap-1]->grid[i] = malloc(listMaps[nbrMap-1]->width*sizeof(int));
					}
					const char *c = json_object_get_string(grid->array[0]);
					for(int i=0; i<listMaps[nbrMap-1]->width; i++)
					{
					    listMaps[nbrMap-1]->grid[0][i] = c[i] - '0';
					}
					for(int i=1; i<grid->length; i++)
					{
					    if(json_object_is_type(grid->array[i], json_type_string))
					    {
						const char *c = json_object_get_string(grid->array[i]);
						for(int j=0; j<listMaps[nbrMap-1]->width; j++)
						{
						    listMaps[nbrMap-1]->grid[i][j] = c[j] - '0';
						}
					    }
					    else
						listMaps[nbrMap-1]->error = MAP_FORMAT_grid_ERROR;
					}
				    }
				    else
				    {
					listMaps[nbrMap-1]->error = MAP_FORMAT_grid_ERROR;
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
			listMaps[nbrMap-1]->error = MAP_FILE_ERROR;
		    }
		    fclose(file);
		}
	    }
	}	
    }
    closedir(mapsDir);
    return nbrMap;
}

void InitMap(map* map, int nbrPlayers, player* listPlayer)
{
    /* int proba = 20; */
    /* map->nbrPlayers = nbrPlayers; */
    /* //On parcourt la grille */
    /* for(int i=0; i<map->height; i++) */
    /* { */
    /* 	for(int j=0; j<map->width; j++) */
    /* 	{ */
    /* 	    //On supprime aléatoirement des blocs déstructibles */
    /* 	    if(map->grid[i][j]==DESTRUCTIBLE_BLOCK && map->autoRemove==true) */
    /* 	    { */
    /* 		if(proba > rand() % 100) */
    /* 		    map->grid[i][j] = UNDESTRUCTIBLE_BLOCK; */
    /* 	    } */
    /* 	} */
    /* } */
    /* int nbrStartingBlock = 0; */
    /* //En fonction du nombre de joueur on ajoute ou supprime des cases de départ */
    /* if(map->nbrPlayers==2) //Cas du 1vs1 */
    /* { */
    /* 	//On supprime les starting block de la partie superieur droite et de la partien inférieur gauche */
	
    /* 	//On met un starting block dans la partie superieur gauche si il n'y en a pas déja un */
    /* 	//On compte le nombre de cases de départ joueurs déjà disponible dans cette zone	 */
    /* 	for(int i=0; i<(int)(map->height/2); i++) */
    /* 	{ */
    /* 	    for(int j=0; j<(int)(<map->width/2); j++) */
    /* 	    { */
    /* 		if(map->grid[i][j]==STARTING_BLOCK) */
    /* 		    ++nbrStartingBlock; */
    /* 	    } */
    /* 	} */
    /* 	while(nbrStartingBlock!=1) //On ajoute ou supprime des blocks */
    /* 	{ */
    /* 	    	for(int i=0; i<(int)(map->height/2); i++) */
    /* 		{ */
    /* 		    for(int j=0; j<(int)(<map->width/2); j++) */
    /* 		    { */
    /* 		    } */
    /* 		} */
    /* 	} */
    /* 	//On met un starting block dans la partie inférieur droite si il n'y en a pas déja un */
    /* 	//On compte le nombre de cases de départ joueurs déjà disponible dans cette zone */
    /* 	nbrStartingBlock=0; */
    /* 	for(int i=(int)(map->height/2); i<map->height; i++) */
    /* 	{ */
    /* 	    for(int j=(int)(<map->width/2); j<map->width; j++) */
    /* 	    { */
    /* 		if(map->grid[i][j]==STARTING_BLOCK) */
    /* 		    ++nbrStartingBlock; */
    /* 	    } */
    /* 	} */
    /* 	while(nbrStartingBlock!=1) //On ajoute ou supprime des blocks */
    /* 	{ */
    /* 	    for(int i=(int)(map->height/2); i<map->height; i++) */
    /* 	    { */
    /* 		for(int j=(int)(<map->width/2); j<map->width; j++) */
    /* 		{ */
    /* 		} */
    /* 	    } */
    /* 	} */
    /* } */
    /* else */
    /* { */
    /* 	//On compte le nombre de cases de départ joueurs déjà disponible */
    /* 	for(int i=0; i<map->height; i++) */
    /* 	{ */
    /* 	    for(int j=0; j<map->width; j++) */
    /* 	    { */
    /* 		if(map->grid[i][j]==STARTING_BLOCK) */
    /* 		    ++nbrStartingBlock; */
    /* 	    } */
    /* 	} */
    /* 	//On dispose des starting blocks supplémentaires si besoin */
    /* 	while(map->nbrPlayers != nbrStartingBlock) */
    /* 	{ */
    /* 	    for(int i=0; i<map->height; i++) */
    /* 	    { */
    /* 		for(int j=0; j<map->width; j++) */
    /* 		{ */
    /* 		    if(proba > rand() % 100) */
    /* 		    { */
    /* 			if(map->nbrPlayers > nbrStartingBlock && map->grid[i][j] == EMPTY_BLOCK) */
    /* 			{ */
    /* 			    map->grid[i][j]=STARTING_BLOCK; */
    /* 			    ++nbrStartingBlock; */
    /* 			} */
    /* 			if(map->nbrPlayers < nbrStartingBlock && map->grid[i][j] == STARTING_BLOCK) */
    /* 			{ */
    /* 			    map->grid[i][j]=EMPTY_BLOCK; */
    /* 			    --nbrStartingBlock; */
    /* 			} */
    /* 		    } */
    /* 		} */
    /* 	    } */
    /* 	} */
    /* } */
    /* //On enregistre les points de départs */
    /* map->startingBlocks = malloc(nbrPlayers*sizeof(int*)); */
    /* for(int i=0; i<nbrPlayers; i++) */
    /* { */
    /* 	map->startingBlocks[i] = malloc(2*sizeof(int)); */
    /* } */
    /* int k=0; */
    /* for(int i=0; i<map->height; i++) */
    /* { */
    /* 	for(int j=0; j<map->width; j++) */
    /* 	{ */
    /* 	    if(map->grid[i][j] == STARTING_BLOCK) */
    /* 	    { */
    /* 		map->startingBlocks[k][0]=i; */
    /* 		map->startingBlocks[k][1]=j; */
    /* 		++k; */
    /* 		map->grid[i][j] = EMPTY_BLOCK; */
    /* 	    } */
    /* 	} */
    /* } */
    /* map->players = malloc(map->nbrPlayers*sizeof(player*)); */
    /* for(int i=0; i<map->nbrPlayers; i++) */
    /* { */
    /* 	map->players[i] = &player[i]; */
    /* 	//On affectes aux joueurs des coordonnées de départ */
    /* 	map->players[i]->x=map->startingBlocks[i][0]; */
    /* 	map->players[i]->y=map->startingBlocks[i][1]; */
    /* } */
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
	for(int i=0; i<listMap[i]->nbrPlayers; i++)
	    free(listMap[i]->startingBlocks[i]);
	free(listMap[i]->startingBlocks);
	//On peux liberer la mémoire de la structure
	free(listMap[i]);
    }
    //Et enfin on libère le tableau de map
    free(listMap);
}
