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
source_group("vulkan" FILES ${VULKAN_SOURCES})

file(GLOB SHADERS_SOURCES 
  "shaders/*.cpp"
  "shaders/*.h"
  "shaders/*.vert"
  "shaders/*.frag"
)
source_group("shaders" FILES ${SHADERS_SOURCES})

IF(WIN32)
    add_library(hal
        ${SOURCES}
        ${VULKAN_SOURCES}
    )

    target_link_libraries(hal
        Vulkan::Vulkan
    )

    # Vulkan
    # TODO: Might want to move this to the actual vulkan hal
    find_package(Vulkan REQUIRED)

    message(${Vulkan_INCLUDE_DIR})

    #add_executable(vulkaninfo ${Vulkan_INCLUDE_DIR}\..\Demos\vulkaninfo.cpp)
    #target_compile_definitions(vulkaninfo PRIVATE VK_USE_PLATFORM_WIN32_KHR)
    #target_include_directories(vulkaninfo PRIVATE Vulkan::Vulkan)
    #target_link_libraries(vulkaninfo Vulkan::Vulkan)
    include_directories(${Vulkan_INCLUDE_DIR})

ELSEIF(APPLE)
    add_library(hal
        ${SOURCES}
    )
ELSE()
    add_library(hal
        ${SOURCES}
    )
ENDIF()



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
  Vulkan::Vulkan
)

add_test(
  NAME 
    all_tests
  COMMAND 
    "hal.tests"
)