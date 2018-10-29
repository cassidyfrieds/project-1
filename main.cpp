#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <map>

#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"

#include "Item.h"
#include "Container.h"
#include "Room.h"
#include "Creature.h"
#include "Trigger.h"

using namespace std;
using namespace rapidxml;

vector<string> splitString(const string& s, char delimiter)
{
   vector<string> tokens;
   string token;
   istringstream tokenStream(s);
   while (getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

int main(int argc, char* argv[] ){

    /*
        Read the file
    */
	xml_document<> doc;
	xml_node<> * root_node;
	ifstream theFile (argv[1]);         // Read the xml file into a vector
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    theFile.close();

	doc.parse<0>(&buffer[0]); 	        // Parse the buffer using the xml file parsing library into doc 
	root_node = doc.first_node("map");  // Find our map node
    if (root_node == NULL){
        cout << "cant get root node" << endl;
        return 0;
    }

    /*
        Parse Items
    */    
    map<std::string, Item> allItems;
    for(xml_node<> * item_node = root_node->first_node("item"); item_node; item_node = item_node->next_sibling("item")) {
        Item temp;
        // string name
        if(item_node->first_node("name")) {
            temp.name = item_node->first_node("name")->value();
        }
        // string writing
        if(item_node->first_node("writing")) {
            temp.writing = item_node->first_node("writing")->value();
        }
        // string status
        if(item_node->first_node("status")) {
            temp.status = item_node->first_node("status")->value();
        }
        // string description
        if(item_node->first_node("description")) {
            temp.descrip = item_node->first_node("description")->value();
        }
        // TurnOn turnon
        if(item_node->first_node("turnon")) {
            if(item_node->first_node("turnon")->first_node("print")) {
                temp.turnon.print = item_node->first_node("turnon")->first_node("print")->value();
            }
            if(item_node->first_node("turnon")->first_node("action")) {
                temp.turnon.action = item_node->first_node("turnon")->first_node("action")->value();
            }
        }
        // vector<Trigger> triggers
        for(xml_node<> * trigger_node = item_node->first_node("trigger"); trigger_node; trigger_node = trigger_node->next_sibling("trigger")) {
            Trigger tempTrigger;
            //string trigger.type;
            if(trigger_node->first_node("type")) {
                tempTrigger.type = trigger_node->first_node("type")->value();
            }
            //string trigger.print
            if(trigger_node->first_node("print")) {
                tempTrigger.print = trigger_node->first_node("print")->value();
            }
            //vector <string> trigger.commands;
            for(xml_node<> * command_node = trigger_node->first_node("command"); command_node; command_node = command_node->next_sibling("command")) {
                tempTrigger.commands.push_back(command_node->value());
            }
            //vector <Condition> trigger.conditions;
            for(xml_node<> * condition_node = trigger_node->first_node("condition"); condition_node; condition_node = condition_node->next_sibling("condition")) {
                Condition tempCondition;
                if(condition_node->first_node("has")) {
                    tempCondition.has = condition_node->first_node("has")->value();
                }
                if(condition_node->first_node("object")) {
                    tempCondition.obj = condition_node->first_node("object")->value();
                }
                if(condition_node->first_node("status")) {
                    tempCondition.status = condition_node->first_node("status")->value();
                }
                if(condition_node->first_node("owner")) {
                    tempCondition.owner = condition_node->first_node("owner")->value();
                }
                tempTrigger.conditions.push_back(tempCondition);
            }
            temp.triggers.push_back(tempTrigger);
        }
        //temp.printItem();
        allItems[temp.name] = temp;
    }

    /*
        Parse creatures
    */
    map<std::string, Creature> allCreatures;
    for(xml_node<> * creature_node = root_node->first_node("creature"); creature_node; creature_node = creature_node->next_sibling("creature")) {
        Creature temp;
        // string name
        if(creature_node->first_node("name")) {
            temp.name = creature_node->first_node("name")->value();
        }
        // string status
        if(creature_node->first_node("status")) {
            temp.status = creature_node->first_node("status")->value();
        }
        // string description
        if(creature_node->first_node("description")) {
            temp.descrip = creature_node->first_node("description")->value();
        }
        xml_node<> * attack_node = creature_node->first_node("attack");
        // Attack attack
        if(attack_node) {
            // vector<Condition> attack.conditions
            for(xml_node<> * condition_node = attack_node->first_node("condition"); condition_node; condition_node = condition_node->next_sibling("condition")) {
                Condition tempCondition;
                if(condition_node->first_node("has")) {
                    tempCondition.has = condition_node->first_node("has")->value();
                }
                if(condition_node->first_node("object")) {
                    tempCondition.obj = condition_node->first_node("object")->value();
                }
                if(condition_node->first_node("status")) {
                    tempCondition.status = condition_node->first_node("status")->value();
                }
                if(condition_node->first_node("owner")) {
                    tempCondition.owner = condition_node->first_node("owner")->value();
                }
                temp.attack.conditions.push_back(tempCondition);
            }
            // string attack.print
            if(attack_node->first_node("print")) {
                temp.attack.print = attack_node->first_node("print")->value();
            }
            // vector<string> attack.actions
            for(xml_node<> * action_node = attack_node->first_node("action"); action_node; action_node = action_node->next_sibling("action")) {
                temp.attack.actions.push_back(action_node->value());
            }
        }
        // vector<String> vulner
        for(xml_node<> * vuln_node = creature_node->first_node("vulnerability"); vuln_node; vuln_node = vuln_node->next_sibling("vulnerability")) {
            temp.vulner.push_back(vuln_node->value());
        }
        // vector<Trigger> triggers
        for(xml_node<> * trigger_node = creature_node->first_node("trigger"); trigger_node; trigger_node = trigger_node->next_sibling("trigger")) {
            Trigger tempTrigger;
            //string trigger.type;
            if(trigger_node->first_node("type")) {
                tempTrigger.type = trigger_node->first_node("type")->value();
            }
            //string trigger.print
            if(trigger_node->first_node("print")) {
                tempTrigger.print = trigger_node->first_node("print")->value();
            }
            //vector <string> trigger.commands;
            for(xml_node<> * command_node = trigger_node->first_node("command"); command_node; command_node = command_node->next_sibling("command")) {
                tempTrigger.commands.push_back(command_node->value());
            }
            //vector <Condition> trigger.conditions;
            for(xml_node<> * condition_node = trigger_node->first_node("condition"); condition_node; condition_node = condition_node->next_sibling("condition")) {
                Condition tempCondition;
                if(condition_node->first_node("has")) {
                    tempCondition.has = condition_node->first_node("has")->value();
                }
                if(condition_node->first_node("object")) {
                    tempCondition.obj = condition_node->first_node("object")->value();
                }
                if(condition_node->first_node("status")) {
                    tempCondition.status = condition_node->first_node("status")->value();
                }
                if(condition_node->first_node("owner")) {
                    tempCondition.owner = condition_node->first_node("owner")->value();
                }
                tempTrigger.conditions.push_back(tempCondition);
            }
            temp.triggers.push_back(tempTrigger);
        }
        //temp.printCreature();
        allCreatures[temp.name] = temp;
    }
    
    /*
        Parse containers
    */   
    map<std::string, Container> allContainers;
    for(xml_node<> * container_node = root_node->first_node("container"); container_node; container_node = container_node->next_sibling("container")) {
        Container temp;
        // string name
        if(container_node->first_node("name")) {
            temp.name = container_node->first_node("name")->value();
        }
        // string status
        if(container_node->first_node("status")) {
            temp.status = container_node->first_node("status")->value();
        }
        // string descrip
        if(container_node->first_node("description")) {
            temp.descrip = container_node->first_node("description")->value();
        }
        // vector <string> accepts
        for(xml_node<> * accept_node = container_node->first_node("accept"); accept_node; accept_node = accept_node->next_sibling("accept")) {
            temp.accepts.push_back(accept_node->value());
        }
        // vector <Item> items
        for(xml_node<> * item_node = container_node->first_node("item"); item_node; item_node = item_node->next_sibling("item")) {
            string itemName = item_node->value();
            temp.items.push_back(allItems[itemName]);
        }
        // vector <Trigger> triggers;   
        for(xml_node<> * trigger_node = container_node->first_node("trigger"); trigger_node; trigger_node = trigger_node->next_sibling("trigger")) {
            Trigger tempTrigger;
            //string trigger.type;
            if(trigger_node->first_node("type")) {
                tempTrigger.type = trigger_node->first_node("type")->value();
            }
            //string trigger.print
            if(trigger_node->first_node("print")) {
                tempTrigger.print = trigger_node->first_node("print")->value();
            }
            //vector <string> trigger.commands;
            for(xml_node<> * command_node = trigger_node->first_node("command"); command_node; command_node = command_node->next_sibling("command")) {
                tempTrigger.commands.push_back(command_node->value());
            }
            //vector <Condition> trigger.conditions;
            for(xml_node<> * condition_node = trigger_node->first_node("condition"); condition_node; condition_node = condition_node->next_sibling("condition")) {
                Condition tempCondition;
                if(condition_node->first_node("has")) {
                    tempCondition.has = condition_node->first_node("has")->value();
                }
                if(condition_node->first_node("object")) {
                    tempCondition.obj = condition_node->first_node("object")->value();
                }
                if(condition_node->first_node("status")) {
                    tempCondition.status = condition_node->first_node("status")->value();
                }
                if(condition_node->first_node("owner")) {
                    tempCondition.owner = condition_node->first_node("owner")->value();
                }
                tempTrigger.conditions.push_back(tempCondition);
            }
            temp.triggers.push_back(tempTrigger);
        }

        //temp.printContainer();
        allContainers[temp.name] = temp;
    }

    /*
        Parse rooms
    */
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

        // vector <Item> items
        for(xml_node<> * item_node = room_node->first_node("item"); item_node; item_node = item_node->next_sibling("item")) {
            string itemName = item_node->value();
            temp.items.push_back(allItems[itemName]);
        }

        // vector <Creature> creatures
        for(xml_node<> * creature_node = room_node->first_node("creature"); creature_node; creature_node = creature_node->next_sibling("creature")) {
            string creatureName = creature_node->value();
            temp.creatures.push_back(allCreatures[creatureName]);
        }

        // vector <Container> containers
        for(xml_node<> * container_node = room_node->first_node("container"); container_node; container_node = container_node->next_sibling("container")) {
            string containerName = container_node->value();
            temp.containers.push_back(allContainers[containerName]);
        }

         // vector <Border> borderss;   
        for(xml_node<> * border_node = room_node->first_node("border"); border_node; border_node = border_node->next_sibling("border")) {
            Border tempBorder;
            //string border.direction
            if(border_node->first_node("direction")) {
                tempBorder.direction = border_node->first_node("direction")->value();
            }
            //string border.name
            if(border_node->first_node("name")) {
                tempBorder.name = border_node->first_node("name")->value();
            }
            temp.borders.push_back(tempBorder);
        }


        // vector <Trigger> triggers;   
        for(xml_node<> * trigger_node = room_node->first_node("trigger"); trigger_node; trigger_node = trigger_node->next_sibling("trigger")) {
            Trigger tempTrigger;
            //string trigger.type;
            if(trigger_node->first_node("type")) {
                tempTrigger.type = trigger_node->first_node("type")->value();
            }
            //string trigger.print
            if(trigger_node->first_node("print")) {
                tempTrigger.print = trigger_node->first_node("print")->value();
            }
            //vector <string> trigger.commands;
            for(xml_node<> * command_node = trigger_node->first_node("command"); command_node; command_node = command_node->next_sibling("command")) {
                tempTrigger.commands.push_back(command_node->value());
            }
            temp.triggers.push_back(tempTrigger);
        }
           
        //temp.printRoom();

        allRooms[temp.name] = temp;
    }

    /**
    //reading in borders into room - you have to do it after you have all the rooms made first
    for(xml_node<> * room_node = root_node->first_node("room"); room_node; room_node = room_node->next_sibling("room")) {
        Border tempBorder;
        string name;
        if(room_node->first_node("name")) {
            name = room_node->first_node("name")->value();
        }
        Room currRoom = allRooms[name];
        for(xml_node<> * border_node = room_node->first_node("border"); border_node; border_node = border_node->next_sibling("border")) {
            if(border_node->first_node("direction")) {
                tempBorder.direction = border_node->first_node("direction")->value();
            }
           if(border_node->first_node("name")) {
                tempBorder.room= allRooms[border_node->first_node("name")->value()];
            }
            currRoom.borders.push_back(tempBorder);
        }

        currRoom.printRoomBorders();
    }
    **/

    /* Start Reading Input */
    while(true) {
        // Gather input string
        string input;
        cout << "> ";
        getline(cin, input);   

        // Split string at spaces
        vector<string> commands = splitString(input, ' ');

        // Parse instructions
        string key = commands[0];
        if (key == "n" || key == "s" || key == "e" || key == "w") {
            cout << "Direction" << endl;
        }
        else if (key == "i") {
            cout << "Inventory:" << endl;
        }
        else if (key == "take" && commands.size() > 1) {
            string itemName = commands[1];
            cout << "Take " << itemName << endl;
        }
        else if (key == "open") {
            if(commands.size() > 1 && commands[1] == "exit") {
                // Open Exit
            }
            else {
                // Open
            }
        }
        else if (key == "read") {

        }
        else if (key == "drop") {

        }
        else if (key == "put") {

        }
        else if (key == "turn" && commands.size() > 1 && commands[1] == "on") {
            cout << "Turn on" << endl;
        }
        else {
            cout << "Error" << endl;
        }
    }
};