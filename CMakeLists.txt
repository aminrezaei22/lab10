cmake_minimum_required(VERSION 3.20)
project(arcade LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# AddressSanitizer: cmake -S . -B build-asan -DENABLE_ASAN=ON
option(ENABLE_ASAN "Build with AddressSanitizer" OFF)

add_compile_options(-Wall -Wextra)
if(ENABLE_ASAN)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address)
endif()

#file(GLOB ARCADE_SOURCES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp)
#file(GLOB ARCADE_HEADERS CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/include/*.h)

#add_library(arcade_lib STATIC ${ARCADE_SOURCES}) 
#target_include_directories(arcade_lib PUBLIC include)
add_library(arcade_lib STATIC
    src/blackjack.cpp
    src/card.cpp
    src/deck.cpp
    src/input.cpp
)
target_include_directories(arcade_lib PUBLIC include)

add_executable(arcade app/main.cpp)
target_link_libraries(arcade PRIVATE arcade_lib)

# Debugging practice programs. They are not built by default:
#   cmake --build build --target buggy_rooms deck_bugs
add_executable(buggy_rooms EXCLUDE_FROM_ALL gdb/buggy_rooms.cpp)
add_executable(deck_bugs EXCLUDE_FROM_ALL gdb/deck_bugs.cpp)
target_link_libraries(deck_bugs PRIVATE arcade_lib)

# Tests (GoogleTest)
include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

# Documentation:    cmake --build build --target docs
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating Doxygen Documentation"
        VERBATIM)
endif()
