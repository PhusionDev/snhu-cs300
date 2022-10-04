//
//  Menu.cpp
//  CS300-Project1
//
//  Created by Jared Hodgkins on 9/23/22.
//

#include "Menu.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include "Utils.hpp"

namespace Menu
{
    namespace // private anonymous namespace
    {
        /**
         * Helper function to ask user to load
         * the data before using other functions.
         */
        void DataFirst()
        {
            std::cout << "Please load data first...\n";
        }

        void Loading()
        {
            std::cout << "\nLoading Courses...\n";
        }

        void Sorting()
        {
            std::cout << "\nSorting Courses...\n";
        }

        void PrintAll()
        {
            std::cout << "\nPrinting all Courses...\n";
        }

        void Searching()
        {
            std::cout << "\nSearching for course...\n";
        }

        /**
         * Display menu options to get course number
         * from user.
         *
         * @return course number
         */
        std::string CourseNumberMenu()
        {
            std::string courseNum;
            std::cout << "Enter course number: ";
            std::cin >> courseNum;
            return courseNum;
        }

        /**
         * Print information about a specific
         * interal of time
         *
         * @param ticks the time as clock_t to be printed
         */
        void ClockMenu(std::clock_t ticks)
        {
            std::cout << "time: " << ticks << " clock ticks" << std::endl;
            std::cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds\n";
        }

        /**
         * Prints the application's sub menu
         */
        void SubMenu()
        {
            std::cout << "1. Load Data\n";
            std::cout << "2. Print Course List (sorted)\n";
            std::cout << "3. Print Course Info\n";
            std::cout << "9. Return to Main Menu\n";
            std::cout << "Enter choice: ";
        }

        /**
         * Prints the applications vector-related menu
         */
        void VectorMenu()
        {
            std::vector<Course> courses; // declare a vector of course objects
            std::clock_t ticks;          // declare an object to store time information
            bool returnMain = false;     // return to main or stay in menu
            int choice = 0;              // store user input choice
            std::string dataLoaded = "No";
            size_t courseCount = 0;
            while (!returnMain)
            { // while not return to main (menu)
                courseCount = courses.size();
                dataLoaded = courseCount > 0 ? "Yes" : "No";
                std::cout << "\nVector Data Structure Menu | Data Loaded: " << dataLoaded << " | Course Count: " << courseCount << std::endl;
                SubMenu();          // print the sub menu options
                std::cin >> choice; // get choice input from user
                switch (choice)
                { // process user's input
                case 1:
                {                                                        // load courses
                    Loading();                                           // print loading info
                    ticks = std::clock();                                // set stopwatch start time
                    courses = utils::vector::LoadCourses("courses.csv"); // load courses from csv
                    ticks = std::clock() - ticks;                        // set stopwatch stop time
                    ClockMenu(ticks);                                    // print time analysis (loading from csv and appending to vector)
                    break;
                }
                case 2:
                { // print all courses
                    if (courses.size() > 0)
                    {                                                     // check that there are any courses
                        Sorting();                                        // display message - sorting all courses
                        std::clock_t ticksSort = std::clock();            // initialize clock to hold sort-time info
                        utils::quickSort(courses, 0, courses.size() - 1); // sort the courses
                        ticksSort = std::clock() - ticksSort;             // stop the stop watch for sorting
                        ClockMenu(ticksSort);                             // print time analysis (sorting)
                        PrintAll();                                       // display message - printing all courses
                        ticks = std::clock();                             // start the stopwatch for access/printing courses
                        for (int i = 0; i < courses.size(); ++i)
                        {                                         // loop through each course
                            utils::printCourse(&(courses.at(i))); // access and print current course
                        }
                        std::cout << std::endl;
                        ticks = std::clock() - ticks; // stop stopwatch for access/printing
                        ClockMenu(ticks);             // print time analysis (access/printing)
                    }
                    else
                    {
                        DataFirst(); // ask user to load courses first
                    }
                    break;
                }
                case 3:
                {
                    if (courses.size() > 0)
                    {
                        bool found = false;                         // whether course number was found or not
                        std::string courseNum = CourseNumberMenu(); // get and store course number from user input
                        Searching();                                // print message - searching for course
                        ticks = std::clock();                       // start the stopwatch
                        for (int i = 0; i < courses.size(); ++i)
                        { // loop through all courses
                            if (courses.at(i).courseNumber == courseNum)
                            {                                         // check if current course matches user input
                                ticks = std::clock() - ticks;         // match found -- stop the stopwatch
                                ClockMenu(ticks);                     // print time analysis (search/access)
                                found = true;                         // match found -- set found to true
                                utils::printCourse(&(courses.at(i))); // display course information
                                break;
                            }
                        }
                        if (!found)
                        {                                 // course wasn't found
                            ticks = std::clock() - ticks; // stop the stopwatch
                            std::cout << "Course not found...\n";
                            ClockMenu(ticks); // print time analysis (search)
                        }
                    }
                    else
                    {
                        DataFirst(); // ask user to load data first
                    }
                    break;
                }
                case 9:
                    std::cout << std::endl;
                    returnMain = true; // return to main menu
                }
            }
        }

