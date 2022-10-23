#include "compressor.h"
#include "decompressor.h"
#include "cmd_options_parser.cpp"

#include <iostream>

int main(int argc, char** argv) {
    OptionsParser working_parser;
    working_parser.AddOption("compress", "-c", "any", "any...");
    working_parser.AddOption("decompress", "-d", "any");
    working_parser.AddOption("help", "-h");

    std::string current_option = working_parser.FindOption(argc, argv);

    if (current_option == "help") {
        std::cout << "Manual-Manul\n";
    } else if (current_option == "decompress") {
        Decompressor current_decompressor;
        std::string arch_name = argv[2];
        Reader current_reader(arch_name + ".cmp");
        while (current_decompressor.DecompressFile(current_reader)) {
        }
    } else if (current_option == "compress") {
        Compressor current_compressor;
        std::string arch_name = argv[2];
        Writer current_writer(arch_name + ".cmp");
        int argv_pos = 3;
        while (argv_pos != argc) {
            std::string filename = argv[argv_pos];
            bool is_last_file = (argv_pos + 1 == argc);
            current_compressor.CompressFile(filename, current_writer, is_last_file);
            ++argv_pos;
        }
    }
    return 0;
}