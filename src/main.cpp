#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include "CSVParser.hpp"
#include "Signal.hpp"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Expect syntax is FFT [Directory of wave files]" << std::endl;
        return -1;
    }

    std::string directory{argv[1]};
    std::vector<std::string> waveFiles;
    std::vector<double> waveResults;

    for(auto& p: std::filesystem::directory_iterator(directory))
    {
        if (p.is_regular_file())
        {
            waveFiles.push_back(p.path());
            CSVParser<double> tmpParse(p.path());

            // Compute the Fourier transform of the signal.
            Signal<double> sig((tmpParse.getAllEntries("Seconds")), (tmpParse.getAllEntries("Volts")));

            waveResults.push_back(sig.getMaxFrequency());

            std::cout << *(waveFiles.end()) << ", " << *(waveResults.end()) << std::endl;
        }
    }

    return 0;
}