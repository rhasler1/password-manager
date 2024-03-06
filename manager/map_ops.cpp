#include <unordered_map>
#include <string>
#include <vector>

#include "map_ops.h"

// find k,v pair with id as key.
std::unordered_map<std::string, std::string>::const_iterator find_k_v(
    std::unordered_map<std::string, std::string> & stored,
    std::string & id
)
{
    std::unordered_map<std::string, std::string>::const_iterator found = stored.find(id);
    return found;
}

// insert key(id) value(username + user_pass_delim + password) pair to stored
// return true if insertion is successful, false otherwise
bool add_k_v(
    std::unordered_map<std::string, std::string> & stored,
    std::string & id,
    std::string & username,
    std::string & password,
    std::vector<char> & ciphertext_alphabet
)
{
    auto semicolon_ascii = 59;
    auto user_pass_delim = ciphertext_alphabet[semicolon_ascii];

    std::pair<std::string, std::string> add_me (id, username + user_pass_delim + password);
    auto result_of_store = stored.insert(add_me);
    if (result_of_store.second) {
        return true;
    }
    return false;
}