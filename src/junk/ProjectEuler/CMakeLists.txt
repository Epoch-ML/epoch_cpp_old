# epoch/src/junk/euler/CMakeLists.txt : CMake configuration file for euler projects

# Euler problems solved for Idan Beck
# TODO: Code org pattern to allow for private code etc

file(GLOB SOURCES 
  "*.cpp"
  "*.h"
)
source_group("" FILES ${SOURCES})

add_executable(project_euler
    ${SOURCES}
)

target_link_libraries(project_euler
  core
  logger
  eptest
)