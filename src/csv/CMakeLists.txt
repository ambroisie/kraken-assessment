add_library(csv STATIC
  csv.hh
  read-csv.cc
  read-csv.hh
  write-csv.cc
  write-csv.hh
)
target_link_libraries(csv PRIVATE common_options)
