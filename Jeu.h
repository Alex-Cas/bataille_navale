#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED
#include "Bateau.h"
#include "vector"
#include <iostream>


class Jeu{

    private :
        int mainGrid_x;
        int mainGrid_y;
        int secondGrid_x;
        int secondGrid_y;
        int posActionChoice_x;
        int posActionChoice_y;

        int posMenuChoice_x;
        int posMenuChoice_y;

        //Indice du bateau selectionné
        int selectedBoat=0;
        //Alternance des joueurs
        int alt=0;

        std::vector<std::vector<Bateau> > boatsList;



    public :
        Jeu();
        virtual ~Jeu();

        void gotoligcol(int, int);

        int _2dto1d(int, int, int);
        int _1dto2d_x(int, int);
        int _1dto2d_y(int, int);

        void clearMainGrid();
        void clearSecondGrid();
        void displayGrid(int, int);
        void displayLayout();
        void displayBoats();
        void displayTouched();
        void initBoats();
        void placeBoat(Bateau boat);
        std::vector<int> fillSquare(int, int);

        bool selectBoat(int, int);
        bool isCollision(int);

        // Actions
        int actionChoice();
        void shootChoice(int);
        void shoot(std::vector<int>);
        void flare(int, int);
        int turnChoice();
        bool turn(int);
        bool shift();
        //Boucle d'un tour
        void round();
        //Jeu principale
        void jeu();

        inline void alternate(){alt= (alt+1)%2;};


        void switchPlayer();
        int playerVictory();

        int menuChoice();
        void messageMenu();
        void messageHelp();


};

#endif // JEU_H_INCLUDED
