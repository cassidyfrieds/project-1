#ifndef PROJECT_1_CONTAINER_H
#define PROJECT_1_CONTAINER_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <vector> 

#include "Trigger.h"

using namespace std;

class Container{
    public:

        string name;
        string status;
        string descrip;
        vector <string> accepts;
        vector <string> items;
        vector <Trigger> triggers; //vector of triggers

};

#endif