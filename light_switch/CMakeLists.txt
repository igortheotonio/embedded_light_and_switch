cmake_minimum_required(VERSION 3.8.2)

set(BOARD_ROOT ${CMAKE_SOURCE_DIR}/../)
set(BOARD nrf52_light_switch)
include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(LIGHT_SWITCH)

# Generate version file
include(build.version.cmake NO_POLICY_SCOPE)

# Schedule a new version file generation after every build
add_custom_command(
    TARGET app
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}"
    -DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}
    -DPROJECT_BINARY_DIR=${PROJECT_BINARY_DIR}
    -P ${PROJECT_SOURCE_DIR}/build.version.cmake
    )

list(APPEND HEADERS
    "${PROJECT_BINARY_DIR}/include/generated"
    "${PROJECT_BINARY_DIR}/zephyr/include/generated"
    "${PROJECT_SOURCE_DIR}/include"
    )

list(APPEND SOURCES
    "${PROJECT_SOURCE_DIR}/src/main.c"
    "${PROJECT_SOURCE_DIR}/src/encoder.c"
    "${PROJECT_SOURCE_DIR}/src/leds.c"
    "${PROJECT_SOURCE_DIR}/src/bt_mesh.c"
    "${PROJECT_SOURCE_DIR}/src/node_composition.c"
    "${PROJECT_SOURCE_DIR}/src/firmware_version.c"
    )

include_directories(${HEADERS})
target_sources(app PRIVATE ${SOURCES})