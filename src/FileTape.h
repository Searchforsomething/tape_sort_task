#ifndef FILETAPE_H
#define FILETAPE_H

#include "ConfigParser.h"
#include "TapeInterface.h"
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>

class FileTape : public ITape {
private:
    std::fstream file;
    std::string fileName;
    int readDelay, writeDelay, moveDelay;
    size_t tapeSize;
    size_t dataSize;
    size_t position;
    bool debug;

    static void applyDelay(int delay);

public:
    explicit FileTape(const std::string &filename, const ConfigParser &config, size_t size, bool debug);

    ~FileTape() override;

    int32_t read() override;

    void write(int32_t value) override;

    void moveForward() override;

    void moveBackward() override;

    size_t getPosition() override;

    size_t getDataSize() override;

    void restart() override;

};

#endif
