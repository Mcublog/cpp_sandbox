cmake_minimum_required(VERSION 3.22)

set_property(GLOBAL PROPERTY RULE_MESSAGES OFF)

# Setup compiler settings
SET(CMAKE_C_COMPILER_WORKS           1)
SET(CMAKE_CXX_COMPILER_WORKS         1)
# set(CMAKE_TRY_COMPILE_TARGET_TYPE   "STATIC_LIBRARY")
set(CMAKE_C_STANDARD                11)
set(CMAKE_C_STANDARD_REQUIRED       ON)
set(CMAKE_C_EXTENSIONS              ON)
set(CMAKE_CXX_STANDARD              20)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            ON)
set(CMAKE_COLOR_DIAGNOSTICS         ON)
if(NOT ${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_SYSTEM_NAME           Generic) # workaround for Windows
endif()

set_property(GLOBAL PROPERTY RULE_MESSAGES OFF)

include(config/utils.cmake)
# endregion

project(CppSandbox VERSION 0.0.1 LANGUAGES C CXX ASM)

LOG_INFO("Version:.........." ${CMAKE_PROJECT_VERSION})
LOG_INFO("Host system:......" ${CMAKE_SYSTEM_NAME})
LOG_INFO("Build type:......." ${CMAKE_BUILD_TYPE})

add_subdirectory(cobs)
add_subdirectory(qmeter)
add_subdirectory(dispemu)
add_subdirectory(sanbox)
add_subdirectory(sockets)
add_subdirectory(alocator)

# region Tests
# message("******* [Unit test list] *******")
enable_testing()
add_subdirectory(test)
# endregion
