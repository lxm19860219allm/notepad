@echo off
setlocal enabledelayedexpansion

echo ========================================
echo    NotepadApp Builder and Deployer
echo ========================================
echo.

echo Step 1: Cleaning previous builds...
if exist build rmdir /s /q build
if exist dist rmdir /s /q dist

echo.
echo Step 2: Creating build directory...
mkdir build
cd build

echo.
echo Step 3: Configuring project...
cmake -G "MinGW Makefiles" ..
if errorlevel 1 (
    echo CMake configuration failed!
    cd ..
    pause
    exit /b 1
)

echo.
echo Step 4: Building application...
cmake --build .
if errorlevel 1 (
    echo Build failed!
    cd ..
    pause
    exit /b 1
)

echo.
echo Step 5: Creating distribution package...
cd ..

mkdir dist
mkdir dist\NotepadApp

echo Copying executable...
copy build\bin\NotepadApp.exe dist\NotepadApp\

echo Deploying Qt dependencies...
windeployqt --release dist\NotepadApp\NotepadApp.exe

echo Creating documentation...
echo NotepadApp - Tabbed Notepad Application > dist\NotepadApp\README.txt
echo. >> dist\NotepadApp\README.txt
echo Features: >> dist\NotepadApp\README.txt
echo - Multiple tab support >> dist\NotepadApp\README.txt
echo - Auto-save functionality >> dist\NotepadApp\README.txt
echo - JSON file format >> dist\NotepadApp\README.txt
echo - Modern user interface >> dist\NotepadApp\README.txt
echo. >> dist\NotepadApp\README.txt
echo Usage: >> dist\NotepadApp\README.txt
echo 1. Run NotepadApp.exe >> dist\NotepadApp\README.txt
echo 2. Use "Add Tab" to create new tabs >> dist\NotepadApp\README.txt
echo 3. Edit content in the right panel >> dist\NotepadApp\README.txt
echo 4. Use File menu to save/open files >> dist\NotepadApp\README.txt

echo Creating ZIP package...
cd dist
powershell -command "Compress-Archive -Path NotepadApp -DestinationPath NotepadApp_v1.0.0_Windows.zip -Force"

echo.
echo ========================================
echo    BUILD SUCCESSFUL!
echo ========================================
echo.
echo Distribution package created: dist\NotepadApp_v1.0.0_Windows.zip
echo.
echo To distribute:
echo 1. Send the ZIP file to users
echo 2. Users simply extract and run NotepadApp.exe
echo.
echo Testing instructions:
echo 1. Extract the ZIP on a different computer
echo 2. Run NotepadApp.exe
echo 3. Verify all features work correctly
echo.
pause