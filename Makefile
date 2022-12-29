CFLAGS		= -Wall -O3 \
			--target=wasm32 \
			-fuse-ld=lld \
			-nostdlib \
			-Wl,--lto-O3 \
			-Wl,-z,stack-size=16777216 \
			-Wl,--no-entry \
			-Wl,--allow-undefined \
			-Wl,--export-all \

CXXFLAGS	= $(CFLAGS) \
			-std=c++17

all:
	clang++ $(CXXFLAGS) -o public/js/zxemul.wasm src/zxemul.cpp src/z80.cpp src/z80io.cpp src/snapshots.cpp

clean:
	rm -f public/js/zxemul.wasm
