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
// 5. switch char to unsigned char


//after
int character_base = 0;
int character_range = 128;

bool validate_universal_key(std::string & universal_key, std::string & master_key, std::string & path_to_key, std::vector<char> & master_plain_alphabet);
bool valid_user_input(std::string & default_string, std::string & id, std::string & username, std::string & password);
bool valid_user_input(std::string & default_string, std::string & id);
bool valid_user_input(std::string & default_string, std::string & universal_key, std::string & master_key);


int main(const int argc, const char *const argv[])
{
    std::string path_to_file = "/* insert path to file to store encrypted usernames and passwords */";
    std::string path_to_key = "/* insert path to file to store encrypted universal key */";
    std::string default_string{"no entry"};
    std::string id = default_string;
    std::string username = default_string;
    std::string password = default_string;
    std::string universal_key = default_string;
    std::string master_key = default_string;

    bool add_entry{false};
    bool fetch_entry{false};
    bool access_details{false};
    bool set_key{false};

    std::vector<char> ciphertext_alphabet;
    std::vector<char> plaintext_alphabet;
    std::vector<char> master_cipher_alphabet;
    std::vector<char> master_plain_alphabet;
    std::unordered_map<std::string, std::string> stored;

    //action::cli-parsing::begin
    CLI::App app("password manager");
    app.option_defaults()->always_capture_default(true);
    app.add_option("-k, --universal", universal_key, "universal key");
    app.add_option("-m, --master", master_key, "master key");
    app.add_option("-i, --id", id, "unique identifier");
    app.add_option("-u, --user", username, "username");
    app.add_option("-p, --pass", password, "password");
    app.add_flag("-a, --add", add_entry);
    app.add_flag("-f, --fetch", fetch_entry);
    app.add_flag("-s, --setkey", set_key);
    CLI11_PARSE(app, argc, argv);
    //action::cli-parsing::end

    //action::ensure 1 action was entered::begin
    if ((add_entry && !fetch_entry && !set_key) ||
    (!add_entry && fetch_entry && !set_key) ||
    (!add_entry && !fetch_entry && set_key)) {
        spdlog::info("one action provided");
    }
    else {
        spdlog::error("exactly one action must be provide: -a, <add_entry>, -f <fetch_entry>, -s <set_key>");
        return 1;
    }
    //action::ensure 1 action was entered::end

    //action::instantiate master key ciphertext and plaintext alphabets::begin
    master_cipher_alphabet = scramble(master_key, character_base, character_range);
    master_plain_alphabet = unscramble(master_key, character_base, character_range);
    //action::instantiate master key ciphertext and plaintext alphabets::end
    //action::instantiate universal key ciphertext and plaintext alphabets::begin
    ciphertext_alphabet = scramble(universal_key, character_base, character_range);
    plaintext_alphabet = unscramble(universal_key, character_base, character_range);
    //action::instantiate ciphertext and plaintext alphabets::end

    //action::ensure user entered correct master key and universal key::begin
    if ( !set_key && !validate_universal_key(universal_key, master_key, path_to_key, master_plain_alphabet) ) {
        spdlog::error("could not validate universal key... exiting system");
        return 1;
    }
    //action::ensure user entered correct master key and universal key::end

    //action::instantiate unordered_map stored::begin
    if ( !read_from_binary(stored, path_to_file, ciphertext_alphabet) ) {
        return 1;
    }
    //action::instantiate unordered_map stored::end

    //action::add entry::begin
    if (add_entry) {

        //action::check if the user input the necessary details::begin
        if ( !valid_user_input(default_string, id, username, password) ) {
            return 1;
        }
        //action::check if the user input the necessary details::end

        //action::encode id, username, and password::begin
        id = substitute(ciphertext_alphabet, id, character_base, character_range);
        username = substitute(ciphertext_alphabet, username, character_base, character_range);
        password = substitute(ciphertext_alphabet, password, character_base, character_range);
        //action::encode id, username, and password::end

        //action::check if the id entered is unique::begin
        if ( add_k_v(stored, id, username, password, ciphertext_alphabet) ) {
            spdlog::info("entry successful");
        }
        else {
            spdlog::error("The entry was unsuccessful because the id provided was not unique.");
            return 1;
        }
        //action::check if the id entered is unique::end

        //action::write map back to file::begin
        write_to_binary(stored, path_to_file, ciphertext_alphabet);
        //action::write map back to file::end
    }
    //action::add entry::end

    //action::fetch entry::begin
    if (fetch_entry) {
        if ( !valid_user_input(default_string, id) ) {
            return 1;
        }
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
    }
    //action::fetch entry::end

    //action::set key::begin
    if (set_key) {
        //action::check if the user input the necessary details::begin
        if ( !valid_user_input(default_string, universal_key, master_key) ) {
            return 1;
        }
        //action::check if the user input the necessary details::end

        //action::check if the file being written to already contains a universal key::begin
        if (check_file_size(path_to_key) > 0) {
            //TODO: if the file contains a universal key, I can add an option to reset the pmanager in case
            //the user forgot his master/universal key combo
            spdlog::error("universal key has already been set");
            spdlog::info("You can manually reset the system by deleting all contents from both the path_to_key and path_to_file files");
            spdlog::info("TODO: add functionality to reset keys -> will result in wiping all stored data from pmanager");
            return 1;
        }
        //action::check if the file being written to already contains a universal key::end

        //action::encode universal key by master key::begin
        universal_key = substitute(master_cipher_alphabet, universal_key, character_base, character_range);
        //action::encode universal key by master key::end

        //action::write encoded universal key to path_to_key::begin
        write_to_binary(universal_key, path_to_key);
        //action::write encoded universal key to path_to_key::end
    }
    //action::set key::end
    return 0;
}

//validate the universal key provided by user matches key stored in path_to_key
bool validate_universal_key(std::string & universal_key, std::string & master_key, std::string & path_to_key, std::vector<char> & master_plain_alphabet)
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

//checking user input for set key action
bool valid_user_input(std::string & default_string, std::string & universal_key, std::string & master_key)
{
    if (universal_key.compare(default_string) == 0 || master_key.compare(default_string) == 0) {
        spdlog::error("must specify -k <uninversal key> and -m <master key> when setting key");
        return false;
    }
    return true;
}

//checking user input for fetch entry action
bool valid_user_input(std::string & default_string, std::string & id)
{
    if (id.compare(default_string) == 0) {
        spdlog::error("must specify -i <unique id> when -f fetching entry");
        return false;
    }
    return true;
}

//checking user input for add entry action
bool valid_user_input(std::string & default_string, std::string & id, std::string & username, std::string & password)
{
    if (id.compare(default_string) == 0 || username.compare(default_string) == 0 || password.compare(default_string) == 0) {
        spdlog::error("must specify -i <unique id>, -u <username>, -p <password> when -a adding entry");
        return false;
    }
    return true;
}