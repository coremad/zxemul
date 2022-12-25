CFLAGS		= -Wall -O3 \
			--target=wasm32 \
			-fuse-ld=lld \
			-Wl,--lto-O3 \
			-Wl,-z,stack-size=16777216 \
			-Wl,--no-entry \
			-nostdlib \
			-Wl,--export-all

CXXFLAGS	= $(CFLAGS) \
			-std=c++17

all:
	clang++ $(CXXFLAGS) -o public/js/zxemul.wasm src/zxemul.cpp src/z80.cpp src/z80io.cpp

clean:
	rm -f public/js/zxemul.wasm
