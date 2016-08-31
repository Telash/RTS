#ifndef RTS_H_INCLUDED
#define RTS_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL_ttf.h>

#define MAP_SIZE_X 30
#define MAP_SIZE_Y 17

/*OBJECT LIMITS*/
#define MAX_TANKS 100
#define MAX_BUILDINGS 50
#define MAX_TURRETS 50
#define MAX_PROJECTILES 5000
/*END PROJECT LIMITS*/

#define TILESIZE 64

#define COMPUTER 0
#define PLAYER 1

#define START 1
#define FINISH 2

/*USER INPUTS*/
#define QUIT 1
#define LEFTCLICK 2
#define RIGHTCLICK 3
#define LEFTCLICK_UP 4
#define RIGHTCLICK_UP 5
/*END USER INPUTS*/

/*FLAGS*/
#define ALIVE 1
#define SELECTED (1<<1)
#define ALLEGIANCE (1<<2)
#define SHOW_HEALTH_BAR (1<<3)
#define BEING_CONSTRUCTED (1<<4)
#define PRODUCING (1<<5)
#define RELEASE_UNIT (1<<6)
#define COLLIDED (1<<7)
/*END FLAGS*/

/*CREDIT COST*/
#define TANK_COST 300
#define TRUCK_COST 200
#define HANGAR_COST 400
#define POWER_PLANT_COST 300
#define TURRET_COST 400
#define CONSTRUCTION_YARD_COST 2000
/*END CREDIT COST*/

/*MISC OBJECT DATA*/
#define TURRET_ATTACK_DAMAGE 100
#define TRUCK_ATTACK_DAMAGE 2
#define TURRET_ATTACK_SPEED 100
#define TRUCK_ATTACK_SPEED 7
#define TANK_ATTACK_SPEED 30
#define TRUCK_RANGE 192
#define TANK_RANGE 256
#define TURRET_RANGE 256
#define TURRET_TURN_SPEED 3
#define TRUCK_GUN_TURN_SPEED 5
#define PROJECTILE_SPEED 16
#define PROJECTILE_DAMAGE 15
#define SPEED 2
/*END MISC OBJECT DATA*/

/*TYPES*/
#define HANGAR 1
#define POWER_PLANT 2
#define CONSTRUCTION_YARD 3
#define TANK 4
#define TURRET 5
#define TRUCK 6
/*END TYPES*/

/*DIRECTIONS*/
#define NORTH 1
#define NORTHEAST 2
#define EAST 3
#define SOUTHEAST 4
#define SOUTH 5
#define SOUTHWEST 6
#define WEST 7
#define NORTHWEST 8
/*END DIRECTIONS*/

#define UNIT 1
#define DEFENCE_TOWER 2
#define BUILDING 3

/*MOUSE STATE*/
#define DEFAULT 0
#define SELECT_TANKS 1
#define PLACE_BUILDING 2
#define SELECT_CONSTRUCTION 3
/*END MOUSE STATE*/

typedef struct
{
    #define M MAX_TURRETS
    int x[M];
    int y[M];
    long flags[M];
    double angle[M];
    double angleGoal[M];
    int target[M];
    int targetGoal[M];
    unsigned char targetGoalType[M];
    unsigned char targetType[M];
    short constructingTime[M];
    int healthRemain[M];
    short attackSpeed[M];
    short attackDamage[M];
    char initiative[M];
    short animation[M];
    #undef M
}Turret;

typedef struct
{
    #define M MAX_TANKS
    int x[M];
    int y[M];
    int destX[M];
    int destY[M];
    long flags[M];
    double angle[M];
    double turretAngle[M];
    double turretAngleGoal[M];
    int target[M];
    int targetGoal[M];
    unsigned char targetType[M];
    unsigned char targetGoalType[M];
    int healthRemain[M];
    short type[M];
    char direction[M];
    int currentTile[M];
    char initiative[M];
    #undef M
}Tank;

typedef struct
{
    #define M MAX_BUILDINGS
    int x[M];
    int y[M];
    long flags[M];
    int healthRemain[M];
    unsigned char type[M];
    short animation[M];
    short constructingTime[M];
    unsigned char constructingType[M];
    #undef M
}Building;

typedef struct
{
    #define M MAX_PROJECTILES
    int x[M];
    int y[M];
    char flags[M];
    double angle[M];
    int rangeLeft[M];
    double moveMultiplier[M];
    double moveTransfered[M];
    #undef M
}Projectile;

void TextFunction();
void TextFunctionCont();

void RTS_CheckCollision();
int RTS_CheckVictory();
void RTS_ComputerBuildQue();
Building CREATE_CreateBuilding();
void CREATE_CreateProjectile();
Tank CREATE_CreateTank();
Turret CREATE_CreateTurret();
void RTS_CreditsIncome();
void FIGHT_DestroyProjectile();
void GRAPH_DrawConstructionBar();
void GRAPH_DrawBuildingFloors();
void GRAPH_DrawBuildings();
void GRAPH_DrawHealthBarBuilding();
void GRAPH_DrawHealthBarTank();
void GRAPH_DrawHealthBarTurret();
void GRAPH_DrawLaser();
void GRAPH_DrawMoneyBar();
void GRAPH_DrawOccupied();
void GRAPH_DrawPlaceableFrame();
void GRAPH_DrawProgressConstruction();
void GRAPH_DrawProjectiles();
void GRAPH_DrawSelectingRectangle();
void GRAPH_DrawTanksBottom();
void GRAPH_DrawTanksTop();
void GRAPH_DrawTurrets();
void CREATE_FillBuildQuePoints();
int RTS_GetInput();
void CREATE_HandleConstructions();
void FIGHT_HandleDeathsBuilding();
void FIGHT_HandleDeathsTank();
void FIGHT_HandleDeathsTurret();
void FIGHT_HandleAttackByTank();
void FIGHT_HandleDamageByTurret();
void RTS_HandleGrid();
void FIGHT_HandleTargetTank();
void FIGHT_HandleTargetTurret();
void RTS_HealthBarTank();
void RTS_HealthBarTurret();
void RTS_HealthBarBuilding();
void GRAPH_InitDrawFrame();
void GRAPH_InitWindow();
void MOVE_MoveProjectile();
void RTS_MoveTank();
int CREATE_PlaceBuilding();
void GRAPH_Quit();
int RTS_SelectingBuildingAndTurret();
int CREATE_SelectConstruction();
void RTS_SelectingUnits();
void RTS_SetDestination();
void RTS_SetDirection();
void RTS_SetTankAngle();
void FIGHT_SetTargetGoal();
void FIGHT_SetTurretAngleForTank();
void FIGHT_SetTurretAngleGoalForTank();
void FIGHT_SetTurretAngleForTurret();
void FIGHT_SetTurretAngleGoalForTurret();
void GRAPH_ShowFrame();
void RTS_StartBuilding();

#endif // RTS_H_INCLUDED
