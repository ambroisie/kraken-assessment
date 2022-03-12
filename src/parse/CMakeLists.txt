add_library(parse STATIC
  parse.cc
  parse.hh
)

target_link_libraries(parse PRIVATE
  book
  csv
  utils
)

target_link_libraries(parse PRIVATE common_options)
