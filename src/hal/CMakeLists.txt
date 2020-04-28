# src/hal/CMakeList.txt : CMake project for epoch HAL

file(GLOB SOURCES 
  "*.cpp"
  "*.h"
)
source_group("" FILES ${SOURCES})

file(GLOB VULKAN_SOURCES 
  "vulkan/*.cpp"
  "vulkan/*.h"
)
source_group("vulkan" FILES ${ANDROID_SOURCES})

# TODO: Switch to the below
add_library(hal
    ${SOURCES}
    ${VULKAN_SOURCES}
)

#IF(APPLE
#   Mac OS X specific code
#   TODO: Add OSX vs. iOS
#   add_library(sandbox
#   ${SOURCES}
#   ${OSX_SOURCES}
#   )
#
#   add_library(sandbox
#   ${SOURCES}
#   ${IOS_SOURCES}
#   )
#)
#ELSEIF(UNIX AND NOT APPLE)
#    add_library(sandbox
#        ${SOURCES}
#        ${LINUX_SOURCES}
#    )
#ELSEIF(ANDROID)
#    add_library(sandbox
#        ${SOURCES}
#        ${LINUX_SOURCES}
#    )
#ELSEIF(WIN32)
#    add_library(sandbox
#        ${SOURCES}
#        ${WIN64_SOURCES}
#    )
#ENDIF()

# Tests

add_executable(hal.tests
    "hal.tests.cpp"
    "Vulkan/VulkanHAL.tests.cpp"
)

target_link_libraries(hal.tests
  core
  logger
  sandbox
  eptest
  hal
)

add_test(
  NAME 
    all_tests
  COMMAND 
    "hal.tests"
)