rem run from libdatachannel scripts dir

rem NOTE: REQUIRES OPENSSL 1.1.1i debug and release builds and install locations

set SAVED_PATH=%PATH%
set SAVED_OPENSSL_ROOT_DIR=%OPENSSL_ROOT_DIR%

rem build debug with openssl static debug

pushd ..\..\libdatachannel

set OPENSSL_ROOT_DIR=C:\openssl\openssl_1.1.1i-debug
set PATH=C:\openssl\openssl_1.1.1i-debug\bin;%SAVED_PATH%

rem checks:
rem dir C:\openssl\openssl_1.1.1i-debug\lib\libssl_static.lib
rem dir C:\openssl\openssl_1.1.1i-debug\lib\libcrypto_static.lib

cmake -B ..\libdatachannel_build_debug_static_openssl_debug -G "NMake Makefiles" -D OPENSSL_USE_STATIC_LIBS=TRUE -D CMAKE_BUILD_TYPE=Debug
pushd ..\libdatachannel_build_debug_static_openssl_debug
nmake
tests

rem build relsym with openssl static

popd

set OPENSSL_ROOT_DIR=C:\Program^ Files\OpenSSL
set PATH=C:\Program^ Files\OpenSSL\bin;%SAVED_PATH%

rem checks:
rem dir C:\Program^ Files\OpenSSL\lib\libssl_static.lib
rem dir C:\Program^ Files\OpenSSL\lib\libcrypto_static.lib

cmake -B ..\libdatachannel_build_relsym_static_openssl -G "NMake Makefiles" -D OPENSSL_USE_STATIC_LIBS=TRUE -D CMAKE_BUILD_TYPE=RelWithDebInfo -D CMAKE_CXX_FLAGS_RELWITHDEBINFO="/MD /O2 /Ob2 /DNDEBUG /Zi /Gy" -D CMAKE_SHARED_LINKER_FLAGS="/DEBUG /INCREMENTAL:NO /OPT:REF /OPT:ICF"
pushd ..\libdatachannel_build_relsym_static_openssl
nmake
tests

set PATH=%SAVED_PATH%
set OPENSSL_ROOT_DIR=%SAVED_OPENSSL_ROOT_DIR%

popd
popd
