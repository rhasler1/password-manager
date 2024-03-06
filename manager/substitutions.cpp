#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <vector>
#include <string>

#include "substitutions.h"

// do substitution on encode_me provided cipertext_alphabet or plaintext_alphabet as substitution alphabet
std::string substitute(std::vector<char> & substitution_alphabet, std::string encode_me, int character_base, int character_range)
{
    std::string encoded;
    auto size = encode_me.size();
    if (size <= 0) {
        spdlog::error("nothing to encode, size = {}", size);
        exit(1);
    }
    for (int i = 0; i < size; i++) {
        encoded.push_back(substitution_alphabet[encode_me[i] - character_base]);
    }
    return encoded;
}