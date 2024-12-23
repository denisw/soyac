set(LIBSR_SOURCES libsr.c)

add_library(sr ${LIBSR_SOURCES})
set_target_properties(sr PROPERTIES LINKER_LANGUAGE C)

# Boehm GC
pkg_check_modules(BdwGC REQUIRED IMPORTED_TARGET bdw-gc)
target_link_libraries(sr PkgConfig::BdwGC)

# Interface files
configure_file(__core__.soyi __core__.soyi COPYONLY)

# Install
install(TARGETS sr DESTINATION lib)
install(FILES __core__.soyi DESTINATION include)
