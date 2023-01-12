#include "widget.h"
#include "window.h"

#include "zxemul.h"
#include "fsnapshots.h"
//#include "zxtape.h"
//#include "bsound.h"


#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include <QKeyEvent>

Window::Window() {
    Widget *w = new Widget(this);
    QLabel *wLabel = new QLabel(tr("F12 - reset, F3 - F8 load test SNA"));
    wLabel->setAlignment(Qt::AlignHCenter);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(w, 0, 0);
    layout->addWidget(wLabel, 1, 0);
    setLayout(layout);
    setWindowTitle(tr("Undead ZX"));

    zx48.init((std::uint32_t *)w->qi->bits());
    if (!loadROM48k("roms/48.rom")) close();
//    bsinit();
//    bsplay();

    this->layout()->setSizeConstraint(QLayout::SetFixedSize);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, w, &Widget::animate);
    timer->start(20);
}

void Window::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
    case Qt::Key_F12:
        z80.reset();
        break;
    case Qt::Key_F3:
        loadSNA48k("roms/elite.sna");
        break;
    case Qt::Key_F4:
        loadSNA48k("roms/ikari.sna");
        break;
    case Qt::Key_F5:
        loadSNA48k("roms/rraid.sna");
        break;
    case Qt::Key_F6:
        loadSNA48k("roms/deathchase.sna");
        break;
    case Qt::Key_F7:
        loadSNA48k("roms/dizzy3.sna");
        break;
    case Qt::Key_F8:
        loadSNA48k("roms/pacmania.sna");
        break;
//#ifdef DEBUG
//    case Qt::Key_F1:
//        zxtape.initTape();
//        break;
//#endif
    default:
        checkKeys(e, 1);
        break;
    }
}

void Window::keyReleaseEvent(QKeyEvent *e) {
    checkKeys(e, 0);
}

