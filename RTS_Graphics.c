#include "RTS.h"

SDL_Window* wind;
SDL_Renderer* rend;
SDL_Texture* tankBottom1;
SDL_Texture* tankGun1;
SDL_Texture* tankBottom2;
SDL_Texture* tankGun2;

SDL_Texture* hangar1;
SDL_Texture* hangar2;
SDL_Texture* hangar_door1;
SDL_Texture* hangar_door2;
SDL_Texture* hangar_icon1;
SDL_Texture* hangar_icon2;
SDL_Texture* hangar_floor;
SDL_Texture* power_plant1;
SDL_Texture* power_plant2;
SDL_Texture* power_plant_icon1;
SDL_Texture* power_plant_icon2;
SDL_Texture* power_plant_fan1;
SDL_Texture* power_plant_fan2;
SDL_Texture* power_plant_fan_center1;
SDL_Texture* power_plant_fan_center2;
SDL_Texture* construction_yard1;
SDL_Texture* construction_yard2;
SDL_Texture* construction_yard_crane1;
SDL_Texture* construction_yard_crane2;
SDL_Texture* construction_yard_crane_block1;
SDL_Texture* construction_yard_crane_block2;
SDL_Texture* turret1;
SDL_Texture* turret2;
SDL_Texture* turret_gun1;
SDL_Texture* turret_gun2;
SDL_Texture* turret_icon1;
SDL_Texture* turret_icon2;
SDL_Texture* truck1;
SDL_Texture* truck2;
SDL_Texture* truck_frontCannon1;
SDL_Texture* truck_frontCannon2;
SDL_Texture* truck_backCannon1;
SDL_Texture* truck_backCannon2;
SDL_Texture* projectileTexture;
SDL_Texture* price_200;
SDL_Texture* price_300;
SDL_Texture* price_400;

SDL_Surface* surface;

TTF_Font* font;
SDL_Texture* text;
SDL_Surface* textsurface;

SDL_Color color = {200, 200, 200, 255};

void TextFunction()
{
    TTF_Init();
    font = TTF_OpenFont("Xerox.ttf", 10);
    textsurface = TTF_RenderText_Solid(font, "200", color);
    price_200 = SDL_CreateTextureFromSurface(rend, textsurface);
    textsurface = TTF_RenderText_Solid(font, "300", color);
    price_300 = SDL_CreateTextureFromSurface(rend, textsurface);
    textsurface = TTF_RenderText_Solid(font, "400", color);
    price_400 = SDL_CreateTextureFromSurface(rend, textsurface);
    font = TTF_OpenFont("Xerox.ttf", 42);
}

void TextFunctionCont(int victory)
{
    SDL_Rect rect = {750, 500, 0, 0};
    if(victory==1)
    {
        color.b=200;
        color.g=200;
        color.r=100;
        textsurface = TTF_RenderText_Solid(font, "Mission succes!", color);
    }
    else
    {
        color.b=50;
        color.g=0;
        color.r=240;
        textsurface = TTF_RenderText_Solid(font, "Mission failed!", color);
    }
    SDL_QueryTexture(text, NULL, NULL, &rect.w, &rect.h);
    text = SDL_CreateTextureFromSurface(rend, textsurface);
    SDL_RenderCopy(rend, text, 0, &rect);
}

int mouseX;
int mouseY;

int startX;
int startY;

void GRAPH_DrawBuildingFloors(int* x, int* y, char* type, long* flags)
{
    int i;
    SDL_Rect rect;
    rect.w=64;
    rect.h=64;
    for(i=1; i<MAX_BUILDINGS; i++)
    {
        if((flags[i] & ALIVE) && type[i]==HANGAR && (!(flags[i] & BEING_CONSTRUCTED)))
        {
            rect.x=x[i];
            rect.y=y[i];
            SDL_RenderCopy(rend, hangar_floor, 0, &rect);
        }
    }
}

