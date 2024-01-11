@echo off

if not exist "%~dp0/setup.bat" goto Error_BatchFileInWrongLocation
cmake -E remove_directory build/intermediate
cmake -B build/intermediate -DBUILD_SHARED_LIBS=ON -DCMAKE_VERBOSE_MAKEFILE:BOOL=TRUE
cmake --build build/intermediate -j4
exit /B %ERRORLEVEL%

:Error_BatchFileInWrongLocation
echo setup ERROR: The batch file does not appear to be located in the root scop directory.  This script must be run from within that directory.
pause
exit /B 1