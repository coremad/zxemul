CFLAGS		= \
			-I'src/include' \
			-Wall -O2 \
			-ftree-vectorize \
			-fomit-frame-pointer
#			-DDEBUG

WFLAGS		 = \
			--target=wasm32 \
			-fuse-ld=lld \
			-nostdlib \
			-fno-rtti \
			-Wl,--no-entry \
			-Wl,--export-all \
			-flto \
			-Wl,-z,stack-size=8388608 \
                        -Wl,--lto-O3 \
			-DRCOLORS \
# 			-Wl,--allow-undefined \

CXXFLAGS	= $(CFLAGS) \
			-std=c++17


SDIR 		= src
IDIR		= src/include
ODIR 		= obj
# SRCS		= $(wildcard $(SDIR)/*.cpp)
SRCS		= \
		z80io.cpp \
		zxports.cpp \
		z80.cpp \
		snapshots.cpp \
		zxemul.cpp \
		fsnapshots.cpp \
#		tape.cpp \
# 		debug.cpp

SRCS_SDL	= $(SRCS) undead-sdl.cpp
OBJS_SDL	= $(patsubst %.cpp,$(ODIR)/%.o,$(SRCS_SDL))
DEPS_SDL	= $(patsubst %.cpp,$(ODIR)/%.d,$(SRCS_SDL))

all:
	# make sdl
	# make qt
	# make web

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
	rm -f src/*.o
	rm -f $(ODIR)/*.d
	rm -f src/*.d
	rm -f undead-*

sdl:	undead-sdl

undead-sdl: $(OBJS_SDL)
	g++ $(CXXFLAGS) $^ -o undead-sdl -lSDL


-include $(OBJS:$(ODIR)/.o=$(ODIR)/.d)

$(ODIR)/%.o: $(SDIR)/%.cpp
	g++ $(CXXFLAGS) -MMD -MP -c $< -o $@

#	clang++ $(CXXFLAGS) -o undead-sdl src/zxemul.cpp src/z80.cpp src/z80io.cpp src/snapshots.cpp src/fsnapshots.cpp src/tape.cpp src/undead-sdl.cpp -lSDL
# 	c++ $(CXXFLAGS) -march=native -mtune=native -o undead-sdl $(SOURCES) -lSDL

qt:
	cd src/Qt && qmake5 && make

web:
	clang++ $(CXXFLAGS) $(WFLAGS) -o public_html/js/undead-zx.wasm src/zxemul.cpp src/z80.cpp src/z80io.cpp src/snapshots.cpp src/undead-web.cpp src/zxports.cpp
#	emcc $(CXXFLAGS) -o public_html/js/undead-zx.wasm src/zxemul.cpp src/z80.cpp src/z80io.cpp src/snapshots.cpp src/undead-web.cpp src/zxports.cpp src/tape.cpp -Wl,--export-all --no-entry

android:
	#coming soon

