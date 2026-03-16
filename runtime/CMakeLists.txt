set(LIBSR_SOURCES libsr.c)

add_library(sr ${LIBSR_SOURCES})
set_target_properties(sr PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(sr PUBLIC BDWgc::gc)

file(GENERATE
    OUTPUT "$<CONFIG>/__core__.soyi"
    INPUT "__core__.soyi"
)

# Install
install(TARGETS sr DESTINATION lib)
install(FILES __core__.soyi DESTINATION include)
