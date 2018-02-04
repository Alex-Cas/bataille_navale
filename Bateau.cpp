#include "Bateau.h"


Bateau::Bateau()
{

}

Bateau::~Bateau(){}


void Bateau::setPosList(){

    bool a= true;
    posList.clear();

    switch(dir){

        case DIR_N:

            //Si les cases au dessus ne sortent pas de la grille
            for(int i=1; i<length; i++){
                if((pos-15*i) < 0) a= false;
            }

            if(a){
                for(int i=0; i<length; i++){
                    posList.push_back(pos-15*i);
                }
            }

            break;

        case DIR_E:

            //S'il n'y a pas de retour a la ligne sur la longueur du bateau
            for(int i=1; i<length; i++){
                if((pos+i)%15 == 0) a= false;
            }

            if(a){
                for(int i=0; i<length; i++){
                    posList.push_back(pos+i);
                }
            }
            break;

        case DIR_S:

            //Si les cases au dessous ne sortent pas de la grille
            for(int i=1; i<length; i++){
                if((pos+15*i) > 224) a= false;
            }

            if(a){
                for(int i=0; i<length; i++){
                    posList.push_back(pos+15*i);
                }
            }

            break;

        case DIR_W:

            //S'il n'y a pas de retour a la ligne sur la longueur du bateau
            for(int i=1; i<length; i++){
                if((pos-i+1)%15 == 0) a= false;
            }

            if(a){
                for(int i=0; i<length; i++){
                    posList.push_back(pos-i);
                }
            }
            break;

    }

}

void Bateau::setPos(int a){

    this->pos= a;
    setPosList();


}

void Bateau::setDir(int a){

    this->dir= a;
    setPosList();
}

//Retire une position de la liste
//Argument: place dans le vecteur
void Bateau::removePos(int a){

    posList.erase(posList.begin() + a);

}
