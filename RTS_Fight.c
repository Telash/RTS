#include "RTS.h"

void FIGHT_DestroyProjectile(Projectile* projectile, int* buildingX, int* buildingY, int* turretX, int* turretY,
                             int* tankX, int* tankY, long* buildingFlags, long* turretFlags, long* tankFlags, char* grid,
                             int* buildingHealthRemain, int* turretHealthRemain, int* tankHealthRemain)
{
    int i;
    int x;
    int y;
    int tileObject=0;
    int tile;
    for(i=1; i<MAX_PROJECTILES; i++)
    {
        if(projectile->flags[i] & ALIVE)
        {
            if(projectile->x[i]<0 || projectile->y[i]<0 || projectile->x[i]>MAP_SIZE_X*TILESIZE ||
               projectile->y[i]>MAP_SIZE_Y*TILESIZE)
            {
                projectile->flags[i] ^= ALIVE;
            }
        }
        if(projectile->flags[i] & ALIVE)
        {
            x=projectile->x[i]/TILESIZE;
            y=projectile->y[i]/TILESIZE;
            tile=y*MAP_SIZE_X+x;
            tileObject=grid[tile];
            if(projectile->rangeLeft[i]<=0)
            {
                projectile->flags[i] ^= ALIVE;
            }
            if(tileObject<0)
            {
                if(buildingX[tileObject*-1]==x*TILESIZE && buildingY[tileObject*-1]==y*TILESIZE &&
                   ((buildingFlags[tileObject*-1] & ALLEGIANCE) != (projectile->flags[i] & ALLEGIANCE)))
                {
                    projectile->flags[i] ^= ALIVE;
                    buildingHealthRemain[tileObject*-1]-=PROJECTILE_DAMAGE;
                }
                else if((turretFlags[tileObject*-1] & ALLEGIANCE) != (projectile->flags[i] & ALLEGIANCE) &&
                        projectile->x[i]%TILESIZE>16 && projectile->x[i]%TILESIZE<48 &&
                        projectile->y[i]%TILESIZE>16 && projectile->y[i]%TILESIZE<48)
                {
                    projectile->flags[i] ^= ALIVE;
                    turretHealthRemain[tileObject*-1]-=PROJECTILE_DAMAGE;
                }
            }
            /*CHECK IF HIT TANK*/
            else if(tileObject>0)
            {
                if((tankFlags[tileObject] & ALLEGIANCE) != (projectile->flags[i] & ALLEGIANCE) &&
                   projectile->x[i]<tankX[tileObject]+50 && projectile->x[i]>tankX[tileObject]+10 &&
                   projectile->y[i]<tankY[tileObject]+50 && projectile->y[i]>tankY[tileObject]+10)
                {
                    projectile->flags[i] ^= ALIVE;
                    tankHealthRemain[tileObject]-=PROJECTILE_DAMAGE;
                }
            }
            /*END CHECK IF HIT TANK*/
        }
    }
}

void FIGHT_HandleDeathsTank(int* healthRemain, long* flags, int* target, int* currentTile, char* grid)
{
    int i;
    for(i=0; i<MAX_TANKS; i++)
    {
        if((flags[i] & ALIVE) && healthRemain[i]<=0)
        {
            target[i] = 0;
            flags[i] = 0;
            grid[currentTile[i]] = 0;
            currentTile[i] = 0;
        }
    }
}

void FIGHT_HandleDeathsBuilding(int* healthRemain, long* flags, char* grid, int* x, int* y)
{
    int i;
    for(i=0; i<MAX_BUILDINGS; i++)
    {
        if((flags[i] & ALIVE) && healthRemain[i]<=0)
        {
            flags[i] = 0;
            grid[y[i]/TILESIZE*MAP_SIZE_X+x[i]/TILESIZE]=0;
        }
    }
}

