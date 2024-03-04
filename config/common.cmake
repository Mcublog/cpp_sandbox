# Common includes
set(COMMON_INCLUDES
    ${PROJECT_SOURCE_DIR}
)

# Common sources
file(GLOB_RECURSE COMMON_SOURCES
    ${PROJECT_SOURCE_DIR}/common/*.c
    ${PROJECT_SOURCE_DIR}/common/*.cpp
)