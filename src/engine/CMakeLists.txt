add_library(engine STATIC
  engine-listener.cc
  engine-listener.hh
)

target_link_libraries(engine PRIVATE
  book
)

target_link_libraries(engine PRIVATE common_options)
