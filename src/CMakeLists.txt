add_executable(kraken kraken.cc)
target_link_libraries(kraken PRIVATE common_options)

add_subdirectory(csv)

target_link_libraries(kraken PRIVATE
  csv
)

install(TARGETS kraken)
