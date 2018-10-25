#include "Item.h"
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
        temp.printItem();
        allItems[temp.name] = temp;
    }
    //creatures
    map<std::string, Creature> allCreatures;
    for(xml_node<> * creature_node = root_node->first_node("creature"); creature_node; creature_node = creature_node->next_sibling("creature")) {
        Creature temp;
        if(creature_node->first_node("name")) {
            temp.name = creature_node->first_node("name")->value();
        }
        if(creature_node->first_node("status")) {
            temp.status = creature_node->first_node("status")->value();
        }
        if(creature_node->first_node("description")) {
            temp.descrip = creature_node->first_node("description")->value();
        }
        xml_node<> * attack_node = creature_node->first_node("attack");
        if(attack_node) {
            xml_node<> * condition_node = attack_node->first_node("condition");
            if(condition_node) {
                if(condition_node->first_node("has")) {
                    temp.attack.condition.has = condition_node->first_node("has")->value();
                }
                if(condition_node->first_node("object")) {
                    temp.attack.condition.obj = condition_node->first_node("object")->value();
                }
                if(condition_node->first_node("status")) {
                    temp.attack.condition.status = condition_node->first_node("status")->value();
                }
                if(condition_node->first_node("owner")) {
                    temp.attack.condition.owner = condition_node->first_node("owner")->value();
                }
            }
            if(attack_node->first_node("print")) {
                temp.attack.print = attack_node->first_node("print")->value();
            }
            for(xml_node<> * action_node = attack_node->first_node("action"); action_node; action_node = action_node->next_sibling("action")) {
                temp.attack.actions.push_back(action_node->value());
            }
        }
        // Arrays
        for(xml_node<> * vuln_node = creature_node->first_node("vulnerability"); vuln_node; vuln_node = vuln_node->next_sibling("vulnerability")) {
            temp.vulner.push_back(vuln_node->value());
        }
        if(creature_node->first_node("trigger")) {
            temp.triggers.push_back(creature_node->first_node("trigger")->value());
        } 
        temp.printCreature();
        allCreatures[temp.name] = temp;
    }
    //rooms
    int borderCount = 0; //counter for number of borders to push name and directions
    //int bDirCount = 0;
    map<std::string, Room> allRooms;
    for(xml_node<> * room_node = root_node->first_node("room"); room_node; room_node = room_node->next_sibling("room")) {
        Room temp;
        if(room_node->first_node("name")) {
            temp.name = room_node->first_node("name")->value();
        }
        if(room_node->first_node("status")) {
            temp.status = room_node->first_node("status")->value();
        }
        if(room_node->first_node("description")) {
            temp.descrip = room_node->first_node("description")->value();
        }
        if(room_node->first_node("type")) {
            temp.type = room_node->first_node("type")->value();
        }
        /*
        if(room_node->first_node("border")) { //THIS SHIZ IS FUNKY
            temp.borders.push_back(Border());
            
            if(room_node->first_node("border")->first_node("name")) {
                temp.borders[borderCount].name = (room_node->first_node("border")->first_node("name")->value());
                //(temp.borders.name).push_back(room_node->first_node("border")->first_node("name")->value()); //might not be indexing right??
            }
            if(room_node->first_node("border")->first_node("direction")) {
                //(temp.borders.direction).push_back(room_node->first_node("border")->first_node("direction")->value());//might not be indexing right??
            }
            borderCount++;
        } */
        temp.printRoom();
        allRooms[temp.name] = temp;
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