#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <vector> 

#include "Trigger.h"

using namespace std;

class Item{
    public:
        struct TurnOn {
            string print;
            string action;
        };
        string name;
        string status;
        string descrip; //description
        string writing;
        TurnOn turnon;
        vector <Trigger> triggers;

        Item();
        ~Item();
        void printItem() const;
};