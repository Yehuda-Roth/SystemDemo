# CMakeList.txt : CMake project for SystemDemo, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("SystemDemo")

# Add source to this project's executable.
add_executable (SystemDemo "SystemDemo.cpp" "SystemDemo.h" "ImplementMsg.h")

# TODO: Add tests and install targets if needed.
