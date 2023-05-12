#include <string>
#include <vector>
#include <istream>

class CSVParser
{
 using record = std::vector<double>;
 public:
   CSVParser(const std::string& filename, bool hasColumnNames = true);
   std::vector<std::string> getColumnNames() const;
   int getNumberOfRecords() const { return _dataCollection.size(); };
   int getNumberOfDataEntries() const { return getRecord(1).size(); };

   std::vector<record> getDataCollection() const { return _dataCollection; };
   bool addDataCollection(std::vector<record>, const int idx = 0);
   bool insertDataCollection(std::vector<record>, const int idx = 0);
   bool replaceDataCollection(std::vector<record>, const int idx = 0);

   record getRecord(const unsigned int idx) const;
   bool addRecord(record, const unsigned int idx = 0);
   bool insertRecord(record, const unsigned int idx = 0);
   bool replaceRecord(record, const unsigned int idx = 0);

   double getEntry(const unsigned int idx, const unsigned int colIdx) const;
   double getEntry(const unsigned int idx, const std::string& colName) const;
   const std::vector<double> getAllEntries(const unsigned int colIdx) const;
   const std::vector<double> getAllEntries(const std::string& colName) const;
   bool replaceEntry(double, const unsigned int idx = 0, const unsigned int colIdx = 0);
   bool replaceEntry(double, const unsigned int idx = 0, const std::string& colName = "");

 private:
   std::vector<std::vector<std::string>> _rawDataCollection;
   std::vector<record> _dataCollection;
   std::vector<std::string> _columnNames;
   std::string _fName;
   bool _hasColumnNames;
   bool parseCSVFile();
   std::vector<std::string> readCSVRow(const std::string &row);
   std::vector<std::vector<std::string>> readCSV(std::istream &in);
   bool convertFromRaw();
   double get_as( std::string& s );
   bool saveCSVFile();

   enum class CSVState {
      UnquotedField,
      QuotedField,
      QuotedQuote
   };
};