//
//  Utils.cpp
//  CS300-Project1
//
//  Created by Jared Hodgkins on 9/23/22.
//

#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "CSVparser.hpp"
#include "Course.hpp"
#include "HashTable.hpp"
#include "BinarySearchTree.hpp"

namespace utils
{
    namespace vector
    {
        /**
         * Load courses from csv into vector
         *
         * @param filePath csv file to load
         * @return vector of course objects
         */
        std::vector<Course> LoadCourses(std::string filePath)
        {
            std::vector<Course> courses;                     // create new vector of course objects
            csv::Parser parser = csv::Parser(filePath); // initialize parser and load csv

            for (int i = 0; i < parser.rowCount(); ++i)
            {                                     // loop through rows in csv data
                csv::Row &row = parser.getRow(i); // get reference to current row
                if (row.size() >= 2)
                {                                 // check if row has at least 2 elements
                    Course course;                // create new course object
                    course.courseNumber = row[0]; // set course number to first element in row
                    course.title = row[1];        // set title to 2nd element in row
                    if (row.size() > 2)
                    { // check if row has any prerequisites
                        for (int j = 2; j < row.size(); ++j)
                        {                                      // loop through prerequisite elements
                            const std::string prereq = row[j]; // get reference to current element
                            if (parser.isValidPrerequisite(prereq))
                            { // check if prereq is a valid course
                                // valid so add to course prereqs
                                course.prerequisites.push_back(prereq);
                            }
                            else
                            { // prereq not found as course in data (invalid)
                                std::cout << "Invalid Prerequisite " << prereq << std::endl;
                                return std::vector<Course>(); // return empty vector of courses
                            }
                        }
                    }
                    courses.push_back(course); // valid course -- add to courses
                }
                else
                { // invalid number of elements in row, need at least 2
                    std::cout << "Invalid number of parameters in current row!";
                    return std::vector<Course>(); // return empty vector of courses
                }
            }
            return courses; // everything good -- return populated courses vector
        }
    }

    namespace hashTable
    {
        /**
         * Load courses from csv into a hash table
         *
         * @param filePath the filepath for the csv to load
         * @return a pointer to a HashTable object
         */
        HashTable *LoadCourses(std::string filePath)
        {
            HashTable *courses = new HashTable(100);         // create hash table with 100 buckets
            csv::Parser parser = csv::Parser(filePath); // load csv file

            for (int i = 0; i < parser.rowCount(); ++i) // loop through rows in csv
            {
                csv::Row &row = parser.getRow(i); // get current row
                if (row.size() >= 2)              // check if row has at least 2 elements
                {
                    Course *course = new Course(); // create a new course object and pointer
                    course->courseNumber = row[0]; // set course number to 1st element in row
                    course->title = row[1];        // set title to 2nd element in row
                    if (row.size() > 2)
                    { // check if there are any prerequisites
                        for (int j = 2; j < row.size(); ++j)
                        {                                      // loop through prerequisites
                            const std::string prereq = row[j]; // get reference to prereq
                            if (parser.isValidPrerequisite(prereq))
                            {                                            // check if current prerequisite is valid
                                course->prerequisites.push_back(prereq); // valid, so add
                            }
                            else
                            {
                                // invalid prereq -- print error message
                                std::cout << "Invalid Prerequisite " << prereq << std::endl;
                                delete course;          // free memory for current course
                                if (courses != nullptr) // check if any courses exist
                                    delete courses;     // free memory from courses pointer
                                return nullptr;         // return null
                            }
                        }
                    }
                    courses->Insert(course); // current course data is good, add to courses
                }
                else
                {
                    // invalid number of parameters in CSV row
                    std::cout << "Invalid number of parameters in current row!";
                    if (courses != nullptr)
                        delete courses; // free memory from courses pointer
                    return nullptr;
                }
            }
            return courses; // everything good -- return courses pointer
        }
    }

    namespace binarySearchTree
    {
        /**
         * EXTRA CREDIT
         * recursive function that tallies up all of the
         * preqrequsites for each course and assigns
         * a weight based on total # prereqs to the current course.
         *
         * @param courseNumber the course number to calculate a weight for
         * @param courses a mapping of course number to course objects
         * @param processed a mapping of course numbers to if they've been processed yet
         */
        void getPrerequisiteWeight(
            std::string courseNumber,
            std::unordered_map<std::string, Course *> &courses,
            std::unordered_map<std::string, bool> &processed
            )
        {
            int prereqCount = courses[courseNumber]->prerequisites.size(); // get number of prereqs
            if (prereqCount > 0)
            { // course has at least 1 prerequisite
                for (int i = 0; i < prereqCount; i++)
                {                                                                    // iterate through all prerequisites
                    std::string prereq = courses[courseNumber]->prerequisites.at(i); // get course number of prereq
                    if (!processed[prereq])
                    {                                  // prerequisite course wasn't processed yet
                        getPrerequisiteWeight(prereq, courses, processed); // process the prereq recursively
                    }
                    // prereq is now guaranteed to be processed
                    if (courses[courseNumber]->hasPrerequisite[prereq] == false) { // current course doesn't have prereq yet
                        courses[courseNumber]->extendedPrereqs.push_back(prereq); // add current prereq to current course's extended prereqs
                        courses[courseNumber]->hasPrerequisite[prereq] = true; // mark current course as having current prereq
                    }
                    for (auto& it : courses[prereq]->extendedPrereqs) { // iterate through prereqs' prereqs
                        if (courses[courseNumber]->hasPrerequisite[it] == false) { // current course doesn't have current prereq yet
                            courses[courseNumber]->extendedPrereqs.push_back(it); // add current prereq to current course's extended prereqs
                            courses[courseNumber]->hasPrerequisite[it] = true; // mark current course as having current prereq
                        }
                    }
                }
                processed[courseNumber] = true;             // mark course number as processed
                return;
            }
            else
            {                                            // course has no prerequisites
                processed[courseNumber] = true;          // mark course number as processed
                return;
            }
        }

