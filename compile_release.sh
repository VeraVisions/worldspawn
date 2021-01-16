#!/bin/sh

if ! [ -x "$(command -v nproc)" ]; then
	BUILD_PROC=1
else
	BUILD_PROC=$(nproc)
fi

cmake -G "Unix Makefiles" -H. -Bbuild -DCMAKE_BUILD_TYPE=Release && cmake --build build -- -j $BUILD_PROC
