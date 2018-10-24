#ifndef PROJECT_1_ROOM_H
#define PROJECT_1_ROOM_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <vector> 

#include "Trigger.h"
#include "Creature.h"
#include "Container.h"
//#include "Item.h"

using namespace std;

struct Border{
    string direction;
    string name;
};

class Room { //type is assumed to be 'regular' unless otherwise specified
    public:
        Room();

        string name;
        string status;
        string descrip;
        string type;
        vector <Border> borders;
        vector <Container> containers; //or <Containter>?
        vector <Item> items;
        vector <Creature> creatures;
        vector <Trigger> triggers;

//NEED: border[] - direction

};

#endif