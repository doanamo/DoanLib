cmake_minimum_required(VERSION 4.1)
project(Game C)

set(CMAKE_C_STANDARD 23)

set(SOURCE_FILES
  "source/main.c"
  "source/application.c"
  "source/platform/window.c"
)

add_executable(Game ${SOURCE_FILES})
target_include_directories(Game PRIVATE "source/")
target_compile_options(Game PRIVATE "-Wall" "-Wextra" "-Wconversion")
