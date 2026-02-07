cmake_minimum_required(VERSION 4.1)

project(DoanLib
  VERSION 0.1.0
  DESCRIPTION "Personal library for creating interactive applications"
  LANGUAGES C
)

#
# Configuration
#

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

set(CMAKE_CONFIGURATION_TYPES "Debug;Develop;Release")
set(CMAKE_C_FLAGS_DEVELOP "${CMAKE_C_FLAGS_RELWITHDEBINFO_INIT}" CACHE INTERNAL "")
set(CMAKE_STATIC_LINKER_FLAGS_DEVELOP "${CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO_INIT}" CACHE INTERNAL "")
set(CMAKE_SHARED_LINKER_FLAGS_DEVELOP "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO_INIT}" CACHE INTERNAL "")
set(CMAKE_EXE_LINKER_FLAGS_DEVELOP "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO_INIT}" CACHE INTERNAL "")
set(CMAKE_RC_FLAGS_DEVELOP "${CMAKE_RC_FLAGS_RELWITHDEBINFO_INIT}" CACHE INTERNAL "")

function(dn_target_setup TARGET)
  target_compile_definitions(${TARGET} PRIVATE
    $<$<CONFIG:Debug>:DN_CONFIG_DEBUG>
    $<$<CONFIG:Develop>:DN_CONFIG_DEVELOP>
    $<$<CONFIG:Release>:DN_CONFIG_RELEASE>
  )

  target_compile_options(${TARGET} PRIVATE
    # Additional warnings
    -Wall
    -Wextra
    -Wconversion

    # Always produce debug symbols
    -g

    # Additional optimizations
    $<$<CONFIG:Release>:-flto>
    -ffast-math
  )

  target_link_options(${TARGET} PRIVATE
    # Always produce debug symbols
    -g

    # Additional optimizations
    $<$<CONFIG:Release>:-flto>
  )
endfunction()

#
# Library
#

set(SOURCE_FILES
  "src/app.c"
  "src/sys/window.c"
  "src/gpu/device.c"
  "src/gpu/swapchain.c"
)

add_library(Doan STATIC ${SOURCE_FILES})
target_include_directories(Doan PRIVATE "src/")
target_include_directories(Doan PUBLIC "include/")
target_link_libraries(Doan PUBLIC dxgi dxguid d3d11 d3dcompiler)
dn_target_setup(Doan)

#
# Example
#

add_subdirectory(Example)