void FIGHT_HandleDeathsTurret(int* healthRemain, long* flags, int* target, char* grid, int* x, int* y)
{
    int i;
    for(i=0; i<MAX_TURRETS; i++)
    {
        if((flags[i] & ALIVE) && healthRemain[i]<=0)
        {
            target[i] = 0;
            flags[i] = 0;
            grid[y[i]/TILESIZE*MAP_SIZE_X+x[i]/TILESIZE]=0;
        }
    }
}

void FIGHT_HandleAttackByTank(int* tankHealthRemain, int* target, int* turretHealthRemain, int* buildingHealthRemain,
                            unsigned char* targetType, int gameTime, char* initiative, short* type, Projectile* projectile,
                            int* tankX, int* tankY, double* tankTurretAngle, int* buildingX, int* buildingY,
                            int* turretX, int* turretY, char* direction, long* tankFlags)
{
    int i;
    for(i=0; i<MAX_TANKS; i++)
    {
        if(target[i]>0)
        {
            if(type[i]==TRUCK && gameTime%TRUCK_ATTACK_SPEED==initiative[i])
            {
                if(targetType[i]==UNIT)
                {
                    tankHealthRemain[target[i]]-=TRUCK_ATTACK_DAMAGE;
                }
                else if(targetType[i]==DEFENCE_TOWER)
                {
                    turretHealthRemain[target[i]]-=TRUCK_ATTACK_DAMAGE;
                }
                else if(targetType[i]==BUILDING)
                {
                    buildingHealthRemain[target[i]]-=TRUCK_ATTACK_DAMAGE;
                }
            }
            else if(type[i]==TANK && gameTime%TANK_ATTACK_SPEED==initiative[i])
            {
                switch(targetType[i])
                {
                    case UNIT:
                    {
                        CREATE_CreateProjectile(projectile, tankX[i], tankY[i], tankTurretAngle[i],
                                                tankX[target[i]], tankY[target[i]], tankFlags[i]);
                    }break;
                    case BUILDING:
                    {
                        CREATE_CreateProjectile(projectile, tankX[i], tankY[i], tankTurretAngle[i],
                                                buildingX[target[i]], buildingY[target[i]], tankFlags[i]);
                    }break;
                    case DEFENCE_TOWER:
                    {
                        CREATE_CreateProjectile(projectile, tankX[i], tankY[i], tankTurretAngle[i],
                                                turretX[target[i]], turretY[target[i]], tankFlags[i]);
                    }break;
                }
            }
        }
    }
}

void FIGHT_HandleDamageByTurret(int* tankHealthRemain, int* target, int* turretHealthRemain, int* buildingHealthRemain,
                            unsigned char* targetType, char* initiative, int gameTime, double* angle, double* angleGoal)
{
    int i;
    for(i=0; i<MAX_TURRETS; i++)
    {
        if(target[i]>0 && angleGoal[i]==angle[i] && gameTime%TURRET_ATTACK_SPEED==initiative[i])
        {
            if(targetType[i]==UNIT)
            {
                tankHealthRemain[target[i]]-=TURRET_ATTACK_DAMAGE;
            }
            else if(targetType[i]==DEFENCE_TOWER)
            {
                turretHealthRemain[target[i]]-=TURRET_ATTACK_DAMAGE;
            }
            else if(targetType[i]==BUILDING)
            {
                buildingHealthRemain[target[i]]-=TURRET_ATTACK_DAMAGE;
            }
        }
    }
}

