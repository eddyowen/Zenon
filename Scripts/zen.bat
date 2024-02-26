@echo off

:: Get an ESC characters
setlocal
call :setESC

echo %ESC%[36m===========================================%ESC%[0m
echo             ZN Project Gen Tool
echo %ESC%[36m===========================================%ESC%[0m

:: Build System commands handling
set "command=%~1"
if "%command%"=="gensln" goto :%command% 
if "%command%"=="opensln" goto :%command%
if "%command%"=="clean" goto :%command%
if "%command%"=="" goto :emptyCommand
if "%command%"=="--help" goto :help
goto :notRecognizedCommand

:: Help menu 
:help
    echo %ESC%[36m                  Help menu%ESC%[36m
    echo %ESC%[36m-------------------------------------------%ESC%[0m
    echo.
    echo %ESC%[36m^>%ESC%[0m Usage: zen [command]
    echo.
    echo %ESC%[36m-%ESC%[0m gensln           Generates and configures your Visual Studio solution.
    echo %ESC%[36m-%ESC%[0m opensln          Opens your Visual Studio solution.
    echo %ESC%[36m-%ESC%[0m clean            Cleans compiled data from the build directory.
    goto :eof

:: Warning message thrown due an empty command
:emptyCommand
    echo %ESC%[33mNo command was entered. Type "zen --help" for help. %ESC%[0m
    goto :eof

:: Error message thrown due an unrecognized command
:notRecognizedCommand
    echo %ESC%[31m[ERROR]: Not such an EON command. Type "zen --help" for help. %ESC%[0m
    goto :eof

:: Generates the solution files
:gensln
    echo.
    pushd ..\
    call cmake -G "Visual Studio 17 2022" -B Generated

    IF %ERRORLEVEL% == 0 (
        goto:successBuild
    ) ELSE (
        echo.
        echo %ESC%[31m[ERROR] Errors encountered while generating the solution. Check the error list above.%ESC%[0m
        popd
        goto :eof 
    )

:successBuild
    echo.
    echo %ESC%[32m[SUCCESS]: Solution successfully generated! %ESC%[0m
    popd
    goto :eof

:: Opens your solution
:opensln
    echo.
    IF EXIST "..\Generated\" (
        pushd "..\Generated\"
        IF EXIST ZenonEngine.sln (
            start ZenonEngine.sln
            echo %ESC%[32m[SUCCESS]: Openning the solution! %ESC%[0m
            popd
            goto :eof
        ) ELSE (
            echo %ESC%[31m[ERROR]: Cannot find solution files. Please, execute "zen gensln" to generate project files %ESC%[0m
            popd
            goto :eof
        )

    ) ELSE (
        echo %ESC%[31m[ERROR]: Cannot find solution files. Please, execute "zen gensln" to generate project files %ESC%[0m
        goto :eof
    )

:: Deteles your build files
:clean
    echo.
    pushd ..\
    
    IF EXIST "Build\" (
        rmdir /S /Q "Build\"
        echo.
        echo %ESC%[32m[SUCCESS]: Build files succesfully deleted %ESC%[0m
        goto :deleteGenerated
    ) ELSE (
        echo %ESC%[33m[WARNING]: Build directory not found. No files to clean %ESC%[0m
        goto :deleteGenerated
    )

    :deleteGenerated

        IF EXIST "Generated\" (
            rmdir /S /Q "Generated\"
            echo %ESC%[32m[SUCCESS]: Solution files succesfully deleted %ESC%[0m
            popd
            goto :eof
        ) ELSE (
            echo %ESC%[33m[WARNING]: Solution files directory not found. No files to clean %ESC%[0m
            popd
            goto :eof
        )

:: Generates a ESC character used to set line foreground and background colors
:setESC
    for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do (
        set ESC=%%b
        exit /B 0
    )