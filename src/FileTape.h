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
    std::fstream file;  // Файл, эмулирующий ленту
    std::string fileName;
    int readDelay, writeDelay, moveDelay;  // Задержки (мс)
    size_t tapeSize;
    size_t dataSize;
    size_t position;

    static void applyDelay(int delay);  // Функция задержки

public:
    explicit FileTape(const std::string &filename, const ConfigParser &config, size_t size);
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
