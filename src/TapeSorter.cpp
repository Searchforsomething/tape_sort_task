#include "TapeSorter.h"
#include <fstream>
#include <iostream>
#include <filesystem>

TapeSorter::TapeSorter(FileTape &input, FileTape &output, const ConfigParser &config, size_t tapeSize)
        : inputTape(input), outputTape(output), config(const_cast<ConfigParser &>(config)),
          tmp_sorted_tape(FileTape("tmp/sorted", config, tapeSize)),
          tmp_tape1("tmp/tape1", config, tapeSize),
          tmp_tape2("tmp/tape2", config, tapeSize),
          tapeSize(tapeSize) {}


void TapeSorter::splitRuns(FileTape &fromTape) {

    bool writeToFirst = true;
    int32_t prev, curr;

    try {
        prev = fromTape.read();
        fromTape.moveForward();
    } catch (...) {
        return;
    }
    tmp_tape1.write(prev);
    tmp_tape1.moveForward();

    while (true) {
        try {
            curr = fromTape.read();
            fromTape.moveForward();
        } catch (...) {
            break;
        }

        if (curr < prev) {
            writeToFirst = !writeToFirst;
        }
        if (writeToFirst) {
            tmp_tape1.write(curr);
            tmp_tape1.moveForward();
        } else {
            tmp_tape2.write(curr);
            tmp_tape2.moveForward();
        }
        prev = curr;
    }

}

bool TapeSorter::mergeRuns() {
    outputTape.restart();

    if (tmp_tape1.getDataSize() == 0 || tmp_tape2.getDataSize() == 0) {
        return false;
    }

    while (tmp_tape1.getPosition() > 0) {
        tmp_tape1.moveBackward();
    }
    while (tmp_tape2.getPosition() > 0) {
        tmp_tape2.moveBackward();
    }
    int32_t val1 = tmp_tape1.read();
    int32_t val2 = tmp_tape2.read();

    while (outputTape.getDataSize() < tapeSize) {

        if (val1 < val2 && tmp_tape1.getPosition() < tmp_tape1.getDataSize()) {
            outputTape.write(val1);
            tmp_tape1.moveForward();
            val1 = tmp_tape1.read();
        } else {
            outputTape.write(val2);
            tmp_tape2.moveForward();
            val2 = tmp_tape2.read();
        }
        outputTape.moveForward();
    }

    tmp_tape1.restart();
    tmp_tape2.restart();

    return true;
}


void TapeSorter::sort() {
    splitRuns(inputTape);
    if (mergeRuns()) {
        do {
            splitRuns(outputTape);
        } while (mergeRuns());
    }

}
