#include "map.h"

int ListMap(map*** ptrListMap)
{
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
			    ptrListMap =NULL;
			    return MAP_ALLOC;
			}
		    }
		    ptrListMap = &listMaps;
		    listMaps[nbrMap-1] = malloc(sizeof(map));
		    if(listMaps[nbrMap-1] == NULL)
		    {
			free(listMaps);
			ptrListMap =NULL;
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
				for(int k=0; k<JSON_KEY_LEN; k++)
				{
				    
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
