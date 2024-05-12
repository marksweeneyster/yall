FetchContent_Declare(googletest
    GIT_REPOSITORY GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(yall_test yall_test.cpp)

target_link_libraries(yall_test
    PUBLIC
    GTest::gtest_main
    yall
)

gtest_discover_tests(yall_test)