        /**
         * Prints the application's Hash Table menu
         */
        void HashTableMenu()
        {
            HashTable *courses = nullptr; // declare courses hash table to null
            std::clock_t ticks;           // declare clock object
            bool returnMain = false;      // return to main or stay in menu
            int choice = 0;               // store user input choice
            std::string dataLoaded = "No";
            size_t courseCount = 0;
            while (!returnMain)
            { // while not return to main (menu)
                courseCount = courses != nullptr ? courses->Size() : 0;
                dataLoaded = courseCount > 0 ? "Yes" : "No";
                std::cout << "\nHash Table Data Structure Menu | Data Loaded: " << dataLoaded << " | Course Count: " << courseCount << std::endl;
                SubMenu();          // print the sub menu options
                std::cin >> choice; // get and store user input (choice)
                switch (choice)
                { // process user input
                case 1:
                { // load courses
                    if (courses != nullptr)
                    {                   // check for existing hash table
                        delete courses; // delete existing hash table
                    }
                    Loading();
                    ticks = std::clock();                                   // start the stop watch
                    courses = utils::hashTable::LoadCourses("courses.csv"); // load courses from csv
                    ticks = std::clock() - ticks;                           // stop the stopwatch
                    ClockMenu(ticks);                                       // print time analysis info (loading from csv and inserting into HT)
                    break;
                }
                case 2:
                { // sort and print all courses
                    if (courses != nullptr)
                    { // check that hash table exists
                        Sorting();
                        std::clock_t ticksSort = std::clock();           // start stopwatch for sorting analysis
                        std::vector<std::string> keys = courses->Keys(); // get all keys in hash table
                        utils::quickSort(keys, 0, keys.size() - 1);      // sort the keys
                        ticksSort = std::clock() - ticksSort;            // end stopwatch for sorting
                        ClockMenu(ticksSort);                            // print time analysis (sorting)
                        PrintAll();
                        ticks = std::clock(); // start stopwatch for access analysis
                        for (int i = 0; i < keys.size(); ++i)
                        {                                                 // loop through all (sorted) keys in hash table
                            Course *course = courses->Search(keys.at(i)); // get reference to course at key
                            if (course != nullptr)
                            {                               // if course isn't null
                                utils::printCourse(course); // print course information
                            }
                        }
                        ticks = std::clock() - ticks; // all keys accessed, end stopwatch
                        ClockMenu(ticks);             // print time analysis (access/printing)
                    }
                    else
                    {
                        DataFirst(); // ask user to load data first
                    }
                    break;
                }
                case 3:
                {
                    if (courses != nullptr)
                    {                                                // check that hash table exists
                        std::string courseNum = CourseNumberMenu();  // get and store course number from user input
                        Searching();                                 // print message -- searching for course
                        ticks = std::clock();                        // start stopwatch (search)
                        Course *course = courses->Search(courseNum); // search for course in table
                        ticks = std::clock() - ticks;                // stop the stopwatch
                        ClockMenu(ticks);                            // print time analysis (search)
                        utils::printCourse(course);                  // print course information
                    }
                    else
                    {
                        DataFirst(); // ask user to load data first
                    }
                    break;
                }
                case 9:
                    std::cout << std::endl;
                    returnMain = true; // return to main menu
                }
            }
            if (courses != nullptr)
            {                   // check if courses exist
                delete courses; // free memory from courses hash table
                std::cout << std::endl;
            }
        }