void FIGHT_HandleTargetTank(int* target, int* tankX, int* tankY, long* tankFlags, unsigned char* targetType, int* buildingX,
                          int* buildingY, int* turretX, int* turretY, long* buildingFlags, long* turretFlags,
                          int* targetGoal, unsigned char* targetGoalType, int* destX, int* destY, char* direction,
                          short* type)
{
    int range;
    int i;
    int j;
    int x;
    int y;
    int flags;
    for(i=0; i<MAX_TANKS; i++)
    {
        if(tankFlags[i] & ALIVE)
        {
            if(type[i]==TRUCK)
            {
                range=TRUCK_RANGE;
            }
            else
            {
                range=TANK_RANGE;
            }
            if(targetGoal[i])
            {
                if(targetGoalType[i]==UNIT)
                {
                    x=tankX[targetGoal[i]];
                    y=tankY[targetGoal[i]];
                }
                else if(targetGoalType[i]==BUILDING)
                {
                    x=buildingX[targetGoal[i]];
                    y=buildingX[targetGoal[i]];
                }
                else
                {
                    x=turretX[targetGoal[i]];
                    y=turretY[targetGoal[i]];
                }
                if(x<=tankX[i]+range && x>=tankX[i]-range &&
                   y<=tankY[i]+range && y>=tankY[i]-range)
                {
                    target[i]=targetGoal[i];
                    targetType[i]=targetGoalType[i];
                    targetGoal[i]=0;
                    if(tankX[i]%TILESIZE==0 && tankY[i]%TILESIZE==0)
                    {
                        destX[i]=tankX[i];
                        destY[i]=tankY[i];
                    }
                    else
                    {
                        switch(direction[i])
                        {
                            case NORTH:
                            {
                                destX[i]=tankX[i]/TILESIZE*TILESIZE;
                                destY[i]=tankY[i]/TILESIZE*TILESIZE;
                            }break;
                            case NORTHEAST:
                            {
                                destX[i]=tankX[i]/TILESIZE*TILESIZE+TILESIZE;
                                destY[i]=tankY[i]/TILESIZE*TILESIZE;
                            }break;
                            case EAST:
                            {
                                destX[i]=tankX[i]/TILESIZE*TILESIZE+TILESIZE;
                                destY[i]=tankY[i]/TILESIZE*TILESIZE;
                            }break;
                            case SOUTHEAST:
                            {
                                destX[i]=tankX[i]/TILESIZE*TILESIZE+TILESIZE;
                                destY[i]=tankY[i]/TILESIZE*TILESIZE+TILESIZE;
                            }break;
                            case SOUTH:
                            {
                                destX[i]=tankX[i]/TILESIZE*TILESIZE;
                                destY[i]=tankY[i]/TILESIZE*TILESIZE+TILESIZE;
                            }break;
                            case SOUTHWEST:
                            {
                                destX[i]=tankX[i]/TILESIZE*TILESIZE;
                                destY[i]=tankY[i]/TILESIZE*TILESIZE+TILESIZE;
                            }break;
                            case WEST:
                            {
                                destX[i]=tankX[i]/TILESIZE*TILESIZE;
                                destY[i]=tankY[i]/TILESIZE*TILESIZE;
                            }break;
                            case NORTHWEST:
                            {
                                destX[i]=tankX[i]/TILESIZE*TILESIZE;
                                destY[i]=tankY[i]/TILESIZE*TILESIZE;
                            }break;
                        }
                    }
                }
            }
            if(target[i])
            {
                if(targetType[i]==UNIT)
                {
                    x=tankX[target[i]];
                    y=tankY[target[i]];
                    flags=tankFlags[target[i]];
                }
                else if(targetType[i]==DEFENCE_TOWER)
                {
                    x=turretX[target[i]];
                    y=turretY[target[i]];
                    flags=turretFlags[target[i]];
                }
                else if(targetType[i]==BUILDING)
                {
                    x=buildingX[target[i]];
                    y=buildingY[target[i]];
                    flags=buildingFlags[target[i]];
                }
                if((tankX[i]>x+range || tankX[i]<x-range || tankY[i]>y+range
                    || tankY[i]<y-range) || !(flags & ALIVE))
                {
                    target[i]=0;
                }
            }
            if(target[i]==0)
            {
                for(j=0; j<MAX_TANKS; j++)
                {
                    if(tankFlags[j] & ALIVE && (tankFlags[j] & ALLEGIANCE) != (tankFlags[i] & ALLEGIANCE))
                    {
                        if(tankX[i]<=tankX[j]+range && tankX[i]>=tankX[j]-range && tankY[i]<=tankY[j]+range &&
                           tankY[i]>=tankY[j]-range)
                        {
                            target[i]=j;
                            targetType[i]=UNIT;
                            j=MAX_TANKS;
                        }
                    }
                }
            }
            if(target[i]==0)
            {
                for(j=0; j<MAX_TURRETS; j++)
                {
                    if(turretFlags[j] & ALIVE && (turretFlags[j] & ALLEGIANCE) != (tankFlags[i] & ALLEGIANCE))
                    {
                        if(tankX[i]<=turretX[j]+range && tankX[i]>=turretX[j]-range &&
                           tankY[i]<=turretY[j]+range && tankY[i]>=turretY[j]-range)
                        {
                            target[i]=j;
                            targetType[i]=DEFENCE_TOWER;
                            j=MAX_TURRETS;
                        }
                    }
                }
            }
            if(target[i]==0)
            {
                for(j=0; j<MAX_BUILDINGS; j++)
                {
                    if(buildingFlags[j] & ALIVE && (buildingFlags[j] & ALLEGIANCE) != (tankFlags[i] & ALLEGIANCE))
                    {
                        if(tankX[i]<=buildingX[j]+range && tankX[i]>=buildingX[j]-range &&
                           tankY[i]<=buildingY[j]+range && tankY[i]>=buildingY[j]-range)
                        {
                            target[i]=j;
                            targetType[i]=BUILDING;
                            j=MAX_BUILDINGS;
                        }
                    }
                }
            }
            /*COMPUTER FIND NEW TARGET OUT OF RANGE*/
            if(target[i]==0 && !(tankFlags[i] & ALLEGIANCE) && targetGoal[i]==0 &&
               tankX[i]==destX[i] && tankY[i]==destY[i])
            {
                for(j=0; j<MAX_TANKS; j++)
                {
                    if((tankFlags[j] & ALLEGIANCE) != (tankFlags[i] & ALLEGIANCE) &&
                       tankFlags[j] & ALIVE)
                    {
                        targetGoal[i]=j;
                        targetGoalType[i]=UNIT;
                        destX[i]=tankX[j];
                        destY[i]=tankY[j];
                        j=MAX_TANKS;
                    }
                }
                if(targetGoal[i]==0)
                {
                    for(j=0; j<MAX_TURRETS; j++)
                    {
                        if((turretFlags[j] & ALLEGIANCE) != (tankFlags[i] & ALLEGIANCE) &&
                            turretFlags[j] & ALIVE)
                        {
                            targetGoal[i]=j;
                            targetGoalType[i]=DEFENCE_TOWER;
                            destX[i]=turretX[j];
                            destY[i]=turretY[j];
                            j=MAX_TURRETS;
                        }
                    }
                }
                if(targetGoal[i]==0)
                {
                    for(j=0; j<MAX_BUILDINGS; j++)
                    {
                        if((buildingFlags[j] & ALLEGIANCE) != (tankFlags[i] & ALLEGIANCE) &&
                            buildingFlags[j] & ALIVE)
                        {
                            targetGoal[i]=j;
                            targetGoalType[i]=BUILDING;
                            destX[i]=buildingX[j];
                            destY[i]=buildingY[j];
                            j=MAX_BUILDINGS;
                        }
                    }
                }
            }
            if(targetGoal[i] && (!(tankFlags[i] & ALLEGIANCE)) &&
               tankX[i]==destX[i] && tankY[i]==destY[i])
            {
                targetGoal[i]=0;
            }
            /*END COMPUTER FIND NEW TARGET OUT OF RANGE*/
        }
    }
}

