#include "Sous-Marin.h"

Sous_Marin::Sous_Marin() : Bateau::Bateau()
{
    this->symbol= '1';
    this->power= 1;
    this->length= 1;
    this->type= SOUS_MARIN;

    this->pos= rand() % 225;
    this->dir= rand() % 4;

    setPosList();

}

Sous_Marin::~Sous_Marin(){}
