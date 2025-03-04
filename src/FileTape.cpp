#include "FileTape.h"
#include <filesystem>


FileTape::FileTape(const std::string &fileName, const ConfigParser &config, size_t size)
        : fileName(fileName),
          position(0),
          tapeSize(size),
          dataSize(0),
          readDelay(config.getIntValue("read_delay", 10)),
          writeDelay(config.getIntValue("write_delay", 10)),
          moveDelay(config.getIntValue("move_delay", 10)) {

    if (!std::filesystem::exists(fileName)) {
        std::ofstream newFile(fileName, std::ios::binary);
        if (!newFile) {
            throw std::runtime_error("Ошибка создания файла: " + fileName);
        }

        int32_t zero = 0;
        for (size_t i = 0; i < tapeSize; ++i) {
            newFile.write(reinterpret_cast<char*>(&zero), sizeof(int32_t));
        }

        if (newFile.fail()) { // Проверяем ошибки записи
            throw std::runtime_error("Ошибка записи в новый файл: " + fileName);
        }

        newFile.close();
    } else {
        dataSize = tapeSize;
    }

    file.open(fileName, std::ios::in | std::ios::out | std::ios::binary);
    if (!file) {
        throw std::runtime_error("Ошибка повторного открытия файла: " + fileName);
    }
}

FileTape::~FileTape() {
    file.close();
}

void FileTape::applyDelay(int delay) {
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
}

int32_t FileTape::read() {
    applyDelay(readDelay);

    file.seekg(position * sizeof(int32_t), std::ios::beg);

    int32_t value;
    file.read(reinterpret_cast<char*>(&value), sizeof(int32_t));

    if (!file) {
        throw std::runtime_error("Ошибка чтения с ленты на позиции " + std::to_string(position));
    }

    return value;
}

void FileTape::write(int32_t value) {
    applyDelay(writeDelay);

    file.seekp(position * sizeof(int32_t), std::ios::beg);
    file.write(reinterpret_cast<const char*>(&value), sizeof(int32_t));

    if (!file) {
        throw std::runtime_error("Ошибка записи на ленту на позиции " + std::to_string(position));
    }

    if (position >= dataSize) {
        dataSize = position + 1;
    }
}

void FileTape::moveForward() {
    applyDelay(moveDelay);
    if (position < tapeSize) {
        position++;
    } else {
        throw std::runtime_error("Ошибка: попытка выйти за пределы ленты при moveForward");
    }
}

void FileTape::moveBackward() {
    applyDelay(moveDelay);
    if (position > 0) {
        position--;
    } else {
        throw std::runtime_error("Ошибка: попытка выйти за пределы ленты при moveBackward");
    }
}

size_t FileTape::getPosition() {
    return position;
}

size_t FileTape::getDataSize() {
    return dataSize;
}

void FileTape::restart() {
    while (position > 0) {
        moveBackward();
    }
    dataSize = 0;
}
