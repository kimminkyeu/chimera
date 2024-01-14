cmake_minimum_required(VERSION 3.24)

set(NAME Scop)
set( CMAKE_CXX_STANDARD 17 ) # Cpp compiler version
set( CMAKE_CXX_STANDARD_REQUIRED True )
project(${NAME} VERSION 1.0 LANGUAGES CXX)

# -----------------------------------------------------------------------------
# Load Chimera's Local cmake util functions
include(../engine/build/build_util/macros.cmake)

# Generate a compile_commands.json in your build directory that lists every file and it's compile command.
# -----------------------------------------------------------------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cm_printf(STATUS "List of every file and it's compile command is saved to 'build/compile_commands.json'")

# -----------------------------------------------------------------------------
# Scop library
set(INC
  # ...
)

set(LIB
  # ...
)

set(SRC 
  source/Scop.cpp
  )

add_executable( ${CMAKE_PROJECT_NAME} ${SRC} )
target_link_libraries( ${CMAKE_PROJECT_NAME} ${LIB} )
target_include_directories( ${CMAKE_PROJECT_NAME} PUBLIC ${INC} )

# -----------------------------------------------------------------------------
# ChimeraEngineCore library

# 시스템 설치 버전
  # find_package(ChimeraEngineCore 0.0.1 REQUIRED)
  # target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${ChimeraEngineCore_LIBRARIES})
  # target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${ChimeraEngineCore_INCLUDE_DIR})

# 그냥 빌드해서 find_package하는 버전
  list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/../engine/build/intermediate")
  find_package(ChimeraEngineCore 0.0.1 REQUIRED)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${ChimeraEngineCore_LIBRARIES})
  target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${ChimeraEngineCore_INCLUDE_DIR})
# -----------------------------------------------------------------------------


