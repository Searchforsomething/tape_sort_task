#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <string>
#include <unordered_map>

class ConfigParser {
public:
    explicit ConfigParser(const std::string &filename);

    int getIntValue(const std::string &key, int defaultValue = 0) const;

private:
    std::unordered_map<std::string, int> configValues;

    void loadConfig(const std::string &filename);
};

#endif // CONFIG_PARSER_H
