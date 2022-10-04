//
//  HashTable.cpp
//  CS300-Project1
//
//  Created by Jared Hodgkins on 9/23/22.
//

#include "HashTable.hpp"
#include <iostream>

/**
 * Default constructor
 */
HashTable::HashTable()
{
    _size = 0;               // set initial recorded size of hash table to 0
    tableSize = 10;          // set initial table size to 10
    nodes.resize(tableSize); // resize the nodes vector
    HashTable::InitNodes();  // initialize the node elements
}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size)
{
    _size = 0;               // set initial recorded size of hash table to 0
    tableSize = size;        // Set local tableSize with given size
    nodes.resize(tableSize); // Resize nodes size
    HashTable::InitNodes();  // Initialize the hash table nodes
}

/**
 * Destructor
 */
HashTable::~HashTable()
{
    HashNode *currentNode;
    HashNode *nextNode;
    std::cout << "Deleting Hash Table...\n";
    for (int i = 0; i < tableSize; ++i)
    {                              // loop through all buckets in hash table
        currentNode = nodes.at(i); // Set current node to first node in bucket
        while (currentNode != nullptr)
        {                                       // Iterate through bucket's linked list
            nextNode = currentNode->next;       // Set temporary pointer to next node
            HashTable::DeleteNode(currentNode); // Delete current node
            currentNode = nextNode;             // Set current node to the next node in linked list
        }
    }
    nodes.clear();         // Remove all elements from vector
    nodes.shrink_to_fit(); // Reduce capacity of vector to free memory
}

/**
 * Helper function to create default nodes
 * for each of the buckets in the hash table.
 */
void HashTable::InitNodes()
{
    for (int i = 0; i < tableSize; ++i)
    {
        nodes.at(i) = new HashNode();
    }
}

/**
 * Return the current number of entries
 * in the hash table.
 *
 * @return number of elements in hash table
 */
unsigned int HashTable::Size()
{
    return _size;
}

/**
 * Helper function to properly delete a node
 * and course (if a course exists) and free the
 * memory the node and course were using.
 *
 * @param node the node to be deleted
 */
