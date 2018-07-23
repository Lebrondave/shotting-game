#ifndef ENEMY_DERIVED_H
#define ENEMY_DERIVED_H

#include "enemy.h"
#include "weapon_derived.h"

//static int enemyID;

class enemy_littlebrother : public enemy
{

public:

    enemy_littlebrother(int X,int Y,float V_x, float V_y);
};

class enemy_secondbrother : public enemy
{

public:

    enemy_secondbrother(int X,int Y,float V_x, float V_y);
};

class enemy_thirdbrother : public enemy
{

public:

    enemy_thirdbrother(int X,int Y,float V_x, float V_y);
};

class enemy_4thbrother : public enemy
{

public:

    enemy_4thbrother(int X,int Y,float V_x, float V_y);
};

class enemy_5thbrother : public enemy
{

public:

    enemy_5thbrother(int X,int Y,float V_x, float V_y);
};








#endif // ENEMY_DERIVED_H
