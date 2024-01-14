
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
if (NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
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
# file(GLOB PLUGINS
	# external/glad
	# external/glfw
	# )
# foreach(plugin IN LISTS PLUGINS)
#	add_subdirectory(${plugin})	
# endforeach()

# ----------------------------------------------------------------------------
# Setting LIBS/SOURCE/INC

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


# use ExternalProject to download & build external library (ex) spdlog, glfw....)
include(ExternalProject)

ExternalProject_Add(
  dep_spdlog
  GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
  GIT_TAG "v1.x"
  INSTALL_DIR ${CMAKE_SOURCE_DIR}/external/spdlog
  CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
)

set(LIBS ${LIBS} 
    spdlog
)

#의존성 라이브러리 변수 (외부 라이브러리이기 때문에 먼저 빌드해야함)
set(DEP_LIBS ${DEP_LIBS}
    dep_spdlog
		 # glfw
		 # imgui
)

set(LIBS_DIR ${LIBS_DIR}
    ${CMAKE_SOURCE_DIR}/external/spdlog/lib
)

set(SOURCE
	${CMAKE_SOURCE_DIR}/source/core/Application.cpp
	${CMAKE_SOURCE_DIR}/source/core/Logger.cpp
)

# Directories for header include
set(INC
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/source
  ${CMAKE_SOURCE_DIR}/include # scop에서 Config.h의 위치를 찾을 때, 여기 설정된 경로가 scop의 find_package를 통해 설정된다.
  ${CMAKE_SOURCE_DIR}/external/spdlog/include
)


# ----------------------------------------------------------------------------
# Set libraries to link to target
if (DEFINED BUILD_SHARED_LIBS AND BUILD_SHARED_LIBS STREQUAL "ON")
  add_library(${CMAKE_PROJECT_NAME} SHARED ${SOURCE})
  set(CM_SHARED_LIB TRUE)
elseif()
  add_library(${CMAKE_PROJECT_NAME} STATIC ${SOURCE})
  set(CM_STATIC_LIB TRUE)
endif()

set_target_properties(${CMAKE_PROJECT_NAME} 
  PROPERTIES 
    # remove default library output file's prefix ( default prefix = "lib" )
    PREFIX ""
    # Set CMAKE output directory : 최종 실행파일을 밖으로 빼기 위한 용도
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binary
)

#Pre-Compiled header

target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/source/core/Pch.h)
# Chimera 프로젝트 내부에서 사용하는 용도의 헤더 인클루드 경로임으로, PRIVATE으로 작성.
target_include_directories( ${CMAKE_PROJECT_NAME} PRIVATE ${INC} )
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${LIBS_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${LIBS}) #의존 라이브러리를 숨긴다.
add_dependencies(${CMAKE_PROJECT_NAME} ${DEP_LIBS})


# ----------------------------------------------------------------------------
# find_package로 패키지 인식하도록 설정
# (1) 시스템에 설치 (cmake --install) : find_package를 통해 Chimera Engine을 하드디스크에 install 하는 방식
# (2) 미설치 방식 : 그냥 현재 경로에 있는 Config.cmake을 이용하는 방식.
#     - https://cmake.org/cmake/help/latest/guide/importing-exporting/index.html
#     - https://cmake.org/cmake/help/latest/guide/tutorial/Adding%20Export%20Configuration.html

target_include_directories( ${CMAKE_PROJECT_NAME}
                            INTERFACE
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/binary>
                            $<INSTALL_INTERFACE:include>
                           )
set( TARGETS_EXPORT_NAME          ${CMAKE_PROJECT_NAME}Targets )
# ************************************************************************************
set( HEADERS                      include/Chimera.h )

if ( CM_SHARED_LIB )
  set( CHIMERA_LIBRARY_NAME ${CMAKE_PROJECT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX} ) # Ex.ChimeraEngineCore.dylib
elseif()
  set( CHIMERA_LIBRARY_NAME ${CMAKE_PROJECT_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX} ) # Ex.ChimeraEngineCore.a
endif()

set( CHIMERA_INSTALL_LOCATION           ${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME} )
# ************************************************************************************
# 아래 두 변수는 Config.cmake.in를 통해 외부로 export됩니다.
set( CHIMERA_EXPORT_INCLUDE_DIR   ${INC} )        # --> ChimeraEngineCore_INCLUDE_DIR
set( CHIMERA_EXPORT_LIBRARIES     ${CMAKE_SOURCE_DIR}/binary/${CHIMERA_LIBRARY_NAME} )  # --> ChimeraEngineCore_LIBRARIES
# ************************************************************************************

