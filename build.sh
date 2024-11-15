#!/bin/bash

if [ ! -d "release-build" ]; then
    mkdir release-build
    cmake . -DCMAKE_BUILD_TYPE=Release -B release-build
fi
make -C release-build -j8