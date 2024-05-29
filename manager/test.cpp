#include <spdlog/spdlog.h>
#include <gtest/gtest.h>

#include "file_ops.h"
#include "map_ops.h"
#include "scramble.h"
#include "substitutions.h"

class ManagerTests : public testing::Test {
    public:
        void SetUp() override {
            spdlog::set_level(spdlog::level::off);
            spdlog::info("spdlog level set to {}", spdlog::get_level()); 
        }
        void TearDown() override {
            id.clear();
            username.clear();
            password.clear();
            universal_key.clear();
            master_key.clear();
        }

        protected:
            std::string key_filepath = "";
            std::string storage = "";

            const int character_base{0};
            const int character_range{128};

            std::string id;
            std::string username;
            std::string password;
            std::string universal_key;
            std::string master_key;

            std::vector<char> master_cipher_alphabet;
            std::vector<char> master_plain_alphabet;
};

// Test functions scramble, unscramble, and substitute
TEST_F(ManagerTests, EncodeDecodeTest) {
    universal_key = "helloworld";
    id = "gmail";
    std::vector<char> cipher_alphabet = scramble(universal_key, character_base, character_range);
    std::string encoded_id = substitute(cipher_alphabet, id, character_base, character_range);
    std::vector<char> plain_alphabet = unscramble(universal_key, character_base, character_range);
    std::string decoded_id = substitute(plain_alphabet, encoded_id, character_base, character_range);
    for (int i = 0; i < id.size(); i++) {
        ASSERT_EQ(id[i], decoded_id[i]);
    }
}