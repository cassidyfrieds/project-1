#include <iostream>
#include <string>
#include <stdio.h>
#include <vector> 

#include "Trigger.h"

using namespace std;

//so i got this from that guy's github but i have no idea
//how we're going to execute the actions if we're
//reading them in as strings

struct TurnOn{
    string print;
    vector <string> actions;
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