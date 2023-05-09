#include "TacticB.h"
TacticB new_TacticB()
{
    TacticB this=(TacticB)malloc(sizeof(struct TacticB));
    this->super=new_Tactic();
    this->fight=&fightB;
    this->super->fight=&fightB;
    this->delete=&deleteTacticB;
    return this;
}

int fightB(const TacticB this)
{
    this->super->power=(rand()%1000)+0;
    return this->super->power;
}

void deleteTacticB(const TacticB this)
{
    this->super->delete(this->super);
    free(this);
}