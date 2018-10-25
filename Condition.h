#ifndef PROJECT_1_CONDITION_H
#define PROJECT_1_CONDITION_H

#include <iostream>
#include <string>

using namespace std;

class Condition {
    public:
        string obj; //object
        string owner;
        bool has; //cass changed this
        string status;

        Condition();
        ~Condition();
        void printCondition() const;
};

#endif