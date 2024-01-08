
# -----------------------------------------------------------------------------
# Initialize Project
cmake_minimum_required(VERSION 3.24)
set( NAME ChimeraEngineCore )               # Executable name
set( CMAKE_CXX_STANDARD 17 )                # Cpp compiler version
set( CMAKE_CXX_STANDARD_REQUIRED True )
project( ${NAME} VERSION 0.1.0 LANGUAGES C CXX )

# -----------------------------------------------------------------------------
# Load Chimera's Local cmake util functions
include(build/build_util/macros.cmake)

# -----------------------------------------------------------------------------
# Avoid having an empty `CMAKE_BUILD_TYPE` : 빌드 타입 명시가 없으면 기본 debug로 설정.
if (NOT DEFINED CMAKE_BUILD_TYPE)
  cm_printf(WARN "===== CMAKE_BUILD_TYPE : Unspecified =====")
  cm_printf(WARN "Setting CMAKE_BUILD_TYPE to Debug by default...")
  cm_printf(WARN "for release build, add '-DCMAKE_BUILD_TYPE=Release' ARGS to Cmake...")
  set(CMAKE_BUILD_TYPE Debug) # Default build type.
else()
  cm_printf(STATUS "===== CMAKE_BUILD_TYPE : ${CMAKE_BUILD_TYPE} =====")
endif()

# -----------------------------------------------------------------------------
# Disbale CMAKE DEPRECATION WARNING
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE) 

# Generate a compile_commands.json in your build directory that lists every file and it's compile command.
# -----------------------------------------------------------------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cm_printf(STATUS "List of every file and it's compile command is saved to 'build/compile_commands.json'")

# -----------------------------------------------------------------------------
# Compiler warning level setting
#   - https://stackoverflow.com/questions/2368811/how-to-set-warning-level-in-cmake
cm_printf(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
if (MSVC)
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]") # Force to always compile with W4
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX) # Update if necessary
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wextra -Wall -Wno-long-long")
endif()

# -------------------------------------------------------------------------------
# Add Global Defines to project. : If CMAKE_BUILD_TYPE is Debug, define CM_DEBUG. else, define CM_RELEASE 
add_compile_definitions(
  CM_PROJECT_ABSOLUTE_PATH="${CMAKE_SOURCE_DIR}" # Project absolute path
  $<IF:$<CONFIG:Debug>,CM_DEBUG,CM_RELEASE> # Set build-type define variable
  GLM_FORCE_SWIZZLE # force GLM to report the configuration as part of the build log
)

# -----------------------------------------------------------------------------
file(GLOB PLUGINS
	# external/glad
	# external/glfw
	)

# foreach(plugin IN LISTS PLUGINS)
#	add_subdirectory(${plugin})	
# endforeach()

if(APPLE)
	INCLUDE_DIRECTORIES(/System/Library/Frameworks)
	FIND_LIBRARY(COCOA_LIBRARY Cocoa)
	FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
	FIND_LIBRARY(IOKit_LIBRARY IOKit)
	FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
	MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
	SET(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
	SET(APPLE_LIBS ${APPLE_LIBS} ${GLFW3_LIBRARY} ${ASSIMP_LIBRARY} ${FREETYPE_LIBRARIES})
	set(LIBS ${LIBS} ${APPLE_LIBS})
endif(APPLE)

set(LIBS ${LIBS}
		 # glfw
		 # imgui
)

file(GLOB SOURCE
	source/core/Application.cpp
)

# add_executable(${CMAKE_PROJECT_NAME} ${SOURCE})

# ----------------------------------------------------------------------------
# Directories for header include
set(INC
  ${CMAKE_SOURCE_DIR}/internal
  ${CMAKE_SOURCE_DIR}/external
  ${CMAKE_SOURCE_DIR}/source
)

# Set libraries to link to target
add_library(${CMAKE_PROJECT_NAME} ${SOURCE})
# remove default library output file's prefix ( default prefix = "lib" )
# Set CMAKE output directory : 최종 실행파일을 밖으로 빼기 위한 용도
set_target_properties(${CMAKE_PROJECT_NAME} 
  PROPERTIES 
    PREFIX ""
    DEFINE_SYMBOL _CHIMERA_BUILD_DLL
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binary
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binary
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binary
)
target_include_directories( ${CMAKE_PROJECT_NAME} PUBLIC ${INC} )
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${LIBS})



# ----------------------------------------------------------------------------
# Add Custom makefile rule (ex make fclean, re)
ADD_CUSTOM_TARGET( re
        COMMENT "Re-building..."
        COMMAND make fclean && make all
        # ...
        )

ADD_CUSTOM_TARGET( fclean
        COMMENT "Cleaning config files, executable and binaries..."
        COMMAND make clean
		COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/build/intermediate
        )