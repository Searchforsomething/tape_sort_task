#include <filesystem>
#include "FileTape.h"
#include "TapeSorter.h"
#include <boost/program_options.hpp>


namespace po = boost::program_options;
namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    bool debug = false;
    std::string inputFile, outputFile;

    po::options_description visible_desc("Allowed options");
    visible_desc.add_options()
            ("help,h", "Show help message")
            ("debug,d", po::bool_switch(&debug), "Enable debug mode");

    po::options_description hidden_desc;
    hidden_desc.add_options()
            ("input", po::value<std::string>(&inputFile), "Input tape file")
            ("output", po::value<std::string>(&outputFile), "Output tape file");

    po::options_description all_options;
    all_options.add(visible_desc).add(hidden_desc);

    po::positional_options_description pos;
    pos.add("input", 1).add("output", 1);

    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv)
                          .options(all_options)
                          .positional(pos)
                          .run(),
                  vm);
        po::notify(vm);

        if (vm.count("help") || inputFile.empty() || outputFile.empty()) {
            std::cout << "Usage: " << argv[0] << " <input_tape> <output_tape> [-d | --debug]\n";
            std::cout << visible_desc << "\n";
            return 0;
        }
    } catch (std::exception &e) {
        std::cerr << "Error parsing arguments: " << e.what() << std::endl;
        return 1;
    }

    ConfigParser config("config.conf");
    std::ifstream file_in(argv[1], std::ios::binary | std::ios::ate);
    if (!file_in) {
        throw std::runtime_error("Не удалось открыть входной файл");
    }
    fs::remove(argv[2]);
    std::streamsize fileSize = file_in.tellg() / 4;

    FileTape inputTape(argv[1], config, fileSize, debug);
    std::ofstream file_out(argv[2], std::ios::binary);
    FileTape outputTape(argv[2], config, fileSize, debug);
    fs::create_directory("tmp");
    if (debug) std::cout << "Start sorting" << std::endl;
    TapeSorter sorter(inputTape, outputTape, config, fileSize, debug);
    sorter.sort();
    fs::remove_all("tmp");

    return 0;
}
