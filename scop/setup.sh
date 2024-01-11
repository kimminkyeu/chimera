@echo off

cmake -E remove_directory build/intermediate
cmake -B build/intermediate -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build "build/intermediate" -j4