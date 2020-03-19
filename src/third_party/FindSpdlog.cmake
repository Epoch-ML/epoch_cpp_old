# CMake package finder for spdlog

# include(ExternalProject)
# find_package(Git REQUIRED)
# # 
# # ExternalProject_Add(
#     # spdlog
#     # PREFIX ${CMAKE_BINARY_DIR}/spdlog
#     # GIT_REPOSITORY https://github.com/gabime/spdlog.git
#     TIMEOUT 10
#     # UPDATE_COMMAND ${GIT_EXECUTABLE} pull --all
#     # CONFIGURE_COMMAND ""
#     # BUILD_COMMAND ""
#     # INSTALL_COMMAND ""
#     # LOG_DOWNLOAD ON
# # )

# # Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
# ExternalProject_Get_Property(spdlog source_dir)
# set(SPDLOG_INCLUDE_DIR ${source_dir}/include CACHE INTERNAL "Path to include folder for spdlog")

# CMake package finder for spdlog

set(SPD_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/src/third_party/spdlog/include")

find_path(SPD_INCLUDE_PATH ...)

if((NOT SPD_INCLUDE_PATH) OR (NOT EXISTS ${SPD_INCLUDE_PATH}))
    
    message("Unable to find spdlog")

    execute_process(COMMAND git submodule update --init -- ${PROJECT_SOURCE_DIR}/src/third_party/spdlog
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    
    set(SPD_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/third_party/spdlog/include
        CACHE PATH "spdlog include directory")

    # for convenience setup a target
    add_library(spdlog INTERFACE)

    target_include_directories(spdlog INTERFACE
                               $<BUILD_INTERFACE:${SPD_INCLUDE_PATH}>
                               $<INSTALL_INTERFACE:${some_dest}>)
endif()

# Set it up
if (NOT TARGET Gabime::Spdlog)
    include(FindPackageHandleStandardArgs)
    find_path(SPD_INCLUDE_PATH NAMES spdlog/spdlog.h)
    find_package_handle_standard_args(Spdlog DEFAULT_MSG SPD_INCLUDE_PATH)

    add_library(spdlog INTERFACE)

    target_include_directories(spdlog INTERFACE ${SPD_INCLUDE_PATH})

    add_library(Gabime::Spdlog ALIAS spdlog)

	include_directories(${SPD_INCLUDE_PATH})

endif ()