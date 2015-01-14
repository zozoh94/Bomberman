#include <stdio.h>
#include <stdlib.h>
#include "bomb.h"

bomb* InitBomb(int x, int y, int rayon, int timer, player* myPlayer, btype type)
{
	bomb bomb = malloc (sizeof(bomb));
	bomb.x = x;
	bomb.y = y;
	bomb.explosion = rayon;
	bomb.timer = timer;
	bomb.myPlayer = myPlayer;
	bomb.type = type;
	return bomb;
}

bomb* CreateBomb(player* p){
  //Changer rayon, timer et type en fonction du joueur
  return InitBomb(p->x, p->y, p->bombR, TIMERBOMB, p, p->bombT);
}

void Explode(map* map, bomb* bomb)
{
	int i,j;
	RemoveBombList(map->bombs, bomb); //retire la bombe de la liste des bombes de la map
	player.bombs --; //décrémente le nombre de bombes posées par le joueur de 1
	for (i=0;i<rayon;i++)
	{
		if (map[bomb.x+i][bomb.y]=1) //bloc indestructible : fin
		{
			break;
		}
		else if (map[bomb.x+i][bomb.y]=3) //bloc destructible : destruction du bloc + fin
		{
			map[bomb.x+i][bomb.y]=0;
			break;
		}
		else if (map[bomb.x+i][bomb.y]=4) //bombe : explosion de la bombe + continue
		{
			//affichage des flammes de la bombe	
			bombTouch = GetBomb (map,x+i,y);
			bombTouch.timer = 0; 
		}
		else //rien ou bonus : continue
		{
			//affichage des flammes de la bombe
		}
	}
	for (i=0;i<rayon;i++)
	{
		if (map[bomb.x-i][bomb.y]=1)
		{
			break;
		}
		else if (map[bomb.x-i][bomb.y]=3)
		{
			map[bomb.x-i][bomb.y]=0;
			break;
		}
		else if (map[bomb.x-i][bomb.y]=4)
		{
			//affichage des flammes de la bombe	
			bombTouch = GetBomb (map,x-i,y);
			bombTouch.timer = 0;
		}
		else
		{
			//affichage des flammes de la bombe
		}
	}
	for (i=0;i<rayon;i++)
	{
		if (map[bomb.x][bomb.y+i]=1)
		{
			break;
		}
		else if (map[bomb.x][bomb.y+i]=3)
		{
			map[bomb.x][bomb.y+i]=0;
			break;
		}
		else if (map[bomb.x][bomb.y+i]=4)
		{
			//affichage des flammes de la bombe	
			bombTouch = GetBomb (map,x,y+i);
			bombTouch.timer = 0;
		}
		else
		{
			//affichage des flammes de la bombe
		}
	}
	for (i=0;i<rayon;i++)
	{
		if (map[bomb.x][bomb.y-i]=1)
		{
			break;
		}
		else if (map[bomb.x][bomb.y-i]=3)
		{
			map[bomb.x][bomb.y-i]=0;
			break;
		}
		else if (map[bomb.x][bomb.y-i]=4)
		{
			//affichage des flammes de la bombe	
			bombTouch = GetBomb (map,x,y-i);
			bombTouch.timer = 0;
		}
		else
		{
			//affichage des flammes de la bombe
		}
	}
}

bomb* GetBomb (map* map, int x, int y)
{
	bombList* bombList = map->bombs;
	while (((bombList->data)->x != x) && ((bombList->data)->y != y)) //parcoure la liste
	{
		bombList = bombList->next;
	}
	return bombList->data; //retourne celle demandée
} 

bombList* NewBombList()
{
	return NULL;
}
	
bombList AddBombList(bombList* l, bomb* b)
{
	bombList bombList = malloc (sizeof(bomb));
	bombList->data = b;
	bombList->next = l;
	return bomblist;
}

bombList RemoveBombList(bombList* l, bomb* b)
{
	bombList *previous = l;
	bombList *temp;
	if (previous->data == b) //cas du premier élément
	{
		l = previous->next;
		free(previous);
	}
	else //parcoure la liste en deux temps, avec temp et avec previois qui a l'ancienne valeur de temp
	{
		temp = previous->next;
		while ((temp!=NULL) || (temp->data==b))
		{
			previous = temp;
			temp = temp->next;
		}
		if (bombList->data==b) //supprime la bombe en faisant ponté previous vers le suivant de temp
		{
			previous->next = temp->next	
			free(temp);
		}
	}
	return l;
}
