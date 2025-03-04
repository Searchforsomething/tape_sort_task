#include <filesystem>
#include "FileTape.h"
#include "TapeSorter.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_tape> <output_tape>\n";
        return 1;
    }

    ConfigParser config("config.ini");
    std::ifstream file_in(argv[1], std::ios::binary | std::ios::ate);
    if (!file_in) {
        throw std::runtime_error("Не удалось открыть входной файл");
    }

    std::streamsize fileSize = file_in.tellg() / 4;

    FileTape inputTape(argv[1], config, fileSize);
    std::ofstream file_out(argv[2], std::ios::binary);
    FileTape outputTape(argv[2], config, fileSize);
    fs::create_directory("tmp");
    TapeSorter sorter(inputTape, outputTape, config, fileSize);
    sorter.sort();
    fs::remove_all("tmp");

    return 0;
}
