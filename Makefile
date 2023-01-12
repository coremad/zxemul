CFLAGS		= \
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
			-DRCOLORS
# 			-Wl,--allow-undefined

CXXFLAGS	= $(CFLAGS) \
			-std=c++17


SDIR 		= src
IDIR		= src/include
ODIR 		= obj
# SRCS		= $(wildcard $(SDIR)/*.cpp)
SRCS		= \
		$(SDIR)/z80io.cpp \
		$(SDIR)/zxports.cpp \
		$(SDIR)/z80.cpp \
		$(SDIR)/snapshots.cpp \
		$(SDIR)/zxemul.cpp
# 		$(SDIR)/fsnapshots.cpp
# 		$(SDIR)/debug.cpp

SRCS_WEB	= $(SRCS) $(SDIR)/undead-web.cpp

SRCS_SDL	= $(SDIR)/fsnapshots.cpp $(SRCS) $(SDIR)/undead-sdl.cpp
OBJS_SDL	= $(patsubst %.cpp,$(ODIR)/%.o,$(SRCS_SDL))
DEPS_SDL	= $(patsubst %.cpp,$(ODIR)/%.d,$(SRCS_SDL))

all:
	# make sdl
	# make sdl2
	# make qt
	# make web

sdl:
	c++ $(CXXFLAGS) -march=native -mtune=native -o undead-sdl -I$(IDIR) $(SRCS_SDL) -lSDL

sdl2:
	c++ $(CXXFLAGS) -march=native -mtune=native -o undead-sdl2 -I$(IDIR) $(SRCS_SDL) -lSDL2 -DSDL2

qt:
	cd src/Qt && qmake5 && make

web:
	clang++ $(CXXFLAGS) $(WFLAGS) -o public_html/js/undead-zx.wasm -I$(IDIR) $(SRCS_WEB)
# 	emcc    $(CXXFLAGS)  -Wl,--export-all --no-entry -o public_html/js/undead-zx.wasm -I$(IDIR) $(SRCS_WEB)

android:
	#coming soon

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
	rm -f src/*.o
	rm -f $(ODIR)/*.d
	rm -f src/*.d
	rm -f undead-*
