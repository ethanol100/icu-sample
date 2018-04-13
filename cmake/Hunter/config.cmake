if(ARCHIVE_MODE)
  hunter_config(
      ICU
      VERSION ${HUNTER_ICU_VERSION}
      CMAKE_ARGS ICU_DATA_ARCHIVE_MODE=ON
  )
endif()
