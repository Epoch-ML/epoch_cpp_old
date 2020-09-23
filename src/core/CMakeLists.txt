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
source_group(ehm FILES ${EHM_SOURCES})

# Math
file(GLOB MATH_SOURCES 
  "math/*.cpp"
  "math/*.h"
)
source_group(math FILES ${MATH_SOURCES})

# Model
file(GLOB MODEL_SOURCES 
  "model/*.cpp"
  "model/*.h"
)
source_group(model FILES ${MODEL_SOURCES})

# Image
file(GLOB IMAGE_SOURCES 
  "image/*.cpp"
  "image/*.h"
)
source_group(image FILES ${IMAGE_SOURCES})

# Math Matrix
file(GLOB MATRIX_SOURCES 
  "math/matrix/*.cpp"
  "math/matrix/*.h" 
)
source_group(math\\matrix FILES ${MATRIX_SOURCES})

# Types 
file(GLOB TYPES_SOURCES 
  "types/*.cpp"
  "types/*.h"
)
source_group(types FILES ${TYPES_SOURCES})

add_library(core 
    ${CORE_SOURCES} 
    ${EHM_SOURCES}
    ${MATH_SOURCES}
    ${MODEL_SOURCES}
    ${IMAGE_SOURCES}
    ${MATRIX_SOURCES}
    ${TYPES_SOURCES}
)

add_executable(types.tests
  types/types.tests.cpp
  types/EPVector.tests.cpp
  types/EPString.tests.cpp
  types/EPRef.tests.cpp
  types/EPTuple.tests.cpp
)

target_link_libraries(types.tests
  core
  logger
  eptest
)

add_test(
  NAME 
    core_tests
  COMMAND 
    "types.tests"
)




