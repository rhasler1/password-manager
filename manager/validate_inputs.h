#ifndef VALIDATE_INPUTS_H
#define VALIDATE_INPUTS_H

bool validate_input_key(
    std::string & universal_key,
    std::string & master_key,
    std::string & path_to_key,
    std::vector<char> & master_plain_alphabet
);

bool validate_input_add(
    std::string & id,
    std::string & username,
    std::string & password
);

bool validate_input_fetch(
    std::string & id
);

bool validate_input_set(
    std::string & universal_key,
    std::string & master_key
);

#endif