void FIGHT_HandleTargetTurret(int* target, int* tankX, int* tankY, long* tankFlags, unsigned char* targetType, int* buildingX,
                          int* buildingY, int* turretX, int* turretY, long* buildingFlags, long* turretFlags, int* targetGoal,
                          unsigned char* targetGoalType)
{
    int i;
    int j;
    int x;
    int y;
    int flags;
    for(i=0; i<MAX_TURRETS; i++)
    {
        if((turretFlags[i] & ALIVE) && (!(turretFlags[i] & BEING_CONSTRUCTED)))
        {
            if(targetGoal[i])
            {
                if(targetGoalType[i]==UNIT)
                {
                    x=tankX[targetGoal[i]];
                    y=tankY[targetGoal[i]];
                }
                else if(targetGoalType[i]==BUILDING)
                {
                    x=buildingX[targetGoal[i]];
                    y=buildingX[targetGoal[i]];
                }
                else
                {
                    x=turretX[targetGoal[i]];
                    y=turretY[targetGoal[i]];
                }
                if(x<=tankX[i]+TANK_RANGE && x>=tankX[i]-TANK_RANGE &&
                   y<=tankY[i]+TANK_RANGE && y>=tankY[i]-TANK_RANGE)
                {
                    target[i]=targetGoal[i];
                    targetType[i]=targetGoalType[i];
                    targetGoal[i]=0;
                }
            }
            if(target[i])
            {
                if(targetType[i]==UNIT)
                {
                    x=tankX[target[i]];
                    y=tankY[target[i]];
                    flags=tankFlags[target[i]];
                }
                else if(targetType[i]==TURRET)
                {
                    x=turretX[target[i]];
                    y=turretY[target[i]];
                    flags=turretFlags[target[i]];
                }
                else if(targetType[i]==BUILDING)
                {
                    x=buildingX[target[i]];
                    y=buildingY[target[i]];
                    flags=buildingFlags[target[i]];
                }
                if((turretX[i]>x+TURRET_RANGE || turretX[i]<x-TURRET_RANGE || turretY[i]>y+TURRET_RANGE
                    || turretY[i]<y-TURRET_RANGE) || !(flags & ALIVE))
                {
                    target[i]=0;
                }
            }
            if(target[i]==0)
            {
                for(j=0; j<MAX_TANKS; j++)
                {
                    if(tankFlags[j] & ALIVE && (tankFlags[j] & ALLEGIANCE) != (turretFlags[i] & ALLEGIANCE))
                    {
                        if(turretX[i]<=tankX[j]+TURRET_RANGE && turretX[i]>=tankX[j]-TURRET_RANGE
                           && turretY[i]<=tankY[j]+TURRET_RANGE && turretY[i]>=tankY[j]-TURRET_RANGE)
                        {
                            target[i]=j;
                            targetType[i]=UNIT;
                            j=MAX_TANKS;
                        }
                    }
                }
            }
            if(target[i]==0)
            {
                for(j=0; j<MAX_TURRETS; j++)
                {
                    if(turretFlags[j] & ALIVE && (turretFlags[j] & ALLEGIANCE) != (turretFlags[i] & ALLEGIANCE))
                    {
                        if(turretX[i]<=turretX[j]+TURRET_RANGE && turretX[i]>=turretX[j]-TURRET_RANGE
                           && turretY[i]<=turretY[j]+TURRET_RANGE && turretY[i]>=turretY[j]-TURRET_RANGE)
                        {
                            target[i]=j;
                            targetType[i]=DEFENCE_TOWER;
                            j=MAX_TURRETS;
                        }
                    }
                }
            }
            if(target[i]==0)
            {
                for(j=0; j<MAX_BUILDINGS; j++)
                {
                    if(buildingFlags[j] & ALIVE && (buildingFlags[j] & ALLEGIANCE) != (turretFlags[i] & ALLEGIANCE))
                    {
                        if(turretX[i]<=buildingX[j]+TURRET_RANGE && turretX[i]>=buildingX[j]-TURRET_RANGE &&
                           turretY[i]<=buildingY[j]+TURRET_RANGE && turretY[i]>=buildingY[j]-TURRET_RANGE)
                        {
                            target[i]=j;
                            targetType[i]=BUILDING;
                            j=MAX_BUILDINGS;
                        }
                    }
                }
            }

        }
    }
}

