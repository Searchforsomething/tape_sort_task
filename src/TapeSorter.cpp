#include "TapeSorter.h"
#include <fstream>
#include <iostream>
#include <filesystem>

TapeSorter::TapeSorter(FileTape &input, FileTape &output, const ConfigParser &config, size_t tapeSize, bool debug)
        : inputTape(input), outputTape(output), config(const_cast<ConfigParser &>(config)),
          tmp_tape1("tmp/tape1", config, tapeSize, debug),
          tmp_tape2("tmp/tape2", config, tapeSize, debug),
          debug(debug),
          tapeSize(tapeSize) {}


void TapeSorter::splitRuns(FileTape &fromTape) {
    if (debug) std::cout << "split run\n";
    bool writeToFirst = true;
    int32_t prev, curr;

    while (fromTape.getPosition() > 0) fromTape.moveBackward();

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
    if (debug)
        std::cout << "merge run\ntmp/tape1 datasize = " << tmp_tape1.getDataSize() << "; tmp/tape2 datasize = "
                  << tmp_tape2.getDataSize() << "\n";

    if (tmp_tape1.getDataSize() == 0 || tmp_tape2.getDataSize() == 0) {
        return false;
    }
    outputTape.restart();

    while (tmp_tape1.getPosition() > 0) {
        tmp_tape1.moveBackward();
    }
    while (tmp_tape2.getPosition() > 0) {
        tmp_tape2.moveBackward();
    }
    int32_t val1 = tmp_tape1.read();
    int32_t val2 = tmp_tape2.read();

    while (outputTape.getDataSize() < tapeSize) {
        if (debug) std::cout << "val1 = " << val1 << "; val2 = " << val2 << "\n";
        if ((val1 < val2 || tmp_tape2.getDataSize() <= tmp_tape2.getPosition()) &&
            tmp_tape1.getPosition() < tmp_tape1.getDataSize()) {
            outputTape.write(val1);
            tmp_tape1.moveForward();
            try {
                val1 = tmp_tape1.read();
            } catch (...) {
                val1 = INT32_MAX;
            }
        } else {
            outputTape.write(val2);
            tmp_tape2.moveForward();
            try {
                val2 = tmp_tape2.read();
            } catch (...) {
                val2 = INT32_MAX;
            }
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
