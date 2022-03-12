add_executable(kraken kraken.cc)
target_link_libraries(kraken PRIVATE common_options)

add_subdirectory(csv)
add_subdirectory(parse)
add_subdirectory(utils)

configure_file(config.h.in config.h)

target_link_libraries(kraken PRIVATE
  csv
  parse
)

install(TARGETS kraken)