void GRAPH_DrawBuildings(int* x, int* y, char* type, long* flags, short* animation, int gameTime, short* constructingTime, char* grid)
{
    int i;
    SDL_Rect rect;
    for(i=1; i<MAX_BUILDINGS; i++)
    {
        if(flags[i] & ALIVE)
        {
            rect.w=64;
            rect.h=64;
            switch(type[i])
            {
                case HANGAR:
                {
                    rect.x=x[i];
                    if(!(flags[i] & BEING_CONSTRUCTED))
                    {
                        if(constructingTime[i]==1)
                        {
                            animation[i]=100;
                        }
                        else if(animation[i]==80 && grid[(y[i]/TILESIZE+1)*MAP_SIZE_X+x[i]/TILESIZE]!=0)
                        {

                        }
                        else if(animation[i]>0)
                        {
                            animation[i]-=1;
                        }
                        if(animation[i]>80)
                        {
                            rect.y=y[i]-100+animation[i];
                        }
                        else if(animation[i]<30)
                        {
                            rect.y=y[i]-animation[i];
                        }
                        else if(animation[i]>0)
                        {
                            rect.y=y[i]-30;
                        }
                        else
                        {
                            rect.y=y[i];
                        }
                    }
                    else
                    {
                        rect.y=y[i];
                    }
                    if(flags[i] & ALLEGIANCE)
                    {
                        if(flags[i] & BEING_CONSTRUCTED)
                        {
                            SDL_SetTextureBlendMode(hangar_icon1, SDL_BLENDMODE_BLEND);
                            SDL_SetTextureAlphaMod(hangar_icon1, 120);
                            SDL_RenderCopy(rend, hangar_icon1, 0, &rect);
                            SDL_SetTextureAlphaMod(hangar_icon1, 255);
                        }
                        else
                        {
                            SDL_RenderCopy(rend, hangar_door1, 0, &rect);
                            rect.y=y[i];
                            SDL_RenderCopy(rend, hangar1, 0, &rect);
                        }
                    }
                    else
                    {
                        if(flags[i] & BEING_CONSTRUCTED)
                        {
                            SDL_SetTextureBlendMode(hangar_icon2, SDL_BLENDMODE_BLEND);
                            SDL_SetTextureAlphaMod(hangar_icon2, 120);
                            SDL_RenderCopy(rend, hangar_icon2, 0, &rect);
                            SDL_SetTextureAlphaMod(hangar_icon2, 255);
                        }
                        else
                        {
                            SDL_RenderCopy(rend, hangar_door2, 0, &rect);
                            rect.y=y[i];
                            SDL_RenderCopy(rend, hangar2, 0, &rect);
                        }
                    }
                }break;
                case POWER_PLANT:
                {
                    rect.x=x[i];
                    rect.y=y[i];
                    if(flags[i] & ALLEGIANCE)
                    {
                        if(flags[i] & BEING_CONSTRUCTED)
                        {
                            SDL_SetTextureBlendMode(power_plant_icon1, SDL_BLENDMODE_BLEND);
                            SDL_SetTextureAlphaMod(power_plant_icon1, 120);
                            SDL_RenderCopy(rend, power_plant_icon1, 0, &rect);
                            SDL_SetTextureAlphaMod(power_plant_icon1, 255);
                        }
                        else
                        {
                            SDL_RenderCopy(rend, power_plant1, 0, &rect);
                            rect.x+=3;
                            rect.y+=3;
                            rect.h=21;
                            rect.w=21;
                            SDL_RenderCopyEx(rend, power_plant_fan1, 0, &rect, (gameTime%180)*2, 0, 0);
                            SDL_RenderCopy(rend, power_plant_fan_center1, 0, &rect);
                            rect.y+=22;
                            SDL_RenderCopyEx(rend, power_plant_fan1, 0, &rect, (gameTime%180)*2, 0, 0);
                            SDL_RenderCopy(rend, power_plant_fan_center1, 0, &rect);
                        }
                    }
                    else
                    {
                        if(flags[i] & BEING_CONSTRUCTED)
                        {
                            SDL_SetTextureBlendMode(power_plant_icon2, SDL_BLENDMODE_BLEND);
                            SDL_SetTextureAlphaMod(power_plant_icon2, 120);
                            SDL_RenderCopy(rend, power_plant_icon2, 0, &rect);
                            SDL_SetTextureAlphaMod(power_plant_icon2, 255);
                        }
                        else
                        {
                            SDL_RenderCopy(rend, power_plant2, 0, &rect);
                            rect.x+=3;
                            rect.y+=3;
                            rect.h=21;
                            rect.w=21;
                            SDL_RenderCopyEx(rend, power_plant_fan2, 0, &rect, (gameTime%180)*2, 0, 0);
                            SDL_RenderCopy(rend, power_plant_fan_center2, 0, &rect);
                            rect.y+=22;
                            SDL_RenderCopyEx(rend, power_plant_fan2, 0, &rect, (gameTime%180)*2, 0, 0);
                            SDL_RenderCopy(rend, power_plant_fan_center2, 0, &rect);
                        }
                    }
                }break;
                case CONSTRUCTION_YARD:
                {
                    rect.x=x[i];
                    rect.y=y[i];
                    if(flags[i] & ALLEGIANCE)
                    {
                        SDL_RenderCopy(rend, construction_yard1, 0, &rect);
                    }
                    else
                    {
                        SDL_RenderCopy(rend, construction_yard2, 0, &rect);
                    }
                    if(constructingTime[i]>0)
                    {
                        if(constructingTime[i]<64)
                        {
                            rect.x-=(constructingTime[i]/2);
                        }
                        else if(constructingTime[i]<160)
                        {
                            rect.x-=32;
                        }
                        else if(constructingTime[i]<224)
                        {
                            rect.x-=112-(constructingTime[i]/2);
                        }
                    }
                    rect.h=36;
                    rect.w=36;
                    rect.y+=27;
                    rect.x+=32;
                    if(flags[i] & ALLEGIANCE)
                    {
                        SDL_RenderCopy(rend, construction_yard_crane1, 0, &rect);
                    }
                    else
                    {
                        SDL_RenderCopy(rend, construction_yard_crane2, 0, &rect);
                    }
                    SDL_SetRenderDrawColor(rend, 71, 46, 2, 255);
                    int wireLength = 12;
                    if(constructingTime[i]>74 && constructingTime[i]<102)
                    {
                        wireLength+=constructingTime[i]/2-37;
                    }
                    else if(constructingTime[i]>=102 && constructingTime[i]<=122)
                    {
                        wireLength=26;
                    }
                    else if(constructingTime[i]>122 && constructingTime[i]<150)
                    {
                        wireLength+=(75-constructingTime[i]/2);
                    }
                    SDL_RenderDrawLine(rend, rect.x+13, rect.y+5, rect.x+13, rect.y+5+wireLength);
                    SDL_RenderDrawLine(rend, rect.x+15, rect.y+5, rect.x+15, rect.y+5+wireLength);
                    rect.h=7;
                    rect.w=7;
                    rect.y+=3+wireLength;
                    rect.x+=11;
                    if(flags[i] & ALLEGIANCE)
                    {
                        SDL_RenderCopy(rend, construction_yard_crane_block1, 0, &rect);
                    }
                    else
                    {
                        SDL_RenderCopy(rend, construction_yard_crane_block2, 0, &rect);
                    }
                }break;
            }
        }
    }
}

