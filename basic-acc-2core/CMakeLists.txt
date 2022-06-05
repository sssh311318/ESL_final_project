file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(platform-basic-acc-2core
ethernet.cpp
display.cpp
${HEADERS})

target_include_directories(platform-basic-acc-2core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(riscv-vp-acc-2core
        main.cpp)

target_link_libraries(riscv-vp-acc-2core rv32 platform-basic-acc-2core platform-common gdb-mc ${Boost_LIBRARIES} ${SystemC_LIBRARIES} pthread)

INSTALL(TARGETS riscv-vp RUNTIME DESTINATION bin)
