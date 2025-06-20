@echo off
echo Compiling Hospital Management System...

REM Check if an existing executable exists and delete it
if exist hospital.exe (
    echo Removing old executable...
    del hospital.exe
)

REM Check if GCC is installed and in the PATH
where gcc >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo GCC compiler not found!
    echo Please install a C compiler such as MinGW or GCC before compiling.
    echo You can download MinGW from: https://sourceforge.net/projects/mingw/
    echo After installation, add the bin directory to your PATH environment variable.
    pause
    exit /b 1
)

REM Compile all source files
echo Compiling source files...
gcc -Wall -Wextra -o hospital main.c register.c login.c admin_login.c admin.c patient.c doctor.c staff.c opd.c pharmacy.c equipment.c bloodbank.c billing.c

REM Check if compilation was successful
if %ERRORLEVEL% NEQ 0 (
    echo Compilation failed!
    pause
    exit /b 1
)

echo.
echo Compilation successful! The executable hospital.exe has been created.
echo To run the program, type: hospital.exe
echo.

REM Ask if the user wants to run the program now
set /p run_now="Do you want to run the program now? (Y/N): "
if /i "%run_now%"=="Y" (
    echo Starting Hospital Management System...
    hospital.exe
)

pause
