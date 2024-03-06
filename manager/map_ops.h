#ifndef MAPOPS_H
#define MAPOPS_H

std::unordered_map<std::string, std::string>::const_iterator find_k_v(
    std::unordered_map<std::string, std::string> & stored,
    std::string & id
);

bool add_k_v(
    std::unordered_map<std::string, std::string> & stored,
    std::string & id,
    std::string & username,
    std::string & password,
    std::vector<char> & ciphertext_alphabet
);

#endif