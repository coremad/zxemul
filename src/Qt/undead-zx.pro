QT          += widgets


QMAKE_CFLAGS += $$(CFLAGS) -ftree-vectorize -fomit-frame-pointer -march=native -mtune=native
QMAKE_CXXFLAGS += $$(CXXFLAGS) -ftree-vectorize -fomit-frame-pointer -march=native -mtune=native

INCLUDEPATH += ../include

#DEFINES += "DEBUG"

HEADERS     = widget.h \
              window.h \
              ../include/zdefs.h \
              ../include/tape.h \
              ../include/zxemul.h \
              ../include/z80io.h \
              ../include/z80.h \
              ../include/snapshots.h \
#              ../include/debug.h

SOURCES     = main.cpp \
              widget.cpp \
              window.cpp \
              ../z80io.cpp \
              ../z80.cpp \
              ../zxemul.cpp \
              ../snapshots.cpp \
              ../fsnapshots.cpp \
#              ../tape.cpp \
#              ../debug.cpp \

TARGET = ../../undead-qt
