#!/usr/bin/env bash
set -e

# in libdatachannel/scripts directory

cd ../../libdatachannel

cmake -B ../libdatachannel_build_mac_debug_static_openssl_debug -DUSE_GNUTLS=0 -DUSE_NICE=0 -DOPENSSL_USE_STATIC_LIBS=TRUE -DOPENSSL_ROOT_DIR=~/github/PS_OPENSSL/debug/mac111 -DCMAKE_BUILD_TYPE=Debug

cd ../libdatachannel_build_mac_debug_static_openssl_debug

make
./tests

cd ../libdatachannel/scripts

cd ../../libdatachannel

cmake -B ../libdatachannel_build_mac_static_openssl -DUSE_GNUTLS=0 -DUSE_NICE=0 -DOPENSSL_USE_STATIC_LIBS=TRUE -DOPENSSL_ROOT_DIR=~/github/PS_OPENSSL/release/mac111 -DCMAKE_BUILD_TYPE=RelWithDebInfo

cd ../libdatachannel_build_mac_static_openssl/
make
./tests

cd ../libdatachannel/scripts

# copy build to qt-shared
#./copy_build_mac_to_qt-shared.sh
