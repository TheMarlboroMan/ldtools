cmake_minimum_required(VERSION 3.5.1)
project(libldtools)

option(BUILD_DEBUG "Build a debug release" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(${BUILD_DEBUG})
	set(CMAKE_BUILD_TYPE Debug)
else()
	set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -ansi -pedantic -fno-rtti)

#Add the include directories
set(LOG_DIR "../log/include/" CACHE STRING "Directory where the log repository is (-DLOG_DIR=xxx")
set(TOOLS_DIR "../tools/include" CACHE STRING "Directory where the tools repository is (-DTOOLS_DIR=xxx")
set(DANSDL2_DIR "../libdansdl2/include" CACHE STRING "Directory where the libdansdl2 is (-DDANSDL2_DIR=xxx")
include_directories("include" ${LOG_DIR} ${TOOLS_DIR} ${DANSDL2_DIR})

set(SOURCE "")
add_subdirectory("${PROJECT_SOURCE_DIR}/lib")

add_library(ldtools_static STATIC ${SOURCE})
set_target_properties(ldtools_static PROPERTIES OUTPUT_NAME ldtools)

add_library(ldtools_shared SHARED ${SOURCE})
set_target_properties(ldtools_shared PROPERTIES OUTPUT_NAME ldtools)

install(TARGETS ldtools_static DESTINATION lib)
install(TARGETS ldtools_shared DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
