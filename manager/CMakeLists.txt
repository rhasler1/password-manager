add_executable(pmanager file_ops.cpp scramble.cpp substitutions.cpp map_ops.cpp main.cpp validate_inputs.cpp)
target_link_libraries(pmanager fmt::fmt spdlog::spdlog CLI11::CLI11)

enable_testing()
add_executable(manager_tests test.cpp scramble.cpp file_ops.cpp substitutions.cpp map_ops.cpp validate_inputs.cpp)
target_link_libraries(manager_tests gtest_main fmt::fmt spdlog::spdlog CLI11::CLI11)
include(GoogleTest)
gtest_discover_tests(manager_tests)