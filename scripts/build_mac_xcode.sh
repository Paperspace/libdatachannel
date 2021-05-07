#!/usr/bin/env bash
set -e

# in libdatachannel/scripts directory

cd ../../libdatachannel

cmake -B ../libdatachannel_build_mac_xcode_static_openssl -DUSE_GNUTLS=0 -DUSE_NICE=0 -G Xcode -DOPENSSL_USE_STATIC_LIBS=TRUE -DOPENSSL_ROOT_DIR=~/github/PS_OPENSSL/release/mac111

cd ../libdatachannel_build_mac_xcode_static_openssl

# requires xcode 11.7

xcodebuild -scheme ALL_BUILD -configuration Debug
./Debug/tests

xcodebuild -scheme ALL_BUILD -configuration RelWithDebInfo
./RelWithDebInfo/tests

cd ../libdatachannel/scripts

# copy build to qt-shared
#./copy_build_mac_xcode_to_qt-shared.sh
