#include "RTS.h"

Building CREATE_CreateBuilding(Building building, SDL_Point point, int allegiance, int type, int* maxHealth,
                            int producingBuilding, char* grid)
{
    int i=0;
    static int nextB=1;
    while(i<MAX_BUILDINGS && building.flags[nextB] & ALIVE)
    {
        nextB++;
        if(nextB>=MAX_BUILDINGS)
        {
            nextB=1;
        }
        i++;
    }
    if(i<MAX_BUILDINGS)
    {
        building.flags[nextB]=0;
        building.x[nextB]=point.x;
        building.y[nextB]=point.y;
        building.type[nextB]=type;

        building.healthRemain[nextB]=maxHealth[type];
        building.flags[nextB] |= ALIVE;
        building.flags[nextB] |= (allegiance<<2);

        if(producingBuilding!=0)
        {
            building.flags[nextB] |= BEING_CONSTRUCTED;
            building.flags[producingBuilding] |= PRODUCING;
            switch(type)
            {
                case HANGAR:
                {
                    building.constructingTime[nextB] = HANGAR_COST;
                    building.constructingTime[producingBuilding] = HANGAR_COST;
                }break;
                case POWER_PLANT:
                {
                    building.constructingTime[nextB] = 300;
                    building.constructingTime[producingBuilding] = 300;
                }break;
                case CONSTRUCTION_YARD:
                {
                    building.constructingTime[nextB] = 2000;
                    building.constructingTime[producingBuilding] = 2000;
                }break;
            }
        }

        grid[(building.y[nextB]/TILESIZE)*MAP_SIZE_X+building.x[nextB]/TILESIZE]=nextB*(-1);
        nextB++;
        if(nextB>=MAX_BUILDINGS)
        {
            nextB=1;
        }
    }
    return building;
}

Tank CREATE_CreateTank(Tank tank, int x, int y, int allegiance, int type, int* maxHealth)
{
    int i=0;
    static int next=1;
    while(i<MAX_TANKS && tank.flags[next] & ALIVE)
    {
        next++;
        if(next>=MAX_TANKS)
        {
            next=1;
        }
        i++;
    }
    tank.flags[next]=0;
    tank.type[next]=type;
    tank.x[next]=x;
    tank.y[next]=y;
    tank.destX[next]=x;
    tank.destY[next]=y+64;
    tank.angle[next]=0;
    tank.healthRemain[next]=maxHealth[type];
    tank.flags[next] |= ALIVE;
    tank.flags[next] |= (allegiance<<2);
    if(type==TANK)
    {
        tank.initiative[next]=rand()%TANK_ATTACK_SPEED;
    }
    else
    {
        tank.initiative[next]=rand()%TRUCK_ATTACK_SPEED;
    }
    next++;
    return tank;
}

void CREATE_CreateProjectile(Projectile* projectile, int X, int Y, double angle, int targetX, int targetY, long tankFlags)
{
    double diffX;
    double diffY;
    static int nextP=1;
    int loop=1;
    while((projectile->flags[nextP] & ALIVE) && loop<MAX_PROJECTILES)
    {
        nextP++;
        if(nextP>=MAX_PROJECTILES)
        {
            nextP=1;
        }
        loop++;
    }
    projectile->flags[nextP] = 0;
    projectile->x[nextP]=X+TILESIZE/2-3;
    projectile->y[nextP]=Y+TILESIZE/2-3;
    projectile->flags[nextP] |= ALIVE;
    projectile->flags[nextP] |= (tankFlags & ALLEGIANCE);
    projectile->angle[nextP]=angle;
    projectile->rangeLeft[nextP]=TANK_RANGE;

    if(X<targetX)
    {
        if(Y<targetY)
        {
            diffX=targetX-X;
            diffY=targetY-Y;
            projectile->moveMultiplier[nextP]=diffX/diffY;
        }
        else if(targetY<Y)
        {
            diffX=targetX-X;
            diffY=Y-targetY;
            projectile->moveMultiplier[nextP]=diffX/diffY;
        }
        else
        {
            projectile->moveMultiplier[nextP]=0;
        }
    }
    else if(targetX<X)
    {
        if(Y<targetY)
        {
            diffX=X-targetX;
            diffY=targetY-Y;
            projectile->moveMultiplier[nextP]=diffX/diffY;
        }
        else if(targetY<Y)
        {
            diffX=X-targetX;
            diffY=Y-targetY;
            projectile->moveMultiplier[nextP]=diffX/diffY;
        }
        else
        {
            projectile->moveMultiplier[nextP]=0;
        }
    }
    else
    {
        projectile->moveMultiplier[nextP]=0;
    }
    nextP++;
    if(nextP>=MAX_PROJECTILES)
    {
        nextP=1;
    }
}

