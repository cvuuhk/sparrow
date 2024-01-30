cmake_minimum_required(VERSION 3.20)

project(
    sparrow
    VERSION 0.0.1
    DESCRIPTION "Sparrow Termial File Manager"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 99)

add_executable(main src/main.c)

set_target_properties(main
  PROPERTIES
  EXPORT_COMPILE_COMMANDS ON
)

set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)
find_package(Curses REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(main PRIVATE ${CURSES_LIBRARIES})
