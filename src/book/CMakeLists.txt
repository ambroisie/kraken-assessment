add_library(book STATIC
  order.cc
  order.hh
)

target_link_libraries(book PRIVATE
  utils
)

target_link_libraries(book PRIVATE common_options)