        /**
         * Prints the application's Menu for
         * Binary Search Tree functions.
         */
        void BSTMenu()
        {
            BinarySearchTree *courses = nullptr; // declare courses BST to null
            std::clock_t ticks;                  // declare clock object
            bool returnMain = false;             // return to main or stay in menu
            int choice = 0;                      // store user input choice
            std::string dataLoaded = "No";
            size_t courseCount = 0;
            while (!returnMain)
            { // while not return to main (menu)
                courseCount = courses != nullptr ? courses->Size() : 0;
                dataLoaded = courseCount > 0 ? "Yes" : "No";
                std::cout << "\nBinary Search Tree Data Structure Menu | Data Loaded: " << dataLoaded << " | Course Count: " << courseCount << std::endl;
                SubMenu();          // print the sub menu options
                std::cin >> choice; // get and store user input (choice)
                switch (choice)
                { // process user input
                case 1:
                { // load courses
                    if (courses != nullptr)
                    {                   // check for existing BST
                        delete courses; // free memory from existing BST
                    }
                    Loading();                                                     // print message -- loading courses
                    ticks = std::clock();                                          // start stopwatch
                    courses = utils::binarySearchTree::LoadCourses("courses.csv"); // load courses from csv
                    ticks = std::clock() - ticks;                                  // stop stopwatch
                    ClockMenu(ticks);                                              // print time analysis (loading)
                    break;
                }
                case 2:
                {
                    if (courses != nullptr)
                    {                                 // check if courses exist
                        Sorting();                    // print message -- sorting courses
                        PrintAll();                   // print message -- printing all courses
                        ticks = std::clock();         // start stopwatch
                        courses->InOrder();           // print the BST in order
                        ticks = std::clock() - ticks; // stop watch
                        std::cout << "\nPrinting BST in order analysis...\n";
                        ClockMenu(ticks); // print time analysis (sorting/access/printing)
                    }
                    else
                    {
                        DataFirst(); // ask user to load data first
                    }
                    break;
                }
                case 3:
                {
                    if (courses != nullptr)
                    {                                                // check if courses exist
                        std::string courseNum = CourseNumberMenu();  // get course number from user
                        Searching();                                 // print message -- searching for course
                        ticks = std::clock();                        // start stopwatch
                        Course *course = courses->Search(courseNum); // search tree for course
                        ticks = std::clock() - ticks;                // stop stopwatch
                        ClockMenu(ticks);                            // print time analysis (search)
                        utils::printCourse(course);                  // print the course
                    }
                    else
                    {
                        DataFirst(); // ask user to load data first
                    }
                    break;
                }
                case 9:
                    std::cout << std::endl;
                    returnMain = true; // return to main menu
                }
            }
            if (courses != nullptr)
            {                   // check for existing courses
                delete courses; // free memory from BST
                std::cout << std::endl;
            }
        }
    }

    /**
     * Print's the applications main menu
     * and runs the application's main loop.
     */
    void MainMenu()
    {
        int choice = 0;
        bool exit = false;
        while (!exit)
        {
            std::cout << "Data Structure Analysis Main Menu -- jared.hodgkins@snhu.edu\n";
            std::cout << "1. Vector Analysis\n";
            std::cout << "2. Hash Table Analysis\n";
            std::cout << "3. Binary Search Tree Analysis* (recommended)\n";
            std::cout << "9. Exit\n";
            std::cout << "Enter choice: ";
            std::cin >> choice;
            switch (choice)
            {
            case 1:
                VectorMenu();
                break;
            case 2:
                HashTableMenu();
                break;
            case 3:
                BSTMenu();
                break;
            case 9:
                exit = true;
            }
        }
    }
}
