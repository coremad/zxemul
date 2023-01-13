QT          += widgets multimedia gamepad

QMAKE_CFLAGS += $$(CFLAGS) -ftree-vectorize -fomit-frame-pointer -march=native -mtune=native
QMAKE_CXXFLAGS += $$(CXXFLAGS) -ftree-vectorize -fomit-frame-pointer -march=native -mtune=native -g

INCLUDEPATH += ../include

#DEFINES += DEBUG

HEADERS     = widget.h \
              window.h \
              jevents.h \
              ../include/zdefs.h \
              ../include/z80io.h \
              ../include/z80.h \
              ../include/zxemul.h \
              ../include/snapshots.h \
              ../include/debug.h \
              ../include/zxports.h \
              ../include/zxkempston.h \
              ../include/zxkeyboard.h \
              ../include/wmem.h \
              ../include/zxborder.h \
              ../include/zxkeyboardp.h \
#              ../include/zxtape.h \
#              ../include/zxbeeper.h \

SOURCES     = main.cpp \
              widget.cpp \
              window.cpp \
              jevents.cpp \
              ../z80io.cpp \
              ../z80.cpp \
              ../zxemul.cpp \
              ../snapshots.cpp \
              ../fsnapshots.cpp \
              ../zxports.cpp \
#              bsound.cpp \
#              ../debug.cpp \

TARGET = ../../undead-qt
