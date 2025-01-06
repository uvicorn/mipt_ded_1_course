cmake_minimum_required(VERSION 3.10)
project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)

include_directories(include src)

option(SANITIZE "Enable sanitizers" OFF)

# SET(COMPILE_FLAGS "-Wall -Wextra -Wpedantic -fpermissive -march=native -ffast-math ")


add_compile_options(-Wall -Wextra -Wpedantic -fpermissive -march=native -ffast-math)
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    message(STATUS "DEBUG MODE")
    add_compile_options(-g -Og -fno-inline-small-functions -fno-inline)
else()
    message(STATUS "RELEASE MODE")
    add_compile_options(-Ofast)
endif()

# -DSANITIZE=ON
if (SANITIZE)
    message(STATUS "Sanitizers enabled")
    add_link_options(-fsanitize=address)
    add_compile_options(-fsanitize=address)
endif()


file(GLOB_RECURSE SOURCES "src/*.cpp" EXCLUDE "src/main.cpp")
list(REMOVE_ITEM SOURCES "src/main.cpp")

add_executable("main" ${SOURCES} src/main.cpp)
# target_link_libraries(${PROJECT_NAME} PRIVATE gtest gmock)

# Если вы хотите собрать тесты отдельно, добавьте следующий блок:
# file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
# add_executable(tests ${TEST_SOURCES})
# target_link_libraries(tests PRIVATE gtest gmock)
