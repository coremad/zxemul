CFLAGS		= \
			-I'src/include' \
			-Wall -O3 \
			-ftree-vectorize

WFLAGS		 = \
			--target=wasm32 \
			-fuse-ld=lld \
			-nostdlib \
			-Wl,--no-entry \
			-Wl,--allow-undefined \
			-Wl,--export-all \
			-flto \
			-Wl,-z,stack-size=8388608 \
			-Wl,--lto-O3

CXXFLAGS	= $(CFLAGS) \
			-std=c++17

web:
	clang++ $(CXXFLAGS) $(WFLAGS) -o public_html/js/undead-zx.wasm src/zxemul.cpp src/z80.cpp src/z80io.cpp src/snapshots.cpp src/undead-web.cpp -DRCOLORS
#	emcc $(CXXFLAGS) -o public_html/js/undead-zx.wasm src/zxemul.cpp src/z80.cpp src/z80io.cpp src/snapshots.cpp src/undead-web.cpp -Wl,--export-all --no-entry

sdl:
	clang++ $(CXXFLAGS) -o undead-sdl src/zxemul.cpp src/z80.cpp src/z80io.cpp src/snapshots.cpp src/fsnapshots.cpp src/undead-sdl.cpp -lSDL

qt:
	cd src/Qt && qmake5 && make

android:
	#coming soon
