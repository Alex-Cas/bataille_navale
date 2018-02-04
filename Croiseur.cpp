#include "Croiseur.h"

Croiseur::Croiseur() : Bateau::Bateau()
{
    this->symbol= '5';
    this->power= 2;
    this->length= 5;
    this->type= CROISEUR;

    this->pos= rand() % 225;
    this->dir= rand() % 4;

    setPosList();

}

Croiseur::~Croiseur(){}
