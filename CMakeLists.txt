cmake_minimum_required(VERSION 3.10)

include("cmake/module/icu_sample_upload_config.cmake")

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/5bd822e8cb8b124c287b9e106941bf85b42c6fe2.zip"
    SHA1 "b05f03f0fe81dc05c0a493d5ec3125bd85ab7fad"
    LOCAL
)

project(icu_sample)

string(COMPARE EQUAL "${CMAKE_SYSTEM_NAME}" "WindowsStore" is_windows_store)

option(ARCHIVE_MODE "ICU data archive mode" OFF)

hunter_add_package(ICU)
find_package(ICU CONFIG REQUIRED)

if(CMAKE_CROSSCOMPILING)
  # When cross-compiling pkgdata/icupkg tools build with host toolchain
  include(hunter_experimental_add_host_project)
  hunter_experimental_add_host_project(cmake/host)

  if(WIN32)
    set(suffix .exe)
  else()
    set(suffix "")
  endif()

  set(ICU_PKGDATA_EXECUTABLE "${HUNTER_HOST_ROOT}/bin/pkgdata${suffix}")
  set(ICU_ICUPKG_EXECUTABLE "${HUNTER_HOST_ROOT}/bin/icupkg${suffix}")
else()
  # Variables ICU_*_EXECUTABLE provided by package itself
endif()

# Check imported targets {

if(ANDROID OR IOS OR is_windows_store)
  set(tu_lib "")
else()
  set(tu_lib tu)
endif()

foreach(i data i18n le lx ${tu_lib} uc)
  if(NOT TARGET ICU::${i})
    message(FATAL_ERROR "ICU::${i} target not found")
  endif()
endforeach()

# }

# Check variables {

if(NOT EXISTS "${ICU_PKGDATA_EXECUTABLE}")
  message(FATAL_ERROR "pkgdata not found")
endif()

if(NOT EXISTS "${ICU_ICUPKG_EXECUTABLE}")
  message(FATAL_ERROR "icupkg not found")
endif()

# }

add_executable(foo convsamp.cpp flagcb.c flagcb.h)
target_link_libraries(foo PUBLIC ICU::uc)

if(ARCHIVE_MODE)
  target_compile_definitions(foo PRIVATE ARCHIVE_MODE)
endif()

# Run native tests {

hunter_add_package(gauze)
find_package(gauze CONFIG REQUIRED)

enable_testing()

if(IOS AND IGNORE_IOS_TEST)
  # ignore native test
else()
  if(ARCHIVE_MODE)
    set(args $<GAUZE_RESOURCE_FILE:${ICU_DATA_FILE}>)
  else()
    set(args "")
  endif()

  gauze_add_test(NAME foo COMMAND foo ${args})

  if(WIN32 OR CYGWIN)
    set(new_path "${ICU_ROOT}/bin")
    list(APPEND new_path $ENV{PATH})

    if(WIN32)
      string(REPLACE ";" "\;" new_path "${new_path}")
    elseif(CYGWIN)
      string(REPLACE ";" ":" new_path "${new_path}")
    else()
      message(FATAL_ERROR "Unreachable")
    endif()

    set_tests_properties(
        foo PROPERTIES ENVIRONMENT "PATH=${new_path}"
    )
  endif()
endif()

# }

# Run host tools {

add_test(NAME pkgdata_help COMMAND ${ICU_PKGDATA_EXECUTABLE} --help)
set_tests_properties(pkgdata_help PROPERTIES WILL_FAIL TRUE)

add_test(NAME icupkg_help COMMAND ${ICU_ICUPKG_EXECUTABLE} --help)

# }
