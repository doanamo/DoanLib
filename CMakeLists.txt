cmake_minimum_required(VERSION 4.1)
project(DoanLib C)

set(CMAKE_C_STANDARD 23)

add_compile_options(
  -Wall
  -Wextra
  -Wconversion
)

set(SOURCE_FILES
  "src/application.c"
  "src/sys/window.c"
  "src/gpu/device.c"
  "src/gpu/swapchain.c"
)

add_library(Doan STATIC ${SOURCE_FILES})
target_include_directories(Doan PUBLIC "src/")
target_link_libraries(Doan PUBLIC dxgi dxguid d3d11 d3dcompiler)

add_subdirectory(Example)
