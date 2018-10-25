#ifndef PROJECT_1_CONTAINER_H
#define PROJECT_1_CONTAINER_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <vector> 

#include "Trigger.h"
#include "Item.h"

using namespace std;

class Container {
    public:

        string name;
        string status;
        string descrip;
        vector <string> accepts; // compare string to items name before putting in??
        vector <Item> items;
        vector <Trigger> triggers; //vector of triggers

        Container();
        ~Container();
        void printContainer() const;

};

#endif