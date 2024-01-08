@echo off

cmake -E remove_directory build/intermediate
cmake -B build/intermediate -DBUILD_SHARED_LIBS=ON -DCMAKE_VERBOSE_MAKEFILE:BOOL=TRUE -DCHIMERA_ENGINE_DIR:PATH=../engine
cmake --build build/intermediate -j4
cmake -E copy ../engine/binary/Debug/ChimeraEngineCore.dll out/Debug/ChimeraEngineCore.dll