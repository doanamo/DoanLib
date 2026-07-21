cmake_minimum_required(VERSION 4.1)

# ============================================================================ #
#   Project                                                                    #
# ============================================================================ #

project(DoanLib
  VERSION 0.1.0
  DESCRIPTION "Personal C library for building real-time applications and tools"
  LANGUAGES C
)

# ============================================================================ #
#   Configuration                                                              #
# ============================================================================ #

include(CMakeShared.txt)
dn_global_setup()

# ============================================================================ #
#   Library                                                                    #
# ============================================================================ #

set(SOURCE_FILES
  "src/memory/memory.c"
  "src/memory/malloc.c"
  "src/memory/virtual.c"
  "src/memory/large.c"
  "src/memory/arena.c"
  "src/memory/temp.c"
  "src/structs/string.c"
  "src/utility/file.c"
  "src/system/system.c"
  "src/system/window.c"
  "src/raster/raster.c"
  "src/shared.c"
  "src/main.c"
  "src/app.c"
)

add_library(DoanLib STATIC ${SOURCE_FILES})
target_include_directories(DoanLib PRIVATE "src/")
target_include_directories(DoanLib PUBLIC "include/")
dn_target_setup(DoanLib)

# ============================================================================ #
#   Example                                                                    #
# ============================================================================ #

add_subdirectory(Example)
