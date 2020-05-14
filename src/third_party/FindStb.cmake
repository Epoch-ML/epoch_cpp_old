# CMake package finder for STB

set(STB_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/src/third_party/stb")

find_path(STB_INCLUDE_PATH ...)

if((NOT STB_INCLUDE_PATH) OR (NOT EXISTS ${STB_INCLUDE_PATH}))
    
    message("Unable to find STB")

    execute_process(COMMAND git submodule update --init -- ${PROJECT_SOURCE_DIR}/src/third_party/stb
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    
    set(STB_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb
        CACHE PATH "STB include directory")

    # for convenience setup a target
    add_library(stb INTERFACE)

    target_include_directories(stb INTERFACE
                               $<BUILD_INTERFACE:${STB_INCLUDE_PATH}>
                               $<INSTALL_INTERFACE:${some_dest}>)
endif()

# Set it up
if (NOT TARGET stb)
    include(FindPackageHandleStandardArgs)
    find_path(STB_INCLUDE_PATH NAMES stb/spdlog.h)
    find_package_handle_standard_args(stb DEFAULT_MSG STB_INCLUDE_PATH)

    add_library(stb INTERFACE)

    target_include_directories(stb INTERFACE ${STB_INCLUDE_PATH})

	include_directories(${STB_INCLUDE_PATH})

endif ()