#ifndef FILEOPS_H
#define FILEOPS_H

bool read_from_binary(
    std::unordered_map<std::string, std::string> & stored,
    std::string & path_to_file,
    std::vector<char> & ciphertext_alphabet
);

bool read_from_binary(
    std::string & contents,
    std::string & path_to_file
);

void write_to_binary(
    std::string & contents,
    std::string & path_to_file
);

void write_to_binary(
    std::unordered_map<std::string, std::string> & stored,
    std::string & path_to_file,
    std::vector<char> & ciphertext_alphabet
);

int check_file_size(
    std::string & path_to_file
);

#endif