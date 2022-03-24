add_executable(kraken kraken.cc)
target_link_libraries(kraken PRIVATE common_options)

add_subdirectory(book)
add_subdirectory(csv)
add_subdirectory(engine)
add_subdirectory(parse)
add_subdirectory(utils)

configure_file(config.h.in config.h)

find_package(Boost REQUIRED COMPONENTS thread)

target_link_libraries(kraken PRIVATE
  csv
  engine
  parse
  Boost::thread
)

install(TARGETS kraken)
