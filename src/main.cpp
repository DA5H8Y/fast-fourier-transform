#include <fstream>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <map>
#include "CSVParser.hpp"
#include "Signal.hpp"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Expect syntax is FFT [Directory of wave files]" << std::endl;
        return -1;
    }

    using resultpair = std::pair<std::string, double>;
    std::string directory{argv[1]};
    std::vector<resultpair> results;

    for(auto& p: std::filesystem::directory_iterator(directory))
    {
        if (p.is_regular_file())
        {
            // std::cout << "\33[2K\r";
            std::cout << "File: " << p.path() << std::endl;
            CSVParser tmpParse(p.path());

            // Compute the Fourier transform of the signal.
            Signal sig((tmpParse.getAllEntries("Seconds")), (tmpParse.getAllEntries("Volts")));

            results.push_back(std::make_pair<std::string, double>(p.path().filename().string(), sig.getMaxFrequency()));
        }
    }

    std::sort(results.begin(), results.end(), [](const resultpair& a, const resultpair& b) { return a.second > b.second; });

    std::ofstream out("Results.csv", std::ios_base::out);
    out << "filename" << ", " << "frequency" << std::endl;
    for(auto r : results )
    {
        std::cout << r.first << ", " << std::scientific << r.second << std::endl;
        out << r.first << ", " << std::scientific << r.second << std::endl;
    }
    
    return 0;
}