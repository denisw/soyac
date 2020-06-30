set(LIBSR_SOURCES libsr.c)

add_library(sr SHARED ${LIBSR_SOURCES})
set_target_properties(sr PROPERTIES LINKER_LANGUAGE C)

# Boehm GC
pkg_check_modules(BdwGC REQUIRED IMPORTED_TARGET bdw-gc)
target_link_libraries(sr PkgConfig::BdwGC)

# Install
install(TARGETS sr DESTINATION lib)
install(FILES __core__.soyi DESTINATION include)
