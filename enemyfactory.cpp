#include "enemyfactory.h"

enemyfactory::enemyfactory(QObject *parent) : QObject(parent)
{

}


void enemyfactory::slot_addenemy(int ID)
{
    if(ID == enemy::enemy_littlebrother)
    {
        ls.push_back(new enemy_littlebrother(qrand()%WIDTH,0,0,1));
    }
    if(ID == enemy::enemy_secondbrother)
    {
        ls.push_back(new enemy_secondbrother(qrand()%WIDTH,0,0,1));
    }
    if(ID == enemy::enemy_thirdbrother)
    {
        ls.push_back(new enemy_thirdbrother(qrand()%WIDTH,0,0,1));
    }
    if(ID == enemy::enemy_4thbrother)
    {
        ls.push_back(new enemy_4thbrother(qrand()%WIDTH,0,0,1));
    }
    if(ID == enemy::enemy_5thbrother)
    {
        ls.push_back(new enemy_5thbrother(WIDTH/2,0,0,0));
    }


}

void enemyfactory::slot_ehealthloss(int i,int damage)
{
    ls[i]->health -= damage;
    if(ls[i]->health <= 0)
    {
        emit appearitems(ls[i]->x,ls[i]->y);
        delete ls[i];
        ls.erase(ls.begin()+i);
    }
}
