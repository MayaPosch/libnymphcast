@echo off & setlocal enableextensions enabledelayedexpansion
::
:: Setup prerequisites and build LibNymphCast library (MSVC).
::
:: Created 23 January 2022.
:: Copyright (c) 2021 Nyanko.ws
::
:: Usage: Setup-NMake-vcpkg [NYMPHRPC_ROOT=path/to/lib] [POCO_ROOT=path/to/lib] [target]
::

:: Install vcpkg tool:
:: > git clone https://github.com/microsoft/vcpkg
:: > .\vcpkg\bootstrap-vcpkg.bat -disableMetrics
::

echo.

::set NC_LNKCRT=-MT

set NC_TGT_BITS=64
set NC_TGT_ARCH=x%NC_TGT_BITS%

:: Check for 64-bit Native Tools Command Prompt

if not [%VSCMD_ARG_TGT_ARCH%] == [x64] (
    echo [Make sure to run these commands in a '64-bit Native Tools Command Prompt'; expecting 'x64', got '%VSCMD_ARG_TGT_ARCH%'. Bailing out.]
    endlocal & goto :EOF
)

if [%VCPKG_ROOT%] == [] (
    echo [Make sure to environment variable 'VCPKG_ROOT' point to you vcpkg installation; it's empty or does not exist. Bailing out.]
    endlocal & goto :EOF
)

if [%NYMPHRPC_ROOT%] == [] (
    set NYMPHRPC_ROOT=D:\Libraries\NymphRPC
)

:: Make sure NymphRPC and LibNymphCast will be build with the same Poco version:

set VCPKG_TRIPLET=%NC_TGT_ARCH%-windows
::set VCPKG_TRIPLET=%NC_TGT_ARCH%-windows-static

if exist "%VCPKG_ROOT%\installed\%VCPKG_TRIPLET%\include\Poco" (
    echo Setup LibNymphCast: Poco is already installed at "%VCPKG_ROOT%\installed\%VCPKG_TRIPLET%\include\Poco".
) else (
    echo [Installing vcpkg Poco; please be patient, this may take about 10 minutes...]
    echo vcpkg install --triplet %VCPKG_TRIPLET% poco
)

echo Setup LibNymphCast: Using POCO_ROOT=%VCPKG_ROOT%\installed\%VCPKG_TRIPLET%

set POCO_ROOT=%VCPKG_ROOT%\installed\%VCPKG_TRIPLET%

:: NymphRPC - Download and build NymphRPC dependency:

if exist "%NYMPHRPC_ROOT%\include\nymph\nymph.h" (
    echo Setup LibNymphCast:: NymphRPC has been installed at "%NYMPHRPC_ROOT%".
) else (
    echo Setup LibNymphCast:: NymphRPC not found at "%NYMPHRPC_ROOT%".

    git clone --depth 1 https://github.com/MayaPosch/NymphRPC.git

    :: TODO temporary: Copy/overwrite Setup and NMakefile for NymphRPC.
    if exist "D:\Own\Martin" (
        xcopy /y ..\MartinMoene\NymphCast-Scenarios\code\NMake\NymphRPC\NMakefile NymphRPC
        xcopy /y ..\MartinMoene\NymphCast-Scenarios\code\NMake\NymphRPC\Setup-NMake-vcpkg.bat NymphRPC
    )

    cd NymphRPC & call Setup-NMake-vcpkg.bat clean all install & cd ..

    rmdir /s /q NymphRPC
)

:: Make LibNymphCast.lib and LibNymphCastmt.lib:

:: set CMD_ARGS=%*
:: if [%CMD_ARGS%] == [] (
::     set CMD_ARGS=clean all install
:: )
:: echo CMD_ARGS: '%CMD_ARGS%'

nmake -nologo -f NMakefile ^
        NC_LNKCRT=-MD ^
        POCO_ROOT=%POCO_ROOT% ^
    NYMPHRPC_ROOT=%NYMPHRPC_ROOT% ^
        clean all install %*

nmake -nologo -f NMakefile ^
        NC_LNKCRT=-MT ^
        POCO_ROOT=%POCO_ROOT% ^
    NYMPHRPC_ROOT=%NYMPHRPC_ROOT% ^
        clean all install %*

echo.

endlocal

:: End of file
