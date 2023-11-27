include(FindPackageHandleStandardArgs)
add_library(qt5menugen SHARED IMPORTED)
find_library(qt5menugen_LIB
  NAMES qt5menugen
  PATHS /usr/lib /usr/local/lib /usr/lib64 /usr/local/lib64)
# find_path(qt5menugen_INCLD
#    HINTS /usr/local/include /usr/include)
set(qt5menugen_VERSION 2.0.1)

find_package_handle_standard_args(qt5menugen
  REQUIRED_VARS qt5menugen_LIB
  VERSION_VAR qt5menugen_VERSION
  HANDLE_COMPONENTS)

if (qt5menugen_LIB)
  # mark_as_advanced(qt5menugen_INCLD)
  mark_as_advanced(qt5menugen_LIB)
  mark_as_advanced(qt5menugen_VERSION)
endif()

set_target_properties(qt5menugen PROPERTIES IMPORTED_LOCATION "${qt5menugen_LIB}")
