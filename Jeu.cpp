#include "Jeu.h"
#include "Sous-Marin.h"
#include "Destroyer.h"
#include "Croiseur.h"
#include "Cuirasse.h"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ENTER 13
#define KEY_BACKSPACE 8

void Jeu::gotoligcol(int lig,int col){
    COORD mycoord;
    mycoord.X = col;
    mycoord.Y = lig;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), mycoord);
}

//Convertit des coordonnées de la console brut
//en coordonnée 1d de la grille
int Jeu::_2dto1d(int _posx, int _posy, int grid){

    int borderLeft, borderUp;

    if(grid==1){
        borderLeft= this->mainGrid_y+4;
        borderUp= this->mainGrid_x+2;
    }else if(grid==2){
        borderLeft= this->secondGrid_y+4;
        borderUp= this->secondGrid_x+2;
    }else return 0;

    _posx-= borderUp;
    _posy-= borderLeft;

    int posx= _posx/2;
    int posy= _posy/3;

    int pos= posx*15 + posy;

    return pos;
}

//Convertit une coordonnée 1d en 2d (axe x)
int Jeu::_1dto2d_x(int pos, int grid){

    int borderUp=0;

    if(grid == 1){
        borderUp= this->mainGrid_x+2;
    }else if(grid == 2){
        borderUp= this->secondGrid_x+2;
    }

    int posx= borderUp + 2 * (pos / 15);

    return posx;
}

//Convertit une coordonnée 1d en 2d (axe y)
int Jeu::_1dto2d_y(int pos, int grid){

    int borderLeft=0;

    if(grid == 1){
        borderLeft= this->mainGrid_y+4;
    }else if(grid == 2){
        borderLeft= this->secondGrid_y+4;
    }

    int posy= borderLeft + 3 * (pos % 15);

    return posy;
}


Jeu::Jeu(){

    //Boucle jeu
    jeu();
}

Jeu::~Jeu(){}

//Cette fonction va permettre au joueur de choisir un bateau
//et de choisir sa cible
void Jeu::round(){
    //Ascii du curseur
    //char cursor = (char)(177);

    //Bords de la grille
    int borderLeft= this->mainGrid_y+4;
    int borderRight= this->mainGrid_y+48;
    int borderUp= this->mainGrid_x+2;
    int borderDown= this->mainGrid_x+32;

    //Position défaut
    int posx= borderUp;
    int posy= borderLeft;

    //Place le curseur
    gotoligcol(posx, posy);

    int choix=0, power=0, winner=0;
    bool p1Jumped= false, p2Jumped= false;

    //Condition du while
    bool a=false;

    while(!a){

        if(((p1Jumped == true) && (this->alt == 0)) || ((p2Jumped == true) && (this->alt == 1))){

            if((p1Jumped == true) && (this->alt == 0)) p1Jumped= false;
            if((p2Jumped == true) && (this->alt == 1)) p2Jumped= false;

            switchPlayer();

        }


        switch(getch()){

            case KEY_LEFT:

                if(posy-3 >= borderLeft){
                    posy-=3;
                    gotoligcol(posx, posy);
                }

                break;

            case KEY_RIGHT:

                if(posy+3 <= borderRight){
                    posy+=3;
                    gotoligcol(posx, posy);
                }

                break;

            case KEY_UP:

                if(posx-2 >= borderUp){
                    posx-=2;
                    gotoligcol(posx, posy);
                }

                break;

            case KEY_DOWN:

                if(posx+2 < borderDown){
                    posx+=2;
                    gotoligcol(posx, posy);
                }

                break;

            case KEY_ENTER:

                if(selectBoat(posx, posy)){

                    choix = actionChoice();

                    if(choix == 1){

                        power= this->boatsList[this->alt][this->selectedBoat].getPower();

                        if(this->boatsList[this->alt][this->selectedBoat].getType() == DESTROYER) shootChoice(4);

                        shootChoice(power);

                        winner= playerVictory();

                        if(winner > 0){
                            system("cls");
                            std::cout<<"Le joueur "<<winner<<" a gagné !";
                            getch();
                            a= true;
                        }

                        if(!a){

                            switchPlayer();

                            posx= borderUp;
                            posy= borderLeft;
                            gotoligcol(posx, posy);
                        }

                    }

                    if(choix == 2){

                        if(shift()){
                            //Si on a réussi a bouger un bateau
                            switchPlayer();
                        }else{
                            //Sinon le joueur recommence
                            clearMainGrid();
                            displayBoats();
                        }
                    }

                    if(choix == 3){

                        if(turn(turnChoice())){

                            //Cuirassé prend 2 tours a tourner
                            if(this->alt == 0){
                                if(this->boatsList[0][this->selectedBoat].getType()==CUIRASSE) p1Jumped= true;
                            }
                            if(this->alt == 1){
                                if(this->boatsList[1][this->selectedBoat].getType()==CUIRASSE) p2Jumped= true;
                            }
                            //Si on a réussi a tourner un bateau
                            switchPlayer();

                        }else{
                            //Sinon le joueur recommence
                            clearMainGrid();
                            displayBoats();
                        }

                    }

                    if(choix == 0) gotoligcol(posx, posy);

                }

                break;

            case KEY_BACKSPACE:

                a= true;

                break;
        }

    }

}


