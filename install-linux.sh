#!/bin/sh
set -e
rm build -r
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
-DCMAKE_BUILD_TYPE:STRING=Debug \
-DCMAKE_C_COMPILER:FILEPATH=/bin/x86_64-linux-gnu-gcc-10 \
-DCMAKE_CXX_COMPILER:FILEPATH=/bin/x86_64-linux-gnu-g++-10 \
-Bbuild \
-G "Unix Makefiles"
cmake --build build --config Debug --target all -j 18 --
sudo $(which cmake) --install build