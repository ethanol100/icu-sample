cmake_minimum_required(VERSION 3.10)

include("cmake/module/icu_sample_upload_config.cmake")

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/4b12860c63df810cf7fc47afa40ce10e3cebc03b.zip"
    SHA1 "b90878661a1a8ddf70df0cd97a8ff79bd7cf4a8c"
)

project(icu_sample)

hunter_add_package(ICU)
find_package(ICU CONFIG REQUIRED)

if(CMAKE_CROSSCOMPILING)
  # When cross-compiling pkgdata/icupkg tools build with host toolchain
  include(hunter_experimental_add_host_project)
  hunter_experimental_add_host_project(cmake/host)

  set(ICU_PKGDATA_EXECUTABLE "${HUNTER_HOST_ROOT}/bin/pkgdata")
  set(ICU_ICUPKG_EXECUTABLE "${HUNTER_HOST_ROOT}/bin/icupkg")
else()
  # Variables ICU_*_EXECUTABLE provided by package itself
endif()

# Check imported targets {

if(ANDROID OR IOS)
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

# Run native tests {

hunter_add_package(gauze)
find_package(gauze CONFIG REQUIRED)

enable_testing()

if(IOS AND IGNORE_IOS_TEST)
  # ignore native test
else()
  gauze_add_test(NAME foo COMMAND foo)
endif()

# }

# Run host tools {

add_test(NAME pkgdata_help COMMAND ${ICU_PKGDATA_EXECUTABLE} --help)
set_tests_properties(pkgdata_help PROPERTIES WILL_FAIL TRUE)

add_test(NAME icupkg_help COMMAND ${ICU_ICUPKG_EXECUTABLE} --help)

# }
