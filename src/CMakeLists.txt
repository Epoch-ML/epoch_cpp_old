# src/CMakeList.txt : CMake project for epoch root source directory

# Both Configurations
set(EPOCH_DIR_VAL "${CMAKE_SOURCE_DIR}")
add_definitions(-DEPOCH_ROOT_DIR="${EPOCH_DIR_VAL}")
add_definitions(-DEPOCH_SRC_DIR="${EPOCH_DIR_VAL}/src")
add_definitions(-DEPOCH_HAL_DIR="${EPOCH_DIR_VAL}/src/hal")
add_definitions(-DEPOCH_SHADERS_DIR="${EPOCH_DIR_VAL}/src/hal/shaders")

# Release Configuration
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -D_WINDOWS")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -D_CRT_SECURE_NO_WARNINGS")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNOMINMAX")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -D__BASE_FILE__=\"%(Filename)%(Extension)\"")

# Debug Configuration
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_CONSOLE")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__STD_C")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_CRT_SECURE_NO_DEPRECATE")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_SCL_SECURE_NO_DEPRECATE")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__STDC_CONSTANT_MACROS")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__STDC_FORMAT_MACROS")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_WIN32_WINNT=0x0A00")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DWINVER=0x0A00")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DWIN32")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_CRT_SECURE_NO_WARNINGS")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DWIN32_LEAN_AND_MEAN")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNOMINMAX")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__BASE_FILE__=\"%(Filename)%(Extension)\"")

set(CMAKE_CXX_FLAGS_CORE "${CMAKE_CXX_FLAGS}")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	message("epoch/src/cmakelists.txt msvc")
	
	# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")      # C++ version
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOCONCEPTS")

	# Compiler Flags Release
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi /W3 /WX /sdl /MP")      # General
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /Ob2 /Oi /Ot /Oy")  # Optimization
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /EHsc /Gy")         # Code Generation
	#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} /MT")         								# Code Generation
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} /MD")         								# Code Generation
	#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /permissive-")          # Language
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /FC")                   # Advanced

	# Compiler Flags Debug
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi /JMC /W3 /WX /sdl /MP")      # General
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od")  # Optimization
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Gm- /EHsc /RTC1 /GS /fp:precise")         # Code Generation
	#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")         								# Code Generation
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")         								# Code Generation
	#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /permissive-")          # Language
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /FC")                   # Advanced

	# Linker flags (Release)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /INCREMENTAL")            # General
	# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB")           # Input
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST")               # Manifest File
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEBUG:FASTLINK")         # Debugging

	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")      # System
	#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")      # System

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")

    message("epoch/src/cmakelists.txt apple clang")

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOCONCEPTS")

	#SET(CMAKE_CXX_FLAGS -pthread)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

	# Debug Configuration
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++17 -Wall -Wextra -Werror -fpermissive -stdlib=libc++ -Wno-reorder -fdeclspec")

	# Release Configuration
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++17 -Wall -Wextra -Werror -fpermissive -stdlib=libc++ -Wno-reorder -fdeclspec")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	
	message("epoch/src/cmakelists.txt GNU")

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOCONCEPTS")

	#SET(CMAKE_CXX_FLAGS -pthread)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors")

	# Debug Configuration
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++17 -Wall -Wextra -Werror -fpermissive -stdlib=libc++ -Wno-reorder -fdeclspec")

	# Release Configuration
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++17 -Wall -Wextra -Werror -fpermissive -stdlib=libc++ -Wno-reorder -fdeclspec")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")


else()

	message("epoch/src/cmakelists.txt other")

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOCONCEPTS")

	#SET(CMAKE_CXX_FLAGS -pthread)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

	# Debug Configuration
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++17 -Wall -Wextra -Werror -fpermissive -stdlib=libc++ -Wno-reorder -fdeclspec")

	# Release Configuration
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++17 -Wall -Wextra -Werror -fpermissive -stdlib=libc++ -Wno-reorder -fdeclspec")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

endif()

# Projects 
# TODO:

# Components
add_subdirectory(logger)
add_subdirectory(core)
add_subdirectory(test)
add_subdirectory(sandbox)
add_subdirectory(hal)

# Adds junk repos
add_subdirectory(junk)

# teaching
add_subdirectory(teaching)