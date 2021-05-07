#/usr/bin/env bash
set -e

PS_QT="$(cd ${PS_QT:-$(pwd)/../../qtdatachan}; pwd)"

mkdir -p ${PS_QT}/libdatachannel/build/mac/release
rm -f ${PS_QT}/libdatachannel/build/mac/release/*.dylib
cp -a ../../libdatachannel_build_mac_static_openssl/*.dylib ${PS_QT}/libdatachannel/build/mac/release/
install_name_tool -id @rpath/libdatachannel.0.11.4.dylib ${PS_QT}/libdatachannel/build/mac/release/libdatachannel.0.11.4.dylib

mkdir -p ${PS_QT}/libdatachannel/build/mac/debug
rm -f ${PS_QT}/libdatachannel/build/mac/debug/*.dylib
cp -a ../../libdatachannel_build_mac_debug_static_openssl_debug/*.dylib ${PS_QT}/libdatachannel/build/mac/debug/
install_name_tool -id @rpath/libdatachannel.0.11.4.dylib ${PS_QT}/libdatachannel/build/mac/debug/libdatachannel.0.11.4.dylib
