#!/bin/bash

build() {
  mkdir -p build && cd build
  rm -rf *
  cmake .. && make -j12
  cd .. && rm -rf build
}

build