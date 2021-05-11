file(GLOB SOURCE_FILES *.cpp)
add_library(norbitlib SHARED ${SOURCE_FILES})
add_executable(norbit main.cpp)
target_link_libraries(norbit norbitlib ${LIBS})