        /**
         * EXTRA CREDIT
         * This function iterates through any top level courses that
         * are not a prerequisite for any other courses and calls a
         * recursive function to calculate the prerequisite weights
         * for each course. It then trims any duplicate weight from
         * courses that were processed more than once.
         *
         * @param noPostReqs a list of course numbers that are not prerequirements
         * @param courses a mapping of course numbers to course objects
         */
        void GetPrequisiteWeights(
            std::vector<std::string> noPostReqs,
            std::unordered_map<std::string, Course *> &courses)
        {
            std::unordered_map<std::string, bool> processed; // mark if a course has been processed yet
            std::vector<std::string> postProcess;            // mark courses that were processed more than once
            for (int i = 0; i < noPostReqs.size(); i++)
            { // iterate through all courses that are not prerequisites of any other courses
                // calculate weight for current top-level course that is not a prerequisite
                getPrerequisiteWeight(noPostReqs.at(i), courses, processed);
            }
        }

        BinarySearchTree *LoadCourses(std::string filePath)
        {
            std::vector<Course *> tempCourses;                 // list of pointers to all courses
            std::vector<std::string> courseNumbers;            // list of all course numbers
            std::vector<std::string> noPostReqs;               // list of courses that aren't prereqs to another class
            std::unordered_map<std::string, bool> hasPostReqs; // map if a course number has post requirements or not
            std::unordered_map<std::string, Course *> courses; // map course number to course object

            csv::Parser parser = csv::Parser(filePath); // create csv parser and load csv
            for (int i = 0; i < parser.rowCount(); ++i)
            {                                     // loop through all rows in csv
                csv::Row &row = parser.getRow(i); // get a reference to current row
                if (row.size() >= 2)
                {                                                  // check if row has at least 2 elements
                    Course *course = new Course();                 // initialize a new course object
                    course->courseNumber = row[0];                 // set course number to 1st element in row
                    courseNumbers.push_back(course->courseNumber); // mark course as seen
                    course->title = row[1];                        // set course title to 2nd element in row
                    if (row.size() > 2)
                    { // check if there are any prerequisites
                        for (int j = 2; j < row.size(); ++j)
                        {                                      // loop through prerequisites
                            const std::string prereq = row[j]; // get reference to prereq.
                            if (parser.isValidPrerequisite(prereq))
                            {                                            // check if current prerequisite is valid
                                course->prerequisites.push_back(prereq); // valid, so add
                                hasPostReqs[prereq] = true;              // a prereq is guaranteed to have postreq(s)
                            }
                            else
                            {
                                // invalid prereq -- print error message
                                std::cout << "Invalid Prerequisite " << prereq << std::endl;
                                for (auto &it : tempCourses)
                                { // iterate through all temp courses and delete them
                                    delete it;
                                }
                                return nullptr;
                            }
                        }
                    }
                    tempCourses.push_back(course);                      // current course data is good, add to courses
                    courses[course->courseNumber] = tempCourses.back(); // map course number to course
                }
                else
                { // invalid number of parameters in CSV row
                    std::cout << "Invalid number of parameters in current row!";
                    for (auto &it : tempCourses)
                    { // iterate through all temp courses and delete them
                        delete it;
                    }
                    return nullptr;
                }
            }

            // find courses without post requirements (top of list/only has prereqs)
            std::cout << "Courses that are not a prerequisite:";
            for (int i = 0; i < courseNumbers.size(); i++)
            { // iterate through all course numbers
                if (hasPostReqs[courseNumbers[i]] == false)
                {                                           // course is not a prerequisite to any other course
                    noPostReqs.push_back(courseNumbers[i]); // add course number to top list
                    std::cout << " " << courseNumbers[i];
                }
            }
            std::cout << std::endl;

            // get prerequisite weights
            GetPrequisiteWeights(noPostReqs, courses);
            for (auto &it : courses)
            { // debugging/logging information about courses and their weights
                std::cout << "Course Number: " << it.first << ", Requirement Weight: " << it.second->extendedPrereqs.size() << std::endl;
            }

            BinarySearchTree *bstCourses = new BinarySearchTree(); // create new BST
            for (auto &it : tempCourses)
            { // iterate through all courses and add them to the BST
                bstCourses->Insert(*it);
                delete it; // remove from temp courses list
            }
            return bstCourses; // everything good -- return courses pointer
        }
    }

