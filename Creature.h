#ifndef PROJECT_1_CREATURE_H
#define PROJECT_1_CREATURE_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <vector> 

#include "Trigger.h"

using namespace std;

class Creature{
    string name;
    string status;
    string descrip;    
    string attack; //if attack matches vulner - action elements in ‘attack’ are to executed if any of the given conditions are met 
   //so i really dont know how to execute the conditions of attack - ayushi
    vector <string> vulner; //vulnerability
    vector <Trigger> triggers;
};

#endif