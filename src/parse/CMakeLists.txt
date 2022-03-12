add_library(parse STATIC
  order.cc
  order.hh
  parse.cc
  parse.hh
)

target_link_libraries(parse PRIVATE
  csv
  utils
)

target_link_libraries(parse PRIVATE common_options)
