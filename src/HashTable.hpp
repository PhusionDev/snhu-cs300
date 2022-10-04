//
//  HashTable.hpp
//  CS300-Project1
//
//  Created by Jared Hodgkins on 9/23/22.
//

#ifndef HashTable_hpp
#define HashTable_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "Course.hpp"
/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable
{

private:
    // Define structures to hold bids
    struct HashNode
    {
        std::string key;
        Course *course;
        HashNode *next;

        // Default constructor
        HashNode()
        {
            key = "";
            course = nullptr;
            next = nullptr;
        }

        // Initialize with a course
        HashNode(Course *aCourse) : HashNode()
        {
            course = aCourse;
        }

        // Initialize with a course and a key
        HashNode(Course *aCourse, std::string aKey) : HashNode(aCourse)
        {
            key = aKey;
        }
    };

    std::vector<HashNode *> nodes;
    unsigned int tableSize;
    unsigned int hash(std::string key);
    unsigned int _size;

public:
    HashTable();
    HashTable(unsigned int size);
    void InitNodes();
    virtual ~HashTable();
    void Insert(Course *course);
    void Remove(std::string courseNum);
    void DeleteNode(HashNode *node);
    Course *Search(std::string courseNum);
    unsigned int Size();
    std::vector<std::string> Keys();
    std::vector<Course> Courses();
};
#endif /* HashTable_hpp */