void GRAPH_DrawConstructionBar(int x, int y, long flags, int type)
{
    SDL_Rect rect;
    if(flags & SELECTED)
    {
        if(type==CONSTRUCTION_YARD)
        {
            rect.x=x;
            rect.y=y;
            SDL_SetRenderDrawColor(rend, 20, 40, 0, 255);
            rect.w=108;
            rect.h=36;
            rect.x-=22;
            rect.y+=46;
            SDL_RenderFillRect(rend, &rect);
            rect.w=32;
            rect.h=32;
            SDL_RenderCopy(rend, power_plant_icon1, 0, &rect);
            rect.x+=38;
            SDL_RenderCopy(rend, hangar_icon1, 0, &rect);
            rect.x+=32;
            rect.y-=1;
            rect.w=40;
            rect.h=40;
            SDL_RenderCopy(rend, turret_icon1, 0, &rect);
            rect.x-=64;
            rect.y+=36;
            rect.h=12;
            rect.w=20;
            SDL_RenderCopy(rend, price_300, 0, &rect);
            rect.x+=38;
            SDL_RenderCopy(rend, price_400, 0, &rect);
            rect.x+=37;
            SDL_RenderCopy(rend, price_400, 0, &rect);
        }
        else if(type==HANGAR)
        {
            rect.x=x;
            rect.y=y;
            SDL_SetRenderDrawColor(rend, 20, 40, 0, 255);
            rect.w=74;
            rect.h=36;
            rect.x-=5;
            rect.y+=46;
            SDL_RenderFillRect(rend, &rect);
            rect.x-=3;
            rect.w=38;
            rect.h=38;
            SDL_RenderCopy(rend, truck1, 0, &rect);
            rect.y-=2;
            SDL_RenderCopy(rend, truck_frontCannon1, 0, &rect);
            rect.x+=38;
            rect.y+=2;
            SDL_RenderCopy(rend, tankBottom1, 0, &rect);
            rect.y-=8;
            SDL_RenderCopy(rend, tankGun1, 0, &rect);
            rect.x-=28;
            rect.y+=44;
            rect.h=12;
            rect.w=20;
            SDL_RenderCopy(rend, price_200, 0, &rect);
            rect.x+=38;
            SDL_RenderCopy(rend, price_300, 0, &rect);
        }
    }
}

void GRAPH_DrawHealthBarBuilding(int* x, int* y, int* healthRemain, long* flags, unsigned char* type, int* maxHealth)
{
    int i;
    SDL_Rect rect;
    rect.h=8;
    for(i=0; i<MAX_BUILDINGS; i++)
    {
        if(flags[i] & SHOW_HEALTH_BAR)
        {
            rect.w=TILESIZE-TILESIZE/4;
            SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
            rect.x=x[i]+TILESIZE/8;
            rect.y=y[i];
            SDL_RenderFillRect(rend, &rect);
            SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
            rect.w=rect.w*(maxHealth[type[i]]-healthRemain[i])/maxHealth[type[i]];
            rect.x+=TILESIZE-TILESIZE/4-rect.w;
            SDL_RenderFillRect(rend, &rect);
        }
    }
}

void GRAPH_DrawHealthBarTurret(int* x, int* y, int* healthRemain, long* flags)
{
    int i;
    SDL_Rect rect;
    rect.h=8;
    for(i=0; i<MAX_TURRETS; i++)
    {
        if(flags[i] & SHOW_HEALTH_BAR)
        {
            rect.w=TILESIZE-TILESIZE/4;
            SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
            rect.x=x[i]+TILESIZE/8;
            rect.y=y[i];
            SDL_RenderFillRect(rend, &rect);
            SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
            rect.w=rect.w*(1000-healthRemain[i])/1000;
            rect.x+=TILESIZE-TILESIZE/4-rect.w;
            SDL_RenderFillRect(rend, &rect);
        }
    }
}

void GRAPH_DrawHealthBarTank(int* x, int* y, int* healthRemain, long* flags, short* type, int* maxHealth)
{
    int i;
    SDL_Rect rect;
    rect.h=8;
    for(i=0; i<MAX_TANKS; i++)
    {
        if(flags[i] & SHOW_HEALTH_BAR)
        {
            rect.w=TILESIZE-TILESIZE/4;
            SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
            rect.x=x[i]+TILESIZE/8;
            rect.y=y[i];
            SDL_RenderFillRect(rend, &rect);
            SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
            rect.w=rect.w*(maxHealth[type[i]]-healthRemain[i])/maxHealth[type[i]];
            rect.x+=TILESIZE-TILESIZE/4-rect.w;
            SDL_RenderFillRect(rend, &rect);
        }
    }
}

