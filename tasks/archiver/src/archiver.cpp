#include "compressor.h"
#include "decompressor.h"
#include "cmd_options_parser.cpp"
#include "custom_exceptions.h"

#include <iostream>

int main(int argc, char** argv) {
    try {
        OptionsParser working_parser;
        working_parser.AddOption("compress", "-c", "any", "any...");
        working_parser.AddOption("decompress", "-d", "any");
        working_parser.AddOption("help", "-h");

        std::string current_option = working_parser.FindOption(argc, argv);

        if (current_option == "help") {
            std::cout << "Manual:\n";
            std::cout << "Use -h to get manual\n";
            std::cout << "Use -c archive_name file1 file2... to compress given files from current directory into "
                         "archive. Archive will also be saved into current directory. file2 and subsequent files are "
                         "optional\n";
            std::cout << "Use -d archive_name to decompress archive into current directory\n";
        } else if (current_option == "decompress") {
            Decompressor current_decompressor;
            std::string arch_name = argv[2];
            Reader current_reader(arch_name);
            while (current_decompressor.DecompressFile(current_reader)) {
            }
        } else if (current_option == "compress") {
            Compressor current_compressor;
            std::string arch_name = argv[2];
            Writer current_writer(arch_name);
            int argv_pos = 3;
            while (argv_pos != argc) {
                std::string filename = argv[argv_pos];
                bool is_last_file = (argv_pos + 1 == argc);
                current_compressor.CompressFile(filename, current_writer, is_last_file);
                ++argv_pos;
            }
        } else {
            throw CustomException("No such command line option");
        }
    } catch (CustomException& exc) {
        std::cout << exc.what() << '\n';
        return 111;
    }
    return 0;
}