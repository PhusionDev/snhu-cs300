//
//  main.cpp
//  CS300-Project1
//
//  Created by Jared Hodgkins on 9/23/22.
//

#include <iostream>
#include "Menu.hpp"

int main(int argc, const char *argv[])
{
    // process command line arguments
    std::string csvPath;
    switch (argc) {
        case 2:
            csvPath = argv[1];
            break;
        default:
            csvPath = "courses.csv";
    }

    Menu::MainMenu(csvPath);

    return 0;
}
