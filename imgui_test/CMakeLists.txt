include(${PROJECT_SOURCE_DIR}/config/common.cmake)
include(${PROJECT_SOURCE_DIR}/libs/libs.cmake)

# For testing using desktop gcc compiler
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

find_package(SDL2 REQUIRED)
find_package(Freetype REQUIRED)
# find_package(GTK REQUIRED gtk+-3.0)

# Set target name
set(TEST_TARGET "imguitest")

file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/imgui_test/*.c
    ${PROJECT_SOURCE_DIR}/imgui_test/*.cpp
    # ${PROJECT_SOURCE_DIR}/libs/imgui/imgui_demo.cpp
    ${PROJECT_SOURCE_DIR}/libs/imgui/misc/freetype/imgui_freetype.cpp
    # ${PROJECT_SOURCE_DIR}/libs/imgui/backends/imgui_impl_opengl2.cpp
    #
    ${CMAKE_CURRENT_SOURCE_DIR}/x11backend/imgui_impl_x11.cpp
    ${PROJECT_SOURCE_DIR}/libs/imgui/backends/imgui_impl_opengl3.cpp
)

# Buid TEST_TARGET target
add_executable(${TEST_TARGET}
    # common
    ${COMMON_SOURCES}
    # libs
    ${PROJECT_SOURCE_DIR}/libs/debug/log_libs.c
    ${DEARIMGUI_SOURCES}
    ${SDL2_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
    # ${NATIVEFILEDIALOG_SOURCES}
    # project
    ${SOURCES}

)

# Include paths
target_include_directories(${TEST_TARGET} PRIVATE
    #
    ${CMAKE_CURRENT_SOURCE_DIR}/x11backend
    # common
    ${COMMON_INCLUDES}
    ${PROJECT_SOURCE_DIR}/libs
    ${PROJECT_SOURCE_DIR}/libs/debug
    ${DEARIMGUI_INCLUDES}
    # ${NATIVEFILEDIALOG_INCLUDES}
    # ${GTK_INCLUDE_DIRS}
)

target_compile_definitions(${TEST_TARGET} PUBLIC "FORCE_LOG" "IMGUI_ENABLE_FREETYPE" ${SDL2_DEFINITIONS})

# Add pthread
target_link_libraries(${TEST_TARGET} pthread GL dl freetype ${SDL2_LIBRARIES} Freetype::Freetype X11)
