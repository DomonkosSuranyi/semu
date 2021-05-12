# Fetch gtest
include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/release-1.10.0.zip
)
FetchContent_MakeAvailable(googletest)

file(GLOB TEST_SOURCE_FILES *.cpp)

add_executable(test_runner ${TEST_SOURCE_FILES})
target_link_libraries(test_runner norbitlib gtest_main)

add_custom_command(
     TARGET test_runner
     COMMENT "Running tests"
     POST_BUILD
     COMMAND test_runner
)

file(GLOB TEST_RESOURCES resources/*)
file(COPY ${TEST_RESOURCES} DESTINATION resources/)
