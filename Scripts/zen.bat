@echo off
setlocal

:: Enable ANSI escape support
>nul 2>&1 reg query "HKCU\Console" || reg add "HKCU\Console"
reg add "HKCU\Console" /v VirtualTerminalLevel /t REG_DWORD /d 1 /f >nul

call :setESC

echo %ESC%[36m===========================================%ESC%[0m
echo          Zenon Gengine Build Tool
echo %ESC%[36m===========================================%ESC%[0m

:: Handle commands
set "command=%~1"
if "%command%"=="gensln" goto :%command% 
if "%command%"=="opensln" goto :%command%
if "%command%"=="clean" goto :%command%
if "%command%"=="" goto :emptyCommand
if "%command%"=="--help" goto :help
goto :notRecognizedCommand

:help
    echo %ESC%[36m                Help menu%ESC%[36m
    echo %ESC%[36m-------------------------------------------%ESC%[0m
    echo.
    echo %ESC%[36m^>%ESC%[0m Usage: zen [command]
    echo.
    echo %ESC%[36m-%ESC%[0m gensln           Generates and configures your Visual Studio solution.
    echo %ESC%[36m-%ESC%[0m opensln          Opens your Visual Studio solution.
    echo %ESC%[36m-%ESC%[0m clean            Cleans compiled data from the build directory.
    goto :eof

:emptyCommand
    echo %ESC%[33mNo command was entered. Type "zen --help" for help.%ESC%[0m
    goto :eof

:notRecognizedCommand
    echo %ESC%[31m[ERROR]: Not a recognized ZN command. Type "zen --help" for help.%ESC%[0m
    goto :eof

:gensln
    echo.
    pushd "..\"
    call cmake -G "Visual Studio 17 2022" -B Generated -DCMAKE_SUPPRESS_DEVELOPER_WARNINGS=ON

    if %ERRORLEVEL% == 0 (
        echo.
        echo %ESC%[32m[SUCCESS]: Solution successfully generated!%ESC%[0m
    ) else (
        echo.
        echo %ESC%[31m[ERROR]: Errors encountered while generating the solution. Check the output above.%ESC%[0m
    )
    popd
    goto :eof

:opensln
    echo.
    if exist "..\Generated\" (
        pushd "..\Generated\"
        if exist "ZenonEngine.sln" (
            start "" "ZenonEngine.sln"
            echo %ESC%[32m[SUCCESS]: Opening the solution!%ESC%[0m
        ) else (
            echo %ESC%[31m[ERROR]: Cannot find solution file. Use "zen gensln" first.%ESC%[0m
        )
        popd
    ) else (
        echo %ESC%[31m[ERROR]: Cannot find solution directory. Use "zen gensln" first.%ESC%[0m
    )
    goto :eof

:clean
    echo.
    pushd "..\"
    
    if exist "Build\" (
        rmdir /S /Q "Build\"
        echo %ESC%[32m[SUCCESS]: Build files successfully deleted.%ESC%[0m
    ) else (
        echo %ESC%[33m[WARNING]: Build directory not found.%ESC%[0m
    )

    if exist "Generated\" (
        rmdir /S /Q "Generated\"
        echo %ESC%[32m[SUCCESS]: Solution files successfully deleted.%ESC%[0m
    ) else (
        echo %ESC%[33m[WARNING]: Solution files directory not found.%ESC%[0m
    )
    
    popd
    goto :eof

:setESC
    for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do (
        set ESC=%%b
    )
    exit /B 0
