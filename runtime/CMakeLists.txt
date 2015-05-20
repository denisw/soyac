set (LIBSR_SOURCES
  libsr.c
)

add_library (sr SHARED ${LIBSR_SOURCES})
install (TARGETS sr DESTINATION lib)
install (FILES __core__.soyi DESTINATION include)
