cmake_minimum_required(VERSION 4.1)

# ============================================================================ #
#   Example                                                                    #
# ============================================================================ #

add_executable(DoanExample "main.c")
target_link_libraries(DoanExample PRIVATE DoanLib)
dn_target_setup(DoanExample)
