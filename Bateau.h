#ifndef BATEAU_H_INCLUDED
#define BATEAU_H_INCLUDED

#include <vector>
#include "string"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define DIR_N 0
#define DIR_E 1
#define DIR_S 2
#define DIR_W 3

#define BATEAU 0
#define SOUS_MARIN 1
#define DESTROYER 2
#define CROISEUR 3
#define CUIRASSE 4

class Bateau
{
    private:


    //sous-programme
    public:
        Bateau() ;
        virtual ~Bateau();

         // getter
        inline int getPos(){return pos;};
        inline int getPower(){return power;};
        inline char getSymbol(){return symbol;};
        inline int getDir(){return dir;};
        inline int getType(){return type;};
        inline std::vector<int> getPosList(){return posList;};
        inline std::vector<int> getTouchedList(){return touchedList;};



        // setter
        void setDir(int);
        void setPos(int);
        void setPosList();

        inline void addTouched(int pos){touchedList.push_back(pos);};

        void removePos(int);

    protected:

        char symbol;
        int length;
        int power;
        int pos;
        int dir;
        int type;
        std::vector<int> posList;
        std::vector<int> touchedList;


};

#endif // BATEAU_H_INCLUDED