void GRAPH_DrawLaser(int* tankX, int* tankY, int* target, int* buildingX, int* buildingY, int* turretX, int* turretY,
                   unsigned char* targetType, int* turretTarget, unsigned char* turretTargetType, char* turretInitiative,
                   int gameTime, double* turretAngle, double* turretAngleGoal, double* tankTurretAngle, short* tankType,
                   char* tankInitiative)
{
    int i;
    int alpha=0;
    int a;
    int x;
    int y;
    SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
    for(i=0; i<MAX_TANKS; i++)
    {
        if(target[i]!=0 && tankType[i]==TRUCK && gameTime%TRUCK_ATTACK_SPEED==tankInitiative[i])
        {
            if(targetType[i]==UNIT)
            {
                SDL_RenderDrawLine(rend, tankX[i]+33, tankY[i]+33, tankX[target[i]]+33, tankY[target[i]]+33);
            }
            else if(targetType[i]==DEFENCE_TOWER)
            {
                SDL_RenderDrawLine(rend, tankX[i]+33, tankY[i]+33, turretX[target[i]]+33, turretY[target[i]]+33);
            }
            else if(targetType[i]==BUILDING)
            {
                SDL_RenderDrawLine(rend, tankX[i]+33, tankY[i]+33, buildingX[target[i]]+33, buildingY[target[i]]+33);
            }
        }
    }
    for(i=0; i<MAX_TURRETS; i++)
    {
        if(turretTarget[i]!=0 && turretAngle[i]==turretAngleGoal[i])
        {
            if((gameTime%TURRET_ATTACK_SPEED>=turretInitiative[i] &&
                gameTime%TURRET_ATTACK_SPEED<=turretInitiative[i]+10)
               || gameTime%TURRET_ATTACK_SPEED<=turretInitiative[i]-90)
            {
                alpha=255;
            }
            else
            {
                a=gameTime%TURRET_ATTACK_SPEED-turretInitiative[i]-10;
                if(a<0)
                {
                    a+=100;
                }
                alpha=255-(a*5);
                if(alpha<0)
                {
                    alpha=0;
                }
            }
            if(turretTargetType[i]==UNIT)
            {
                x=tankX[turretTarget[i]];
                y=tankY[turretTarget[i]];
            }
            else if(turretTargetType[i]==DEFENCE_TOWER)
            {
                x=turretX[turretTarget[i]];
                y=turretY[turretTarget[i]];
            }
            else if(turretTargetType[i]==BUILDING)
            {
                x=buildingX[turretTarget[i]];
                y=buildingY[turretTarget[i]];
            }
            SDL_SetRenderDrawColor(rend, 100, 100, 255, alpha);
            SDL_RenderDrawLine(rend, turretX[i]+32, turretY[i]+32, x+32, y+32);
            SDL_SetRenderDrawColor(rend, 0, 0, 255, alpha);
            if(turretAngle[i]<=90 || (turretAngle[i]>180 && turretAngle[i]<=270))
            {
                SDL_RenderDrawLine(rend, turretX[i]+31, turretY[i]+31, x+31, y+31);
                SDL_RenderDrawLine(rend, turretX[i]+33, turretY[i]+33, x+33, y+33);
            }
            else
            {
                SDL_RenderDrawLine(rend, turretX[i]+31, turretY[i]+33, x+31, y+33);
                SDL_RenderDrawLine(rend, turretX[i]+33, turretY[i]+31, x+33, y+31);
            }
        }
    }
}

