cmake -G "Unix Makefiles" -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug && cmake --build build -- -j$(nproc)
