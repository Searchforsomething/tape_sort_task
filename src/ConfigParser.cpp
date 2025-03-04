#include "ConfigParser.h"
#include <fstream>
#include <sstream>
#include <iostream>

ConfigParser::ConfigParser(const std::string& filename) {
    loadConfig(filename);
}

void ConfigParser::loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Ошибка: не удалось открыть файл конфигурации: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        int value;

        if (std::getline(iss, key, '=') && (iss >> value)) {
            configValues[key] = value;
        }
    }
}

int ConfigParser::getIntValue(const std::string& key, int defaultValue) const {
    auto it = configValues.find(key);
    return (it != configValues.end()) ? it->second : defaultValue;
}
