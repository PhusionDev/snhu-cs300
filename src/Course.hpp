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
#include <unordered_map>

struct Course
{
    std::string courseNumber;
    std::string title;
    std::vector<std::string> prerequisites;
    std::vector<std::string> extendedPrereqs; // generated list of all prerequisites, not just the ones listed
    std::unordered_map<std::string, bool> hasPrerequisite; // mapping of if the course has a prerequisite or not
};

#endif /* Course_hpp */