Turret CREATE_CreateTurret(Turret turret, SDL_Point point, int allegiance, int* maxHealth, int selectedBuilding,
                         short* constructingTime, long* buildingFlags, char* grid)
{
    int i=0;
    static int nextT=1;
    while(i<MAX_TURRETS && (turret.flags[nextT] & ALIVE))
    {
        nextT++;
        if(nextT>=MAX_TURRETS)
        {
            nextT=1;
        }
        i++;
    }
    turret.flags[nextT]=0;
    turret.x[nextT]=point.x;
    turret.y[nextT]=point.y;

    if(selectedBuilding!=0)
    {
        turret.flags[nextT] |= BEING_CONSTRUCTED;
        buildingFlags[selectedBuilding] |= PRODUCING;
        turret.constructingTime[nextT] = TURRET_COST;
        constructingTime[selectedBuilding] = TURRET_COST;
    }

    turret.healthRemain[nextT]=maxHealth[TURRET];
    turret.flags[nextT] |= ALIVE;
    turret.flags[nextT] |= (allegiance<<2);
    turret.initiative[nextT] = rand()%TURRET_ATTACK_SPEED;

    grid[turret.y[nextT]/TILESIZE*MAP_SIZE_X+turret.x[nextT]/TILESIZE]=nextT*(-1);

    nextT++;
    return turret;
}

void CREATE_FillBuildQuePoints(SDL_Point* point, SDL_Point center)
{
    point[NORTH].x=center.x;
    point[NORTH].y=center.y-TILESIZE;
    point[NORTHEAST].x=center.x+TILESIZE;
    point[NORTHEAST].y=center.y-TILESIZE;
    point[EAST].x=center.x+TILESIZE;
    point[EAST].y=center.y;
    point[SOUTHEAST].x=center.x+TILESIZE;
    point[SOUTHEAST].y=center.y+TILESIZE;
    point[SOUTH].x=center.x;
    point[SOUTH].y=center.y+TILESIZE;
    point[SOUTHWEST].x=center.x-TILESIZE;
    point[SOUTHWEST].y=center.y+TILESIZE;
    point[WEST].x=center.x-TILESIZE;
    point[WEST].y=center.y;
    point[NORTHWEST].x=center.x-TILESIZE;
    point[NORTHWEST].y=center.y-TILESIZE;
}

void CREATE_HandleConstructions(long* buildingFlags, short* buildingConstructingTime, int* credits,
                             long* turretFlags, short* turretConstructingTime, unsigned char* type,
                             int* continousCreditIncome, unsigned char* constructingType, short* animation)
{
    int i;
    for(i=0; i<MAX_TURRETS; i++)
    {
        /*TURRET PRODUCTION*/
        if(turretFlags[i] & BEING_CONSTRUCTED)
        {
            if(turretFlags[i] & ALLEGIANCE)
            {
                if(credits[PLAYER]>0)
                {
                    credits[PLAYER]-=1;
                    turretConstructingTime[i]-=1;
                    if(turretConstructingTime[i]==0)
                    {
                        turretFlags[i] ^= BEING_CONSTRUCTED;
                    }
                }
            }
            else
            {
                if(credits[COMPUTER]>0)
                {
                    credits[COMPUTER]-=1;
                    turretConstructingTime[i]-=1;
                    if(turretConstructingTime[i]==0)
                    {
                        turretFlags[i] ^= BEING_CONSTRUCTED;
                    }
                }
            }
        }
        /*END TURRET PRODUTION*/
    }
    for(i=0; i<MAX_BUILDINGS; i++)
    {
        /*COMPUTER START PRODUCING UNIT*/
        if(type[i]==HANGAR && (!(buildingFlags[i] & ALLEGIANCE)) && (!(buildingFlags[i] & BEING_CONSTRUCTED)) &&
           (!(buildingFlags[i] & PRODUCING)) && animation[i]==0)
        {
            buildingFlags[i] |= PRODUCING;
            if(rand()%2==0)
            {
                constructingType[i]=TANK;
                buildingConstructingTime[i]=TANK_COST;
            }
            else
            {
                constructingType[i]=TRUCK;
                buildingConstructingTime[i]=TRUCK_COST;
            }
        }
        /*END COMPUTER START PRODUCING UNIT*/


        if(buildingFlags[i] & BEING_CONSTRUCTED)
        {
            if(buildingFlags[i] & ALLEGIANCE)
            {
                if(credits[PLAYER]>0)
                {
                    credits[PLAYER]-=1;
                    buildingConstructingTime[i]-=1;
                    if(buildingConstructingTime[i]==0)
                    {
                        buildingFlags[i] ^= BEING_CONSTRUCTED;
                        if(type[i]!=HANGAR)
                        {
                            continousCreditIncome[PLAYER] +=5;
                        }
                    }
                }
            }
            else
            {
                if(credits[COMPUTER]>0)
                {
                    credits[COMPUTER]-=1;
                    buildingConstructingTime[i]-=1;
                    if(buildingConstructingTime[i]==0)
                    {
                        buildingFlags[i] ^= BEING_CONSTRUCTED;
                        if(type[i]!=HANGAR)
                        {
                            continousCreditIncome[COMPUTER] +=5;
                        }
                    }
                }
            }
        }
        else if(buildingFlags[i] & PRODUCING && buildingFlags[i] & ALIVE)
        {
            if(buildingFlags[i] & ALLEGIANCE)
            {
                if(credits[PLAYER]>0)
                {
                    buildingConstructingTime[i]-=1;
                    if(type[i]==HANGAR)
                    {
                        credits[PLAYER]-=1;
                    }
                    if(buildingConstructingTime[i]==0)
                    {
                        if(type[i]==HANGAR)
                        {
                            buildingFlags[i] |= RELEASE_UNIT;
                        }
                        buildingFlags[i] ^= PRODUCING;
                    }
                }
            }
            else
            {
                if(credits[COMPUTER]>0)
                {
                    buildingConstructingTime[i]-=1;
                    if(type[i]==HANGAR)
                    {
                        credits[COMPUTER]-=1;
                    }
                    if(buildingConstructingTime[i]==0)
                    {
                        if(type[i]==HANGAR)
                        {
                            buildingFlags[i] |= RELEASE_UNIT;
                        }
                        buildingFlags[i] ^= PRODUCING;
                    }
                }
            }
        }
    }
}