void GRAPH_DrawMoneyBar(int credits)
{
    SDL_Rect rect;
    int nextNumber;
    int total = credits;
    int digits=0;
    int moneyTextY=4;
    int moneyTextX=200;
    while(total)
    {
        total=total/10;
        digits++;
    }
    total=credits;
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    rect.x=moneyTextX+10-(16*digits);
    rect.y=moneyTextY-4;
    rect.h=24;
    rect.w=8+(16*digits);
    if(credits>0)
    {
        SDL_RenderFillRect(rend, &rect);
    }
    SDL_SetRenderDrawColor(rend, 120, 100, 100, 255);
    while(digits)
    {
        nextNumber=total%10;
        total=total/10;
        switch(nextNumber)
        {
            case 0:
            {
                rect.w=2;
                rect.h=16;
                rect.x=moneyTextX;
                rect.y=moneyTextY;
                SDL_RenderFillRect(rend, &rect);
                rect.x=moneyTextX+10;
                SDL_RenderFillRect(rend, &rect);
                rect.x=moneyTextX;
                rect.w=10;
                rect.h=2;
                SDL_RenderFillRect(rend, &rect);
                rect.y=moneyTextY+14;
                SDL_RenderFillRect(rend, &rect);
            }break;
            case 1:
            {
                rect.w=2;
                rect.h=16;
                rect.y=moneyTextY;
                rect.x=moneyTextX+6;
                SDL_RenderFillRect(rend, &rect);
                rect.w=12;
                rect.h=2;
                rect.y=moneyTextY+14;
                rect.x=moneyTextX+1;
                SDL_RenderFillRect(rend, &rect);
                rect.x=moneyTextX+2;
                rect.w=6;
                rect.y=moneyTextY;
                SDL_RenderFillRect(rend, &rect);
            }break;
            case 2:
            {
                rect.w=2;
                rect.h=8;
                rect.x=moneyTextX;
                rect.y=moneyTextY+8;
                SDL_RenderFillRect(rend, &rect);
                rect.x=moneyTextX+10;
                rect.y=moneyTextY;
                SDL_RenderFillRect(rend, &rect);
                rect.x=moneyTextX;
                rect.w=12;
                rect.h=2;
                SDL_RenderFillRect(rend, &rect);
                rect.y=moneyTextY+7;
                SDL_RenderFillRect(rend, &rect);
                rect.y=moneyTextY+14;
                SDL_RenderFillRect(rend, &rect);
            }break;
            case 3:
            {
                rect.w=2;
                rect.h=16;
                rect.y=moneyTextY;
                rect.x=moneyTextX+10;
                SDL_RenderFillRect(rend, &rect);
                rect.x=moneyTextX;
                rect.w=10;
                rect.h=2;
                SDL_RenderFillRect(rend, &rect);
                rect.y=moneyTextY+7;
                SDL_RenderFillRect(rend, &rect);
                rect.y=moneyTextY+14;
                SDL_RenderFillRect(rend, &rect);
            }break;
            case 4:
            {
                rect.w=2;
                rect.h=8;
                rect.x=moneyTextX;
                rect.y=moneyTextY;
                SDL_RenderFillRect(rend, &rect);
                rect.h=16;
                rect.x=moneyTextX+10;
                SDL_RenderFillRect(rend, &rect);
                rect.x=moneyTextX;
                rect.w=10;
                rect.h=2;
                rect.y=moneyTextY+7;
                SDL_RenderFillRect(rend, &rect);
            }break;
            case 5:
            {
                rect.w=2;
                rect.h=8;
                rect.x=moneyTextX;
                rect.y=moneyTextY;
                SDL_RenderFillRect(rend, &rect);
                rect.h=16;
                rect.x=moneyTextX+10;
                rect.y=moneyTextY+8;
                rect.h=8;
                SDL_RenderFillRect(rend, &rect);
                rect.x=moneyTextX;
                rect.y=moneyTextY;
                rect.w=12;
                rect.h=2;
                SDL_RenderFillRect(rend, &rect);
                rect.y=moneyTextY+7;
                SDL_RenderFillRect(rend, &rect);
                rect.y=moneyTextY+14;
                SDL_RenderFillRect(rend, &rect);
            }break;
            case 6:
            {
                rect.w=2;
                rect.h=16;
                rect.x=moneyTextX;
                rect.y=moneyTextY;
                SDL_RenderFillRect(rend, &rect);
                rect.x=moneyTextX+10;
                rect.y=moneyTextY+8;
                rect.h=8;
                SDL_RenderFillRect(rend, &rect);
                rect.x=moneyTextX+2;
                rect.y=moneyTextY;
                rect.w=10;
                rect.h=2;
                SDL_RenderFillRect(rend, &rect);
                rect.y=moneyTextY+7;
                SDL_RenderFillRect(rend, &rect);
                rect.y=moneyTextY+14;
                SDL_RenderFillRect(rend, &rect);
            }break;
            case 7:
            {
                rect.w=2;
                rect.h=16;
                rect.y=moneyTextY;
                rect.x=moneyTextX+10;
                SDL_RenderFillRect(rend, &rect);
                rect.x=moneyTextX;
                rect.w=10;
                rect.h=2;
                SDL_RenderFillRect(rend, &rect);
            }break;
            case 8:
            {
                rect.w=2;
                rect.h=16;
                rect.x=moneyTextX;
                rect.y=moneyTextY;
                SDL_RenderFillRect(rend, &rect);
                rect.x=moneyTextX+10;
                SDL_RenderFillRect(rend, &rect);
                rect.x=moneyTextX;
                rect.w=10;
                rect.h=2;
                SDL_RenderFillRect(rend, &rect);
                rect.y=moneyTextY+7;
                SDL_RenderFillRect(rend, &rect);
                rect.y=moneyTextY+14;
                SDL_RenderFillRect(rend, &rect);
            }break;
            case 9:
            {
                rect.w=2;
                rect.h=8;
                rect.x=moneyTextX;
                rect.y=moneyTextY;
                SDL_RenderFillRect(rend, &rect);
                rect.h=16;
                rect.x=moneyTextX+10;
                SDL_RenderFillRect(rend, &rect);
                rect.x=moneyTextX;
                rect.w=10;
                rect.h=2;
                SDL_RenderFillRect(rend, &rect);
                rect.y=moneyTextY+7;
                SDL_RenderFillRect(rend, &rect);
                rect.y=moneyTextY+14;
                SDL_RenderFillRect(rend, &rect);
            }break;
        }
        digits--;
        moneyTextX-=16;
    }
}

void GRAPH_DrawOccupied(char* grid)
{
    int i;
    int j;
    SDL_Rect rect;
    SDL_SetRenderDrawColor(rend, 50, 0, 150, 255);
    rect.w=64;
    rect.h=64;
    for(i=0; i<MAP_SIZE_Y; i++)
    {
        for(j=0; j<MAP_SIZE_X; j++)
        {
            if(grid[i*MAP_SIZE_X+j]!=0)
            {
                rect.y=i*64;
                rect.x=j*64;
                SDL_RenderFillRect(rend, &rect);
            }
        }
    }
}

