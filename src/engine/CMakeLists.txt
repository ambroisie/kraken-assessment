add_library(engine STATIC
  csv-engine-listener.cc
  csv-engine-listener.hh
  engine-listener.cc
  engine-listener.hh
)

target_link_libraries(engine PRIVATE
  book
)

target_link_libraries(engine PRIVATE common_options)
