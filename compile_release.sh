cmake -G "Unix Makefiles" -H. -Bbuild -DCMAKE_BUILD_TYPE=Release && cmake --build build -- -j$(nproc)