/*SETS TARGET GOAL FOR PLAYER*/
void FIGHT_SetTargetGoal(char* grid, int* tankX, int* tankY, int* turretX, int* turretY, int* buildingX, int* buildingY,
                       long* tankFlags, long* turretFlags, long* buildingFlags, int* turretTarget, int* tankTarget,
                       int* tankTargetGoal, int* turretTargetGoal, unsigned char* tankTargetGoalType,
                       unsigned char* turretTargetGoalType)
{
    int i;
    int mX;
    int mY;
    SDL_GetMouseState(&mX, &mY);
    mX=mX/TILESIZE;
    mY=mY/TILESIZE;
    for(i=1; i<MAX_TANKS; i++)
    {
        if(tankFlags[i] & SELECTED)
        {
            if(grid[mY*MAP_SIZE_X+mX]>0 && (!(tankFlags[grid[mY*MAP_SIZE_X+mX]] & ALLEGIANCE)))
            {
                tankTargetGoal[i]=grid[mY*MAP_SIZE_X+mX];
                tankTargetGoalType[i]=UNIT;
            }
            else if(grid[mY*MAP_SIZE_X+mX]<0)
            {
                if(turretX[grid[mY*MAP_SIZE_X+mX]*-1]==mX*TILESIZE &&
                   turretY[grid[mY*MAP_SIZE_X+mX]*-1]==mY*TILESIZE)
                {
                    if(!(turretFlags[grid[mY*MAP_SIZE_X+mX]] & ALLEGIANCE))
                    {
                        tankTargetGoalType[i]=DEFENCE_TOWER;
                        tankTargetGoal[i]=grid[mY*MAP_SIZE_X+mX]*(-1);
                    }
                }
                else
                {
                    if(!(buildingFlags[grid[mY*MAP_SIZE_X+mX]] & ALLEGIANCE))
                    {
                        tankTargetGoalType[i]=BUILDING;
                        tankTargetGoal[i]=grid[mY*MAP_SIZE_X+mX]*(-1);
                    }
                }
            }
        }
    }
    for(i=1; i<MAX_TURRETS; i++)
    {
        if(turretFlags[i] & SELECTED)
        {
            if(grid[mY*MAP_SIZE_X+mX]>0 && (!(tankFlags[grid[mY*MAP_SIZE_X+mX]] & ALLEGIANCE)))
            {
                turretTargetGoal[i]=grid[mY*MAP_SIZE_X+mX];
                turretTargetGoalType[i]=UNIT;
            }
            else if(grid[mY*MAP_SIZE_X+mX]<0)
            {
                if(turretX[tankTargetGoal[i]]==mX*TILESIZE && turretY[tankTargetGoal[i]]==mY*TILESIZE)
                {
                    if(!(turretFlags[grid[mY*MAP_SIZE_X+mX]] & ALLEGIANCE))
                    {
                        turretTargetGoal[i]=grid[mY*MAP_SIZE_X+mX]*(-1);
                        turretTargetGoalType[i]=DEFENCE_TOWER;
                    }
                }
                else
                {
                    if(!(buildingFlags[grid[mY*MAP_SIZE_X+mX]] & ALLEGIANCE))
                    {
                        turretTargetGoal[i]=grid[mY*MAP_SIZE_X+mX]*(-1);
                        turretTargetGoalType[i]=BUILDING;
                    }
                }
            }
        }
    }
}

