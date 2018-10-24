#include <iostream>
#include <string>
#include <stdio.h>
#include <vector> 

#include "Trigger.h"

using namespace std;

struct TurnOn{
    string print;
    string action;
};

class Item{
    public:
       // Item(xml_node<> * node);

        string name;
        string status;
        string descrip; //description
        string writing;
        TurnOn turnon;
        //vector <Trigger> triggers;

        Item();
        ~Item();
        void printItem() const;
};