void Window::checkKeys(QKeyEvent * event, int state) {
    switch(event->key()) {
    case Qt::Key_Control: z80io.setKey(ZX_KeyRow_CShift, ZX_KeyBit_CShift, state); break;
    case Qt::Key_Z: z80io.setKey(ZX_KeyRow_Z, ZX_KeyBit_Z, state); break;
    case Qt::Key_X: z80io.setKey(ZX_KeyRow_X, ZX_KeyBit_X, state); break;
    case Qt::Key_C: z80io.setKey(ZX_KeyRow_C, ZX_KeyBit_C, state); break;
    case Qt::Key_V: z80io.setKey(ZX_KeyRow_V, ZX_KeyBit_V, state); break;

    case Qt::Key_A: z80io.setKey(ZX_KeyRow_A, ZX_KeyBit_A, state); break;
    case Qt::Key_S: z80io.setKey(ZX_KeyRow_S, ZX_KeyBit_S, state); break;
    case Qt::Key_D: z80io.setKey(ZX_KeyRow_D, ZX_KeyBit_D, state); break;
    case Qt::Key_F: z80io.setKey(ZX_KeyRow_F, ZX_KeyBit_F, state); break;
    case Qt::Key_G: z80io.setKey(ZX_KeyRow_G, ZX_KeyBit_G, state); break;

    case Qt::Key_H: z80io.setKey(ZX_KeyRow_H, ZX_KeyBit_H, state); break;
    case Qt::Key_J: z80io.setKey(ZX_KeyRow_J, ZX_KeyBit_J, state); break;
    case Qt::Key_K: z80io.setKey(ZX_KeyRow_K, ZX_KeyBit_K, state); break;
    case Qt::Key_L: z80io.setKey(ZX_KeyRow_L, ZX_KeyBit_L, state); break;
    case Qt::Key_Return: z80io.setKey(ZX_KeyRow_Enter, ZX_KeyBit_Enter, state); break;

    case Qt::Key_Q: z80io.setKey(ZX_KeyRow_Q, ZX_KeyBit_Q, state); break;
    case Qt::Key_W: z80io.setKey(ZX_KeyRow_W, ZX_KeyBit_W, state); break;
    case Qt::Key_E: z80io.setKey(ZX_KeyRow_E, ZX_KeyBit_E, state); break;
    case Qt::Key_R: z80io.setKey(ZX_KeyRow_R, ZX_KeyBit_R, state); break;
    case Qt::Key_T: z80io.setKey(ZX_KeyRow_T, ZX_KeyBit_T, state); break;

    case Qt::Key_P: z80io.setKey(ZX_KeyRow_P, ZX_KeyBit_P, state); break;
    case Qt::Key_O: z80io.setKey(ZX_KeyRow_O, ZX_KeyBit_O, state); break;
    case Qt::Key_I: z80io.setKey(ZX_KeyRow_I, ZX_KeyBit_I, state); break;
    case Qt::Key_U: z80io.setKey(ZX_KeyRow_U, ZX_KeyBit_U, state); break;
    case Qt::Key_Y: z80io.setKey(ZX_KeyRow_Y, ZX_KeyBit_Y, state); break;

    case Qt::Key_1: z80io.setKey(ZX_KeyRow_1, ZX_KeyBit_1, state); break;
    case Qt::Key_2: z80io.setKey(ZX_KeyRow_2, ZX_KeyBit_2, state); break;
    case Qt::Key_3: z80io.setKey(ZX_KeyRow_3, ZX_KeyBit_3, state); break;
    case Qt::Key_4:	z80io.setKey(ZX_KeyRow_4, ZX_KeyBit_4, state); break;
    case Qt::Key_5: z80io.setKey(ZX_KeyRow_5, ZX_KeyBit_5, state); break;

    case Qt::Key_0: z80io.setKey(ZX_KeyRow_0, ZX_KeyBit_0, state); break;
    case Qt::Key_9: z80io.setKey(ZX_KeyRow_9, ZX_KeyBit_9, state); break;
    case Qt::Key_8: z80io.setKey(ZX_KeyRow_8, ZX_KeyBit_8, state); break;
    case Qt::Key_7: z80io.setKey(ZX_KeyRow_7, ZX_KeyBit_7, state); break;
    case Qt::Key_6: z80io.setKey(ZX_KeyRow_6, ZX_KeyBit_6, state); break;

    case Qt::Key_Space: z80io.setKey(ZX_KeyRow_Space, ZX_KeyBit_Space, state); break;
    case Qt::Key_Shift: z80io.setKey(ZX_KeyRow_SShift, ZX_KeyBit_SShift, state); break;
    case Qt::Key_M: z80io.setKey(ZX_KeyRow_M, ZX_KeyBit_M, state); break;
    case Qt::Key_N: z80io.setKey(ZX_KeyRow_N, ZX_KeyBit_N, state); break;
    case Qt::Key_B: z80io.setKey(ZX_KeyRow_B, ZX_KeyBit_B, state); break;

    case Qt::Key_Right:
        z80io.setKey(ZX_KeyRow_8, ZX_KeyBit_8, state);
        z80io.setKey(ZX_KeyRow_CShift, ZX_KeyBit_CShift, state);
        break;
    case Qt::Key_Up:
        z80io.setKey(ZX_KeyRow_7, ZX_KeyBit_7, state);
        z80io.setKey(ZX_KeyRow_CShift, ZX_KeyBit_CShift, state);
        break;
    case Qt::Key_Down:
        z80io.setKey(ZX_KeyRow_6, ZX_KeyBit_6, state);
        z80io.setKey(ZX_KeyRow_CShift, ZX_KeyBit_CShift, state);
        break;
    case Qt::Key_Left:
        z80io.setKey(ZX_KeyRow_5, ZX_KeyBit_5, state);
        z80io.setKey(ZX_KeyRow_CShift, ZX_KeyBit_CShift, state);
        break;
    case Qt::Key_Backspace:
        z80io.setKey(ZX_KeyRow_0, ZX_KeyBit_0, state);
        z80io.setKey(ZX_KeyRow_CShift, ZX_KeyBit_CShift, state);
        break;
    case Qt::Key_Apostrophe:
        z80io.setKey(ZX_KeyRow_P, ZX_KeyBit_P, state);
        z80io.setKey(ZX_KeyRow_SShift, ZX_KeyBit_SShift, state);
        break;

    default:
        if (state) QWidget::keyPressEvent(event);
        else QWidget::keyReleaseEvent(event);
    }
}
