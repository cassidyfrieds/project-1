#include <iostream>
#include <string>
#include <stdio.h>
#include <vector> 

#include "rapidxml.hpp"
#include "Trigger.h"

using namespace std;
using namespace rapidxml;

class Creature{
    string name;
    string status;
    string descrip;    
    string attack; //if attack matches vulner - action elements in ‘attack’ are to executed if any of the given conditions are met 
    vector <string> vulner; //vulnerability
    vector <Trigger> triggers;
    
}