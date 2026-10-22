# GoogleTest: Use the system copy if there is one, otherwise download it.
find_package(GTest QUIET)
if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
        GIT_SHALLOW TRUE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    set(gtest_main_target gtest_main)
else()
    set(gtest_main_target GTEST::gtest_main)
endif()

include(GoogleTest)

add_executable(deck_tests test_deck.cpp)
target_link_libraries(deck_tests PRIVATE arcade_lib ${gtest_main_target})
gtest_discover_tests(deck_tests)
