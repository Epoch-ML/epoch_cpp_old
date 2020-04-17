# epoch/src/teaching/igm451/simd/CMakeLists.txt : CMake configuration file for Hello World project

file(GLOB SOURCES 
  "*.cpp"
  "*.h"
)
source_group("" FILES ${SOURCES})

add_executable(igm451.simd
    ${SOURCES}
)

target_link_libraries(igm451.simd
  core
  logger
)