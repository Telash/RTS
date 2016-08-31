#include "RTS.h"

SDL_Event event;

void RTS_CreditsIncome(int* continousIncome, int* credits, int gameTime)
{
    if(gameTime%80==0)
    {
        int i;
        for(i=0; i<2; i++)
        {
            credits[i]+=continousIncome[i];
        }
    }
}

int RTS_CheckVictory(long* tankFlags, long* buildingFlags, long* turretFlags)
{
    int i;
    int victory=0;
    for(i=0; i<MAX_BUILDINGS; i++)
    {
        if(victory<3)
        {
            if(buildingFlags[i] & ALIVE)
            {
                if(buildingFlags[i] & ALLEGIANCE)
                {
                    victory |= 1;
                }
                else
                {
                    victory |= 2;
                }
            }
        }
        else
        {
            i=MAX_BUILDINGS;
        }
    }
    for(i=0; i<MAX_TANKS; i++)
    {
        if(victory<3)
        {
            if(tankFlags[i] & ALIVE)
            {
                if(tankFlags[i] & ALLEGIANCE)
                {
                    victory |= 1;
                }
                else
                {
                    victory |= 2;
                }
            }
        }
        else
        {
            i=MAX_TANKS;
        }
    }
    for(i=0; i<MAX_TURRETS; i++)
    {
        if(victory<3)
        {
            if(turretFlags[i] & ALIVE)
            {
                if(turretFlags[i] & ALLEGIANCE)
                {
                    victory |= 1;
                }
                else
                {
                    victory |= 2;
                }
            }
        }
        else
        {
            i=MAX_TURRETS;
        }
    }
    return victory;
}

int RTS_GetInput()
{
    if(SDL_PollEvent(&event)!=0)
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    {
                        return QUIT;
                    }break;
                }
            }break;
            case SDL_MOUSEBUTTONDOWN:
            {
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    return LEFTCLICK;
                }
                else if(event.button.button == SDL_BUTTON_RIGHT)
                {
                    return RIGHTCLICK;
                }
            }
            case SDL_MOUSEBUTTONUP:
            {
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    return LEFTCLICK_UP;
                }
                else if(event.button.button == SDL_BUTTON_RIGHT)
                {
                    return RIGHTCLICK_UP;
                }
            }
        }
    }
    return 0;
}

void RTS_HealthBarTank(long* flags, int* healthRemain, int* maxHealth, short* type)
{
    int i;
    for(i=1; i<MAX_TANKS; i++)
    {
        if(((flags[i] & SELECTED) || healthRemain[i]!=maxHealth[type[i]]) && flags[i] & ALIVE)
        {
            flags[i] |= SHOW_HEALTH_BAR;
        }
        else if(flags[i] & SHOW_HEALTH_BAR)
        {
            flags[i] ^= SHOW_HEALTH_BAR;
        }
    }
}

void RTS_HealthBarTurret(long* flags, int* healthRemain, int* MaxHealth)
{
    int i;
    for(i=1; i<MAX_TANKS; i++)
    {
        if(((flags[i] & SELECTED) || healthRemain[i]!=MaxHealth[TURRET]) && (flags[i] & ALIVE))
        {
            flags[i] |= SHOW_HEALTH_BAR;
        }
        else if(flags[i] & SHOW_HEALTH_BAR)
        {
            flags[i] ^= SHOW_HEALTH_BAR;
        }
    }
}

void RTS_HealthBarBuilding(long* flags, int* healthRemain, unsigned char* type, int* maxHealth)
{
    int i;
    for(i=1; i<MAX_BUILDINGS; i++)
    {
        if(((flags[i] & SELECTED) || healthRemain[i]!=maxHealth[type[i]]) && flags[i] & ALIVE)
        {
            flags[i] |= SHOW_HEALTH_BAR;
        }
        else if(flags[i] & SHOW_HEALTH_BAR)
        {
            flags[i] ^= SHOW_HEALTH_BAR;
        }
    }
}

int RTS_SelectingBuildingAndTurret(long* flags, int* x, int* y, int* leftClickState, long* flagTanks, long* flagTurrets,
                                   int* turretX, int* turretY)
{
    int i;
    int mX;
    int mY;
    int selectedBuilding = 0;
    SDL_GetMouseState(&mX, &mY);
    *leftClickState = SELECT_TANKS;
    for(i=1; i<MAX_BUILDINGS; i++)
    {
        if(flags[i] & SELECTED)
        {
            flags[i] ^= SELECTED;
        }
    }
    for(i=1; i<MAX_TURRETS; i++)
    {
        if(flagTurrets[i] & SELECTED)
        {
            flagTurrets[i] ^= SELECTED;
        }
    }
    for(i=1; i<MAX_TANKS; i++)
    {
        if(flagTanks[i] & SELECTED)
        {
            flagTanks[i] ^= SELECTED;
        }
    }
    for(i=1; i<MAX_BUILDINGS; i++)
    {
        if(flags[i] & ALIVE)
        {
            if(x[i]<mX && x[i]+63>mX && y[i]<mY && y[i]+63>mY && (flags[i] & ALLEGIANCE))
            {
                flags[i] |= SELECTED;
                selectedBuilding = i;
                *leftClickState = SELECT_CONSTRUCTION;
            }
        }
    }
    if(selectedBuilding==0)
    {
        for(i=1; i<MAX_TURRETS; i++)
        {
            if(flagTurrets[i] & ALIVE)
            {
                if(turretX[i]<mX && turretX[i]+63>mX && turretY[i]<mY && turretY[i]+63>mY && (flagTurrets[i] & ALLEGIANCE))
                {
                    flagTurrets[i] |= SELECTED;
                    selectedBuilding = i;
                    *leftClickState = DEFAULT;
                }
            }
        }
    }
    return selectedBuilding;
}
