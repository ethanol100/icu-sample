cmake_minimum_required(VERSION 3.10)

option(HUNTER_STATUS_DEBUG "Print debug info" ON)

include("${CMAKE_CURRENT_LIST_DIR}/../HunterGate.cmake")

HunterGate(
    URL "${HUNTER_HOST_URL}"
    SHA1 "${HUNTER_HOST_SHA1}"
)

project(SampleHost)

hunter_add_package(ICU)
