### Waveform Analyser
This program takes csv files of signal data in the format Seconds, Volts.  Calculates the frequency of the highest amplitude wave in each file. Then, outputs a CSV file containing the waveform filename in the first column and the waveform frequency in the second column. Sorted in ascending frequency.

## Build
This program has been develop to use premake, avaliable at [https://premake.github.io/](https://premake.github.io/)  to generate the build files.

To build run :
```
premake5 --file=./build/premake5.lua gmake
cd ./build/gmake
make config=release_x64
```

If you wish to cross-platform build add ```--os=windows,``` to the premake arguments


## Run
To execute run:

```
./bin/WaveFileAnalysis ./WaveFiles/
```

## Testing
This program has been testing under Linux with an _x86_64 architecture. Using the gmake action in premake5.
