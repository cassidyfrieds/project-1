#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>

#include "rapidxml.hpp"
#include "Trigger.h"

using namespace std;
using namespace rapidxml;

class Container{
    public:
        Container(xml_node<> * node);

        string name; // added comments
        string status;
        string descrip;
        vector <string> accepts;
        vector <string> items;
        vector <Trigger> triggers; //vector of triggers

}
