include(${PROJECT_SOURCE_DIR}/config/common.cmake)
include(${PROJECT_SOURCE_DIR}/libs/libs.cmake)

# For testing using desktop gcc compiler
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

find_package(SDL2 REQUIRED)

# Set target name
set(TEST_TARGET "imguitest")

file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/imgui_test/*.c
    ${PROJECT_SOURCE_DIR}/imgui_test/*.cpp
    ${PROJECT_SOURCE_DIR}/libs/imgui/imgui_demo.cpp
    ${PROJECT_SOURCE_DIR}/libs/imgui/backends/imgui_impl_opengl2.cpp
)

# Buid TEST_TARGET target
add_executable(${TEST_TARGET}
    # common
    ${COMMON_SOURCES}
    # libs
    ${PROJECT_SOURCE_DIR}/libs/debug/log_libs.c
    ${DEARIMGUI_SOURCES}
    ${SDL2_INCLUDE_DIRS}
    # project
    ${SOURCES}

)

# Include paths
target_include_directories(${TEST_TARGET} PRIVATE
    # common
    ${COMMON_INCLUDES}
    ${PROJECT_SOURCE_DIR}/libs
    ${PROJECT_SOURCE_DIR}/libs/debug
    ${DEARIMGUI_INCLUDES}
)

target_compile_definitions(${TEST_TARGET} PUBLIC "FORCE_LOG" ${SDL2_DEFINITIONS})

# Add pthread
target_link_libraries(${TEST_TARGET} pthread GL dl ${SDL2_LIBRARIES})