//Va permettre au joueur de choisir son action
//apres avoir selectionné son bateau
int Jeu::actionChoice(){

    //Position du texte pour choisir les actions
    int posx= this->posActionChoice_x;
    int posy= this->posActionChoice_y-1;
    int borderUp= posx;
    int borderDown= posx+4;

    //Affichage du curseur
    gotoligcol(posx ,posy);
    std::cout<<"#";
    gotoligcol(posx, posy);

    int choix=1; // 1:Tirer, 2:Deplacer, 3:Tourner, 0:Annuler

    //Condition de la boucle
    bool a=false;

    while(!a){
        switch(getch()){

            case KEY_DOWN:
                if(posx+2 <= borderDown){
                    std::cout<<" ";
                    posx+=2;
                    gotoligcol(posx, posy);
                    std::cout<<"#";
                    gotoligcol(posx, posy);

                    choix++;
                }

                break;

            case KEY_UP:
                if(posx-2 >= borderUp){
                    std::cout<<" ";
                    posx-=2;
                    gotoligcol(posx, posy);
                    std::cout<<"#";
                    gotoligcol(posx, posy);

                    choix--;
                }
                break;

            case KEY_ENTER:
                a=true;
                break;

            case KEY_BACKSPACE:
                std::cout<<" ";
                choix=0; //Choix 0 pour annuler
                a=true;
                break;
        }
    }

    //On efface le curseur
    gotoligcol(posx ,posy);
    std::cout<<" ";

    return choix;
}

//Choix pour tourner
int Jeu::turnChoice(){

    //Position du texte pour choisir les actions
    int posx= this->posActionChoice_x+8;
    int posy= this->posActionChoice_y-1;


    gotoligcol(posx, posy+1);
    std::cout<<"Gauche";
    gotoligcol(posx+2, posy+1);
    std::cout<<"Droite";


    int borderUp= posx;
    int borderDown= posx+2;

    //Affichage du curseur
    gotoligcol(posx ,posy);
    std::cout<<"#";
    gotoligcol(posx, posy);

    int choix=-1; // 1:Tirer, 2:Deplacer, 3:Tourner, 0:Annuler

    //Condition de la boucle
    bool a=false;

    while(!a){
        switch(getch()){

            case KEY_DOWN:
                if(posx+2 <= borderDown){
                    std::cout<<" ";
                    posx+=2;
                    gotoligcol(posx, posy);
                    std::cout<<"#";
                    gotoligcol(posx, posy);

                    choix+=2;
                }

                break;

            case KEY_UP:
                if(posx-2 >= borderUp){
                    std::cout<<" ";
                    posx-=2;
                    gotoligcol(posx, posy);
                    std::cout<<"#";
                    gotoligcol(posx, posy);

                    choix-=2;
                }
                break;

            case KEY_ENTER:
                a=true;
                break;

            case KEY_BACKSPACE:
                std::cout<<" ";
                choix=0; //Choix 0 pour annuler
                a=true;
                break;
        }
    }

    //On efface le curseur
    gotoligcol(posx ,posy);
    std::cout<<" ";

    gotoligcol(this->posActionChoice_x+8, this->posActionChoice_y);
    std::cout<<"      ";
    gotoligcol(this->posActionChoice_x+10,this->posActionChoice_y);
    std::cout<<"      ";

    return choix;

}

