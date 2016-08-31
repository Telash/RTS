#include "RTS.h"

int mouseX;
int mouseY;

int startX=0;
int startY=0;

void RTS_HandleGrid(char* grid, int* x, int* y, long* flags, char* direction, int* currentTile)
{
    int i;
    int ya;
    int xa;
    for(i=0; i<MAX_TANKS; i++)
    {
        if(x[i]%TILESIZE==0 && y[i]%TILESIZE==0 && direction[i]!=0)
        {
            xa=x[i]/TILESIZE;
            ya=y[i]/TILESIZE;
            switch(direction[i])
            {
                case NORTH:
                {
                    if(grid[(ya-1)*MAP_SIZE_X+xa]==0)
                    {
                        grid[(ya)*MAP_SIZE_X+xa]=0;
                        grid[(ya-1)*MAP_SIZE_X+xa]=i;
                        currentTile[i]=(ya-1)*MAP_SIZE_X+xa;
                    }
                }break;
                case NORTHEAST:
                {
                    if(grid[(ya-1)*MAP_SIZE_X+xa+1]==0)
                    {
                        grid[(ya)*MAP_SIZE_X+xa]=0;
                        grid[(ya-1)*MAP_SIZE_X+xa+1]=i;
                        currentTile[i]=(ya-1)*MAP_SIZE_X+xa+1;
                    }
                }break;
                case EAST:
                {
                    if(grid[(ya)*MAP_SIZE_X+xa+1]==0)
                    {
                        grid[(ya)*MAP_SIZE_X+xa]=0;
                        grid[(ya)*MAP_SIZE_X+xa+1]=i;
                        currentTile[i]=(ya)*MAP_SIZE_X+xa+1;
                    }
                }break;
                case SOUTHEAST:
                {
                    if(grid[(ya+1)*MAP_SIZE_X+xa+1]==0)
                    {
                        grid[(ya)*MAP_SIZE_X+xa]=0;
                        grid[(ya+1)*MAP_SIZE_X+xa+1]=i;
                        currentTile[i]=(ya+1)*MAP_SIZE_X+xa+1;
                    }
                }break;
                case SOUTH:
                {
                    if(grid[(ya+1)*MAP_SIZE_X+xa]==0)
                    {
                        grid[(ya+1)*MAP_SIZE_X+xa]=i;
                        if(grid[(ya)*MAP_SIZE_X+xa]==i)
                        {
                            grid[(ya)*MAP_SIZE_X+xa]=0;
                            currentTile[i]=(ya+1)*MAP_SIZE_X+xa;
                        }
                    }
                }break;
                case SOUTHWEST:
                {
                    if(grid[(ya+1)*MAP_SIZE_X+xa-1]==0)
                    {
                        grid[(ya)*MAP_SIZE_X+xa]=0;
                        grid[(ya+1)*MAP_SIZE_X+xa-1]=i;
                        currentTile[i]=(ya+1)*MAP_SIZE_X+xa-1;
                    }
                }break;
                case WEST:
                {
                    if(grid[(ya)*MAP_SIZE_X+xa-1]==0)
                    {
                        grid[(ya)*MAP_SIZE_X+xa]=0;
                        grid[(ya)*MAP_SIZE_X+xa-1]=i;
                        currentTile[i]=(ya)*MAP_SIZE_X+xa-1;
                    }
                }break;
                case NORTHWEST:
                {
                    if(grid[(ya-1)*MAP_SIZE_X+xa-1]==0)
                    {
                        grid[(ya)*MAP_SIZE_X+xa]=0;
                        grid[(ya-1)*MAP_SIZE_X+xa-1]=i;
                        currentTile[i]=(ya-1)*MAP_SIZE_X+xa-1;
                    }
                }break;
            }
        }
    }
}

