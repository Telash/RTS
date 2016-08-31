#include "RTS.h"

Tank tank;
Building building;
Turret turret;
Projectile projectile = {{0}};

int main(int argc, char* argv[])
{
    SDL_Point resolution = {30*64, 17*64};
    char grid[MAP_SIZE_X*MAP_SIZE_Y] = {};
    int i;
    int run=1;
    int play=1;
    int input=0;
    int leftClickState=0;
    int gameTime=0;
    int credits[2] = {4000, 4000};
    int maxHealth[7] = {0, 2000, 1000, 3000, 800, 1000, 500};
    int selectedBuilding=0;
    int createBuilding=0;
    int continousCreditIncome[2] = {5, 5};
    int computerCy=1;
    int victory=3;
    int computerBuildQueIndex=0;
    int computerBuildQue[8] = {HANGAR, TURRET, POWER_PLANT, POWER_PLANT, TURRET, TURRET, POWER_PLANT, POWER_PLANT};
    SDL_Point computerBuildQuePoint[9];
    SDL_Point point;

    srand(time(0));

    GRAPH_InitWindow(resolution);
    TextFunction();

    point.x=128;
    point.y=128;
    building = CREATE_CreateBuilding(building, point, COMPUTER, CONSTRUCTION_YARD, maxHealth, 0, grid);
    CREATE_FillBuildQuePoints(computerBuildQuePoint, point);

    point.x=1728;
    point.y=832;
    building = CREATE_CreateBuilding(building, point, PLAYER, CONSTRUCTION_YARD, maxHealth, 0, grid);

    while(run)
    {
    if(play)
    {
        /*START HANDLE INPUTS*/
        input = RTS_GetInput();
        switch(input)
        {
            case QUIT:
            {
                play=0;
            }break;
            case LEFTCLICK:
            {
                if(leftClickState==DEFAULT)
                {
                    selectedBuilding = RTS_SelectingBuildingAndTurret(building.flags, building.x, building.y, &leftClickState,
                                                             tank.flags, turret.flags, turret.x, turret.y);
                }
                else if(leftClickState==PLACE_BUILDING)
                {
                    createBuilding = CREATE_PlaceBuilding(&leftClickState, building.x, building.y, &point,
                                                          building.constructingType[selectedBuilding], building.flags, grid,
                                                          building.type);
                }
                else if(leftClickState==SELECT_CONSTRUCTION)
                {
                    if((building.flags[selectedBuilding] & PRODUCING) || (building.type[selectedBuilding]!=CONSTRUCTION_YARD
                        && building.type[selectedBuilding]!=HANGAR))
                    {
                        leftClickState = SELECT_TANKS;
                    }
                    else
                    {
                        building.constructingType[selectedBuilding] =
                        CREATE_SelectConstruction(&leftClickState, &building.flags[selectedBuilding],
                                               building.x[selectedBuilding], building.y[selectedBuilding],
                                               building.type[selectedBuilding], &building.constructingTime[selectedBuilding],
                                               building.animation[selectedBuilding], building.constructingType[selectedBuilding]);
                    }
                }
                if(leftClickState==SELECT_TANKS)
                {
                    RTS_SelectingUnits(START, tank.x, tank.y, tank.flags);
                }
            }break;
            case LEFTCLICK_UP:
            {
                if(leftClickState==SELECT_TANKS)
                {
                    RTS_SelectingUnits(FINISH, tank.x, tank.y, tank.flags);
                    leftClickState=DEFAULT;
                }
            }break;
            case RIGHTCLICK:
            {
                RTS_SetDestination(tank.destX, tank.destY, tank.flags);
                FIGHT_SetTargetGoal(grid, tank.x, tank.y, turret.x, turret.y, building.x, building.y, tank.flags, turret.flags,
                                  building.flags, turret.target, tank.target, tank.targetGoal, turret.targetGoal,
                                  tank.targetGoalType, turret.targetGoalType);
            }break;
        }
        /*END HANDLE INPUTS*/
        /*------------------------*/
        /*PLAYER PRODUCTIONS*/
        if(createBuilding==1)
        {
            if(building.constructingType[selectedBuilding]==TURRET)
            {
                turret = CREATE_CreateTurret(turret, point, PLAYER, maxHealth, selectedBuilding, building.constructingTime,
                                          building.flags, grid);
            }
            else
            {
                building = CREATE_CreateBuilding(building, point, PLAYER,
                                              building.constructingType[selectedBuilding],
                                              maxHealth, selectedBuilding, grid);
            }
            createBuilding=0;
        }
        RTS_CreditsIncome(continousCreditIncome, credits, gameTime);
        /*END PLAYER PRODUCTION*/
        /*---------------------*/
        /*PRODUCTION FOR BOTH*/
        CREATE_HandleConstructions(building.flags, building.constructingTime, credits, turret.flags,
                                turret.constructingTime, building.type, continousCreditIncome, building.constructingType,
                                building.animation);
        for(i=0; i<MAX_BUILDINGS; i++)
        {
            if((building.flags[i] & RELEASE_UNIT) && building.animation[i]<70)
            {
                int j;
                if(building.flags[i] & ALLEGIANCE)
                {
                    j=1;
                }
                else
                {
                    j=0;
                }
                tank = CREATE_CreateTank(tank, building.x[i], building.y[i], j, building.constructingType[i], maxHealth);
                building.flags[i] ^= RELEASE_UNIT;
            }
        }
        /*END PRODUCTION FOR BOTH*/
        /*---------------------*/
        /*COMPUTER PRODUCTIONS*/
        if(computerCy)
        {
            if(!(building.flags[computerCy] & PRODUCING) && (building.flags[computerCy] & ALIVE))
            {
                if(computerBuildQue[computerBuildQueIndex]==HANGAR)
                {
                    point.x=computerBuildQuePoint[SOUTH].x;
                    computerBuildQuePoint[SOUTH].x=(-1);
                    point.y=computerBuildQuePoint[SOUTH].y;
                    building = CREATE_CreateBuilding(building, point, COMPUTER, computerBuildQue[computerBuildQueIndex],
                                                  maxHealth, computerCy, grid);
                    computerBuildQueIndex++;
                }
                else
                {
                    i=1;
                    while(computerBuildQuePoint[i].x==(-1))
                    {
                        i++;
                        if(i==SOUTH)
                        {
                            i++;
                        }
                    }
                    if(i<9)
                    {
                        point.x=computerBuildQuePoint[i].x;
                        computerBuildQuePoint[i].x=(-1);
                        point.y=computerBuildQuePoint[i].y;
                        if(computerBuildQue[computerBuildQueIndex]==TURRET)
                        {
                            turret = CREATE_CreateTurret(turret, point, COMPUTER, maxHealth, computerCy, building.constructingTime,
                                                      building.flags, grid);
                        }
                        else
                        {
                            building = CREATE_CreateBuilding(building, point, COMPUTER, computerBuildQue[computerBuildQueIndex],
                                                          maxHealth, computerCy, grid);
                        }
                        computerBuildQueIndex++;
                    }
                }
            }
        }
        /*END COMPUTER PRODUTIONS*/
        FIGHT_HandleTargetTank(tank.target, tank.x, tank.y, tank.flags, tank.targetType, building.x, building.y,
                             turret.x, turret.y, building.flags, turret.flags, tank.targetGoal, tank.targetGoalType,
                             tank.destX, tank.destY, tank.direction, tank.type);
        FIGHT_HandleTargetTurret(turret.target, tank.x, tank.y, tank.flags, turret.targetType, building.x, building.y,
                             turret.x, turret.y, building.flags, turret.flags, turret.targetGoal, turret.targetGoalType);
        RTS_SetDirection(tank.x, tank.y, tank.destX, tank.destY, tank.flags, tank.direction);
        FIGHT_HandleAttackByTank(tank.healthRemain, tank.target, turret.healthRemain, building.healthRemain, tank.targetType,
                               gameTime, tank.initiative, tank.type, &projectile, tank.x, tank.y, tank.turretAngle,
                               building.x, building.y, turret.x, turret.y, tank.direction, tank.flags);
        FIGHT_HandleDamageByTurret(tank.healthRemain, turret.target, turret.healthRemain, building.healthRemain,
                                 turret.targetType, turret.initiative, gameTime, turret.angle, turret.angleGoal);
        RTS_HandleGrid(grid, tank.x, tank.y, tank.flags, tank.direction, tank.currentTile);
        FIGHT_HandleDeathsTank(tank.healthRemain, tank.flags, tank.target, tank.currentTile, grid);
        FIGHT_HandleDeathsBuilding(building.healthRemain, building.flags, grid, building.x, building.y);
        FIGHT_HandleDeathsTurret(turret.healthRemain, turret.flags, turret.target, grid, turret.x, turret.y);
        RTS_HealthBarTank(tank.flags, tank.healthRemain, maxHealth, tank.type);
        RTS_HealthBarTurret(turret.flags, turret.healthRemain, maxHealth);
        RTS_HealthBarBuilding(building.flags, building.healthRemain,
                             building.type, maxHealth);
        FIGHT_SetTurretAngleGoalForTank(tank.turretAngleGoal, tank.x, tank.y, tank.target,
                           tank.angle, tank.flags, building.x, building.y, turret.x, turret.y, tank.targetType);
        FIGHT_SetTurretAngleForTank(tank.turretAngle, tank.turretAngleGoal, tank.target, tank.direction);
        FIGHT_SetTurretAngleGoalForTurret(turret.angleGoal, tank.x, tank.y, turret.target, turret.flags, building.x, building.y,
                                        turret.x, turret.y, turret.targetType);
        FIGHT_SetTurretAngleForTurret(turret.angle, turret.angleGoal);
        RTS_SetTankAngle(tank.angle, tank.direction);
        RTS_MoveTank(tank.x, tank.y, tank.direction, tank.flags, grid);
        MOVE_MoveProjectile(&projectile);
        FIGHT_DestroyProjectile(&projectile, building.x, building.y, turret.x, turret.y, tank.x, tank.y, building.flags, turret.flags,
                                tank.flags, grid, building.healthRemain, turret.healthRemain, tank.healthRemain);
        victory = RTS_CheckVictory(tank.flags, building.flags, turret.flags);
    }
        /*START DRAWING*/
        GRAPH_InitDrawFrame();
        GRAPH_DrawOccupied(grid);
        if(leftClickState==PLACE_BUILDING)
        {
            GRAPH_DrawPlaceableFrame(building.constructingType[selectedBuilding], grid, building.flags, building.x, building.y,
                                     building.type);
        }
        GRAPH_DrawBuildingFloors(building.x, building.y, building.type, building.flags);
        GRAPH_DrawTanksBottom(tank.x, tank.y, tank.angle, tank.flags, tank.turretAngle, tank.type);
        GRAPH_DrawProjectiles(&projectile);
        GRAPH_DrawLaser(tank.x, tank.y, tank.target, building.x, building.y, turret.x, turret.y, tank.targetType, turret.target,
                       turret.targetType, turret.initiative, gameTime, turret.angle, turret.angleGoal, tank.turretAngle, tank.type,
                       tank.initiative);
        GRAPH_DrawTanksTop(tank.x, tank.y, tank.angle, tank.flags, tank.turretAngle, tank.type);
        GRAPH_DrawHealthBarTank(tank.x, tank.y, tank.healthRemain, tank.flags, tank.type, maxHealth);
        GRAPH_DrawHealthBarTurret(turret.x, turret.y, turret.healthRemain, turret.flags);
        GRAPH_DrawBuildings(building.x, building.y, building.type, building.flags, building.animation, gameTime,
                          building.constructingTime, grid);
        GRAPH_DrawTurrets(turret.x, turret.y, turret.angle, turret.flags, gameTime);
        GRAPH_DrawProgressConstruction(building.constructingTime, building.flags, building.x,
                                     building.y, building.type, turret.constructingTime, turret.flags,
                                     turret.x, turret.y, building.constructingType);
        GRAPH_DrawHealthBarBuilding(building.x, building.y, building.healthRemain,
                                  building.flags, building.type, maxHealth);
        GRAPH_DrawConstructionBar(building.x[selectedBuilding], building.y[selectedBuilding],
                                building.flags[selectedBuilding], building.type[selectedBuilding]);
        if(leftClickState==SELECT_TANKS)
        {
            GRAPH_DrawSelectingRectangle();
        }
        GRAPH_DrawMoneyBar(credits[1]);
        if(victory<3)
        {
            play=0;
            TextFunctionCont(victory);
        }
        GRAPH_ShowFrame();
        /*FINISHED DRAWING*/
        gameTime++;
    }
    GRAPH_Quit();
    return 0;
}
