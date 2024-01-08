cmake_minimum_required(VERSION 3.27)

set(NAME Scop)
set( CMAKE_CXX_STANDARD 17 )                # Cpp compiler version
set( CMAKE_CXX_STANDARD_REQUIRED True )

project(${NAME} VERSION 1.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "../engine/build/build_util")

# Set modules(internal)

# Set include path
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../engine/include
)

add_executable(Scop source/Scop.cpp )

 target_link_libraries(Scop
                      ${CMAKE_CURRENT_SOURCE_DIR}/../engine/build/intermediate/ChimeraEngineCore.dylib
)