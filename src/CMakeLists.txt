set(tgt aq_drv_tester)
add_executable(${tgt} main.c)
target_link_libraries(${tgt} acquire-core-platform acquire-device-kit)