//Va permettre au joueur de choisir une zone de tir
//dans la grille de tir
//L'argument est la puissance de tir du bateau précedemment selectionné (1, 2, 3)
void Jeu::shootChoice(int power){

    //Ascii du curseur
    char cursor = (char)(177);

    //Change de grille
    int borderRight= this->secondGrid_y+48;
    int borderLeft= this->secondGrid_y+4;
    int borderUp= this->secondGrid_x+2;
    int borderDown= this->secondGrid_x+30;

    //Change la position du curseur
    int posx= borderUp;
    int posy= borderLeft;
    //Position en 1D
    int pos=0;

    //Liste des cases pour tirer
    std::vector<int> shootList;

    //Affiche le curseur
    for(int i=0; i<power; i++){
        for(int j=0; j<power; j++){
            gotoligcol(posx + (2*i), posy + (3*j) -1);
            std::cout<<cursor<<cursor;
        }
    }

    bool a=false;

    while(!a){

        switch(getch()){

            case KEY_LEFT:

                if(posy - 3 >= borderLeft){

                    posy-=3;

                    //Curseur de la taille d'un carré de longueur
                    //donné par la variable power (passée en argument)
                    for(int i=0; i<power; i++){
                        for(int j=0; j<power; j++){
                            //Va remplir les cases
                            gotoligcol(posx + 2*i, posy + (3*j) - 1);
                            std::cout<<cursor<<cursor;
                        }
                        //Va effacer les cases
                        gotoligcol(posx + 2*i, posy + (3*power) - 1);
                        std::cout<<"  ";
                    }

                    gotoligcol(posx, posy);
                }

                break;


            case KEY_RIGHT:

                if(posy + (3*power) <= borderRight){

                    posy+=3;

                    //Curseur de la taille d'un carré de longueur
                    //donné par la variable power (passée en argument)
                    for(int i=0; i<power; i++){
                        for(int j=0; j<power; j++){
                            //Va remplir les cases
                            gotoligcol(posx + 2*i, posy + (3*j) - 1);
                            std::cout<<cursor<<cursor;
                        }
                        //Va effacer les cases
                        gotoligcol(posx + 2*i, posy - 4);
                        std::cout<<"  ";
                    }

                    gotoligcol(posx, posy);
                }

                break;

            case KEY_UP:

                if(posx - 2 >= borderUp){

                    posx-=2;

                    //Curseur de la taille d'un carré de longueur
                    //donné par la variable power (passée en argument)
                    for(int i=0; i<power; i++){
                        for(int j=0; j<power; j++){
                            //Va remplir les cases
                            gotoligcol(posx + 2*j, posy + (3*i) - 1);
                            std::cout<<cursor<<cursor;
                        }
                        //Va effacer les cases
                        gotoligcol(posx + (2*power), posy + (3*i) - 1);
                        std::cout<<"  ";
                    }

                    gotoligcol(posx, posy);
                }

                break;

            case KEY_DOWN:

                if(posx + (2*power) <= borderDown){

                    posx+=2;

                    //Curseur de la taille d'un carré de longueur
                    //donné par la variable power (passée en argument)
                    for(int i=0; i<power; i++){
                        for(int j=0; j<power; j++){
                            //Va remplir les cases
                            gotoligcol(posx + 2*j, posy + (3*i) - 1);
                            std::cout<<cursor<<cursor;
                        }
                        //Va effacer les cases
                        gotoligcol(posx - 2, posy + (3*i) - 1);
                        std::cout<<"  ";
                    }

                    gotoligcol(posx, posy);
                }

                break;

            case KEY_ENTER:

                if(power == 4){
                    flare(posx, posy);
                    a=true;
                }else{

                //Les positions posx et posy sont en 2D pour l'instant
                pos= _2dto1d(posx, posy, 2);
                shootList= fillSquare(pos, power);
                shoot(shootList);

                a=true;
                }

                break;

            case KEY_BACKSPACE:

                //Efface le curseur
                for(int i=0; i<power; i++){
                    for(int j=0; j<power; j++){
                        gotoligcol(posx + (2*i), posy + (3*j) -1);
                        std::cout<<"  ";
                    }
                }

                //Sors de la selection de tir
                a=true;

                break;
        }

    }

}

