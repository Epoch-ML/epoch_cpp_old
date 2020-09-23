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
# TODO: replace with Freeimage
option(DISABLE_STB "Disable STB")
if (DISABLE_STB)
	message(STATUS "STB is disabled")
else()
    message(STATUS "STB is enabled")
	find_package(stb REQUIRED)
endif()
message(${STB_INCLUDE_PATH})

# Tiny Obj Loader
# TODO: Replace with ass imp
option(DISABLE_TINY_OBJ_LOADER "Disable tinyobjloader")
if (DISABLE_TINY_OBJ_LOADER)
	message(STATUS "tinyobjloader is disabled")
else()
    message(STATUS "tinyobjloader is enabled")
	find_package(tinyobjloader REQUIRED)
endif()
message(${TINY_OBJ_LOADER_INCLUDE_PATH})

include_directories(src)
include_directories(${STB_INCLUDE_PATH})
include_directories(${SPD_INCLUDE_PATH})
include_directories(${TINY_OBJ_LOADER_INCLUDE_PATH})

ADD_CUSTOM_TARGET(debug
	COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
  	COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
  	COMMENT "Switch CMAKE_BUILD_TYPE to Debug"
)

ADD_CUSTOM_TARGET(release
	COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
	COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
	COMMENT "Switch CMAKE_BUILD_TYPE to Release"
)

# make CTEST_OUTPUT_ON_FAILURE=1 test types.tests

add_subdirectory(src)

# Set start up project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT helloworld)
