# src/core/CmakeLists.txt - Core EP CMakeLists 

# Core
file(GLOB CORE_SOURCES 
  "*.cpp"
  "*.h"
)
source_group("" FILES ${CORE_SOURCES})

# EHM
file(GLOB EHM_SOURCES 
  "ehm/*.cpp"
  "ehm/*.h"
)
source_group("ehm" FILES ${EHM_SOURCES})

# # Math
# file(GLOB MATH_SOURCES 
#   "math/*.cpp"
#   "math/*.h"
# )
# source_group("math" FILES ${MATH_SOURCES})

# Types 
file(GLOB TYPES_SOURCES 
  "types/*.cpp"
  "types/*.h"
)
source_group("types" FILES ${TYPES_SOURCES})

add_library(core 
    ${CORE_SOURCES} 
    ${EHM_SOURCES}
#    ${MATH_SOURCES}
    ${TYPES_SOURCES}
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




