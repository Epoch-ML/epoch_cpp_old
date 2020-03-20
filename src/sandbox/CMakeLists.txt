# src/sandbox/CMakeList.txt : CMake project for epoch sandbox

file(GLOB SOURCES 
  "*.cpp"
  "*.h"
)
source_group("" FILES ${SOURCES})

file(GLOB ANDROID_SOURCES 
  "android/*.cpp"
  "android/*.h"
)
source_group("android" FILES ${ANDROID_SOURCES})

file(GLOB IOS_SOURCES 
  "ios/*.cpp"
  "ios/*.h"
)
source_group("ios" FILES ${IOS_SOURCES})

file(GLOB LINUX_SOURCES 
  "linux/*.cpp"
  "linux/*.h"
)
source_group("linux" FILES ${LINUX_SOURCES})

file(GLOB OSX_SOURCES 
  "osx/*.cpp"
  "osx/*.h"
)
source_group("osx" FILES ${OSX_SOURCES})

file(GLOB WIN64_SOURCES 
  "win64/*.cpp"
  "win64/*.h"
)
source_group("win64" FILES ${WIN64_SOURCES})

# TODO: Switch to the below
add_library(sandbox
    ${SOURCES}
    ${WIN64_SOURCES}
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

add_executable(sandbox.tests
    "sandbox.tests.cpp"
)

target_link_libraries(sandbox.tests
  core
  logger
  sandbox
)

add_test(
  NAME 
    all_tests
  COMMAND 
    "sandbox.tests"
)

# Copy over the DLL so things don't implode
# add_custom_command(TARGET sandbox.tests POST_BUILD        
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different  
#     $<TARGET_FILE:unitydlltest>      
#     $<TARGET_FILE_DIR:unitydlltest.tests>)                 