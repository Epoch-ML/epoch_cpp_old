# epoch/src/teaching/igm451/memoization/CMakeLists.txt : CMake configuration file for Hello World project

file(GLOB SOURCES 
  "*.cpp"
  "*.h"
)
source_group("" FILES ${SOURCES})

add_executable(igm451.memoization
    ${SOURCES}
)

target_link_libraries(igm451.memoization
  core
  logger
)