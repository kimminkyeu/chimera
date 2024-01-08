cmake_minimum_required(VERSION 3.27)

set(NAME Scop)
set( CMAKE_CXX_STANDARD 17 )                # Cpp compiler version
set( CMAKE_CXX_STANDARD_REQUIRED True )

project(${NAME} VERSION 1.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/build/build_util)

# Set modules(internal)
find_package(ChimeraEngineCore REQUIRED)
if(ChimeraEngineCore_NOT_FOUND)
    message(FATAL_ERROR "ChimeraEngineCore not found")
endif()

add_executable(Scop source/Scop.cpp)

# Set include path
target_include_directories(Scop PUBLIC
    ${ChimeraEngineCore_INCLUDE_DIR}
)


target_link_libraries(Scop PUBLIC
    ${ChimeraEngineCore_LIBRARY}
)

set_target_properties(Scop PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out
    )


