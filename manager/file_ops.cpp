#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <unordered_map>

#include "file_ops.h"

// read_from_binary: read contents of path_to_file into unordered_map stored.
bool read_from_binary(std::unordered_map<std::string, std::string> & stored, std::string & path_to_file, std::vector<char> & ciphertext_alphabet)
{
    std::string line;
    std::ifstream input(path_to_file, std::ios::binary);

    auto ascii_comma = 44;  // <username>,<password> delimiter as plaintext
    auto ascii_newline = 10; // entry delimiter as plaintext
    auto delimiter_c = ciphertext_alphabet[ascii_comma]; // encoded delimiter
    auto delimiter_n = ciphertext_alphabet[ascii_newline]; // encoded delimiter

    if (!input.is_open()) {
        return false;
    }

    while (std::getline(input, line)) {
        std::istringstream line_stream(line);
        std::string entry;
        while (std::getline(line_stream, entry, delimiter_n)) {
            std::istringstream entry_stream(entry);
            std::string key, value;
            std::getline(entry_stream, key, delimiter_c);
            std::getline(entry_stream, value, delimiter_c);
            stored[key] = value;
        }
    }
    input.close();
    return true;
}

// write_to_binary: write contents of unordered_map stored into path_to_file
// entries are stored in the following format (how it would look if not encrypted):
// <username1>,<password1>
// <username2>,<password2>,
// ...
//
bool write_to_binary(std::unordered_map<std::string, std::string> & stored, std::string & path_to_file, std::vector<char> & ciphertext_alphabet)
{
    auto ascii_comma = 44; // <username>,<password> delimiter as plaintext
    auto ascii_newline = 10; // entry delimiter as plaintext
    auto delimiter_c = ciphertext_alphabet[ascii_comma]; // encoded delimiter
    auto delimiter_n = ciphertext_alphabet[ascii_newline]; // encoded delimiter

    std::ofstream output(path_to_file, std::ios::binary);
    if (!output.is_open()) {
        return false;
    }
    for (auto & kv : stored) { 
    //    std::cout << kv.first << "," << kv.second << std::endl; // prints the entry to terminal
        output << kv.first << delimiter_c << kv.second << delimiter_n;
    }
    output.close();
    return true;
}

//read_from_binary: read contents of path_to_file to string
bool read_from_binary(std::string & contents, std::string & path_to_file)
{
    std::ifstream input(path_to_file, std::ios::binary);
    if (!input.is_open()) {
        return false;
    }
    contents.assign(std::istreambuf_iterator<char>(input), {});
    input.close();
    return true;
}

// write_to_binary: write contents of string into path_to_file
bool write_to_binary(std::string & contents, std::string & path_to_file)
{
    std::ofstream output(path_to_file, std::ios::binary);
    if (!output.is_open()) {
        return false;
    }
    output << contents;
    output.close();
    return true;
}

// check_file_size: read the contents of path_to_file and count characters
int check_file_size(std::string & path_to_file)
{
    std::string line;
    std::ifstream input(path_to_file, std::ios::binary);
    int count = 0;

    if (!input.is_open()) {
        return 0;
    }
    while (std::getline(input, line)) {
        count += line.size();
    }
    return count;
}