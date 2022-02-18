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

set INSTALL_PREFIX=D:\Libraries\LibNymphCast

:: Note: static building does not yet work.
set NC_STATIC=0
:: set NC_STATIC=1

set NC_CONFIG=Release
:: set NC_CONFIG=Debug

set NC_TGT_BITS=64
set NC_TGT_ARCH=x%NC_TGT_BITS%

set NC_LNKCRT=-MD
set VCPKG_TRIPLET=x64-windows

if [%NC_STATIC%] == [1] (
    set NC_LNKCRT=-MT
    set VCPKG_TRIPLET=x64-windows-static
    echo [Setup LibNymphCast: static build does not yet work. Continuing.]
)

:: Check for 64-bit Native Tools Command Prompt

if not [%VSCMD_ARG_TGT_ARCH%] == [%NC_TGT_ARCH%] (
    echo [Setup LibNymphCast: Make sure to run these commands in a '64-bit Native Tools Command Prompt'; expecting 'x64', got '%VSCMD_ARG_TGT_ARCH%'. Bailing out.]
    endlocal & goto :EOF
)

if [%VCPKG_ROOT%] == [] (
    echo [Setup LibNymphCast: Make sure environment variable 'VCPKG_ROOT' points to your vcpkg installation; it's empty or does not exist. Bailing out.]
    endlocal & goto :EOF
)

if [%NYMPHRPC_ROOT%] == [] (
    set NYMPHRPC_ROOT=D:\Libraries\NymphRPC
)

:: Make sure NymphRPC and LibNymphCast will be build with the same Poco version:

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
    echo Setup LibNymphCast: NymphRPC has been installed at "%NYMPHRPC_ROOT%".
) else (
    echo Setup LibNymphCast: NymphRPC not found at "%NYMPHRPC_ROOT%".

    git clone --depth 1 https://github.com/MayaPosch/NymphRPC.git

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
        NC_STATIC=%NC_STATIC% ^
        NC_LNKCRT=-MD ^
        NC_CONFIG=%NC_CONFIG% ^
        POCO_ROOT=%POCO_ROOT% ^
    NYMPHRPC_ROOT=%NYMPHRPC_ROOT% ^
   INSTALL_PREFIX=%INSTALL_PREFIX% ^
        clean all install %*

nmake -nologo -f NMakefile ^
        NC_STATIC=%NC_STATIC% ^
        NC_LNKCRT=-MT ^
        NC_CONFIG=%NC_CONFIG% ^
        POCO_ROOT=%POCO_ROOT% ^
    NYMPHRPC_ROOT=%NYMPHRPC_ROOT% ^
   INSTALL_PREFIX=%INSTALL_PREFIX% ^
        clean all install %*

echo.

endlocal

:: End of file
