#include "../src/map.c"

int main()
{
    map** listMap = NULL;
    int e = ListMaps(&listMap);
    int i=0;
    while(listMap[i] != NULL)
    {
	printf("Fichier map %d nom : %s\n", i, listMap[i]->name);
	++i;
    }
    FreeMaps(listMap, e);
    return 0;
}
