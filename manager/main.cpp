#include <CLI/CLI.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <iostream>
#include <vector>
#include <unordered_map>

#include "file_ops.h"
#include "scramble.h"
#include "substitutions.h"
#include "map_ops.h"
#include "validate_inputs.h"

// IMPORANT:
// user must not input comma, newline, or semicolon -> these are used as delimeters.
// consider parsing user input for delimeters, if found output error and exit system.

//TODO:
//
// DONE 0.0 ensure user does not try to enter more than one action
// DONE 1.1 make universal universal_key parametric and implement use of master universal_key
// DONE 1.2 set universal key by writing encoded universal key (by master key) to path_to_key.
// DONE 1.3 run a necessary validate universal key action before add entry and fetch entry
// 2. implement a clear function to remove all entries
// 3. implement a function to change the universal universal_key -> destructive action?
// 4. implement a function to change the master universal_key -> destructive action?
// 5. switch char to uint_8
// 6. add action to show all entries
//

int character_base = 0;
int character_range = 128;

bool validate_input_keys(std::string & universal_key, std::string & master_key, std::string & path_to_key, std::vector<char> & master_plain_alphabet);

int main(const int argc, const char *const argv[])
{
    // for now, paths are hard coded.
    std::string path_to_storage = ""; // replace empty string with full path to: /necessary-files/storage.txt
    std::string path_to_key = ""; // replace empty string with full path to: /necessary-files/key.txt 
    std::string id;
    std::string username;
    std::string password;
    std::string universal_key;
    std::string master_key;

    bool add_entry{false};
    bool fetch_entry{false};
    bool set_key{false};

    std::vector<char> ciphertext_alphabet;
    std::vector<char> plaintext_alphabet;
    std::vector<char> master_cipher_alphabet;
    std::vector<char> master_plain_alphabet;
    std::unordered_map<std::string, std::string> stored;

    //cli-parsing::begin
    CLI::App app("password manager");
    app.option_defaults()->always_capture_default(true);
    // every action requires both a universal and master key
    app.add_option("-k, --universal", universal_key, "universal key")
        ->required("error");
    app.add_option("-m, --master", master_key, "master key")
        ->required();
    app.add_option("-i, --id", id, "unique identifier");
    app.add_option("-u, --user", username, "username");
    app.add_option("-p, --pass", password, "password");
    app.add_flag("-a, --add", add_entry);
    app.add_flag("-f, --fetch", fetch_entry);
    app.add_flag("-s, --setkey", set_key);
    CLI11_PARSE(app, argc, argv);
    //cli-parsing::end

    //check if one action was entered::begin
    if ( (add_entry && !fetch_entry && !set_key) ||
    (!add_entry && fetch_entry && !set_key) ||
    (!add_entry && !fetch_entry && set_key) )
    {
        spdlog::info("one action provided");
    }
    else {
        spdlog::error("exactly one action must be provide: -a <add_entry>, -f <fetch_entry>, -s <set_key>");
        return 1;
    }

    //instantiate master key ciphertext and plaintext alphabets::begin
    master_cipher_alphabet = scramble(master_key, character_base, character_range);
    master_plain_alphabet = unscramble(master_key, character_base, character_range);
    //instantiate master key ciphertext and plaintext alphabets::end
    //instantiate universal key ciphertext and plaintext alphabets::begin
    ciphertext_alphabet = scramble(universal_key, character_base, character_range);
    plaintext_alphabet = unscramble(universal_key, character_base, character_range);
    //instantiate ciphertext and plaintext alphabets::end

    //action::add entry::begin
    if (add_entry) {
        spdlog::info("adding entry...");

        //populate stored with entries::begin
        if ( !read_from_binary(stored, path_to_storage, ciphertext_alphabet) ) {
            spdlog::info("error reading from file: {}", path_to_storage);
            return 1;
        }
        //populate stored with entries::end

        if ( !validate_input_keys(universal_key, master_key, path_to_key, master_plain_alphabet) ) {
            spdlog::error("could not validate universal key... exiting system");
            return 1;
        }

        //check if the user input the necessary details::begin
        if ( !validate_input_add(id, username, password) ) {
            spdlog::error("must specify -i <unique id>, -u <username>, -p <password> when -a adding entry");
            return 1;
        }
        //check if the user input the necessary details::end

        //encode id, username, and password::begin
        id = substitute(ciphertext_alphabet, id, character_base, character_range);
        username = substitute(ciphertext_alphabet, username, character_base, character_range);
        password = substitute(ciphertext_alphabet, password, character_base, character_range);
        //encode id, username, and password::end

        //check if the id entered is unique::begin
        if ( !add_k_v(stored, id, username, password, ciphertext_alphabet) ) {
            spdlog::error("entry could not be added... please provide unique id");
            return 1;
        }
        //check if the id entered is unique::end

        //write map back to file::begin
        if ( !write_to_binary(stored, path_to_storage, ciphertext_alphabet) ) {
            spdlog::error("failed to write storage to {}", path_to_storage);
            return 1;
        }
        //write map back to file::end
    }
    //action::add entry::end

    //action::fetch entry::begin
    if (fetch_entry) {
        spdlog::info("fetching entry...");

        //populate stored with entries::begin
        if ( !read_from_binary(stored, path_to_storage, ciphertext_alphabet) ) {
            spdlog::info("error reading from file: {}", path_to_storage);
            return 1;
        }
        //populate stored with entries::end

        if ( !validate_input_keys(universal_key, master_key, path_to_key, master_plain_alphabet) ) {
            spdlog::error("error validating universal key... exiting system");
            return 1;
        }

        if ( !validate_input_fetch(id) ) {
            spdlog::error("must specify -i <unique id> when -f fetching entry");
            return 1;
        }
        // find argument id::begin
        std::string encoded_id = substitute(ciphertext_alphabet, id, character_base, character_range);
        std::unordered_map<std::string, std::string>::const_iterator found = find_k_v(stored, encoded_id);
        if (found == stored.end()) {
            spdlog::error("-i <unique id> provided was not found");
            return 1;
        }
        else {
            std::string username_password = found->second;
            username_password = substitute(plaintext_alphabet, username_password, character_base, character_range);
            std::cout << "<username;password> = " << username_password << std::endl;
        }
        // find argument id::end
    }
    //action::fetch entry::end

    //action::set_key::begin
    if (set_key) {
        spdlog::info("setting keys...");

        //check if the user input the necessary details::begin
        if ( !validate_input_set(universal_key, master_key) ) {
            spdlog::error("must specify -k <uninversal key> and -m <master key> when setting key");
            return 1;
        }
        //check if the user input the necessary details::end

        //check if the file being written to already contains a universal key::begin
        if (check_file_size(path_to_key) > 0) {
            //TODO: add an option to reset the pmanager in case user forgot universal/master keys
            spdlog::error("universal key has already been set");
            spdlog::info("You can manually reset the system by deleting all contents from both files {} and {}", path_to_key, path_to_storage);
            spdlog::info("TODO: add functionality to reset keys -> will result in wiping all stored data from pmanager");
            return 1;
        }
        //check if the file being written to already contains a universal key::end

        //encode universal key by master key::begin
        universal_key = substitute(master_cipher_alphabet, universal_key, character_base, character_range);
        //encode universal key by master key::end

        //write encoded universal key to path_to_key::begin
        if ( !write_to_binary(universal_key, path_to_key) ) {
            spdlog::error("failed to write key to {}", path_to_key);
            return 1;
        }
        //write encoded universal key to path_to_key::end
    }
    //action::set_key::end
    spdlog::info("done");
    return 0;
}

bool validate_input_keys(std::string & universal_key, std::string & master_key, std::string & path_to_key, std::vector<char> & master_plain_alphabet)
{
    std::string key_to_compare;
    if ( !read_from_binary(key_to_compare, path_to_key) ) {
        spdlog::error("could not validate universal key");
        return false;
    }
    key_to_compare = substitute(master_plain_alphabet, key_to_compare, character_base, character_range);
    if (key_to_compare != universal_key) {
        return false;
    }
    return true;
}