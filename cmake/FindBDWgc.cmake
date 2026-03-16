# Try upstream config package first
find_package(BDWgc CONFIG QUIET)

if(NOT BDWgc_FOUND)
  find_package(PkgConfig QUIET)

  if(PkgConfig_FOUND)
    pkg_check_modules(BDWGC QUIET bdw-gc)
  endif()

  if(BDWGC_FOUND)
    add_library(BDWgc::gc UNKNOWN IMPORTED)

    set_target_properties(BDWgc::gc PROPERTIES
            IMPORTED_LOCATION "${BDWGC_LINK_LIBRARIES}"
            INTERFACE_INCLUDE_DIRECTORIES "${BDWGC_INCLUDE_DIRS}"
        )

    set(BDWgc_FOUND TRUE)
  endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BDWgc DEFAULT_MSG BDWgc_FOUND)
