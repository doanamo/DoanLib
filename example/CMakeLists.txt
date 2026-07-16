add_executable(Example "main.c")
target_link_libraries(Example PRIVATE DoanLib)
dn_target_setup(Example)
