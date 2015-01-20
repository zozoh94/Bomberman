#include "../src/map.c"

int main()
{
    map** listMap = NULL;
    int e = ListMap(&listMap);
    int i=0;
    while(listMap[i] != NULL)
    {
	printf("Fichier map %d nom : %s\n", i, listMap[i]->name);
	++i;
    }
    FreeMap(listMap, e);
    return 0;
}
