#include <iostream>
#include <string>
#include <stdio.h>
#include <vector> 

#include "rapidxml.hpp"
#include "Trigger.h"

using namespace std;
using namespace rapidxml;

class Room{ //type is assumed to be 'regular' unless otherwise specified 
    public:
        string name;
        string status;
        string descrip;
        string type;
        vector <string> containers; //or <Containter>?
        vector <string> items;
        vector <string> creatures;
        vector <Trigger> triggers;

//NEED: border[] - direction

}