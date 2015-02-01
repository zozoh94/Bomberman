#include "audio.h"

Mix_Music *music = NULL;
Mix_Chunk *bombSound[NBR_BOMB_SOUND] = {NULL};
Mix_Chunk *bonusSound = NULL;
Mix_Chunk *winSound = NULL;

int InitSounds()
{
    fprintf(stderr,"Initialisation des sons...\n");
    //Chargement de la musique
    music = Mix_LoadMUS("getlucky.mp3");
    Mix_VolumeMusic(MIX_MAX_VOLUME/2);

    //S'il y a eu une erreur au chargement de la musique
    if( music == NULL )
    {
        return 1;
    }
    
    //Chargement des effets sonores
    bonusSound = Mix_LoadWAV("bonus.wav");
    winSound = Mix_LoadWAV("win.wav");
    
    char bombFile[10]; 
    for(int i=0; i<NBR_BOMB_SOUND; i++)
    {
	sprintf(bombFile, "bomb%d.wav", i+1);		
        bombSound[i] = Mix_LoadWAV(bombFile);
    }
    
    //S'il y a eu un problème au chargement des effets sonore
    if((bonusSound == NULL) || (winSound == NULL))
    {
        return 1;
    }

    //S'il y a eu un problème au chargement des effets sonore des bombes
    for(int i=0; i<NBR_BOMB_SOUND; i++)
    {
	if(bombSound[i] == NULL)
	{
	    fprintf(stderr,"%d...\n", i);
	    return 1;
	}
    }
	
    //Si tout s'est bien chargé
    return 0;
}

void FreeSounds()
{
    Mix_FreeChunk(bonusSound);
    Mix_FreeChunk(winSound);
    for(int i=0; i<NBR_BOMB_SOUND; i++)
	Mix_FreeChunk(bombSound[i]);

    Mix_FreeMusic(music);
}

Mix_Chunk* RandomBomb()
{
    return bombSound[rand()%NBR_BOMB_SOUND];
}
