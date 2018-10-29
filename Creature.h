#ifndef PROJECT_1_CREATURE_H
#define PROJECT_1_CREATURE_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <vector> 

#include "Trigger.h"
#include "Condition.h"

using namespace std;

class Creature{
    public:
        struct Attack {
            Condition condition;
            string print;
            vector<string> actions;
        };   
        string name;
        string status;
        string descrip; 
        Attack attack;  
        //if attack matches vulner - action elements in ‘attack’ are to executed if any of the given conditions are met 
        //so i really dont know how to execute the conditions of attack - ayushi
        vector <string> vulner; //vulnerability
        //vector <Trigger> triggers;
        vector <string> triggers; //!!!!! DID THIS bc trigger isnt done yet so i just put in the strings
        //to make it easier to test if parsing correctly

        Creature();
        ~Creature();
        void printCreature() const;
};

#endif