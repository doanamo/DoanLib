cmake_minimum_required(VERSION 4.1)
project(Game C)

set(CMAKE_C_STANDARD 23)

set(SOURCE_FILES
  "source/main.c"
  "source/application.c"
  "source/platform/window.c"
  "source/graphics/device.c"
  "source/graphics/swapchain.c"
)

add_executable(Game ${SOURCE_FILES})
target_include_directories(Game PRIVATE "source/")
target_compile_options(Game PRIVATE -Wall -Wextra -Wconversion)
target_link_libraries(Game PRIVATE dxgi dxguid d3d11 d3dcompiler)
