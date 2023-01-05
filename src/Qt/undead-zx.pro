QT          += widgets

INCLUDEPATH += ../include

HEADERS     = widget.h \
              window.h \

SOURCES     = main.cpp \
              widget.cpp \
              window.cpp \
              ../z80io.cpp \
              ../z80.cpp \
              ../zxemul.cpp \
              ../snapshots.cpp \
              ../fsnapshots.cpp

TARGET = ../../undead-qt
