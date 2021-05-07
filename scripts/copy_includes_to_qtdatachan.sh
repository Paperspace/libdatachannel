#/usr/bin/env bash
set -e

PS_QT="$(cd ${PS_QT:-$(pwd)/../../qtdatachan}; pwd)"

rm -rf ${PS_QT}/libdatachannel/include
rm -rf ${PS_QT}/libdatachannel/deps

cp -R ../../libdatachannel/include  ${PS_QT}/libdatachannel/.

mkdir -p ${PS_QT}/libdatachannel/deps/json
cp -R ../../libdatachannel/deps/json/single_include  ${PS_QT}/libdatachannel/deps/json/.

mkdir -p ${PS_QT}/libdatachannel/deps/plog
cp -R ../../libdatachannel/deps/plog/include  ${PS_QT}/libdatachannel/deps/plog/.
