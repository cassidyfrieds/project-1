cmake_minimum_required(VERSION 3.12)
project(project_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(project_1
        Container.cpp
        Container.h
        Creature.cpp
        Creature.h
        Item.cpp
        Item.h
        main.cpp
        README.md
        Room.cpp
        Room.h
        Trigger.cpp
        Trigger.h)