void HashTable::DeleteNode(HashNode *node)
{
    if (node != nullptr)
    { // Check if node pointer is not null
        if (node->course != nullptr)
        {                        // Check if the course pointer in node is not null
            delete node->course; // free memory allocated to the course object
        }
        delete node; // free memory allocated to the node object
        _size--;     // reduce recorded size of hash table
    }
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index. Uses string method
 * by Daniel J. Bernstein with initial value
 * and hash multiplier.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(std::string key)
{
    unsigned int stringHash = 5381; // set initial hash value
    for (int i = 0; i < key.length(); ++i)
    {                                                  // loop through each char in string
        stringHash = (stringHash * 33) + char(key[i]); // hash char and add to hash
    }
    unsigned int retVal = stringHash % tableSize; // perform the hash mod
    return retVal;                                // return hashed value
}

/**
 * Insert a course
 *
 * @param course The course to insert
 */
void HashTable::Insert(Course *course)
{
    std::string key = course->courseNumber;                // store course
    int nodeIndex = HashTable::hash(course->courseNumber); // Retrieve node index (bucket) using key

    // Check for an existing course in hash table
    Course *existingCourse = HashTable::Search(course->courseNumber);

    // Check for existing course/node (UPDATE)
    if (existingCourse != nullptr)
    {                                                // Check if existing course is not null
        HashNode *currentNode = nodes.at(nodeIndex); // Set current node to first node in bucket
        while (currentNode != nullptr)
        { // Iterate through bucket's linked list
            if (currentNode->key == key)
            { // Check if current node matches key
                // course's node has been found
                delete existingCourse;        // Clear old course data
                currentNode->course = course; // Update old course's node to point to new course data
                return;                       // Terminate loop and return -- match was found
            }
            currentNode = currentNode->next; // Increment node in linked list
        }
    }

    // Course does not yet exist in hash table
    // Check if first node in bucket is unused/unintialized
    if (nodes.at(nodeIndex)->key == "")
    {
        // First node is uninitialized: set node's key with course number and course with course data
        nodes.at(nodeIndex)->key = key;
        nodes.at(nodeIndex)->course = course;
        _size++; // increase recorded size of hash table
    }
    else
    {
        // the first node in the bucket's linked list is already in use, implement chaining
        HashNode *newNode = new HashNode(course, key); // Create a new node
        HashNode *currentNode = nodes.at(nodeIndex);   // Set current node to first node in linked list
        // Find the last node in bucket's linked list
        while (currentNode->next != nullptr)
        {
            currentNode = currentNode->next;
        }
        // Set last node's next pointer to point to the new node
        currentNode->next = newNode;
        _size++; // increase recorded size of hash table
    }
}

/**
 * Remove a course
 *
 * @param courseNumber The course number to search for
 */
void HashTable::Remove(std::string courseNumber)
{
    std::string key = courseNumber;       // set key equal to course number
    int nodeIndex = HashTable::hash(key); // hash table bucket id

    HashNode *delNode = nullptr; // Node pointer for removal

    if (HashTable::Search(courseNumber) == nullptr)
    {
        std::cout << "Course Number " << courseNumber << " wasn't found, nothing to remove...\n";
        return; // Empty course, nothing to remove (not in hash table)
    }

    // Set to the beginning node of the bucket's linked list
    HashNode *currentNode = nodes.at(nodeIndex);

    // Check if matching course is the first node in linked list
    if (currentNode->key == key)
    {
        // First node's key is a match
        delNode = currentNode; // Mark target node for deletion

        // cCheck if bucket's linked list is empty after current node
        if (currentNode->next == nullptr)
        {
            // Set initial node to a newly created node
            nodes.at(nodeIndex) = new HashNode();
        }
        else
        {
            // Bucket's linked list is not empty, point bucket's beginning to next node in linked list
            nodes.at(nodeIndex) = currentNode->next;
        }
    }
    else
    { // Matching key was not the first node
        while (currentNode != nullptr)
        { // Search through linked list for a matching key
            // Check if the next node is not a null pointer
            if (currentNode->next != nullptr)
            {
                // Check for item where the next item's key matches specified key
                if (currentNode->next->key == key)
                {
                    // The next item matches the specified key
                    delNode = currentNode->next; // mark target node for deletion
                    // Set current node's next to the node after the target node's next node
                    currentNode->next = delNode->next;
                    break; // Matching key found, terminate search loop
                }
            }
            // Go to next node
            currentNode = currentNode->next;
        }
    }

    DeleteNode(delNode); // Call helper function to delete the marked node (and course)
}

/**
 * Search for the specified course
 *
 * @param courseNumber The course number to search for
 */
Course *HashTable::Search(std::string courseNumber)
{
    std::string key = courseNumber;                // key for the given course
    unsigned int nodeIndex = HashTable::hash(key); // Get hash value from key (bucket id)
    HashNode *currentNode = nodes.at(nodeIndex);   // Set current node to first node in bucket

    do
    {
        if (currentNode->key == key)
        {
            // Current node matches the key - return a pointer to the matching course
            return currentNode->course;
        }
        currentNode = currentNode->next; // Continue iterating through linked list
    } while (currentNode != nullptr);    // Until end of linked list

    return nullptr; // Matching course number was not found, return null pointer
}

/**
 * Get all keys in Hash Table
 * @return a vector containing all of the keys in the hash table
 */
std::vector<std::string> HashTable::Keys()
{
    std::vector<std::string> keys;
    HashNode *current = nullptr;
    for (int bucket = 0; bucket < nodes.size(); ++bucket)
    {
        current = nodes.at(bucket);
        while (current != nullptr)
        {
            if (current->key != "")
            {
                keys.push_back(current->key);
            }
            current = current->next;
        }
    }
    return keys;
}

/**
 * Get all of the courses in hash table
 *
 * @return vector of all courses in hash table
 */
std::vector<Course> HashTable::Courses()
{
    std::vector<Course> courses;
    HashNode *current = nullptr;
    for (int bucket = 0; bucket < nodes.size(); ++bucket)
    {
        current = nodes.at(bucket);
        while (current != nullptr)
        {
            if (current->key != "")
            {
                if (!current->course->courseNumber.empty())
                {
                    courses.push_back(*(current->course));
                }
            }
            current = current->next;
        }
    }
    return courses;
}
