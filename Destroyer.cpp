#include "Destroyer.h"

Destroyer::Destroyer() : Bateau::Bateau()
{
    this->symbol= '3';
    this->power= 1;
    this->length= 3;
    this->type= DESTROYER;

    this->pos= rand() % 225;
    this->dir= rand() % 4;

    setPosList();

}

Destroyer::~Destroyer(){}
