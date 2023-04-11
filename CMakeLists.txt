cmake_minimum_required(VERSION 3.17)
project(MyMusic C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})
add_definitions(-DCSS_FILE_PATH="${CMAKE_SOURCE_DIR}/MyMusicCSS.css")


set(CMAKE_C_STANDARD 11)

add_executable(MyMusic main.c)

target_link_libraries(MyMusic ${GTK3_LIBRARIES})
