include(${PROJECT_SOURCE_DIR}/config/common.cmake)
include(${PROJECT_SOURCE_DIR}/libs/libs.cmake)

# For testing using desktop gcc compiler
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Set target name
set(TEST_TARGET "flowcontrol")

file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/flowcontrol/*.c
    ${PROJECT_SOURCE_DIR}/flowcontrol/*.cpp
)

# Buid TEST_TARGET target
add_executable(${TEST_TARGET}
    # common
    ${COMMON_SOURCES}
    # libs
    ${PROJECT_SOURCE_DIR}/libs/debug/log_libs.c
    # project
    ${SOURCES}
)

# Include paths
target_include_directories(${TEST_TARGET} PRIVATE
    # common
    ${COMMON_INCLUDES}
    ${PROJECT_SOURCE_DIR}/libs
    ${PROJECT_SOURCE_DIR}/libs/debug
)

target_compile_definitions(${TEST_TARGET} PUBLIC "FORCE_LOG")

# Add pthread
target_link_libraries(${TEST_TARGET} pthread)