# library 설치
install(TARGETS ${CMAKE_PROJECT_NAME}
        EXPORT        ${TARGETS_EXPORT_NAME} 
        DESTINATION   ${CHIMERA_INSTALL_LOCATION}
        LIBRARY   DESTINATION ${CHIMERA_INSTALL_LOCATION}/lib
        ARCHIVE   DESTINATION ${CHIMERA_INSTALL_LOCATION}/lib
        INCLUDES  DESTINATION ${CHIMERA_INSTALL_LOCATION}/include
        PUBLIC_HEADER  DESTINATION include/Chimera.h
)

# install include header to destination.
# cmake --install로 Chimera를 시스템 설치 시 헤더 경로.
# install(FILES ${HEADERS} DESTINATION ${CHIMERA_INSTALL_LOCATION}/include)

# install export target and config for find_package
install(EXPORT ${TARGETS_EXPORT_NAME}
  FILE "${TARGETS_EXPORT_NAME}.cmake"
  NAMESPACE "${CMAKE_PROJECT_NAME}::"
  DESTINATION ${CHIMERA_INSTALL_LOCATION}
)

# generate the Config file that includes the exports
include(CMakePackageConfigHelpers)
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION ${CHIMERA_INSTALL_LOCATION}
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# generate the Version file that includes the exports
set(CHIMERA_VERSION_MAJOR 0)
set(CHIMERA_VERSION_MINOR 0)
set(CHIMERA_VERSION_PATCH 1)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake"
  VERSION         "${CHIMERA_VERSION_MAJOR}.${CHIMERA_VERSION_MINOR}.${CHIMERA_VERSION_PATCH}"
  COMPATIBILITY   AnyNewerVersion
  # AnyNewerVersion|SameMajorVersion|SameMinorVersion|ExactVersion 중 선택
)

# 위에서 만든 Config, ConfigVersion 파일 두개를 설치할 시스템 위치.
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake
  DESTINATION ${CHIMERA_INSTALL_LOCATION}
) 

# 동일한 .cmake 파일을, 이번엔 시스템이 아닌 현재 디렉토리에 다시 저장 (install 없이 사용하기 위함)
export(EXPORT ${TARGETS_EXPORT_NAME}
  FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGETS_EXPORT_NAME}.cmake"
)

# .... 설정 끝 .....
# cmake --build 혹은 make 을 통해 라이브러리를 빌드한 후...
# 두가지 방식으로 find_package(...)가 가능해집니다.
#
# -----------------------------------------------------------------
#    (1) "나는 내 컴 시스템에 Chimera를 설치해두고싶다" (Scop만으로 빌드 가능하도록)
# -----------------------------------------------------------------
#     sudo cmake --install 혹은 sudo make install을 실행합니다. (**관리자 권한 필요**)
#     시스템 디렉토리에 Chimera의 lib파일과 헤더, cmakeConfig 파일이 설치됩니다. 
#     Ex) Unix   : usr/local/
#         Window : c:/Program Files/${PROJECT_NAME}/
#
#     (윈도우, 유닉스 각각 다른 경로로 설치되며, 확인하고 싶으면 아래 주석을 풀고 실행해보세요.)
      # cm_printf(WARN "System Installation Path : ${CHIMERA_INSTALL_LOCATION}")
#
#     시스템 설치가 완료되면 Scop의 CMakeLists.txt에서 아래와 같이 Chimera를 import할 수 있습니다.
#
#     - find_package( ChimeraEngineCore 0.0.1 REQUIRED )
#     - target_link_libraries( Foo PRIVATE ${ChimeraEngineCore_LIBRARIES} )
#     - target_include_directories( Foo PUBLIC ${ChimeraEngineCore_INCLUDE_DIR} )
#
#
# -----------------------------------------------------------------
#    (2) 아니다. 나는 시스템에 설치하기 싫다. (ex. 클러스터처럼 관리자 권한이 없는 경우)
# -----------------------------------------------------------------
#     시스템에 설치하지 않았기 때문에, 
#     아래와 같이 ChimeraEngineCoreConfig.cmake이 있는 경로를 직접 제공해줘야 합니다.
#
#     - list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/../engine/build/intermediate")
#     - find_package(ChimeraEngineCore 0.0.1 REQUIRED)
#     - target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${ChimeraEngineCore_LIBRARIES})
#     - target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${ChimeraEngineCore_INCLUDE_DIR})
#
# 


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