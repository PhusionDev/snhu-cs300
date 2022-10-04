//
//  CSVparser.hpp
//  CS300-Project1
//
//  Code originally from SNHU
//  Modified by Jared Hodgkins on 9/23/22.
//  Stripped functionality for header row,
//  since it's not being used in this application
//

#ifndef CSVparser_hpp
#define CSVparser_hpp

#include <stdexcept>
#include <string>
#include <vector>
#include <list>
#include <sstream>

namespace csv
{
    class Error : public std::runtime_error
    {

    public:
        Error(const std::string &msg) : std::runtime_error(std::string("CSVparser : ").append(msg))
        {
        }
    };

    class Row
    {
    public:
        Row(void);
        ~Row(void);

    public:
        unsigned int size(void) const;
        void push(const std::string &);
        bool set(const std::string &, const std::string &);

    private:
        std::vector<std::string> _values;

    public:
        template <typename T>
        const T getValue(unsigned int pos) const
        {
            if (pos < _values.size())
            {
                T res;
                std::stringstream ss;
                ss << _values[pos];
                ss >> res;
                return res;
            }
            throw Error("can't return this value (doesn't exist)");
        }
        const std::string operator[](unsigned int) const;
    };

    class Parser
    {

    public:
        Parser(const std::string &data);
        ~Parser(void);

    public:
        Row &getRow(unsigned int row) const;
        unsigned int rowCount(void) const;
        const std::string &getFileName(void) const;
        bool isValidPrerequisite(const std::string &courseNumber);

    protected:
        void parseContent(void);

    private:
        std::string _file;
        std::vector<std::string> _originalFile;
        std::vector<Row *> _content;

    public:
        Row &operator[](unsigned int row) const;
    };
}

#endif /* CSVparser_hpp */
