#!/bin/bash

#clang --target=wasm32 \
#-Os \
#-nostdlib \
#-fuse-ld=lld \
#-Wl,--lto-O3 \
#-Wl,--export-all \
#-Wl,--no-entry \
#-Wl,-z,stack-size=$[8*1024 * 1024] \
#zxemul.c -o ../public/zxemul.wasm

#emcc fire.c -o public/fire.wasm  -O3 -Wl,--export-all --no-entry

#emcc z80.cpp -c -o z80.0 -Os -Wl,--export-all --no-entry
#em++ zxemul.cpp z80.cpp -o ../public/zxemul.wasm -s STANDALONE_WASM -O1 --no-entry \
#-s EXPORTED_FUNCTIONS='["_init"]' -s WASM=1
em++ -O1 zxemul.cpp z80.cpp -o ../public/zxemul.wasm --no-entry \
-s EXPORTED_FUNCTIONS='["_init"]' -s WASM=1
