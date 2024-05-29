#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <unordered_map>

#include "validate_inputs.h"


//checking user input for set key action
bool validate_input_set(std::string & universal_key, std::string & master_key)
{
    if (universal_key.size() <= 0 || master_key.size() <= 0) {
        return false;
    }
    return true;
}

bool validate_input_fetch(std::string & id)
{
    if (id.size() <= 0) {
        return false;
    }
    return true;
}

bool validate_input_add(std::string & id, std::string & username, std::string & password)
{
    if (id.size() <= 0 || username.size() <= 0 || password.size() <= 0) {
        return false;
    }
    return true;
}