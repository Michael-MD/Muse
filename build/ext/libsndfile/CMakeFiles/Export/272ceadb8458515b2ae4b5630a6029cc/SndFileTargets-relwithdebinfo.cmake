#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SndFile::sndfile" for configuration "RelWithDebInfo"
set_property(TARGET SndFile::sndfile APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(SndFile::sndfile PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "C"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/sndfile.lib"
  )

list(APPEND _cmake_import_check_targets SndFile::sndfile )
list(APPEND _cmake_import_check_files_for_SndFile::sndfile "${_IMPORT_PREFIX}/lib/sndfile.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