//Fusee eclairante du destroyer
void Jeu::flare(int posx, int posy){

    clearSecondGrid();
    displayTouched();


    int pos= _2dto1d(posx, posy, 2);
    std::vector<int> square = fillSquare(pos, 4);

    int player= (this->alt + 1)%2;

    for(unsigned int i=0; i<square.size(); i++){
        for(unsigned int j=0; j<this->boatsList[player].size(); j++){
            for(unsigned int k=0; k<this->boatsList[player][j].getPosList().size(); k++){

                if(square[i] == this->boatsList[player][j].getPosList()[k]){

                     //Conversion de la position 1d -> 2d
                    pos= this->boatsList[player][j].getPosList()[k];
                    posx= _1dto2d_x(pos, 2);
                    posy= _1dto2d_y(pos, 2);

                    gotoligcol(posx, posy - 1);
                    std::cout<<"OO";

                }
            }
        }
    }

    getch();

    clearSecondGrid();
    displayTouched();

}

//Cette fonction va gérer les bateaux touchés
//a partir d'un liste de positions donnée en argument
void Jeu::shoot(std::vector<int> shootList){


    //On va tirer sur l'autre joueur
    int player = (this->alt + 1) % 2;


    for(unsigned int i=0; i<shootList.size(); i++){
        for(unsigned int j=0; j<this->boatsList[player].size(); j++){
            for(unsigned int k=0; k<this->boatsList[player][j].getPosList().size(); k++){
                if(shootList[i] == this->boatsList[player][j].getPosList()[k]){

                    this->boatsList[player][j].removePos(k);
                    this->boatsList[player][j].addTouched(shootList[i]);

                }
            }
        }
    }


}


//Va déplacer le bateau selectionné d'une case vers sa direction
//Renvoies true si le bateau a bien été bougé (fin du tour)
//Renvoies false si le bateau a pas été bougé (le joueur peut refaire son tour)
bool Jeu::shift(){

    int newPos=0, oldPos;
    bool collision= false;

    //On stocke le bateau sélectionné
    Bateau boat = this->boatsList[this->alt][this->selectedBoat];
    //On le retire de la liste (pour verifier les collisions)
    this->boatsList[this->alt].erase(this->boatsList[this->alt].begin() + selectedBoat);

    //On stocke sa position initiale
    oldPos= boat.getPos();


    switch(boat.getDir()){

            case DIR_W:
                newPos= oldPos - 1;
                break;

            case DIR_E:
                newPos= oldPos + 1;
                break;

            case DIR_N:
                newPos= oldPos - 15;
                break;

            case DIR_S:
                newPos= oldPos + 15;
                break;

    }

    //On refait les positions du bateau
    boat.setPos(newPos);

    //Si les nouvelles positions ont bien été crées
    if (!boat.getPosList().empty()){

        for (unsigned int i=0; i<boat.getPosList().size(); i++){

            //Si une case se superpose avec un autre bateau, alors il y a collisions
            if (isCollision(boat.getPosList()[i])== true) collision= true;
        }
    }else collision= true;


    //Si les nouvelles positions n'ont pas été crées, ou qu'il y a eu collision
    //alors, on remet le bateau a sa position initiale
    if (collision) boat.setPos(oldPos);


    //On replace le bateau dans la liste
    this->boatsList[this->alt].push_back(boat);

    return (!collision);
}