void FIGHT_SetTurretAngleForTank(double* turretAngle, double* turretAngleGoal, long* target, char* direction)
{
    int i;
    for(i=1; i<MAX_TANKS; i++)
    {
        if(target[i]==0 && direction[i]!=0)
        {
            turretAngle[i]=turretAngleGoal[i];
        }
        if(turretAngle[i]!=turretAngleGoal[i])
        {
            if(turretAngleGoal[i]<=180)
            {
                if(turretAngle[i]<turretAngleGoal[i] || turretAngle[i]>turretAngleGoal[i]+180)
                {
                    turretAngle[i]+=TRUCK_GUN_TURN_SPEED;
                    if(turretAngle[i]>turretAngleGoal[i] && turretAngle[i]<turretAngleGoal[i]+TRUCK_GUN_TURN_SPEED)
                    {
                        turretAngle[i]=turretAngleGoal[i];
                    }
                }
                else
                {
                    turretAngle[i]-=TRUCK_GUN_TURN_SPEED;
                    if(turretAngle[i]<turretAngleGoal[i] && turretAngle[i]>turretAngleGoal[i]-TRUCK_GUN_TURN_SPEED)
                    {
                        turretAngle[i]=turretAngleGoal[i];
                    }
                }
            }
            else
            {
                if(turretAngle[i]>turretAngleGoal[i] || turretAngle[i]<turretAngleGoal[i]-180)
                {
                    turretAngle[i]-=TRUCK_GUN_TURN_SPEED;
                    if(turretAngle[i]<turretAngleGoal[i] && turretAngle[i]>turretAngleGoal[i]-TRUCK_GUN_TURN_SPEED)
                    {
                        turretAngle[i]=turretAngleGoal[i];
                    }
                }
                else
                {
                    turretAngle[i]+=TRUCK_GUN_TURN_SPEED;
                    if(turretAngle[i]>turretAngleGoal[i] && turretAngle[i]<turretAngleGoal[i]+TRUCK_GUN_TURN_SPEED)
                    {
                        turretAngle[i]=turretAngleGoal[i];
                    }
                }
            }
            if(turretAngle[i]<0)
            {
                turretAngle[i]+=360;
            }
            else if(turretAngle[i]>=360)
            {
                turretAngle[i]-=360;
            }
        }
    }
}

