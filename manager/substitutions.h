#ifndef SUBSTITUTIONS_H
#define SUBSTITUTIONS_H

std::string substitute(
    std::vector<char> & substitution_alphabet, 
    std::string encode_me, 
    int character_base, 
    int character_range
);

#endif