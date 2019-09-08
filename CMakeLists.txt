cmake_minimum_required(VERSION 3.5.1)

project(libdansdl2-tools)

set(CMAKE_BUILD_TYPE Release)

add_compile_options(-Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -ansi -pedantic -fno-rtti)

#TODO: Should also allow for c++14
add_compile_options(-std=c++11)

#TODO: Add the rest of flags...
#y_n_choice "With optimizations" "OPTIMIZATION=-O2" "#OPTIMIZATION=-O2"
#optimizations=$__retval;

#y_n_choice "With debug" "DEBUG=-g" "#DEBUG=-g"
#debug=$__retval;

#y_n_choice "With C++14" "CPPREV=-std=c++14" "CPPREV=-std=c++11"
#cpprev=$__retval


#TODO: This should be given in the command line
include_directories(../libdansdl2/ ../tools ../log)

set(SOURCE "")

add_subdirectory("${PROJECT_SOURCE_DIR}/class")

add_library(dansdl2-tools STATIC ${SOURCE})