void FIGHT_SetTurretAngleGoalForTank(double* turretAngle, int* tankX, int* tankY, int* target, double* tankAngle, long* flags,
                        int* buildingX, int* buildingY, int* turretX, int* turretY, unsigned char* targetType)
{
    int i;
    double diffX;
    double diffY;
    int x;
    int y;
    for(i=0; i<MAX_TANKS; i++)
    {
        if(flags[i] & ALIVE)
        {
            if(target[i]==0)
            {
                turretAngle[i]=tankAngle[i];
            }
            else
            {
                if(targetType[i]==UNIT)
                {
                    x=tankX[target[i]];
                    y=tankY[target[i]];
                }
                else if(targetType[i]==DEFENCE_TOWER)
                {
                    x=turretX[target[i]];
                    y=turretY[target[i]];
                }
                else if(targetType[i]==BUILDING)
                {
                    x=buildingX[target[i]];
                    y=buildingY[target[i]];
                }
                if(tankX[i]<x)
                {
                    diffX=x-tankX[i];
                    if(tankY[i]<y)
                    {
                        diffY=y-tankY[i];
                        turretAngle[i]=atan(diffY/diffX)*180/M_PI+90;
                    }
                    else if(tankY[i]>y)
                    {
                        diffY=tankY[i]-y;
                        turretAngle[i]=atan(diffX/diffY)*180/M_PI;
                    }
                    else
                    {
                        turretAngle[i]=90;
                    }
                }
                else if(tankX[i]>x)
                {
                    diffX=tankX[i]-x;
                    if(tankY[i]<y)
                    {
                        diffY=y-tankY[i];
                        turretAngle[i]=atan(diffX/diffY)*180/M_PI+180;
                    }
                    else if(tankY[i]>y)
                    {
                        diffY=tankY[i]-y;
                        turretAngle[i]=atan(diffY/diffX)*180/M_PI+270;
                    }
                    else
                    {
                        turretAngle[i]=270;
                    }
                }
                else
                {
                    if(tankY[i]<y)
                    {
                        turretAngle[i]=180;
                    }
                    else if(tankY[i]>y)
                    {
                        turretAngle[i]=0;
                    }
                }
            }
        }
    }
}

