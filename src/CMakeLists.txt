message(STATUS "Fetching GeographicLib...")
# Fetch GeographicLib
include(FetchContent)
FetchContent_Declare(
    GeographicLib
    GIT_REPOSITORY https://git.code.sf.net/p/geographiclib/code
    GIT_TAG r1.51
)
FetchContent_MakeAvailable(GeographicLib)

file(GLOB SOURCE_FILES *.cpp)

LIST(REMOVE_ITEM ${SOURCE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_library(norbitlib SHARED ${SOURCE_FILES})
target_link_libraries(norbitlib GeographicLib pthread)

add_executable(norbit main.cpp)
target_link_libraries(norbit norbitlib)
