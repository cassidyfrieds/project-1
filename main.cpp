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

    // Parse Items
    map<std::string, Item> allItems;
    for(xml_node<> * item_node = root_node->first_node("item"); item_node; item_node = item_node->next_sibling("item")) {
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
            if(item_node->first_node("turnon")->first_node("print")) {
                temp.turnon.print = item_node->first_node("turnon")->first_node("print")->value();
            }
            if(item_node->first_node("turnon")->first_node("action")) {
                temp.turnon.action = item_node->first_node("turnon")->first_node("action")->value();
            }
        }
        //temp.printItem();
        allItems[temp.name] = temp;
    }

    // Parse Containers
    /*
    map<std::string, Container> allContainers;
    for(xml_node<> * container_node = root_node->first_node("container"); container_node; container_node = container_node->next_sibling("container")) {
        Container temp;
        //name, status, description, accept[ ], item[ ], trigger[ ]
        if(container_node->first_node("name")) {
            temp.name = container_node->first_node("name")->value();
        }
        if(container_node->first_node("status")) {
            temp.status = container_node->first_node("status")->value();
        }
        if(container_node->first_node("description")) {
            temp.descrip = container_node->first_node("description")->value();
        }
        for(xml_node<> * accept_node = container_node->first_node("accept"); accept_node; accept_node = accept_node->next_sibling("accept")) {
            temp.accepts.push_back(accept_node->value());
        }
        for(xml_node<> * item_node = container_node->first_node("item"); item_node; item_node = item_node->next_sibling("item")) {
            string itemName = item_node->value();
            temp.items.push_back(allItems[itemName]);
        }
        for(xml_node<> * trigger_node = container_node->first_node("trigger"); trigger_node; trigger_node = trigger_node->next_sibling("trigger")) {
            Trigger tempTrigger;
            if(trigger_node->first_node("type")) {
                tempTrigger.type = trigger_node->first_node("type")->value();
            }
            for(xml_node<> * condition_node = trigger_node->first_node("condition"); condition_node; condition_node = condition_node->next_sibling("condition")) {
                Trigger::Condition newCondition;
                if(condition_node->first_node("object")) {
                    newCondition.obj = allItems[condition_node->first_node("object")->value()];
                }
                if(condition_node->first_node("owner")) {
                    newCondition.owner = condition_node->first_node("owner")->value();
                }
                if(condition_node->first_node("has")) {
                    newCondition.has = condition_node->first_node("has")->value();
                }
                if(condition_node->first_node("status")) {
                    newCondition.status = condition_node->first_node("status")->value();
                }
                tempTrigger.conditions.push_back(newCondition);
            }
            temp.triggers.push_back(tempTrigger);
        }

        //temp.printItem();
        allContainers[temp.name] = temp;
    }
    */

};