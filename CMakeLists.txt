cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD pico_w)
include(pico_sdk_import.cmake)

project(searcher C CXX ASM)

set(BTSTACK_ROOT ${PICO_SDK_PATH}/lib/btstack)
set(BTSTACK_EXAMPLE_PATH ${BTSTACK_ROOT}/example)
set(BTSTACK_3RD_PARTY_PATH ${BTSTACK_ROOT}/3rd-party)
set(BT_EXAMPLE_COMMON_DIR "${CMAKE_CURRENT_LIST_DIR}")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_BUILD_TYPE Release)
pico_sdk_init()

file(GLOB CPPFILE "*.c")
message("Adding ${PROJECT_NAME} C files: " )
foreach(source ${CPPFILE})
    message("  - " ${source})
endforeach(source)

file(GLOB HFILE "*.h")
message("Adding ${PROJECT_NAME} H files: " )
foreach(source ${HFILE})
    message("  - " ${source})
endforeach(source)

add_executable(${PROJECT_NAME}
${CPPFILE}
${HFILE}
)

target_link_libraries(${PROJECT_NAME}
pico_stdlib
pico_btstack_ble
pico_btstack_cyw43
pico_cyw43_arch_none
hardware_adc
    )
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    )

    target_compile_definitions(${PROJECT_NAME} PRIVATE
    RUNNING_AS_CLIENT=1
)

pico_add_extra_outputs(${PROJECT_NAME})
pico_enable_stdio_usb(${PROJECT_NAME} 1)
#pico_enable_stdio_uart(blink 1)
