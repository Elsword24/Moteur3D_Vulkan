@echo off
echo ========================================
echo    Build ENGINE 3D 
echo ========================================

if not exist Build (
    echo Create Build Directory
    mkdir Build
)

echo.
echo Setting CMake...
cd Build
cmake ..
if errorlevel 1 goto error 

echo.
echo Build Release...
cmake --build . --config Release
if errorlevel 1 goto error 

echo.
echo Build Debug...
cmake --build . --config Debug
if errorlevel 1 goto error 

cd ..
echo.
echo ========================================
echo    Build completed successfully !
echo ========================================
pause
exit /b 0

:error
cd ..
echo.
echo ========================================
echo    ERROR during build !
echo ========================================
pause
exit /b 1