#/usr/bin/env bash
PS_QT="${PS_QT:-/mnt/c/ps}"
mkdir -p ${PS_QT}/qt-shared/libs/libdatachannel/build/linux/release
cp ../../libdatachannel_build_linux_relsym_with_static_openssl/libdatachannel.so.0.11.4 ${PS_QT}/qt-shared/libs/libdatachannel/build/linux/release/
rm -f ${PS_QT}/qt-shared/libs/libdatachannel/build/linux/release/libdatachannel.so
ln -rs ${PS_QT}/qt-shared/libs/libdatachannel/build/linux/release/libdatachannel.so.0.11.4 ${PS_QT}/qt-shared/libs/libdatachannel/build/linux/release/libdatachannel.so
mkdir -p ${PS_QT}/qt-shared/libs/libdatachannel/build/linux/debug
cp ../../libdatachannel_build_linux_debug_with_static_openssl/libdatachannel.so.0.11.4 ${PS_QT}/qt-shared/libs/libdatachannel/build/linux/debug/
rm -f ${PS_QT}/qt-shared/libs/libdatachannel/build/linux/debug/libdatachannel.so
ln -rs ${PS_QT}/qt-shared/libs/libdatachannel/build/linux/debug/libdatachannel.so.0.11.4 ${PS_QT}/qt-shared/libs/libdatachannel/build/linux/debug/libdatachannel.so
