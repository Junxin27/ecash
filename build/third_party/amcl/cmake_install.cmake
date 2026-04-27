# Install script for directory: D:/EDU/FYP2/ecash-fyp/third_party/amcl

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/ecash_fyp")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "D:/EDU/FYP2/MSYS2/mingw64/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/lib/libamcl_core.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/amcl" TYPE FILE FILES
    "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/include/amcl.h"
    "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/include/arch.h"
    "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/include/version.h"
    "D:/EDU/FYP2/ecash-fyp/third_party/amcl/include/utils.h"
    "D:/EDU/FYP2/ecash-fyp/third_party/amcl/include/randapi.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/amcl" TYPE FILE FILES "D:/EDU/FYP2/ecash-fyp/third_party/amcl/include/ecdh_support.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/lib/libamcl_curve_BLS381.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/amcl" TYPE FILE FILES
    "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/include/big_384_58.h"
    "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/include/config_big_384_58.h"
    "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/include/config_field_BLS381.h"
    "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/include/config_curve_BLS381.h"
    "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/include/fp_BLS381.h"
    "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/include/ecdh_BLS381.h"
    "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/include/ecp_BLS381.h"
    "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/include/hash_to_field_BLS381.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/lib/libamcl_pairing_BLS381.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/amcl" TYPE FILE FILES
    "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/include/fp2_BLS381.h"
    "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/include/fp4_BLS381.h"
    "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/include/fp12_BLS381.h"
    "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/include/ecp2_BLS381.h"
    "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/include/pair_BLS381.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/amcl.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/amcl/AMCLTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/amcl/AMCLTargets.cmake"
         "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/CMakeFiles/Export/c2d8cd56cfa84a14e9895d8ff591d988/AMCLTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/amcl/AMCLTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/amcl/AMCLTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/amcl" TYPE FILE FILES "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/CMakeFiles/Export/c2d8cd56cfa84a14e9895d8ff591d988/AMCLTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/amcl" TYPE FILE FILES "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/CMakeFiles/Export/c2d8cd56cfa84a14e9895d8ff591d988/AMCLTargets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/amcl" TYPE FILE FILES
    "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/AMCLConfig.cmake"
    "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/AMCLConfigVersion.cmake"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "D:/EDU/FYP2/ecash-fyp/build/third_party/amcl/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
