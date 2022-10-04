//
//  Course.hpp
//  CS300-Project1
//
//  Created by Jared Hodgkins on 9/23/22.
//

#ifndef Course_hpp
#define Course_hpp

#include <stdio.h>
#include <string>
#include <vector>

struct Course
{
    std::string courseNumber;
    std::string title;
    std::vector<std::string> prerequisites;
};

#endif /* Course_hpp */
