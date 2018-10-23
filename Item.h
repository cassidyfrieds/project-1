#include <iostream>
#include <string>
#include <stdio.h>
#include <vector> 

#include "Trigger.h"

using namespace std;

//Turnon struct? GITHUB example has it but we cant make it too similar!!!

class Item{
    public:
       // Item(xml_node<> * node);

        string name;
        string status;
        string descrip; //description
        string writing;
        bool turnon; // ? action elements in turnon are executed if conditions are met
        vector <Trigger> triggers;
    

};