#ifndef TAPESORTER_H
#define TAPESORTER_H

#include "FileTape.h"
#include <vector>
#include <string>

class TapeSorter {
private:
    FileTape &inputTape;
    FileTape &outputTape;
    ConfigParser &config;
    FileTape tmp_tape1;
    FileTape tmp_tape2;
    size_t tapeSize;
    bool debug;

    void splitRuns(FileTape &fromTape);

    bool mergeRuns();

public:
    TapeSorter(FileTape &input, FileTape &output, const ConfigParser &config, size_t tapeSize, bool debug);

    void sort();
};

#endif
