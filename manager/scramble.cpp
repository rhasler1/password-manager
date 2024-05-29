#include <algorithm>
#include <list>
#include <vector>
#include <set>
#include <string>

#include "scramble.h"

// scramble: plaintext_alphabet (k)-> ciphertext_alphabet
// arguments character_base and character_range used to initialize plaintext alphabet
std::vector<char> scramble(std::string keyword, const int character_base, const int character_range)
{
    // setting up keyword domain
    remove_duplicates(keyword);
    const auto keyword_size = keyword.size();

    // declaring plaintext and ciphertext alphabets
    std::list<char> plaintext_alphabet;
    std::vector<char> ciphertext_alphabet;
    
    // initializing plaintext_alphabet
    for (int i = character_base; i < (character_range + character_base); i++) {
        char character = char(i);
        plaintext_alphabet.push_back(character);
    }
    
    // writing keyword characters to ciphertext_alphabet and removing keyword characters from plaintext_alphabet
    for (int i = 0; i < keyword_size; i++) {
        ciphertext_alphabet.push_back(keyword[i]);
        plaintext_alphabet.remove(keyword[i]);
    }
    
    // writing remaining characters from plaintext_alphabet to ciphertext_alphabet
    for (int i = 0; i < (character_range - keyword_size); i++) {
        char character = plaintext_alphabet.front();
        ciphertext_alphabet.push_back(character);
        plaintext_alphabet.pop_front();
    }     
    return ciphertext_alphabet;
}

// unscramble: ciphertext_alphabet (k)-> plaintext_alphabet
//  
//  0 1 2 3 4 5 6 7 8  <- indices represent cipher text, IF character domain is [a-z] THEN index[0] == a, index[1] == b, and so on...
// |p|l|a|i|n|t|e|x|t| <- mapping to plaintext, ie: ciphertext: a (k)=> plaintext: p
//
std::vector<char> unscramble(std::string keyword, const int character_base, const int character_range)
{
    remove_duplicates(keyword);
    const auto keyword_size = keyword.size();
    auto counter_not = keyword_size;
    char character;
    std::vector<char> plaintext_alphabet;

    for (int i = character_base; i < (character_range + character_base); i++) {
        char index_char = char(i);
        auto found = std::find_if(keyword.begin(), keyword.end(), [&index_char](char c) {
            return c == index_char;
        });
        if (found == keyword.end()) {
            character = char(counter_not++);
            plaintext_alphabet.push_back(character);
        }
        else {
            character = std::distance(keyword.begin(), found);
            plaintext_alphabet.push_back(character);
        }
    }
    return plaintext_alphabet;
}

// remove duplicate characters from a string
void remove_duplicates(std::string& keyword)
{
    std::set<char> unique_characters;
    keyword.erase(
        std::remove_if(
            keyword.begin(),
            keyword.end(),
            [&unique_characters] (char i) {
                if (unique_characters.count(i)) { return true; }
                unique_characters.insert(i);
                return false;
            }
        ),
        keyword.end()
    );
}