void MOVE_MoveProjectile(Projectile* projectile)
{
    int i;
    int movement;
    int speed;
    for(i=1; i<MAX_PROJECTILES; i++)
    {
        if(projectile->flags[i] & ALIVE)
        {
            if(projectile->rangeLeft[i]==TANK_RANGE)
            {
                speed=42;
            }
            else
            {
                speed=PROJECTILE_SPEED;
            }
            projectile->moveTransfered[i]+=speed;
            if(projectile->angle[i]>=315 || projectile->angle[i]<=45)
            {
                projectile->y[i]-=speed;
            }
            if(projectile->angle[i]>=45 && projectile->angle[i]<=135)
            {
                projectile->x[i]+=speed;
            }
            if(projectile->angle[i]>=135 && projectile->angle[i]<=225)
            {
                projectile->y[i]+=speed;
            }
            if(projectile->angle[i]>=225 && projectile->angle[i]<=315)
            {
                projectile->x[i]-=speed;
            }
            if(projectile->angle[i]>0 && projectile->angle[i]<45)
            {
                movement=projectile->moveTransfered[i]*projectile->moveMultiplier[i];
                projectile->moveTransfered[i]-=movement/projectile->moveMultiplier[i];
                projectile->x[i]+=movement;
            }
            else if(projectile->angle[i]>45 && projectile->angle[i]<90)
            {
                movement=projectile->moveTransfered[i]/projectile->moveMultiplier[i];
                projectile->moveTransfered[i]-=movement*projectile->moveMultiplier[i];
                projectile->y[i]-=movement;
            }
            else if(projectile->angle[i]>90 && projectile->angle[i]<135)
            {
                movement=projectile->moveTransfered[i]/projectile->moveMultiplier[i];
                projectile->moveTransfered[i]-=movement*projectile->moveMultiplier[i];
                projectile->y[i]+=movement;
            }
            else if(projectile->angle[i]>135 && projectile->angle[i]<180)
            {
                movement=projectile->moveTransfered[i]*projectile->moveMultiplier[i];
                projectile->moveTransfered[i]-=movement/projectile->moveMultiplier[i];
                projectile->x[i]+=movement;
            }
            else if(projectile->angle[i]>180 && projectile->angle[i]<225)
            {
                movement=projectile->moveTransfered[i]*projectile->moveMultiplier[i];
                projectile->moveTransfered[i]-=movement/projectile->moveMultiplier[i];
                projectile->x[i]-=movement;
            }
            else if(projectile->angle[i]>225 && projectile->angle[i]<270)
            {
                movement=projectile->moveTransfered[i]/projectile->moveMultiplier[i];
                projectile->moveTransfered[i]-=movement*projectile->moveMultiplier[i];
                projectile->y[i]+=movement;
            }
            else if(projectile->angle[i]>270 && projectile->angle[i]<315)
            {
                movement=projectile->moveTransfered[i]/projectile->moveMultiplier[i];
                projectile->moveTransfered[i]-=movement*projectile->moveMultiplier[i];
                projectile->y[i]-=movement;
            }
            else if(projectile->angle[i]>315 && projectile->angle[i]<360)
            {
                movement=projectile->moveTransfered[i]*projectile->moveMultiplier[i];
                projectile->moveTransfered[i]-=movement/projectile->moveMultiplier[i];
                projectile->x[i]-=movement;
            }
            projectile->rangeLeft[i]-=speed;
        }
    }
}

void RTS_SelectingUnits(int state, int* x, int* y, long* flags)
{
    if(state==START)
    {
        SDL_GetMouseState(&startX, &startY);
    }
    else if(state==FINISH)
    {
        int i;
        for(i=0; i<MAX_TANKS; i++)
        {
            if(flags[i] & SELECTED)
            {
                flags[i] ^= SELECTED;
            }
        }
        SDL_GetMouseState(&mouseX, &mouseY);
        for(i=0; i<MAX_TANKS; i++)
        {
            if(flags[i] & ALIVE && flags[i] & ALLEGIANCE)
            {
                if(((x[i]<=startX && x[i]>=mouseX-TILESIZE+1) || (x[i]<=mouseX && x[i]>=startX-TILESIZE+1)) &&
                   ((y[i]<=startY && y[i]>=mouseY-TILESIZE+1) || (y[i]<=mouseY && y[i]>=startY-TILESIZE+1)))
                {
                    flags[i] |= SELECTED;
                }
            }
        }
    }
}

void RTS_SetDestination(int* destX, int* destY, long* flags)
{
    SDL_GetMouseState(&mouseX, &mouseY);
    int i;
    for(i=0; i<MAX_TANKS; i++)
    {
        if(flags[i] & SELECTED)
        {
            destX[i]=mouseX/TILESIZE*TILESIZE;
            destY[i]=mouseY/TILESIZE*TILESIZE;
        }
    }
}

