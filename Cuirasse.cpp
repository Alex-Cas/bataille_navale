#include "Cuirasse.h"

Cuirasse::Cuirasse() : Bateau::Bateau()
{
    this->symbol= '7';
    this->power= 3;
    this->length= 7;
    this->type= CUIRASSE;

    this->pos= rand() % 225;
    this->dir= rand() % 4;

    setPosList();

}

Cuirasse::~Cuirasse(){}
