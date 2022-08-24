cmake_minimum_required(VERSION 3.5.1)
project(libldtools)

option(BUILD_DEBUG "Build a debug library" OFF)
option(BUILD_SHARED "Build a shared library" OFF)
option(BUILD_STATIC "Build a static library" OFF)
option(BUILD_TESTS "Build test code" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#library version
set(MAJOR_VERSION 1)
set(MINOR_VERSION 0)
set(PATCH_VERSION 1)

message("will build ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")

if(${CMAKE_VERSION} VERSION_LESS "3.22.0")

	add_definitions(-DMAJOR_VERSION=${MAJOR_VERSION})
	add_definitions(-DMINOR_VERSION=${MINOR_VERSION})
	add_definitions(-DPATCH_VERSION=${PATCH_VERSION})
else()

	add_compile_definitions(MAJOR_VERSION=${MAJOR_VERSION})
	add_compile_definitions(MINOR_VERSION=${MINOR_VERSION})
	add_compile_definitions(PATCH_VERSION=${PATCH_VERSION})
endif()

add_compile_options(-Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -ansi -pedantic -fno-rtti)

#Add the include directories
include_directories("include")

set(SOURCE "")
add_subdirectory("${PROJECT_SOURCE_DIR}/lib")

if(${BUILD_STATIC})

	add_library(ldtools_static STATIC ${SOURCE})
	target_compile_definitions(ldtools_static PUBLIC "-DBUILD_VERSION=\"archive\"")
	set_target_properties(ldtools_static PROPERTIES OUTPUT_NAME ldtools)
	set_target_properties(ldtools_static PROPERTIES COMPILE_FLAGS "-O3 -DNDEBUG")
	install(TARGETS ldtools_static DESTINATION lib)
endif()

if(${BUILD_SHARED})

	add_library(ldtools_shared SHARED ${SOURCE})
	target_compile_definitions(ldtools_shared PUBLIC "-DBUILD_VERSION=\"lib\"")
	set_target_properties(ldtools_shared PROPERTIES OUTPUT_NAME ldtools)
	set_target_properties(ldtools_shared PROPERTIES COMPILE_FLAGS "-O3 -DNDEBUG")
	install(TARGETS ldtools_shared DESTINATION lib)
endif()


if(${BUILD_DEBUG})

	add_library(ldtools_debug SHARED ${SOURCE})
	target_compile_definitions(ldtools_debug PUBLIC "-DBUILD_VERSION=\"debug\"")
	#add the libdandsl2 debug part. Should not be needed, but I don't know
	#if this library may use debug pieces some day.
	target_compile_definitions(ldtools_debug PUBLIC "-DLIBDANSDL2_DEBUG")
	set_target_properties(ldtools_debug PROPERTIES OUTPUT_NAME ldtools_debug)
	set_target_properties(ldtools_debug PROPERTIES COMPILE_FLAGS "-O0 -g")
	install(TARGETS ldtools_debug DESTINATION lib)
endif()

install(DIRECTORY include/ DESTINATION include)

IF(WIN32)

	target_compile_definitions(ldtools_static PUBLIC -DWINBUILD)
	target_compile_definitions(ldtools_shared PUBLIC -DWINBUILD)

	include_directories("${CMAKE_CURRENT_LIST_DIR}/../log/include/")
	include_directories("${CMAKE_CURRENT_LIST_DIR}/../libdansdl2/include/")
	include_directories("${CMAKE_CURRENT_LIST_DIR}/../rapidjson/include/")
	include_directories("${CMAKE_CURRENT_LIST_DIR}/../tools/include/")

	add_library(lm SHARED IMPORTED)
	set_target_properties(lm PROPERTIES IMPORTED_LOCATION "C:/Program Files (x86)/lm/lib/liblm.dll")
	set_target_properties(lm PROPERTIES IMPORTED_IMPLIB "C:/Program Files (x86)/lm/lib/liblm.dll")

	add_library(dansdl2 SHARED IMPORTED)
	set_target_properties(dansdl2 PROPERTIES IMPORTED_LOCATION "C:/Program Files (x86)/libdansdl2/lib/libdansdl2.dll")
	set_target_properties(dansdl2 PROPERTIES IMPORTED_IMPLIB "C:/Program Files (x86)/libdansdl2/lib/libdansdl2.dll")

	add_library(tools SHARED IMPORTED)
	set_target_properties(tools PROPERTIES IMPORTED_LOCATION "C:/Program Files (x86)/tools/lib/libtools.dll")
	set_target_properties(tools PROPERTIES IMPORTED_IMPLIB "C:/Program Files (x86)/tools/lib/libtools.dll")

	target_link_libraries(ldtools_static lm dansdl2 tools SDL2 SDL2_ttf SDL2_mixer SDL2_image glu32 opengl32 stdc++fs)
	target_link_libraries(ldtools_shared lm dansdl2 tools SDL2 SDL2_ttf SDL2_mixer SDL2_image glu32 opengl32 stdc++fs)
endif()

if(${BUILD_TESTS})

	if(WIN32)

	else()
		add_library(dansdl2 SHARED IMPORTED)
		set_target_properties(dansdl2 PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdansdl2.so)
	endif()


	if(WIN32)

	else()
		add_library(tools SHARED IMPORTED)
		set_target_properties(tools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libtools.so)
	endif()

	if(WIN32)

	else()
		add_library(lm SHARED IMPORTED)
		set_target_properties(lm PROPERTIES IMPORTED_LOCATION /usr/local/lib/liblm.so)
	endif()

	add_executable(sprite_table tests/sprite_table/main.cpp)
	if(WIN32)
		target_link_libraries(sprite_table ldtools_shared tools dansdl2 lm SDL2 SDL2_ttf SDL2_mixer SDL2_image glu32 opengl32 stdc++fs)
		target_link_options(sprite_table PRIVATE -lmingw32 -lSDL2main -lSDL2 -mwindows)
	else()
		target_link_libraries(sprite_table ldtools_shared tools dansdl2 lm SDL2 SDL2_ttf SDL2_mixer SDL2_image GL)
		add_custom_command(TARGET sprite_table POST_BUILD COMMAND cp -r ../tests/sprite_table/*.txt ./)
	endif()
endif()
