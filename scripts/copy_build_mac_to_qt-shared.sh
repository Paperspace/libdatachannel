#/usr/bin/env bash
set -e

PS_QT="${PS_QT:-$(pwd)/../../PS_QTNativeApp}"

mkdir -p ${PS_QT}/qt-shared/libs/libdatachannel/build/mac/release
rm -f ${PS_QT}/qt-shared/libs/libdatachannel/build/linux/release/*.dylib
cp -a ../../libdatachannel_build_mac_static_openssl/RelWithDebInfo/*.dylib ${PS_QT}/qt-shared/libs/libdatachannel/build/mac/release/
install_name_tool -id @rpath/libdatachannel.0.11.4.dylib ${PS_QT}/qt-shared/libs/libdatachannel/build/mac/release/libdatachannel.0.11.4.dylib

mkdir -p ${PS_QT}/qt-shared/libs/libdatachannel/build/mac/debug
rm -f ${PS_QT}/qt-shared/libs/libdatachannel/build/linux/debug/*.dylib
cp -a ../../libdatachannel_build_mac_static_openssl/Debug/*.dylib ${PS_QT}/qt-shared/libs/libdatachannel/build/mac/debug/
install_name_tool -id @rpath/libdatachannel.0.11.4.dylib ${PS_QT}/qt-shared/libs/libdatachannel/build/mac/debug/libdatachannel.0.11.4.dylib
