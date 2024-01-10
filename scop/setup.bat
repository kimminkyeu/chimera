@echo off

cmake -E remove_directory Intermediate
cmake -B Intermediate -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build Intermediate -j4