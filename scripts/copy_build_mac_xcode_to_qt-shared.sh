#/usr/bin/env bash
set -e

PS_QT="$(cd ${PS_QT:-$(pwd)/../../PS_QTNativeApp}; pwd)"

mkdir -p ${PS_QT}/qt-shared/libs/libdatachannel/build/mac/release_xcode
rm -f ${PS_QT}/qt-shared/libs/libdatachannel/build/mac/release_xcode/*.dylib
cp -a ../../libdatachannel_build_mac_xcode_static_openssl/RelWithDebInfo/*.dylib ${PS_QT}/qt-shared/libs/libdatachannel/build/mac/release_xcode/
install_name_tool -id @rpath/libdatachannel.0.11.4.dylib ${PS_QT}/qt-shared/libs/libdatachannel/build/mac/release_xcode/libdatachannel.0.11.4.dylib

mkdir -p ${PS_QT}/qt-shared/libs/libdatachannel/build/mac/debug_xcode
rm -f ${PS_QT}/qt-shared/libs/libdatachannel/build/mac/debug_xcode/*.dylib
cp -a ../../libdatachannel_build_mac_xcode_static_openssl/Debug/*.dylib ${PS_QT}/qt-shared/libs/libdatachannel/build/mac/debug_xcode/
install_name_tool -id @rpath/libdatachannel.0.11.4.dylib ${PS_QT}/qt-shared/libs/libdatachannel/build/mac/debug_xcode/libdatachannel.0.11.4.dylib
