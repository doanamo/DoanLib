cmake_minimum_required(VERSION 4.1)
project(Game C)

set(CMAKE_C_STANDARD 23)

set(SOURCE_FILES
  "src/main.c"
  "src/application.c"
  "src/sys/window.c"
  "src/gpu/device.c"
  "src/gpu/swapchain.c"
)

add_executable(Game ${SOURCE_FILES})
target_include_directories(Game PRIVATE "src/")
target_compile_options(Game PRIVATE -Wall -Wextra -Wconversion)
target_link_libraries(Game PRIVATE dxgi dxguid d3d11 d3dcompiler)
