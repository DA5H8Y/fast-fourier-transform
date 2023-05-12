#include <string>
#include <fstream>
#include <istream>
#include <algorithm>
#include <filesystem>
#include "CSVParser.hpp"

template<typename T>
CSVParser<T>::CSVParser(const std::string& fName, bool hasColumnNames) :
  _fName(fName),
  _hasColumnNames(hasColumnNames)
{
    if (std::filesystem::exists(_fName))
    {
        _rawDataCollection = readCSV();
        convertFromRaw();
    }
    else
    {
        std::stringstream msg("");
        msg << "The file: " << _fName << " does not exist." << std::endl;
        throw std::exception(msg);
    }

}

template<typename T>
bool CSVParser<T>::convertFromRaw()
{
    bool success = true;

    int idx = 0;

    if (_hasColumnNames)
    {
        _columnNames = _rawDataCollection[idx];
        idx;
    }

    for(;idx<_rawDataCollection.size();idx++)
    {
        std::vector<std::string> rawRow = _rawDataCollection[idx];
        record row;
        for(int j=0;j<rawRow.size();j++)
        {
            row.push_back(get_as<T>(rawRow[j]));
        }
        _dataCollection.push_back(row);
    }


    return true;
}

template<typename T>
std::vector<std::string> CSVParser<T>::readCSVRow(const std::string &row) {
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

template<typename T>
std::vector<std::vector<std::string>> CSVParser<T>::readCSV(std::istream &in) {
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

template<typename T>
std::vector<T> CSVParser<T>::getRecord(const unsigned int idx) const
{
    if (idx >= getNumberOfRecords() )
    {
        throw std::exception("Index exceeds number of records.");
    }
    else
    {
        return _dataCollection[idx];
    }
    
}

template<typename T>
T CSVParser<T>::getEntry(const unsigned int idx, const unsigned int colIdx) const
{
    if (idx >= getNumberOfRecords() )
    {
        throw std::exception("Index exceeds number of records.");
    }
    
    if (colIdx >= getNumberOfDataEntries() )
    {
        throw std::exception("Index exceeds number of data entries.");
    }
    
    return _dataCollection[idx][colIdx];
}

template<typename T>
const std::vector<T> CSVParser<T>::getAllEntries(const std::string& colName) const
{
    auto maxIt = std::find(_columnNames.begin(), _columnNames.end(), colName);
    unsigned int idx = maxIt - _columnNames.begin();
    std::vector<T> data;
    for (int i=0; i<getNumberOfRecords(); i++)
    {
        data.push_back(getEntry(i, idx));
    }
    return data;
}