//Va changer la direction du bateau (ne peut seulement faire
//un quart de tours a chaque fois)
bool Jeu::turn(int dir){

    int newDir, oldDir;
    bool collision= false;

    //On stocke le bateau sélectionné
    Bateau boat = this->boatsList[this->alt][this->selectedBoat];
    //On le retire de la liste (pour verifier les collisions)
    this->boatsList[this->alt].erase(this->boatsList[this->alt].begin() + selectedBoat);

    //On stocke l'ancienne direction et la nouvelle
    oldDir= boat.getDir();
    newDir= (oldDir + dir) % 4;

    //Si on passe de 0 à -1, on met la direction vers l'ouest (3)
    if(newDir==-1)newDir= 3;

    //On refait les positions du bateau
    boat.setDir(newDir);

    //Si les nouvelles positions ont bien été crées
    if (!boat.getPosList().empty()){

        for (unsigned int i=0; i<boat.getPosList().size(); i++){

            //Si une case se superpose avec un autre bateau, alors il y a collisions
            if (isCollision(boat.getPosList()[i])== true) collision= true;
        }
    }else collision= true;


    //Si les nouvelles positions n'ont pas été crées, ou qu'il y a eu collision
    //alors, on remet le bateau a sa position initiale
    if (collision) boat.setDir(oldDir);


    //On replace le bateau dans la liste
    this->boatsList[this->alt].push_back(boat);

    return (!collision);
}


//Va afficher les grilles et du texte complémentaire
void Jeu::displayLayout(){

    //Choisir grille de jeu / grille de tir
    this->mainGrid_x=2;
    this->mainGrid_y=2;
    this->secondGrid_x=2;
    this->secondGrid_y=75;

    //Liste des grilles
    displayGrid(this->mainGrid_x,this->mainGrid_y);
    displayGrid(this->secondGrid_x,this->secondGrid_y);

    //Texte entre les deux grilles
    this->posActionChoice_x= this->mainGrid_x+2;
    this->posActionChoice_y= this->mainGrid_y+52;
    gotoligcol(this->posActionChoice_x, this->posActionChoice_y);
    std::cout<< "Tirer";
    gotoligcol(this->posActionChoice_x+2, this->posActionChoice_y);
    std::cout<< "Deplacer";
    gotoligcol(this->posActionChoice_x+4, this->posActionChoice_y);
    std::cout<< "Tourner";
}

//Trace une grille de jeu avec les labels
//Une grille est de taille 48*32
//Les arguments définissent l'endroit du coin haut/gauche de la grille
void Jeu::displayGrid(int lig, int col){

//Grid
    //Point d'ou tracer la grille
    gotoligcol(lig, col);
    int k=1; //Variable pour retour a la ligne

    for(int i=0; i<16; i++){
        for (int j=0; j<16; j++){
            std::cout<<"  |";
        }
        gotoligcol(lig+k, col); //On revient a la ligne, a la prochaine colonne

        for (int j=0; j<16; j++){
            std::cout<<"--|";
        }
        gotoligcol(lig+k+1, col); //On revient a la ligne, a la prochaine colonne

        k+=2;
    }

//Labels
    //Horizontal
    k=1;
    gotoligcol(lig, col+1);
    for(int i=1; i<16; i++){
        gotoligcol(lig, col+i+2*k); //Il faut sauter 3 cases a chaque fois
        std::cout<<k-1; //Va tracer les chiffres de 0 a 14
        k++;
    }
    //Vertical (ascii table : A=65 O=79 | a=97 o=111)
    k=65; //On trace en majuscule
    for(int i=1; i<16; i++){
        gotoligcol(lig+2*i, col); //Il faut sauter 2 cases a chaque fois
        std::cout<<(char)(k); //Va tracer les lettres (majuscules) de A a O
        k++;
    }
}

//Selectionne un bateau et le stocke dans
//l'attribut privé de la classe Jeu : selectedBoat
//retourne True si il y a bien un bateau a la position donnée
//False sinon
bool Jeu::selectBoat(int posx, int posy){

    int pos= _2dto1d(posx, posy, 1);

    for(unsigned int i=0; i<this->boatsList[this->alt].size(); i++){

        for(unsigned int j=0; j<this->boatsList[this->alt][i].getPosList().size(); j++){
            //On regarde si cette case contient un bateau
            if(pos == this->boatsList[this->alt][i].getPosList()[j]){
                //Si on trouve un bateau, on le stocke dans selectedBoat et on termine
                this->selectedBoat= i;
                return true;

           }
        }
    }
    return false;
}


