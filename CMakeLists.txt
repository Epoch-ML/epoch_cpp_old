# /CMakeLists.txt : CMake project for epoch

cmake_minimum_required(VERSION 3.8)

set(EPOCH_VERSION 0.0.1)

project(epoch LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/src/third_party")

# Enable testing
enable_testing()

# Spdlog
option(DISABLE_SPDLOG "Disable Spdlog")
if (DISABLE_SPDLOG)
	message(STATUS "Spdlog is disabled")
else()
    message(STATUS "Spdlog is enabled")
	find_package(Spdlog REQUIRED)
endif()
message(${SPD_INCLUDE_PATH}) 

# STB
option(DISABLE_STB "Disable STB")
if (DISABLE_STB)
	message(STATUS "STB is disabled")
else()
    message(STATUS "STB is enabled")
	find_package(stb REQUIRED)
endif()
message(${STB_INCLUDE_PATH})

include_directories(src)
include_directories(${STB_INCLUDE_PATH})
include_directories(${SPD_INCLUDE_PATH})

add_subdirectory(src)

# Set start up project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT helloworld)
