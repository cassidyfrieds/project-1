#ifndef PROJECT_1_ROOM_H
#define PROJECT_1_ROOM_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <vector> 

#include "Trigger.h"

using namespace std;

class Room { //type is assumed to be 'regular' unless otherwise specified
    public:
        Room();

        string name;
        string status;
        string descrip;
        string type;
        vector <string> containers; //or <Containter>?
        vector <string> items;
        vector <string> creatures;
        vector <Trigger> triggers;

//NEED: border[] - direction

};

#endif