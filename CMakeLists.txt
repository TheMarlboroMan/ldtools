cmake_minimum_required(VERSION 3.5.1)
project(libdansdl2-tools)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -ansi -pedantic -fno-rtt)

#Add the include directories
option(BUILD_SHARED "Build a shared library" OFF)
set(LOG_DIR "../log/include/" CACHE STRING "Directory where the log repository is (-DLOG_DIR=xxx")
set(TOOLS_DIR "../tools/include" CACHE STRING "Directory where the tools repository is (-DTOOLS_DIR=xxx")
set(DANSDL2_DIR "../libdansdl2/include" CACHE STRING "Directory where the libdansdl2 is (-DDANSDL2_DIR=xxx")
include_directories("include" ${LOG_DIR} ${TOOLS_DIR} ${DANSDL2_DIR})

set(SOURCE "")
add_subdirectory("${PROJECT_SOURCE_DIR}/lib")

if(${BUILD_SHARED})
	add_library(dansdl2-tools SHARED ${SOURCE})
else()
	add_library(dansdl2-tools STATIC ${SOURCE})
endif()

install(TARGETS dansdl2-tools DESTINATION install/lib)
install(DIRECTORY include/ DESTINATION install/include)
