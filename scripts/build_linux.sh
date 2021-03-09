#!/usr/bin/env bash
set -e

# in libdatachannel/scripts directory

cd ../../libdatachannel
cmake -B ../libdatachannel_build_linux_debug_with_static_openssl -DUSE_GNUTLS=0 -DUSE_NICE=0 -DOPENSSL_USE_STATIC_LIBS=TRUE -DCMAKE_BUILD_TYPE=Debug
cd ../libdatachannel_build_linux_debug_with_static_openssl
make
./tests

cd ../libdatachannel
cmake -B ../libdatachannel_build_linux_relsym_with_static_openssl -DUSE_GNUTLS=0 -DUSE_NICE=0 -DOPENSSL_USE_STATIC_LIBS=TRUE -DCMAKE_BUILD_TYPE=RelWithDebInfo
cd ../libdatachannel_build_linux_relsym_with_static_openssl/
make
./tests

cd ../libdatachannel/scripts

# copy builds to qt-shared
#./copy_build_linux_to_qt-shared.sh
