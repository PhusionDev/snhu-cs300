//
//  Utils.hpp
//  CS300-Project1
//
//  Created by Jared Hodgkins on 9/23/22.
//

#ifndef Utils_hpp
#define Utils_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "Course.hpp"
#include "HashTable.hpp"
#include "BinarySearchTree.hpp"

namespace utils
{
    namespace vector
    {
        std::vector<Course> LoadCourses(std::string filePath);
    }

    namespace hashTable
    {
        HashTable *LoadCourses(std::string filePath);
    }

    namespace binarySearchTree
    {
        BinarySearchTree *LoadCourses(std::string filePath);
    }

    void printCourse(Course *course);
    int partition(std::vector<Course> &courses, int begin, int end);
    int partition(std::vector<std::string> &keys, int begin, int end);
    void quickSort(std::vector<Course> &courses, int begin, int end);
    void quickSort(std::vector<std::string> &keys, int begin, int end);
}

#endif /* Utils_hpp */
