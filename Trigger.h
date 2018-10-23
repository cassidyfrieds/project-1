#include <iostream>
#include <string>
#include <stdio.h>
#include <vector> 

#include "rapidxml.hpp"
#include "Trigger.h"

using namespace std;
using namespace rapidxml;

//ayushi i am still v confused by triggers so sorry this is a mess VVV

struct Conditions{ //from github!!!
    string obj; //object
    string owner;
    string has;
    string status;
}
//command struct too? bc it says conditions of "type command"
class Trigger{
    //only triggers in present contex of game should be checked
    //like inventory, present room
    //and any items, containers, creaturesm and items in containers in present room 
        //the actions preformed by triggers can act on any obj in game!!!
    vector <string> commands;
    vector <Conditions> conditions; 
    bool triggered; 

}