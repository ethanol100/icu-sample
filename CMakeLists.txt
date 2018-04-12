cmake_minimum_required(VERSION 3.10)

include("cmake/module/icu_sample_upload_config.cmake")

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/943d8317e7435e5e3cb5bb958943f6288f8616c5.zip"
    SHA1 "090cd7c73462828853c9fa8b9be7a3ef5d15daf3"
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