void GRAPH_DrawPlaceableFrame(int typeToPlace, char* grid, long* buildingFlags, int* buildingX, int* buildingY,
                              unsigned char* type)
{
    int i;
    int mx;
    int my;
    int pass=0;
    SDL_Rect rect;
    rect.h=64;
    rect.w=64;
    SDL_GetMouseState(&mx, &my);
    mx=mx/TILESIZE*TILESIZE;
    my=my/TILESIZE*TILESIZE;
    rect.x=mx;
    rect.y=my;
    for(i=0; i<MAX_BUILDINGS; i++)
    {
        if(buildingX[i]<=mx+128 && buildingX[i]>=mx-128 &&
           buildingY[i]<=my+128 && buildingY[i]>=my-128 &&
           buildingFlags[i] & ALLEGIANCE)
        {
            pass=1;
        }
        if(type[i]==HANGAR && buildingX[i]==mx && buildingY[i]==my-64)
        {
            pass=0;
            i=MAX_BUILDINGS;
        }
    }
    if(pass==1)
    {
        if(typeToPlace==HANGAR)
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
        switch(typeToPlace)
        {
            case HANGAR:
            {
                SDL_SetTextureBlendMode(hangar_icon1, SDL_BLENDMODE_BLEND);
                SDL_SetTextureAlphaMod(hangar_icon1, 90);
                SDL_RenderCopy(rend, hangar_icon1, 0, &rect);
                SDL_SetTextureAlphaMod(hangar_icon1, 255);
                rect.y+=TILESIZE;
                SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
            }break;
            case POWER_PLANT:
            {
                SDL_SetTextureBlendMode(power_plant_icon1, SDL_BLENDMODE_BLEND);
                SDL_SetTextureAlphaMod(power_plant_icon1, 90);
                SDL_RenderCopy(rend, power_plant_icon1, 0, &rect);
                SDL_SetTextureAlphaMod(power_plant_icon1, 255);
            }break;
            case CONSTRUCTION_YARD:
            {
                SDL_SetTextureBlendMode(construction_yard1, SDL_BLENDMODE_BLEND);
                SDL_SetTextureAlphaMod(construction_yard1, 90);
                SDL_RenderCopy(rend, construction_yard1, 0, &rect);
                SDL_SetTextureAlphaMod(construction_yard1, 255);
            }break;
            case TURRET:
            {
                SDL_SetTextureBlendMode(turret_icon1, SDL_BLENDMODE_BLEND);
                SDL_SetTextureAlphaMod(turret_icon1, 90);
                SDL_RenderCopy(rend, turret_icon1, 0, &rect);
                SDL_SetTextureAlphaMod(turret_icon1, 255);
            }break;
        }
    }
    else
    {
        SDL_SetRenderDrawColor(rend, 100, 0, 0, 255);
        SDL_RenderFillRect(rend, &rect);
    }
}

void GRAPH_DrawProgressConstruction(short* constructingTime, long* flags, int* x, int* y, unsigned char* type,
                                   short* turretConstructingTime, long* turretFlags, int* turretX, int* turretY,
                                   unsigned char* constructingType)
{
    int i;
    SDL_Rect rect;
    rect.w=10;
    for(i=0; i<MAX_BUILDINGS; i++)
    {
        if(flags[i] & BEING_CONSTRUCTED && flags[i] & ALIVE)
        {
            SDL_SetRenderDrawColor(rend, 20, 30, 100, 255);
            rect.x=x[i];
            SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
            rect.h=54;
            rect.y=y[i]+5;
            SDL_RenderFillRect(rend, &rect);
            SDL_SetRenderDrawColor(rend, 20, 30, 150, 255);
            switch(type[i])
            {
                case HANGAR:
                {
                    rect.h=54-(constructingTime[i]*54/HANGAR_COST);
                }break;
                case POWER_PLANT:
                {
                    rect.h=54-(constructingTime[i]*54/POWER_PLANT_COST);
                }break;
                case CONSTRUCTION_YARD:
                {
                    rect.h=54-(constructingTime[i]*54/CONSTRUCTION_YARD_COST);
                }break;
            }
            rect.y=y[i]+(59-rect.h);
            SDL_RenderFillRect(rend, &rect);
        }
        else if((flags[i] & PRODUCING) && type[i]==HANGAR && flags[i] & ALIVE)
        {
            SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
            rect.h=54;
            rect.x=x[i];
            rect.y=y[i]+5;
            SDL_RenderFillRect(rend, &rect);
            SDL_SetRenderDrawColor(rend, 20, 30, 150, 255);
            switch(constructingType[i])
            {
                case TANK:
                {
                    rect.h=54-(constructingTime[i]*54/300);
                }break;
                case TRUCK:
                {
                    rect.h=54-(constructingTime[i]*54/200);
                }break;
            }
            rect.y+=54-rect.h;
            SDL_RenderFillRect(rend, &rect);
        }
    }
    for(i=0; i<MAX_TURRETS; i++)
    {
        if(turretFlags[i] & BEING_CONSTRUCTED && turretFlags[i] & ALIVE)
        {
            SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
            rect.x=turretX[i];
            rect.h=54;
            rect.y=turretY[i]+5;
            SDL_RenderFillRect(rend, &rect);
            SDL_SetRenderDrawColor(rend, 20, 30, 150, 255);
            rect.h=54-(turretConstructingTime[i]*54/500);
            rect.y=turretY[i]+(59-rect.h);
            SDL_RenderFillRect(rend, &rect);
        }
    }
}

