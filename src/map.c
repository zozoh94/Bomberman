#include "map.h"

int ListMap(map*** ptrListMap)
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
		    if(nbrMap==1)
		    {
			listMaps = malloc(nbrMap*sizeof(map*));
			if(listMaps == NULL)
			    return MAP_ALLOC;
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
			    return MAP_ALLOC;
			}
		    }
		    *ptrListMap = listMaps;
		    listMaps[nbrMap-1] = malloc(sizeof(map));
		    if(listMaps[nbrMap-1] == NULL)
		    {
			free(listMaps);
			*ptrListMap =NULL;
			return MAP_ALLOC;
		    }
			
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
					listMaps[nbrMap-1]->error = MAP_FORMAT_name;
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
					listMaps[nbrMap-1]->error = MAP_FORMAT_author;
				    }
				}
				if(strcmp(key,  map_JSON_Key_Str[KEY_author]) == 0)
				{
				    if (json_object_is_type(val, json_type_array))
				    {
					array_list *grid = json_object_get_array(val);
					if(grid->length<=0)
					    listMaps[nbrMap-1]->error = MAP_FORMAT_grid;
					listMaps[nbrMap-1]->height = grid->length;
					if(!json_object_is_type(grid->array[0], json_type_string))
					    listMaps[nbrMap-1]->error = MAP_FORMAT_grid;
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
						listMaps[nbrMap-1]->error = MAP_FORMAT_grid;
					}
				    }
				    else
				    {
					listMaps[nbrMap-1]->error = MAP_FORMAT_grid;
				    }
				}
			    }
			}
			else
			{
			    return MAP_FORMAT;
			}
		    }
		    else
		    {
			listMaps[nbrMap-1]->error = MAP_FILE;
		    }
		    fclose(file);
		}
	    }
	}	
    }
    closedir(mapsDir);
    return EXIT_SUCCESS;
}
