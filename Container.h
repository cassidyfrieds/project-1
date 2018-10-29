#pragma once

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
        bool open;
        vector <string> accepts; // compare string to items name before putting in??
        vector <Item> items;
        vector <Trigger> triggers; //vector of triggers

        Container();
        ~Container();
        void printContainer() const;

};