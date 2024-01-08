#! /bin/bash
rm -rf "$PWD/engine/build/intermediate"
cmake -S "$PWD/engine" -B "$PWD/engine/build/intermediate" -DBUILD_SHARED_LIBS=ON -DCMAKE_VERBOSE_MAKEFILE=TRUE -DCMAKE_BUILD_TYPE=Debug
cmake --build "$PWD/engine/build/intermediate" -j4