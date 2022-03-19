#!/bin/sh
set -e
rm build -r
cmake -DCMAKE_TOOLCHAIN_FILE:STRING=/home/debian/Android/Sdk/ndk/21.4.7075529/build/cmake/android.toolchain.cmake -DANDROID_ABI:STRING=arm64-v8a -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_C_COMPILER:FILEPATH=/home/debian/Android/Sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/linux-x86_64/bin/clang -DCMAKE_CXX_COMPILER:FILEPATH=/home/debian/Android/Sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ -H/home/debian/AndroidStudioProjects/MyOpenSSL/app/src/main/cpp/common-cpp -B/home/debian/AndroidStudioProjects/MyOpenSSL/app/src/main/cpp/common-cpp/build -G "Unix Makefiles"
cmake --build build --config Debug --target all -j 18 --
sudo $(which cmake) --install build