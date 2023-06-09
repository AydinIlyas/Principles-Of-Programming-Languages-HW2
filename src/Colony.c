#include "Colony.h"
#include "TacticA.h"
#include "TacticB.h"
#include "ProductionA.h"
#include "ProductionB.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
Colony new_Colony(char symbol, int population, char tactic, char production)
{
    Colony this = (Colony)malloc(sizeof(struct Colony));
    this->symbol = symbol;
    this->population = population;
    this->food = population * population;
    this->tacticCh = tactic;
    if (this->tacticCh == 'a')
        this->Tactic = new_TacticA();
    else
        this->Tactic = new_TacticB();

    this->productionCh = production;
    if (this->productionCh == 'a')
        this->Production = new_ProductionA();
    else
        this->Production = new_ProductionB();

    this->wins = 0;
    this->loses = 0;
    this->alive=true;
    this->fight = &fightColonies;
    this->produce=&produceColony;
    this->checkAndReset = &checkAndReset;
    this->roundImpact=&roundImpact;
    this->delete = &deleteColony;
    this->toString = &toString;
}
void effects(const Colony winner, const Colony loser, int difference)
{
    winner->wins += 1;
    loser->loses += 1;
    loser->population -= ((double)difference / 1000) * loser->population;
    int tmpFood = loser->food * ((double)difference / 1000);
    loser->food -= tmpFood;
    winner->food += tmpFood;
}
bool checkAndReset(const Colony this)
{
    if(!this->alive)return false;
    if (this->food <= 0 || this->population <= 0)
    {
        this->alive=false;
        return true;
    }
    return false;
}
void fightColonies(const Colony first, const Colony second)
{
    if (!first->alive||!second->alive)
    {
        return;
    }

    int scoreF = first->tacticCh == 'a' ? ((TacticA)first->Tactic)->super->fight(first->Tactic) : ((TacticB)first->Tactic)->super->fight(first->Tactic);
    int scoreS = second->tacticCh == 'a' ? ((TacticA)second->Tactic)->super->fight(second->Tactic) : ((TacticB)second->Tactic)->super->fight(second->Tactic);
    int difference = scoreF - scoreS;
    if (difference > 0)
    {
        effects(first, second, difference);
    }
    else if (difference < 0)
    {
        effects(second, first, abs(difference));
    }
    else
    {
        if (first->population > second->population)
        {
            first->wins += 1;
            second->loses += 1;
        }
        else if (first->population < second->population)
        {
            first->loses += 1;
            second->wins += 1;
        }
        else
        {
            int random = rand() % 2;
            if (random == 1)
            {
                first->wins += 1;
                second->loses += 1;
            }
            else
            {
                second->wins += 1;
                first->loses += 1;
            }
        }
    }
}
void roundImpact(const Colony this)
{
    if(!this->alive)return;
    this->food-=this->population*2; 
    this->population=(double)this->population*12/10;
}
void produceColony(const Colony this)
{
    if(!this->alive)return;
    if (this->productionCh == 'a')
    {
        this->food += ((ProductionA)this->Production)->super->produce(this->Production);
    }
    else
    {
        this->food += ((ProductionB)this->Production)->super->produce(this->Production);
    }
}
int LengthInt(int num)
{
    int length = floor(log10(abs(num))) + 1;
    return length;
}
char* toString(const Colony this)
{
    int length = 0;
    length += 1;
    char *str;
    if (this->alive)
    {
        length += LengthInt(this->population);
        length += LengthInt(this->food);
        if(this->wins==0)length+=1;
        else length += LengthInt(this->wins);
        if(this->loses==0) length+=1;
        else length += LengthInt(this->loses);
        
        str = (char *)malloc(sizeof(char)*length+10);
        sprintf(str, "%c\t\t%d\t\t%d\t\t%d\t\t%d\n", this->symbol, this->population, this->food, this->wins, this->loses);

    }
    else{
        length+=8;
        const char* lost="--"; 
        str = (char *)malloc(sizeof(char) * length+10);
        sprintf(str, "%c\t\t%s\t\t%s\t\t%s\t\t%s\n", this->symbol, lost,lost,lost,lost);
    }
    

    
    return str;
}

void deleteColony(const Colony this)
{
    if (this == NULL)
        return;

    if (this->tacticCh == 'a')
        ((TacticA)this->Tactic)->delete (this->Tactic);
    else
        ((TacticB)this->Tactic)->delete (this->Tactic);

    if (this->productionCh == 'a')
        ((ProductionA)this->Production)->delete (this->Production);
    else
        ((ProductionB)this->Production)->delete (this->Production);
    free(this);
}