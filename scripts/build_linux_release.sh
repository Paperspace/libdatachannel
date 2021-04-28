#!/usr/bin/env bash
set -e

# in libdatachannel/scripts directory


cd ..
OPENSSL_ROOT_DIR=/home/paperspace/projects/PS_OPENSSL/release/linux111u18/ cmake -DUSE_GNUTLS=0 -DUSE_NICE=0 -DOPENSSL_USE_STATIC_LIBS=TRUE -DCMAKE_BUILD_TYPE=RelWithDebInfo
make