//Initialise la liste des bateaux du jeu
//et les place dans la grille
void Jeu::initBoats(){

    //Sous_Marin boat;
    srand(time(NULL)); // constructeur

    this->boatsList.clear();

    std::vector<Bateau> temp;

    this->boatsList.push_back(temp);
    this->boatsList.push_back(temp);

    for(int i=0; i<2; i++){
        while(this->boatsList[i].size() < 4){
            {
                Sous_Marin boat;
                placeBoat(boat);
            }
        }
        while(this->boatsList[i].size() < 7){
            {
                Destroyer boat;
                placeBoat(boat);
            }
        }
        while(this->boatsList[i].size() < 9){
            {
                Croiseur boat;
                placeBoat(boat);
            }
        }
        while(this->boatsList[i].size() < 10){
            {
                Cuirasse boat;
                placeBoat(boat);
            }
        }
        alternate();
    }
}


//Cette méthode va placer un bateau dans la grille
void Jeu::placeBoat(Bateau boat){

    bool isEmpty= true;

    //Il faut que le bateau soit bien initialisé
    if(boat.getPosList().size()!=0){

        for(unsigned i=0; i<boat.getPosList().size(); i++){

                //On verifie les collisions pour toute la longueur du bateau
                if(isCollision(boat.getPosList()[i])) isEmpty= false;
        }

        if(isEmpty== true) this->boatsList[this->alt].push_back(boat);
    }

}

//Cette fonction va regarder si une position est déja occupée par un autre bateau
//Renvoi True si oui, False si non
bool Jeu::isCollision (int pos){

    for(unsigned int i=0; i<this->boatsList[this->alt].size(); i++){

        for(unsigned int j=0; j<this->boatsList[this->alt][i].getPosList().size(); j++){

            if (pos== this->boatsList[this->alt][i].getPosList()[j]) return true;
        }

        for(unsigned int j=0; i<this->boatsList[this->alt][i].getTouchedList().size(); j++){

            if(pos== this->boatsList[this->alt][i].getTouchedList()[j]) return true;
        }
    }

    return false;
}


//Cette méthode va remplir un carré en 1D avec une position
//initiale et la longueur d'un coté (power)
//Retourne un vecteur conenant les positions 1D du carré créé
std::vector<int> Jeu::fillSquare(int pos, int power){

    std::vector<int> temp;

    for(int i=0; i<power; i++){
        for(int j=0; j<power; j++){
            temp.push_back(pos + 15*i + j);
        }
    }
    return temp;
}


//Cette fonction va afficher les symboles des bateaux
//a leur emplacements
void Jeu::displayBoats(){

    int posx, posy, pos;

    //Couleurs
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    //Va stocker le symbole du bateau
    char c;

    for(unsigned int i=0; i<this->boatsList[this->alt].size(); i++){

        for(unsigned int j=0; j<this->boatsList[this->alt][i].getPosList().size(); j++){

            c= this->boatsList[this->alt][i].getSymbol();

            //Conversion de la position 1d -> 2d
            pos= this->boatsList[this->alt][i].getPosList()[j];
            //On traite la grille numero 1 ici
            posx= _1dto2d_x(pos, 1);
            posy= _1dto2d_y(pos, 1);

            gotoligcol(posx, posy-1);

            switch(this->boatsList[this->alt][i].getType()){

                case SOUS_MARIN:
                    SetConsoleTextAttribute(hConsole, 10);
                    break;

                case DESTROYER:
                    SetConsoleTextAttribute(hConsole, 12);
                    break;

                case CROISEUR:
                    SetConsoleTextAttribute(hConsole, 11);
                    break;

                case CUIRASSE:
                    SetConsoleTextAttribute(hConsole, 9);
                    break;

            }


            std::cout<<c<<c;
            SetConsoleTextAttribute(hConsole, 7);

        }
    }
}

