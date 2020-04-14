# src/test/CmakeLists.txt - Core EP test CMakeLists.txt

file(GLOB SOURCES 
  "*.cpp"
  "*.h"
)
source_group("" FILES ${SOURCES})

add_library(eptest
    ${SOURCES} 
)

# add_executable(math.tests
#   math/math.tests.cpp
# )

# target_link_libraries(math.tests
#   core
#   logger
# )

# add_test(
#   NAME 
#     all_tests
#   COMMAND 
#     "math.tests"
# )




