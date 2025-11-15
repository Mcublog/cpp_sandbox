file(GLOB_RECURSE CMETER_SOURCES
    ${PROJECT_SOURCE_DIR}/libs/CapacityMeter/*.c
    ${PROJECT_SOURCE_DIR}/libs/CapacityMeter/*.cpp
)

file(GLOB SMACLLOC_SOURCES
    ${PROJECT_SOURCE_DIR}/libs/smalloc/*.c
)

set(DEARIMGUI_SOURCES
    ${PROJECT_SOURCE_DIR}/libs/imgui/imgui.cpp
    ${PROJECT_SOURCE_DIR}/libs/imgui/imgui_tables.cpp
    ${PROJECT_SOURCE_DIR}/libs/imgui/imgui_draw.cpp
    ${PROJECT_SOURCE_DIR}/libs/imgui/imgui_widgets.cpp
    ${PROJECT_SOURCE_DIR}/libs/imgui/backends/imgui_impl_sdl2.cpp
)

set(DEARIMGUI_INCLUDES
    ${PROJECT_SOURCE_DIR}/libs/imgui
    ${PROJECT_SOURCE_DIR}/libs/imgui/backends
)

set(DEARIMGUI_SOURCES
    ${PROJECT_SOURCE_DIR}/libs/imgui/imgui.cpp
    ${PROJECT_SOURCE_DIR}/libs/imgui/imgui_tables.cpp
    ${PROJECT_SOURCE_DIR}/libs/imgui/imgui_draw.cpp
    ${PROJECT_SOURCE_DIR}/libs/imgui/imgui_widgets.cpp
    ${PROJECT_SOURCE_DIR}/libs/imgui/backends/imgui_impl_sdl2.cpp
)

# set(NATIVEFILEDIALOG_INCLUDES
#     ${PROJECT_SOURCE_DIR}/libs/nativefiledialog/src/include
# )

# file(GLOB NATIVEFILEDIALOG_SOURCES
#     ${PROJECT_SOURCE_DIR}/libs/nativefiledialog/src/nfd_common.c
#     # ${PROJECT_SOURCE_DIR}/libs/nativefiledialog/src/nfd_gtk.c
#     ${PROJECT_SOURCE_DIR}/libs/nativefiledialog/src/nfd_zenity.c
# )