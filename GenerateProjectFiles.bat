@echo off

if not exist "%~dp0Engine/CMakeLists.txt" goto Error_BatchFileInWrongLocation
cmake -S "%~dp0Engine" -B "%~dp0Engine/build/intermediate" -DBUILD_SHARED_LIBS=ON -G "MinGW Makefiles" -DCMAKE_VERBOSE_MAKEFILE:BOOL=TRUE
cmake --build "%~dp0Engine/build/intermediate"
exit /B %ERRORLEVEL%

:Error_BatchFileInWrongLocation
echo GenerateProjectFiles ERROR: The batch file does not appear to be located in the root Chimera directory.  This script must be run from within that directory.
pause
exit /B 1