void FIGHT_SetTurretAngleForTurret(double* angle, double* goal)
{
    int i;
    for(i=1; i<MAX_TURRETS; i++)
    {
        if(angle[i]!=goal[i])
        {
            if(angle[i]<180)
            {
                if(goal[i]<angle[i] || goal[i]>angle[i]+180)
                {
                    angle[i]-=TURRET_TURN_SPEED;
                    if(angle[i]<goal[i] && angle[i]>goal[i]-TURRET_TURN_SPEED)
                    {
                        angle[i]=goal[i];
                    }
                }
                else
                {
                    angle[i]+=TURRET_TURN_SPEED;
                    if(angle[i]>goal[i] && angle[i]<goal[i]+TURRET_TURN_SPEED)
                    {
                        angle[i]=goal[i];
                    }
                }
            }
            else
            {
                if(goal[i]>angle[i] || goal[i]<angle[i]-180)
                {
                    angle[i]+=TURRET_TURN_SPEED;
                    if(angle[i]>goal[i] && angle[i]<goal[i]+TURRET_TURN_SPEED)
                    {
                        angle[i]=goal[i];
                    }

                }
                else
                {
                    angle[i]-=TURRET_TURN_SPEED;
                    if(angle[i]<goal[i] && angle[i]>goal[i]-TURRET_TURN_SPEED)
                    {
                        angle[i]=goal[i];
                    }
                }
            }
            if(angle[i]>=360)
            {
                angle[i]-=360;
            }
            else if(angle[i]<0)
            {
                angle[i]+=360;
            }
        }
    }
}

void FIGHT_SetTurretAngleGoalForTurret(double* turretAngle, int* tankX, int* tankY, int* target, long* flags,
                        int* buildingX, int* buildingY, int* turretX, int* turretY, unsigned char* targetType)
{
    int i;
    double diffX;
    double diffY;
    int x;
    int y;
    for(i=0; i<MAX_TURRETS; i++)
    {
        if(flags[i] & ALIVE)
        {
            if(target[i]==0)
            {
                turretAngle[i]=0;
            }
            else
            {
                if(targetType[i]==UNIT)
                {
                    x=tankX[target[i]];
                    y=tankY[target[i]];
                }
                else if(targetType[i]==DEFENCE_TOWER)
                {
                    x=turretX[target[i]];
                    y=turretY[target[i]];
                }
                else if(targetType[i]==BUILDING)
                {
                    x=buildingX[target[i]];
                    y=buildingY[target[i]];
                }
                if(turretX[i]<x)
                {
                    diffX=x-turretX[i];
                    if(turretY[i]<y)
                    {
                        diffY=y-turretY[i];
                        turretAngle[i]=atan(diffY/diffX)*180/M_PI+90;
                    }
                    else if(turretY[i]>y)
                    {
                        diffY=turretY[i]-y;
                        turretAngle[i]=atan(diffX/diffY)*180/M_PI;
                    }
                    else
                    {
                        turretAngle[i]=90;
                    }
                }
                else if(turretX[i]>x)
                {
                    diffX=turretX[i]-x;
                    if(turretY[i]<y)
                    {
                        diffY=y-turretY[i];
                        turretAngle[i]=atan(diffX/diffY)*180/M_PI+180;
                    }
                    else if(turretY[i]>y)
                    {
                        diffY=turretY[i]-y;
                        turretAngle[i]=atan(diffY/diffX)*180/M_PI+270;
                    }
                    else
                    {
                        turretAngle[i]=270;
                    }
                }
                else
                {
                    if(turretY[i]<y)
                    {
                        turretAngle[i]=180;
                    }
                    else if(turretY[i]>y)
                    {
                        turretAngle[i]=0;
                    }
                }
            }
        }
    }
}
