add_executable(pmanager file_ops.cpp scramble.cpp substitutions.cpp map_ops.cpp main.cpp)
target_link_libraries(pmanager fmt::fmt spdlog::spdlog CLI11::CLI11)