void GRAPH_DrawProjectiles(Projectile* projectile)
{
    int i;
    SDL_Rect rect;
    rect.h=7;
    rect.w=7;
    for(i=1; i<MAX_PROJECTILES; i++)
    {
        if(projectile->flags[i] & ALIVE)
        {
            rect.x=projectile->x[i];
            rect.y=projectile->y[i];
            SDL_RenderCopyEx(rend, projectileTexture, 0, &rect, projectile->angle[i], 0, 0);
        }
    }
}

void GRAPH_DrawSelectingRectangle()
{
    SDL_GetMouseState(&mouseX, &mouseY);
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    if(mouseX>startX+20 || mouseY>startY+20 ||
       mouseX>startX+20 || startY>mouseY+20 ||
       startX>mouseX+20 || mouseY>startY+20 ||
       startX>mouseX+20 || startY>mouseY+20)
    {
        SDL_RenderDrawLine(rend, startX, startY, startX, mouseY);
        SDL_RenderDrawLine(rend, startX, startY, mouseX, startY);
        SDL_RenderDrawLine(rend, startX, mouseY, mouseX, mouseY);
        SDL_RenderDrawLine(rend, mouseX, startY, mouseX, mouseY);
    }
}

void GRAPH_DrawTanksBottom(int* x, int* y, double* angle, long* flags, double* turretAngle, short* type)
{
    int i;
    SDL_Rect rect;
    rect.h=64;
    rect.w=64;
    for(i=0; i<MAX_TANKS; i++)
    {
        if(flags[i] & ALIVE)
        {
            rect.x=x[i];
            rect.y=y[i];
            if(flags[i] & ALLEGIANCE)
            {
                if(type[i]==TANK)
                {
                    SDL_RenderCopyEx(rend, tankBottom1, 0, &rect, angle[i], 0, 0);
                }
                else
                {
                    SDL_RenderCopyEx(rend, truck1, 0, &rect, angle[i], 0, 0);
                }
            }
            else
            {
                if(type[i]==TANK)
                {
                    SDL_RenderCopyEx(rend, tankBottom2, 0, &rect, angle[i], 0, 0);
                }
                else
                {
                    SDL_RenderCopyEx(rend, truck2, 0, &rect, angle[i], 0, 0);
                }
            }
        }
    }
}

void GRAPH_DrawTanksTop(int* x, int* y, double* angle, long* flags, double* turretAngle, short* type, int time)
{
    int i;
    SDL_Rect rect;
    rect.h=64;
    rect.w=64;
    SDL_Point point = {32, 44};
    SDL_Point point2 = {32, 37};
    for(i=0; i<MAX_TANKS; i++)
    {
        if(flags[i] & ALIVE)
        {
            rect.x=x[i];
            rect.y=y[i];
            if(flags[i] & ALLEGIANCE)
            {
                if(type[i]==TANK)
                {
                    rect.y-=12;
                    SDL_RenderCopyEx(rend, tankGun1, 0, &rect, turretAngle[i], &point, 0);
                }
                else
                {
                    rect.y-=4;
                    SDL_RenderCopyEx(rend, truck_frontCannon1, 0, &rect, turretAngle[i], &point2, 0);
                }
            }
            else
            {
                if(type[i]==TANK)
                {
                    rect.y-=12;
                    SDL_RenderCopyEx(rend, tankGun2, 0, &rect, turretAngle[i], &point, 0);
                }
                else
                {
                    rect.y-=4;
                    SDL_RenderCopyEx(rend, truck_frontCannon2, 0, &rect, turretAngle[i], &point2, 0);
                }
            }
        }
    }
}

void GRAPH_DrawTurrets(int* x, int* y, double* angle, long* flags, int time)
{
    int i;
    SDL_Rect rect;
    SDL_Point point = {32, 32};
    for(i=1; i<MAX_TURRETS; i++)
    {
        if(flags[i] & ALIVE)
        {
            rect.h=64;
            rect.w=64;
            rect.x=x[i];
            rect.y=y[i];
            if(flags[i] & ALLEGIANCE)
            {
                if(flags[i] & BEING_CONSTRUCTED)
                {
                    SDL_SetTextureBlendMode(turret_icon1, SDL_BLENDMODE_BLEND);
                    SDL_SetTextureAlphaMod(turret_icon1, 120);
                    SDL_RenderCopy(rend, turret_icon1, 0, &rect);
                    SDL_SetTextureAlphaMod(turret_icon1, 255);
                }
                else
                {
                    SDL_RenderCopy(rend, turret1, 0, &rect);
                    SDL_RenderCopyEx(rend, turret_gun1, 0, &rect, angle[i], &point, 0);
                }
            }
            else
            {
                if(flags[i] & BEING_CONSTRUCTED)
                {
                    SDL_SetTextureBlendMode(turret_icon2, SDL_BLENDMODE_BLEND);
                    SDL_SetTextureAlphaMod(turret_icon2, 120);
                    SDL_RenderCopy(rend, turret_icon2, 0, &rect);
                    SDL_SetTextureAlphaMod(turret_icon2, 255);
                }
                else
                {
                    SDL_RenderCopy(rend, turret2, 0, &rect);
                    SDL_RenderCopyEx(rend, turret_gun2, 0, &rect, angle[i], &point, 0);
                }
            }
        }
    }
}

