cmake_minimum_required(VERSION 4.1)

# ============================================================================ #
#   Example                                                                    #
# ============================================================================ #

add_executable(Example "main.c")
target_link_libraries(Example PRIVATE DoanLib)
dn_target_setup(Example)
