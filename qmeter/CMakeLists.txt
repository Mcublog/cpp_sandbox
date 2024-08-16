include(${PROJECT_SOURCE_DIR}/config/common.cmake)
include(${PROJECT_SOURCE_DIR}/libs/libs.cmake)

# For testing using desktop gcc compiler
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Set defines
add_definitions(-DprojCOVERAGE_TEST=0)
add_definitions(-D_WINDOWS_)
add_definitions(-DDESKTOP)

# Set target name
set(TEST_TARGET "cmeter")

file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/qmeter/*.c
    ${PROJECT_SOURCE_DIR}/qmeter/*.cpp
)

# Buid TEST_TARGET target
add_executable(${TEST_TARGET}
    # common
    ${COMMON_SOURCES}
    # libs
    ${PROJECT_SOURCE_DIR}/libs/debug/log_libs.c
    ${CMETER_SOURCES}
    # project
    ${SOURCES}
)

# Include paths
target_include_directories(${TEST_TARGET} PRIVATE
    # common
    ${COMMON_INCLUDES}
    ${PROJECT_SOURCE_DIR}/libs
    ${PROJECT_SOURCE_DIR}/libs/CapacityMeter
    ${PROJECT_SOURCE_DIR}/libs/debug
)

# Add pthread
target_link_libraries(${TEST_TARGET} pthread)

# # Added test
# add_test(NAME ${TEST_TARGET}
#     COMMAND ${TEST_TARGET})
