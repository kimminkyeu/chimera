cmake_minimum_required(VERSION 3.24)

set(NAME Scop)
set( CMAKE_CXX_STANDARD 17 ) # Cpp compiler version
set( CMAKE_CXX_STANDARD_REQUIRED True )
project(${NAME} VERSION 1.0 LANGUAGES CXX)

# -----------------------------------------------------------------------------
# Load Chimera's Local cmake util functions
include(../engine/build/build_util/macros.cmake)

# -----------------------------------------------------------------------------
# Avoid having an empty `CMAKE_BUILD_TYPE` : 빌드 타입 명시가 없으면 기본 debug로 설정.
if (NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
  cm_printf_important(WARN "CMAKE BUILD TYPE : Unspecified")
  cm_printf(WARN "  - Setting CMAKE_BUILD_TYPE to Debug by default...")
  cm_printf(WARN "  - For release build, please add '-DCMAKE_BUILD_TYPE=Release' ARGS to Cmake...")
  set(CMAKE_BUILD_TYPE Debug) # Default build type.
else()
  cm_printf_important(WARN "CMAKE BUILD TYPE : ${CMAKE_BUILD_TYPE}")
endif()

# -----------------------------------------------------------------------------
# Disbale CMAKE DEPRECATION WARNING
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

# Generate a compile_commands.json in your build directory that lists every file and it's compile command.
# -----------------------------------------------------------------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cm_printf(STATUS "Platform : ${CMAKE_HOST_SYSTEM_NAME}-${CMAKE_HOST_SYSTEM_VERSION}")
cm_printf(STATUS "Compiler : ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
cm_printf(STATUS "Compile commands are saved to \"build/compile_commands.json\"")

# -----------------------------------------------------------------------------
# Compiler warning level setting
#   - https://stackoverflow.com/questions/2368811/how-to-set-warning-level-in-cmake
if (MSVC)
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]") # Force to always compile with W4
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX) # Update if necessary
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wextra -Wall -Wno-long-long")
endif()

# -----------------------------------------------------------------------------
# Set project source file
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

# Set executable output directory
set_target_properties(${CMAKE_PROJECT_NAME}
  PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binary
)
cm_printf_important(WARN "Executable output path : ${CMAKE_SOURCE_DIR}/binary")

# -----------------------------------------------------------------------------
# ChimeraEngineCore library

# 시스템 설치 버전 (김민규: 현재 미지원 기능입니다)
  # find_package(ChimeraEngineCore 0.0.1 REQUIRED)
  # target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${ChimeraEngineCore_LIBRARIES})
  # target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${ChimeraEngineCore_INCLUDE_DIR})

# 그냥 빌드해서 find_package하는 버전
  list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/../engine/build/intermediate")
  find_package(ChimeraEngineCore 0.0.1 REQUIRED)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${ChimeraEngineCore_LIBRARIES})
  target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${ChimeraEngineCore_INCLUDE_DIR})
# -----------------------------------------------------------------------------


