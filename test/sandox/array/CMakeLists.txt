include(${PROJECT_SOURCE_DIR}/config/common.cmake)

# For testing using desktop gcc compiler
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Set target name
set(TEST_TARGET "sandbox_array")

file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# Buid TEST_TARGET target
add_executable(${TEST_TARGET}
    # common
    # ${COMMON_SOURCES}
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

# Add pthread
target_link_libraries(${TEST_TARGET} pthread)

# # Added test
# add_test(NAME ${TEST_TARGET}
#     COMMAND ${TEST_TARGET})
