# epoch/src/junk/CMakeLists.txt : CMake configuration file for Hello World project
# Hello World is really just a validation of the CMake build system to ensure 
# CMake is working well in build and debug on all platforms

file(GLOB SOURCES 
  "*.cpp"
  "*.h"
)
source_group("" FILES ${SOURCES})

add_executable(helloworld
    ${SOURCES}
)

target_link_libraries(helloworld
  core
  logger
)