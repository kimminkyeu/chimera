# Locate the ChimeraEngineCore library

find_path(ChimeraEngineCore_INCLUDE_DIR Chimera.h
    PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}/../engine
    PATH_SUFFIXES
        include
)

find_library(ChimeraEngineCore_LIBRARY
    NAMES
        ChimeraEngineCore
    PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}/../engine/binary
    PATH_SUFFIXES
      Debug
      Release
) 

mark_as_advanced(
  ChimeraEngineCore_INCLUDE_DIR
  ChimeraEngineCore_LIBRARY
  )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ChimeraEngineCore
  REQUIRED_VARS
    ChimeraEngineCore_INCLUDE_DIR
    ChimeraEngineCore_LIBRARY
  )

if(CHIMERA_FOUND)
  add_library(CHIMERA::CHIMERA UNKNOWN IMPORTED)
  set_target_properties(CHIMERA::CHIMERA PROPERTIES
    IMPORTED_LINK_INTERFACE_LANGUAGES ["C"|"CXX"]
    IMPORTED_LOCATION "${ChimeraEngineCore_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${ChimeraEngineCore_INCLUDE_DIR}"
    )
endif()