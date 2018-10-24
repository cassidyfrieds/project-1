//#include "Item.h"
#include "Container.h"
#include "Room.h"
#include "Creature.h"
#include "Trigger.h"

#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <map>

#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;


int main(int argc, char* argv[] ){

	xml_document<> doc;
	xml_node<> * root_node;
	// Read the xml file into a vector
	ifstream theFile (argv[1]);
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    theFile.close();

	buffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	doc.parse<0>(&buffer[0]);
	// Find our map node
	root_node = doc.first_node("map");
    if (root_node == NULL){
        cout << "cant get root node" << endl;
        return 0;
    }

    // creating hashmaps of items
    map<std::string, Item> allItems;


    for(xml_node<> * item_node = root_node->first_node("item"); item_node; item_node = item_node->next_sibling("item")) {
        //creating temp Item to put into hashmap later
        Item temp;
        if(item_node->first_node("name")) {
            temp.name = item_node->first_node("name")->value();
        }
        if(item_node->first_node("writing")) {
            temp.writing = item_node->first_node("writing")->value();
        }
        if(item_node->first_node("status")) {
            temp.status = item_node->first_node("status")->value();
        }
        if(item_node->first_node("description")) {
            temp.descrip = item_node->first_node("description")->value();
        }
        if(item_node->first_node("turnon")) {
            if(item_node->first_node("print")) {
                temp.turnon.print = item_node->first_node("print")->value();
            }
            if(item_node->first_node("action")) {
                temp.turnon.print = item_node->first_node("action")->value();
            }
        }
    //turnon.actions[0]={""}; //{0}
        temp.printItem();
    }

};