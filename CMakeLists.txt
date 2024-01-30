cmake_minimum_required(VERSION 3.27.9)
project(
    sparrow
    VERSION 0.0.1
    DESCRIPTION "a termial file manager"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(main src/main.c)

set_target_properties(main
  PROPERTIES
  EXPORT_COMPILE_COMMANDS ON
)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(main PRIVATE ${CURSES_LIBRARIES})
