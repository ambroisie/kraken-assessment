add_executable(kraken kraken.cc)
target_link_libraries(kraken PRIVATE common_options)

install(TARGETS kraken)
