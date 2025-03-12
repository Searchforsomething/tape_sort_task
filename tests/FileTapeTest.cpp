#include <gtest/gtest.h>
#include "../src/FileTape.h"
#include "../src/TapeSorter.h"
#include <vector>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

class ConfigTest {
public:
    std::string configName = "test_config.conf";

    void SetUpConfig() {
        std::ofstream configFile(configName);
        configFile << "read_delay=15\nwrite_delay=15\nmove_delay=15";
    }

    void TearDownConfig() {
        if (fs::exists(configName)) {
            fs::remove(configName);
        }
    }
};

class FileTapeTest : public ::testing::Test {
protected:
    std::string testFile = "test_tape.bin";
    FileTape *tape;
    ConfigTest configFile;

    void SetUp() override {
        configFile.SetUpConfig();
        if (fs::exists(testFile)) {
            fs::remove(testFile);
        }
        ConfigParser config(configFile.configName);

        tape = new FileTape(testFile, config, 5, false);
    }

    void TearDown() override {
        delete tape;
        if (fs::exists(testFile)) {
            fs::remove(testFile);
        }
        configFile.TearDownConfig();
    }
};

TEST_F(FileTapeTest, FileCreation) {
    ASSERT_TRUE(fs::exists(testFile));
}

TEST_F(FileTapeTest, WriteAndRead) {
    tape->write(42);
    int32_t value = tape->read();
    ASSERT_EQ(value, 42);
}

TEST_F(FileTapeTest, MoveForwardAndBackward) {
    tape->moveForward();
    ASSERT_EQ(tape->getPosition(), 1);
    tape->moveBackward();
    ASSERT_EQ(tape->getPosition(), 0);
}

TEST_F(FileTapeTest, MoveBeyondBoundaries) {
    for (size_t i = 0; i < 5; ++i) {
        tape->moveForward();
    }
    ASSERT_THROW(tape->moveForward(), std::runtime_error);

    tape->restart();
    ASSERT_THROW(tape->moveBackward(), std::runtime_error);
}

TEST_F(FileTapeTest, Restart) {
    tape->write(100);
    tape->moveForward();
    tape->write(200);
    tape->restart();
    ASSERT_EQ(tape->getPosition(), 0);
    ASSERT_EQ(tape->getDataSize(), 0);
}

FileTape vectorToFileTape(std::string fileName, std::vector<int32_t> data, ConfigParser *config, size_t size) {
    std::ofstream file(fileName, std::ios::binary);
    for (int32_t num: data) {
        file.write(reinterpret_cast<const char *>(&num), sizeof(int32_t));
    }
    file.close();
    return FileTape(fileName, *config, size, false);
}

std::vector<int32_t> fileTapeToVector(FileTape *tape) {
    std::vector<int32_t> tapeData(tape->getDataSize());
    while (tape->getPosition() < tape->getDataSize()) {
        tapeData[tape->getPosition()] = tape->read();
        tape->moveForward();
    }
    return tapeData;
}

class TapeSorterTest : public ::testing::Test {
protected:
    ConfigTest configFile;
    ConfigParser *config;

    void SetUp() override {
        configFile.SetUpConfig();
        config = new ConfigParser(configFile.configName);
        fs::create_directory("tmp");
    }

    void TearDown() override {
        configFile.TearDownConfig();
        fs::remove_all("tmp");
    }

};

TEST_F(TapeSorterTest, TestSorter) {
    std::vector<int32_t> data = {5, 4, 3, 3, 4, 0, -1, -2, -1, 0};
    FileTape tape = vectorToFileTape("test_tape.bin", data, config, 10);
    FileTape outputTape("output.bin", *config, 10, false);
    TapeSorter sorter(tape, outputTape, *config, 10, false);
    sorter.sort();
    FileTape outTape("output.bin", *config, 10, false);
    std::vector<int32_t> resultData = fileTapeToVector(&outTape);
    std::vector<int32_t> expectedData = {-2, -1, -1, 0, 0, 3, 3, 4, 4, 5};
    ASSERT_EQ(resultData, expectedData);
}


TEST_F(TapeSorterTest, TestSorter2) {
    std::vector<int32_t> data = {INT32_MAX, 150, 3, INT32_MIN, 0, INT32_MAX, -2, -150, INT32_MIN};
    FileTape tape = vectorToFileTape("test_tape.bin", data, config, 10);
    FileTape outputTape("output.bin", *config, 9, false);
    TapeSorter sorter(tape, outputTape, *config, 9, false);
    sorter.sort();
    FileTape outTape("output.bin", *config, 9, false);
    std::vector<int32_t> resultData = fileTapeToVector(&outTape);
    std::vector<int32_t> expectedData = {INT32_MIN, INT32_MIN, -150, -2, 0, 3, 150, INT32_MAX, INT32_MAX};
    ASSERT_EQ(resultData, expectedData);
}


int main(int argc, char *argv[]) {

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}