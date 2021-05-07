#/usr/bin/env bash
set -e

PS_QT="$(cd ${PS_QT:-$(pwd)/../../PS_QTNativeApp}; pwd)"

rm -rf ${PS_QT}/qt-shared/libs/libdatachannel/include
rm -rf ${PS_QT}/qt-shared/libs/libdatachannel/deps

cp -R ../../libdatachannel/include  ${PS_QT}/qt-shared/libs/libdatachannel/.

mkdir -p ${PS_QT}/qt-shared/libs/libdatachannel/deps/json
cp -R ../../libdatachannel/deps/json/single_include  ${PS_QT}/qt-shared/libs/libdatachannel/deps/json/.

mkdir -p ${PS_QT}/qt-shared/libs/libdatachannel/deps/plog
cp -R ../../libdatachannel/deps/plog/include  ${PS_QT}/qt-shared/libs/libdatachannel/deps/plog/.
