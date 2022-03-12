add_library(parse STATIC
  order.cc
  order.hh
)

target_link_libraries(parse PRIVATE
  utils
)

target_link_libraries(parse PRIVATE common_options)