    /**
     * Helper function to take a course and print
     * the course's information
     *
     * @param course reference to course to be printed
     */
    void printCourse(Course *course)
    {
        if (course != nullptr)
        {                                                              // check if course provided is not empty
            std::cout << course->courseNumber << " " << course->title; // print course number and title
            if (course->prerequisites.size() > 0)
            {                                     // check if course has any prerequisites
                std::cout << ", Prerequisites: "; // print prerequisites
                for (int i = 0; i < course->prerequisites.size(); ++i)
                {                                             // loop through each prerequisite
                    if (i > 0)                                // if not the first prereq...
                        std::cout << ", ";                    // print a comma
                    std::cout << course->prerequisites.at(i); // print the prereq. course
                }
            }
            std::cout << std::endl; // print a new line
        }
        else
        { // course provided had empty data/not found
            std::cout << "Course not found...\n";
        }
    }

    /**
     * Partition the vector of courses into two parts, low and high
     *
     * @param courses Address of the vector<Course> instance to be partitioned
     * @param begin Beginning index to partition
     * @param end Ending index to partition
     */
    int partition(std::vector<Course> &courses, int begin, int end)
    {
        // set low and high equal to begin and end
        int low = begin;
        int high = end;

        // pick the middle element as pivot point
        int midpoint = low + (high - low) / 2;
        std::string pivot = courses[midpoint].courseNumber;

        bool done = false;

        // while not done
        while (!done)
        {
            // keep incrementing low index while bids[low] < bids[pivot]
            while (courses[low].courseNumber < pivot)
            {
                low++; // increment low
            }

            // keep decrementing high index while bids[pivot] < bids[high]
            while (pivot < courses[high].courseNumber)
            {
                high--; // decrement high
            }
            /* If there are zero or one elements remaining,
                all bids are partitioned. Return high */
            if (low >= high)
            {
                done = true;
            }
            // else swap the low and high bids (built in vector method)
            else
            {
                std::swap(courses[low], courses[high]);

                // move low and high closer ++low, --high
                ++low;
                --high;
            }
        }

        return high;
    }

    /**
     * Partition the vector of courses into two parts, low and high
     *
     * @param keys Address of the vector<int> instance to be partitioned
     * @param begin Beginning index to partition
     * @param end Ending index to partition
     */
    int partition(std::vector<std::string> &keys, int begin, int end)
    {
        // set low and high equal to begin and end
        int low = begin;
        int high = end;

        // pick the middle element as pivot point
        int midpoint = low + (high - low) / 2;
        std::string pivot = keys[midpoint];

        bool done = false;

        // while not done
        while (!done)
        {
            // keep incrementing low index while keys[low] < keys[pivot]
            while (keys[low] < pivot)
            {
                low++; // increment low
            }

            // keep decrementing high index while keys[pivot] < keys[high]
            while (pivot < keys[high])
            {
                high--; // decrement high
            }
            /* If there are zero or one elements remaining,
                all keys are partitioned. Return high */
            if (low >= high)
            {
                done = true;
            }
            // else swap the low and high bids (built in vector method)
            else
            {
                std::swap(keys[low], keys[high]);

                // move low and high closer ++low, --high
                ++low;
                --high;
            }
        }

        return high;
    }

    /**
     * Perform a quick sort on course number
     * Average performance: O(n log(n))
     * Worst case performance O(n^2))
     *
     * @param courses address of the vector<Course> instance to be sorted
     * @param begin the beginning index to sort on
     * @param end the ending index to sort on
     */
    void quickSort(std::vector<Course> &courses, int begin, int end)
    {
        // set mid equal to 0
        int mid = 0;

        int low = begin;
        int high = end;
        /* Base case: If there are 1 or zero courses to sort,
         * partition is already sorted otherwise if begin is greater
         * than or equal to end then return
         */
        if (low >= high)
        {
            return;
        }

        /* Partition bids into low and high such that
         midpoint is location of last element in low */
        mid = partition(courses, low, high);

        // recursively sort low partition (begin to mid)
        quickSort(courses, low, mid);

        // recursively sort high partition (mid+1 to end)
        quickSort(courses, mid + 1, high);
    }

    /**
     * Perform a quick sort on course number
     * Average performance: O(n log(n))
     * Worst case performance O(n^2))
     *
     * @param keys address of the vector<int> instance to be sorted
     * @param begin the beginning index to sort on
     * @param end the ending index to sort on
     */
    void quickSort(std::vector<std::string> &keys, int begin, int end)
    {
        // set mid equal to 0
        int mid = 0;

        int low = begin;
        int high = end;
        /* Base case: If there are 1 or zero bids to sort,
         * partition is already sorted otherwise if begin is greater
         * than or equal to end then return
         */
        if (low >= high)
        {
            return;
        }

        /* Partition bids into low and high such that
         midpoint is location of last element in low */
        mid = partition(keys, low, high);

        // recursively sort low partition (begin to mid)
        quickSort(keys, low, mid);

        // recursively sort high partition (mid+1 to end)
        quickSort(keys, mid + 1, high);
    }
}
