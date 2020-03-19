# epoch/src/logger/CMakeLists.txt : CMake configuration file for logger

# The logger is essentially a wrapper around Spdlog, but improved 
# facilities are inevitable

set(EP_LOG_DIR "${CMAKE_BINARY_DIR}/logs")
add_definitions(-DEPOCHLOGDIR="${EP_LOG_DIR}")
file(MAKE_DIRECTORY ${EP_LOG_DIR})

file(GLOB SOURCES 
  "*.cpp"
  "*.h"
)
source_group("" FILES ${SOURCES})

add_library(logger
    ${SOURCES}
)

target_link_libraries(logger
    core
)

add_dependencies(logger 
    spdlog
)