//Cette fonction va afficher les bateaux touchés
//dans la grille 2
void Jeu::displayTouched(){

    int posx, posy, pos;
    int player = (this->alt + 1) % 2;

    //Va stocker le symbole du bateau
    char c= 'X';

    for(unsigned int i=0; i<this->boatsList[player].size(); i++){

        for(unsigned int j=0; j<this->boatsList[player][i].getTouchedList().size(); j++){

            //Conversion de la position 1d -> 2d
            pos= this->boatsList[player][i].getTouchedList()[j];
            posx= _1dto2d_x(pos, 2);
            posy= _1dto2d_y(pos, 2);

            gotoligcol(posx, posy-1);
            std::cout<<c<<c;

        }
    }

    for(unsigned int i=0; i<this->boatsList[this->alt].size(); i++){

        for(unsigned int j=0; j<this->boatsList[this->alt][i].getTouchedList().size(); j++){

            //Conversion de la position 1d -> 2d
            pos= this->boatsList[this->alt][i].getTouchedList()[j];
            posx= _1dto2d_x(pos, 1);
            posy= _1dto2d_y(pos, 1);

            gotoligcol(posx, posy-1);
            std::cout<<c<<c;

        }
    }

}


//Cette fonction va nettoyer la grille 1
void Jeu::clearMainGrid(){

    int posx, posy;

    //Va stocker le symbole du bateau
    char c= ' ';

    for(int i=0; i<255; i++){

            //Conversion de la position 1d -> 2d grille 1
            posx= _1dto2d_x(i, 1);
            posy= _1dto2d_y(i, 1);

            gotoligcol(posx, posy-1);
            std::cout<<c<<c;

    }

}

//Cette fonction va nettoyer la grille 2
void Jeu::clearSecondGrid(){

    int posx, posy;

    for(int i=0; i<255; i++){

            //Conversion de la position 1d -> 2d grille 2
            posx= _1dto2d_x(i, 2);
            posy= _1dto2d_y(i, 2);

            gotoligcol(posx, posy-1);
            std::cout<<"  ";
    }

}

//Animation pour finir le tour
void Jeu::switchPlayer(){

    bool a=false;

    clearMainGrid();
    clearSecondGrid();
    displayTouched();
    displayBoats();


    gotoligcol(this->posActionChoice_x+8, this->posActionChoice_y-1);
    std::cout<<"#Finir le tour";
    gotoligcol(this->posActionChoice_x+8, this->posActionChoice_y-1);

    while(!a){
        if(getch()==KEY_ENTER)a= true;
    }

    gotoligcol(this->posActionChoice_x+8, this->posActionChoice_y-1);
    std::cout<<"              ";

    alternate();
    clearMainGrid();
    clearSecondGrid();
    displayTouched();
    displayBoats();

}

//Condition de victoire
//Renvoies le numero du joueur gagnant
int Jeu::playerVictory(){

    bool a;

    for(int i=0; i<2; i++){

        a= true;

        for(unsigned int j=0; j<this->boatsList[i].size(); j++){

            if(!(this->boatsList[i][j].getPosList().empty())) a= false;
        }

        if(a){
            //Si vecteur 0 est vide, alors 2 a gagné
            if(i==0) return 2;
            if(i==1) return 1;
        }
    }

    return 0;
}