void GRAPH_InitDrawFrame()
{
    SDL_SetRenderDrawColor(rend, 30, 60, 10, 255);
    SDL_RenderClear(rend);
}

void GRAPH_InitWindow(SDL_Point resolution)
{
    wind = SDL_CreateWindow("wind", 600, 200, resolution.x, resolution.y, 0);
    rend = SDL_CreateRenderer(wind, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
    surface = IMG_Load("assets/tank1_kropp.gif");
    tankBottom1 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/tank1_kanon.gif");
    tankGun1 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/tank2_kropp.gif");
    tankBottom2 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/tank2_kanon.gif");
    tankGun2 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/hangar_liten_1.gif");
    hangar1 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/hangar_liten_2.gif");
    hangar2 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/hangar_liten1_door.gif");
    hangar_door1 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/hangar_liten2_door.gif");
    hangar_door2 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/hangar_inside.gif");
    hangar_floor = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/hangar_liten1_icon.gif");
    hangar_icon1 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/hangar_liten2_icon.gif");
    hangar_icon2 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/building_1.gif");
    power_plant1 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/building_2.gif");
    power_plant2 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/building_1_fan.gif");
    power_plant_fan1 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/building_2_fan.gif");
    power_plant_fan2 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/building_1_fancenter.gif");
    power_plant_fan_center1 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/building_2_fancenter.gif");
    power_plant_fan_center2 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/building_1_icon.gif");
    power_plant_icon1 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/building_2_icon.gif");
    power_plant_icon2 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/cy_1.gif");
    construction_yard1 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/cy_2.gif");
    construction_yard2 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/cy_crane_1.gif");
    construction_yard_crane1 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/cy_crane_2.gif");
    construction_yard_crane2 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/crane_block_1.gif");
    construction_yard_crane_block1 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/crane_block_2.gif");
    construction_yard_crane_block2 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/turret_1.gif");
    turret1 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/turret_2.gif");
    turret2 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/turret_gun_1.gif");
    turret_gun1 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/turret_gun_2.gif");
    turret_gun2 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/turret_icon_1.gif");
    turret_icon1 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/turret_icon_2.gif");
    turret_icon2 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/truck1_body.gif");
    truck1 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/truck2_body.gif");
    truck2 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/truck1_canon_back.gif");
    truck_backCannon1 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/truck2_canon_back.gif");
    truck_backCannon2 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/truck1_canon_front.gif");
    truck_frontCannon1 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/truck2_canon_front.gif");
    truck_frontCannon2 = SDL_CreateTextureFromSurface(rend, surface);
    surface = IMG_Load("assets/projectile.gif");
    projectileTexture = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);
}

void GRAPH_Quit()
{
    SDL_FreeSurface(textsurface);

    SDL_DestroyTexture(tankBottom1);
    SDL_DestroyTexture(tankGun1);
    SDL_DestroyTexture(tankBottom2);
    SDL_DestroyTexture(tankGun2);
    SDL_DestroyTexture(projectileTexture);

    SDL_DestroyTexture(truck1);
    SDL_DestroyTexture(truck2);
    SDL_DestroyTexture(truck_backCannon1);
    SDL_DestroyTexture(truck_backCannon2);
    SDL_DestroyTexture(truck_frontCannon1);
    SDL_DestroyTexture(truck_frontCannon2);

    SDL_DestroyTexture(turret1);
    SDL_DestroyTexture(turret2);
    SDL_DestroyTexture(turret_gun1);
    SDL_DestroyTexture(turret_gun2);
    SDL_DestroyTexture(turret_icon1);
    SDL_DestroyTexture(turret_icon2);

    SDL_DestroyTexture(hangar1);
    SDL_DestroyTexture(hangar2);
    SDL_DestroyTexture(hangar_icon1);
    SDL_DestroyTexture(hangar_icon2);
    SDL_DestroyTexture(hangar_door1);
    SDL_DestroyTexture(hangar_door2);
    SDL_DestroyTexture(hangar_floor);
    SDL_DestroyTexture(power_plant1);
    SDL_DestroyTexture(power_plant2);
    SDL_DestroyTexture(power_plant_fan1);
    SDL_DestroyTexture(power_plant_fan2);
    SDL_DestroyTexture(power_plant_fan_center1);
    SDL_DestroyTexture(power_plant_fan_center2);
    SDL_DestroyTexture(power_plant_icon1);
    SDL_DestroyTexture(power_plant_icon2);
    SDL_DestroyTexture(construction_yard1);
    SDL_DestroyTexture(construction_yard2);
    SDL_DestroyTexture(construction_yard_crane1);
    SDL_DestroyTexture(construction_yard_crane2);
    SDL_DestroyTexture(construction_yard_crane_block1);
    SDL_DestroyTexture(construction_yard_crane_block2);

    SDL_DestroyTexture(price_200);
    SDL_DestroyTexture(price_300);
    SDL_DestroyTexture(price_400);

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(wind);
    SDL_Quit();
}

void GRAPH_ShowFrame()
{
    SDL_RenderPresent(rend);
}
