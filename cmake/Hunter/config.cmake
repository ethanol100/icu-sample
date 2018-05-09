set(icu_args U_USING_ICU_NAMESPACE=ON)
if(ARCHIVE_MODE)
  list(APPEND icu_args ICU_DATA_ARCHIVE_MODE=ON)
endif()

hunter_config(
    ICU
    VERSION ${HUNTER_ICU_VERSION}
    CMAKE_ARGS ${icu_args}
)