int CREATE_PlaceBuilding(int* leftClickState, int* x, int* y, SDL_Point* p, int typeToConstruct, long* flags, char* grid,
                         unsigned char* type)
{
    int mx;
    int my;
    int i;
    int returnvalue=0;
    int pass=0;
    SDL_GetMouseState(&mx, &my);
    mx=mx/TILESIZE*TILESIZE;
    my=my/TILESIZE*TILESIZE;
    for(i=0; i<MAX_BUILDINGS; i++)
    {
        if(x[i]<=mx+128 && x[i]>=mx-128 &&
           y[i]<=my+128 && y[i]>=my-128 &&
           (flags[i] & ALLEGIANCE))
        {
            if(y[i]==my-64 && x[i]==mx && type[i]==HANGAR)
            {
                pass=0;
                i=MAX_BUILDINGS;
            }
            else
            {
                pass=1;
            }
        }
    }
    if(pass==1)
    {
        if(typeToConstruct==HANGAR)
        {
            if(grid[my/TILESIZE*MAP_SIZE_X+mx/TILESIZE]==0 && grid[(my/TILESIZE+1)*MAP_SIZE_X+mx/TILESIZE]==0)
            {
                pass=2;
            }

        }
        else
        {
            if(grid[my/TILESIZE*MAP_SIZE_X+mx/TILESIZE]==0)
            {
                pass=2;
            }
        }
    }
    if(pass==2)
    {
        p->x=mx;
        p->y=my;
        returnvalue=1;
    }
    *leftClickState = DEFAULT;
    return returnvalue;
}

int CREATE_SelectConstruction(int* leftClickState, int* flags, int x, int y, int type, short* constructingTime, int animation, int constructingType)
{
    int mX;
    int mY;
    SDL_GetMouseState(&mX, &mY);
    if(type==CONSTRUCTION_YARD)
    {
        if(mX>=x-22 && mX<x+14 && mY>=y+44 && mY<y+76 && (!(*flags & BEING_CONSTRUCTED)))
        {
            *leftClickState = PLACE_BUILDING;
            constructingType = POWER_PLANT;
        }
        else if(mX>=x+14 && mX<x+50 && mY>=y+44 && mY<y+76 && (!(*flags & BEING_CONSTRUCTED)))
        {
            *leftClickState = PLACE_BUILDING;
            constructingType = HANGAR;
        }
        else if(mX>=x+50 && mX<x+86 && mY>=y+44 && mY<y+76 && (!(*flags & BEING_CONSTRUCTED)))
        {
            *leftClickState = PLACE_BUILDING;
            constructingType = TURRET;
        }
        else
        {
            *leftClickState = DEFAULT;
        }
    }
    else if(type==HANGAR)
    {
        if(animation<80)
        {
            if(mX>=x-3 && mX<x+32 && mY>=y+44 && mY<y+76 && (!(*flags & BEING_CONSTRUCTED)))
            {
                constructingType = TRUCK;
                *constructingTime = TRUCK_COST;
                *flags |= PRODUCING;
            }
            else if(mX>=x+32 && mX<x+67 && mY>=y+44 && mY<y+76 && (!(*flags & BEING_CONSTRUCTED)))
            {
                constructingType = TANK;
                *constructingTime = TANK_COST;
                *flags |= PRODUCING;
            }
        }
        *leftClickState = DEFAULT;
    }
    *flags ^= SELECTED;
    return constructingType;
}
