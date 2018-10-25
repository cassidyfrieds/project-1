#ifndef PROJECT_1_TRIGGER_H
#define PROJECT_1_TRIGGER_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <vector> 

#include "Item.h"
#include "Condition.h"

using namespace std;

//command struct too? bc it says conditions of "type command"
class Trigger {
    //only triggers in present contex of game should be checked
    //like inventory, present room
    //and any items, containers, creaturesm and items in containers in present room 
        //the actions preformed by triggers can act on any obj in game!!!
    public:
        string type;
        vector <string> commands;
        vector <Condition> conditions;
        bool triggered;
        Trigger();
        ~Trigger();
};

#endif