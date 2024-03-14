 cmake_minimum_required(VERSION 3.0)
project(main)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find SFML
# If SFML is not in a standard path, set SFML_DIR to the path where SFMLConfig.cmake is located
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add your source files
add_executable(main main.cpp)

# Link SFML
target_link_libraries(main sfml-graphics sfml-window sfml-system)
