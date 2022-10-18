//
//  CSVparser.cpp
//  CS300-Project1
//
//  Most code originally from CS300 SNHU
//  Modified by Jared Hodgkins on 9/23/22.
//  Modified version assumes csv file does
//  not have a header row as provided by
//  CS300 materials.
//

#include <fstream>
#include <iomanip>
#include <iostream>
#include "CSVparser.hpp"

namespace csv
{
    /**
     * Create a new csv parser object with
     * specified csv file and parse the data
     *
     * @param data the csv file to load
     */
    Parser::Parser(const std::string &data)
    {
        std::string line;                   // value to hold current line in csv file
        _file = data;                       // alias to filename/filepath
        std::cout << "Loading csv file: " << data << std::endl;
        std::ifstream ifile(_file.c_str()); // create input file stream and load csv file
        if (ifile.is_open())
        { // if file is open
            while (ifile.good())
            {                         // loop while not end of file or no error
                getline(ifile, line); // get current line in csv and store it
                if (line != "")
                    _originalFile.push_back(line); // add current line to list
            }
            ifile.close(); // finished with file -- close it

            if (_originalFile.size() == 0)                             // if no lines added to internal list
                throw Error(std::string("No Data in ").append(_file)); // throw no data error

            parseContent(); // parse the lines read from file
        }
        else
            throw Error(std::string("Failed to open ").append(_file)); // couldn't open file
    }

    /**
     * Free memory from all rows in parser
     */
    Parser::~Parser(void)
    {
        std::vector<Row *>::iterator it; // create an iterator

        for (it = _content.begin(); it != _content.end(); it++) // loop through all the rows
            delete *it;                                         // free row from memory
    }

    /**
     * Parse a csv file and extract its data
     * into row objects
     */
    void Parser::parseContent(void)
    {
        std::vector<std::string>::iterator it; // create an iterator
        it = _originalFile.begin();            // set iterator to first line

        for (; it != _originalFile.end(); it++)
        { // loop through all lines
            int tokenStart = 0;
            unsigned int i = 0;
            Row *row = new Row();

            for (; i != it->length(); i++)
            { // loop through all characters in line
                if (it->at(i) == ',')
                {                                                      // if current character is a comma (separator)
                    row->push(it->substr(tokenStart, i - tokenStart)); // add value in line up to comma to row
                    tokenStart = i + 1;                                // set new starting character
                }
            }

            // end
            row->push(it->substr(tokenStart, it->length() - tokenStart)); // add final value to row
            _content.push_back(row);                                      // add finished row to content
        }
    }

    /**
     * Helper method to get a row within the
     * parser's loaded data from csv
     *
     * @param rowPosition the row to access
     * @return the row object being asked for
     */
    Row &Parser::getRow(unsigned int rowPosition) const
    {
        if (rowPosition < _content.size())                    // make sure row is within content bounds
            return *(_content[rowPosition]);                  // return row being asked for
        throw Error("can't return this row (doesn't exist)"); // index out of bounds
    }

    /**
     * Operator overload to access a row within
     * the parser.
     *
     * @param rowPosition the row to access
     * @return the row in parser at given position
     */
    Row &Parser::operator[](unsigned int rowPosition) const
    {
        return Parser::getRow(rowPosition); // return bound-checked row
    }

    /**
     * Get the number of rows that the parser
     * has loaded into.
     *
     * @return number of rows loaded from csv
     */
    unsigned int Parser::rowCount(void) const
    {
        return _content.size();
    }

    /**
     * Get the file name of the file for the parser
     *
     * @return the parser's file name
     */
    const std::string &Parser::getFileName(void) const
    {
        return _file; // return file name
    }

    /**
     * Validates if a specific course number is
     * a vaild course number by checking all of the rows
     * in the csv data (rows).
     *
     * @param courseNumber the course number to verify
     * @return whether or not the course is valid
     */
    bool Parser::isValidPrerequisite(const std::string &courseNumber)
    {
        for (int i = 0; i < rowCount(); ++i)
        { // loop through all rows in csv data
            if (getRow(i).getValue<std::string>(0) == courseNumber)
                // first value of current row matches course number (is valid)
                return true;
        }
        // course with specified course number wasn't found (not valid)
        return false;
    }

    /*
    ** ROW
    */

    Row::Row(void) {} // blank constructor

    Row::~Row(void) {} // blank destructor

    /**
     * Helper function to get the size of a row
     *
     * @return number of values in the row
     */
    unsigned int Row::size(void) const
    {
        return _values.size();
    }

    /**
     * Helper to push a value onto _values
     *
     * @param value the value to add
     */
    void Row::push(const std::string &value)
    {
        _values.push_back(value);
    }

    /**
     * Provide out of bounds checking on
     * accessing indices of values in row
     *
     * @param valuePosition the index in _values to get
     * @return the value at given index
     */
    const std::string Row::operator[](unsigned int valuePosition) const
    {
        if (valuePosition < _values.size())                     // check that index given is within bounds
            return _values[valuePosition];                      // return value at index
        throw Error("can't return this value (doesn't exist)"); // index out of bounds
    }
}