void RTS_MoveTank(int* x, int* y, char* direction, long* flags, char* grid)
{
    int i;
    int xa;
    int ya;
    for(i=0; i<MAX_TANKS; i++)
    {
        if(flags[i] & ALIVE)
        {
            xa=x[i]/TILESIZE;
            ya=y[i]/TILESIZE;
            switch(direction[i])
            {
                case NORTH:
                {
                    if(y[i]%TILESIZE==0)
                    {
                        if(grid[(ya-1)*MAP_SIZE_X+xa]==i)
                        {
                            y[i]-=SPEED;
                        }
                    }
                    else
                    {
                        y[i]-=SPEED;
                    }
                }break;
                case NORTHEAST:
                {
                    if(y[i]%TILESIZE==0 && x[i]%TILESIZE==0)
                    {
                        if(grid[(ya-1)*MAP_SIZE_X+xa+1]==i)
                        {
                            y[i]-=SPEED;
                            x[i]+=SPEED;
                        }
                    }
                    else
                    {
                        y[i]-=SPEED;
                        x[i]+=SPEED;
                    }
                }break;
                case EAST:
                {
                    if(x[i]%TILESIZE==0)
                    {
                        if(grid[(ya)*MAP_SIZE_X+xa+1]==i)
                        {
                            x[i]+=SPEED;
                        }
                    }
                    else
                    {
                        x[i]+=SPEED;
                    }
                }break;
                case SOUTHEAST:
                {
                    if(y[i]%TILESIZE==0 && x[i]%TILESIZE==0)
                    {
                        if(grid[(ya+1)*MAP_SIZE_X+xa+1]==i)
                        {
                            y[i]+=SPEED;
                            x[i]+=SPEED;
                        }
                    }
                    else
                    {
                        y[i]+=SPEED;
                        x[i]+=SPEED;
                    }
                }break;
                case SOUTH:
                {
                    if(y[i]%TILESIZE==0)
                    {
                        if(grid[(ya+1)*MAP_SIZE_X+xa]==i)
                        {
                            y[i]+=SPEED;
                        }
                    }
                    else
                    {
                        y[i]+=SPEED;
                    }
                }break;
                case SOUTHWEST:
                {
                    if(y[i]%TILESIZE==0 && x[i]%TILESIZE==0)
                    {
                        if(grid[(ya+1)*MAP_SIZE_X+xa-1]==i)
                        {
                            y[i]+=SPEED;
                            x[i]-=SPEED;
                        }
                    }
                    else
                    {
                        y[i]+=SPEED;
                        x[i]-=SPEED;
                    }
                }break;
                case WEST:
                {
                    if(x[i]%TILESIZE==0)
                    {
                        if(grid[(ya)*MAP_SIZE_X+xa-1]==i)
                        {
                            x[i]-=SPEED;
                        }
                    }
                    else
                    {
                        x[i]-=SPEED;
                    }
                }break;
                case NORTHWEST:
                {
                    if(y[i]%TILESIZE==0 && x[i]%TILESIZE==0)
                    {
                        if(grid[(ya-1)*MAP_SIZE_X+xa-1]==i)
                        {
                            y[i]-=SPEED;
                            x[i]-=SPEED;
                        }
                    }
                    else
                    {
                        y[i]-=SPEED;
                        x[i]-=SPEED;
                    }
                }break;
            }
        }
    }
}

void RTS_SetDirection(int* x, int* y, int* destX, int* destY, long* flags, char* direction)
{
    int i;
    for(i=0; i<MAX_TANKS; i++)
    {
        if((flags[i] & ALIVE) && x[i]%TILESIZE==0 && y[i]%TILESIZE==0)
        {
            if(x[i]<destX[i])
            {
                if(y[i]<destY[i])
                {
                    direction[i]=SOUTHEAST;
                }
                else if(y[i]>destY[i])
                {
                    direction[i]=NORTHEAST;
                }
                else
                {
                    direction[i]=EAST;
                }
            }
            else if(x[i]>destX[i])
            {
                if(y[i]<destY[i])
                {
                    direction[i]=SOUTHWEST;
                }
                else if(y[i]>destY[i])
                {
                    direction[i]=NORTHWEST;
                }
                else
                {
                    direction[i]=WEST;
                }
            }
            else
            {
                if(y[i]<destY[i])
                {
                    direction[i]=SOUTH;
                }
                else if(y[i]>destY[i])
                {
                    direction[i]=NORTH;
                }
                else
                {
                    direction[i]=0;
                }
            }
        }
    }
}

void RTS_SetTankAngle(double* angle, char* direction)
{
    int i;
    for(i=0; i<MAX_TANKS; i++)
    {
        if(direction[i]!=0)
        {
            angle[i]=(direction[i]-1)*45;
        }
    }

}
