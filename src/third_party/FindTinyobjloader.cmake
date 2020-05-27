# CMake package finder for STB

# https://github.com/tinyobjloader/tinyobjloader

set(TINY_OBJ_LOADER_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/src/third_party/tinyobjloader")

find_path(TINY_OBJ_LOADER_INCLUDE_PATH ...)

if((NOT TINY_OBJ_LOADER_INCLUDE_PATH) OR (NOT EXISTS ${TINY_OBJ_LOADER_INCLUDE_PATH}))
    
    message("Unable to find Tiny Obj Loader")

    execute_process(COMMAND git submodule update --init -- ${PROJECT_SOURCE_DIR}/src/third_party/tinyobjloader
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    
    set(TINY_OBJ_LOADER_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/third_party/tinyobjloader
        CACHE PATH "Tiny Object Loader include directory")

    # for convenience setup a target
    add_library(tinyobjloader INTERFACE)

    target_include_directories(tinyobjloader INTERFACE
                               $<BUILD_INTERFACE:${TINY_OBJ_LOADER_INCLUDE_PATH}>
                               $<INSTALL_INTERFACE:${some_dest}>)
endif()

# Set it up
if (NOT TARGET tinyobjloader)
    include(FindPackageHandleStandardArgs)
    find_path(TINY_OBJ_LOADER_INCLUDE_PATH NAMES tiny_obj_loader.h)
    find_package_handle_standard_args(tinyobjloader DEFAULT_MSG TINY_OBJ_LOADER_INCLUDE_PATH)

    add_library(tinyobjloader INTERFACE)

    target_include_directories(tinyobjloader INTERFACE ${TINY_OBJ_LOADER_INCLUDE_PATH})

	include_directories(${TINY_OBJ_LOADER_INCLUDE_PATH})

endif ()