//Affiche le menu principal
void Jeu::messageMenu(){

    posMenuChoice_x = 25;
    posMenuChoice_y = 65;


    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 9);
    std::cout <<"\n\n\n"<< std::endl << std::endl;
    std::cout << "                                                       ====================================================\n" << std::endl;
    std::cout << "                                                                  BIENVENUE DANS LA BATAILLE NAVALE        \n " << std::endl;
    std::cout << "                                                       ====================================================\n" << std::endl;

    std::cout << "                                                _______       ______    __________   _____        __   __         __"<< std::endl;
    std::cout << "                                               |       \\   /        | |   ________| |     \\      |  | |  |       |  |"<< std::endl;
    std::cout << "                                               |        \\ /         | |  |          |  |\\  \\     |  | |  |       |  |"<< std::endl;
    std::cout << "                                               |                    | |  |          |  | \\  \\    |  | |  |       |  |"<< std::endl;
    std::cout << "                                               |   |  \\     /   |   | |  |________  |  |  \\  \\   |  | |  |       |  | "<< std::endl;
    std::cout << "                                               |   |   \\   /    |   | |   ________| |  |   \\  \\  |  | |  |       |  |"<< std::endl;
    std::cout << "                                               |   |    \\ /     |   | |  |          |  |    \\  \\ |  | |  |       |  | "<< std::endl;
    std::cout << "                                               |   |            |   | |  |          |  |     \\  \\   | |  |_______|  |"<< std::endl;
    std::cout << "                                               |   |            |   | |  |________  |  |      \\     | |             |"<< std::endl;
    std::cout << "                                               |__ |            |__ | |___________| |__|       \\____| |_____________|    \n\n"<< std::endl;

    SetConsoleTextAttribute(hConsole, 7);
    std::cout << "                                                             * * * * * * * * * * * * * * * * *" << std::endl << std::endl;
    std::cout << "                                                             *   1. Jouer une partie         *" << std::endl << std::endl;
    std::cout << "                                                             *                               *" << std::endl << std::endl;
    std::cout << "                                                             *                               *" << std::endl << std::endl;
    std::cout << "                                                             *   2. Charger une partie       *" << std::endl << std::endl;
    std::cout << "                                                             *                               *" << std::endl << std::endl;
    std::cout << "                                                             *                               *" << std::endl << std::endl;
    std::cout << "                                                             *   3. Aide                     *" << std::endl << std::endl;
    std::cout << "                                                             *                               *" << std::endl << std::endl;
    std::cout << "                                                             *                               *" << std::endl << std::endl;
    std::cout << "                                                             *   4. Quitter                  *" << std::endl << std::endl;
    std::cout << "                                                             * * * * * * * * * * * * * * * * *" << std::endl << std::endl;
}


void Jeu::messageHelp(){
    std::cout << "\n\n\n\n                  Bienvenue dans la bataille navale\n\n Vous pouvez vous déplacer avec les touches directionnelles sur le menu\
    \n\n\ Appuyez entrer pour choisir une option" << std::endl;
    std::cout << "\n\nUne fois dans le jeu vous pouvez vous deplacer au moyen des touches directionnelles.Appuyez sur entre pour sélectionner"<<std::endl;
    std::cout << "\n\n\n\n\nAppuyer sur R pour retourner au menu";

    getch();

    system("cls");
    messageMenu();
}

int Jeu::menuChoice(){

    //Position du texte pour choisir les actions
    int posx= this->posMenuChoice_x;
    int posy= this->posMenuChoice_y-1;
    int borderUp= posx;
    int borderDown= posx+18;

    //Affichage du curseur
    gotoligcol(posx,posy);
    std::cout<<"#";
    gotoligcol(posx, posy);

    int choix=1;

    //Condition de la boucle
    bool a=false;

    while(!a)
    {
        switch(getch())
        {

        case KEY_DOWN:
            if(posx+2 <= borderDown)
            {
                std::cout<<" ";
                posx+=6;
                gotoligcol(posx, posy);
                std::cout<<"#";
                gotoligcol(posx, posy);

                choix++;
            }

            break;

        case KEY_UP:
            if(posx-6 >= borderUp)
            {
                std::cout<<" ";
                posx-=6;
                gotoligcol(posx, posy);
                std::cout<<"#";
                gotoligcol(posx, posy);

                choix--;
            }
            break;

        case KEY_ENTER:
            a=true;
            break;

        case KEY_BACKSPACE:
            std::cout<<" ";
            choix=0; //Choix 0 pour annuler
            a=true;
            break;
        }
    }

    //On efface le curseur
    gotoligcol(posx,posy);
    std::cout<<" ";

    return choix;
}


//Boucle de Jeu
void Jeu::jeu(){



    bool a= false;

    while(!a){

    system("cls");

    //Creation des bateaux
    initBoats();

    messageMenu();

        switch(menuChoice()){
            case 1:
                system("cls");
                //Grilles
                displayLayout();
                //Affiche les bateaux
                displayBoats();
                //Round
                round();

                break;

            /*case 2:
                system("cls");

                break;*/

            case 3:
                system("cls");
                messageHelp();
                break;

            case 4:
                system("cls");
                a= true;
                break;
        }
    }
}
