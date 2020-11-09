#!/bin/bash

set_ld_path() {
  export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:`pwd`/lib/gtest
}
run() {
  # Connect 127.0.0.1:22007, then send 50 rows 10 times for 2 threads
  ./parallel_sender 127.0.0.1 22007 50 10 2
}

main() {
  set_ld_path
  run
}

main