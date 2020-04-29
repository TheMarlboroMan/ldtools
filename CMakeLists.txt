cmake_minimum_required(VERSION 3.5.1)
project(libldtools)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -ansi -pedantic -fno-rtti)

#Add the include directories
option(BUILD_STATIC "Build a static library" OFF)
set(LOG_DIR "../log/include/" CACHE STRING "Directory where the log repository is (-DLOG_DIR=xxx")
set(TOOLS_DIR "../tools/include" CACHE STRING "Directory where the tools repository is (-DTOOLS_DIR=xxx")
set(DANSDL2_DIR "../libdansdl2/include" CACHE STRING "Directory where the libdansdl2 is (-DDANSDL2_DIR=xxx")
include_directories("include" ${LOG_DIR} ${TOOLS_DIR} ${DANSDL2_DIR})

set(SOURCE "")
add_subdirectory("${PROJECT_SOURCE_DIR}/lib")

if(${BUILD_STATIC})
	add_library(ldtools STATIC ${SOURCE})
else()
	add_library(ldtools SHARED ${SOURCE})
endif()

install(TARGETS ldtools DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
