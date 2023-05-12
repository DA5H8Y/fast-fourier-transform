#include <string>
#include <fstream>
#include <istream>
#include <algorithm>
#include <filesystem>
#include <stdexcept>
#include "CSVParser.hpp"

CSVParser::CSVParser(const std::string& fName, bool hasColumnNames) :
  _fName(fName),
  _hasColumnNames(hasColumnNames)
{
    if (std::filesystem::exists(_fName))
    {
        std::ifstream file(_fName, std::ios_base::in);
        _rawDataCollection = readCSV(file);
        convertFromRaw();
        file.close();
    }
    else
    {
        std::stringstream msg("");
        msg << "The file: " << _fName << " does not exist." << std::endl;
        throw std::invalid_argument(msg.str().c_str());
    }

}

double CSVParser::get_as( std::string& s )
{
    std::stringstream convert(s);

    double value;
    convert >> value;
    return value;
}

bool CSVParser::convertFromRaw()
{
    bool success = true;

    int idx = 0;

    if (_hasColumnNames)
    {
        _columnNames = _rawDataCollection[idx];
        idx++;
    }

    for(;idx<_rawDataCollection.size();idx++)
    {
        std::vector<std::string> rawRow = _rawDataCollection[idx];
        record row;
        for(int j=0;j<rawRow.size();j++)
        {
            row.push_back(get_as(rawRow[j]));
        }
        _dataCollection.push_back(row);
    }


    return true;
}

std::vector<std::string> CSVParser::readCSVRow(const std::string &row) {
    CSVState state = CSVState::UnquotedField;
    std::vector<std::string> fields {""};
    size_t i = 0; // index of the current field
    for (char c : row) {
        switch (state) {
            case CSVState::UnquotedField:
                switch (c) {
                    case ',': // end of field
                              fields.push_back(""); i++;
                              break;
                    case '"': state = CSVState::QuotedField;
                              break;
                    default:  fields[i].push_back(c);
                              break; }
                break;
            case CSVState::QuotedField:
                switch (c) {
                    case '"': state = CSVState::QuotedQuote;
                              break;
                    default:  fields[i].push_back(c);
                              break; }
                break;
            case CSVState::QuotedQuote:
                switch (c) {
                    case ',': // , after closing quote
                              fields.push_back(""); i++;
                              state = CSVState::UnquotedField;
                              break;
                    case '"': // "" -> "
                              fields[i].push_back('"');
                              state = CSVState::QuotedField;
                              break;
                    default:  // end of quote
                              state = CSVState::UnquotedField;
                              break; }
                break;
        }
    }
    return fields;
}

std::vector<std::vector<std::string>> CSVParser::readCSV(std::istream &in) {
    std::vector<std::vector<std::string>> table;
    std::string row;
    while (!in.eof()) {
        std::getline(in, row);
        if (in.bad() || in.fail()) {
            break;
        }
        auto fields = readCSVRow(row);
        table.push_back(fields);
    }
    return table;
}

std::vector<double> CSVParser::getRecord(const unsigned int idx) const
{
    if (idx >= getNumberOfRecords() )
    {
        throw std::invalid_argument("Index exceeds number of records.");
    }
    else
    {
        return _dataCollection[idx];
    }
    
}

double CSVParser::getEntry(const unsigned int idx, const unsigned int colIdx) const
{
    if (idx >= getNumberOfRecords() )
    {
        throw std::invalid_argument("Index exceeds number of records.");
    }
    
    if (colIdx >= getNumberOfDataEntries() )
    {
        throw std::invalid_argument("Index exceeds number of data entries.");
    }
    
    return _dataCollection[idx][colIdx];
}

const std::vector<double> CSVParser::getAllEntries(const std::string& colName) const
{
    auto maxIt = std::find(_columnNames.begin(), _columnNames.end(), colName);
    unsigned int idx = (maxIt - _columnNames.begin());
    ( idx == 0 ) ? (idx = idx) : (idx = idx - 1);
    std::vector<double> data;
    for (int i=0; i<getNumberOfRecords(); i++)
    {
        data.push_back(getEntry(i, idx));
    }
    return data;
}