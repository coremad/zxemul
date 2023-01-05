#include "widget.h"
#include "window.h"

#include "zxemul.h"
#include "fsnapshots.h"

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
    case Qt::Key_Control:
            if (state)
                z80io.ZXKeyboard.kfe &= (~0x1);
            else
                z80io.ZXKeyboard.kfe |= (0x1);
        break;
    case Qt::Key_Z:
            if (state)
                z80io.ZXKeyboard.kfe &= (~0x2);
            else
                z80io.ZXKeyboard.kfe |= (0x2);
        break;
    case Qt::Key_X:
            if (state)
                z80io.ZXKeyboard.kfe &= (~0x4);
            else
                z80io.ZXKeyboard.kfe |= (0x4);
        break;
    case Qt::Key_C:
            if (state)
                z80io.ZXKeyboard.kfe &= (~0x8);
            else
                z80io.ZXKeyboard.kfe |= (0x8);
        break;
    case Qt::Key_V:
            if (state)
                z80io.ZXKeyboard.kfe &= (~0x10);
            else
                z80io.ZXKeyboard.kfe |= (0x10);
        break;

    case Qt::Key_A:
            if (state)
                z80io.ZXKeyboard.kfd &= (~0x1);
            else
                z80io.ZXKeyboard.kfd |= (0x1);
        break;
    case Qt::Key_S:
            if (state)
                z80io.ZXKeyboard.kfd &= (~0x2);
            else
                z80io.ZXKeyboard.kfd |= (0x2);
        break;
    case Qt::Key_D:
            if (state)
                z80io.ZXKeyboard.kfd &= (~0x4);
            else
                z80io.ZXKeyboard.kfd |= (0x4);
        break;
    case Qt::Key_F:
            if (state)
                z80io.ZXKeyboard.kfd &= (~0x8);
            else
                z80io.ZXKeyboard.kfd |= (0x8);
        break;
    case Qt::Key_G:
            if (state)
                z80io.ZXKeyboard.kfd &= (~0x10);
            else
                z80io.ZXKeyboard.kfd |= (0x10);
        break;

    case Qt::Key_H:
            if (state)
                z80io.ZXKeyboard.kbf &= (~0x10);
            else
                z80io.ZXKeyboard.kbf |= (0x10);
        break;
    case Qt::Key_J:
            if (state)
                z80io.ZXKeyboard.kbf &= (~0x8);
            else
                z80io.ZXKeyboard.kbf |= (0x8);
        break;
    case Qt::Key_K:
            if (state)
                z80io.ZXKeyboard.kbf &= (~0x4);
            else
                z80io.ZXKeyboard.kbf |= (0x4);
        break;
    case Qt::Key_L:
            if (state)
                z80io.ZXKeyboard.kbf &= (~0x2);
            else
                z80io.ZXKeyboard.kbf |= (0x2);
        break;
    case Qt::Key_Return:
            if (state)
                z80io.ZXKeyboard.kbf &= (~0x1);
            else
                z80io.ZXKeyboard.kbf |= (0x1);
        break;

    case Qt::Key_Q:
            if (state)
                z80io.ZXKeyboard.kfb &= (~0x1);
            else
                z80io.ZXKeyboard.kfb |= (0x1);
        break;
    case Qt::Key_W:
            if (state)
                z80io.ZXKeyboard.kfb &= (~0x2);
            else
                z80io.ZXKeyboard.kfb |= (0x2);
        break;
    case Qt::Key_E:
            if (state)
                z80io.ZXKeyboard.kfb &= (~0x4);
            else
                z80io.ZXKeyboard.kfb |= (0x4);
        break;
    case Qt::Key_R:
            if (state)
                z80io.ZXKeyboard.kfb &= (~0x8);
            else
                z80io.ZXKeyboard.kfb |= (0x8);
        break;
    case Qt::Key_T:
            if (state)
                z80io.ZXKeyboard.kfb &= (~0x10);
            else
                z80io.ZXKeyboard.kfb |= (0x10);
        break;

    case Qt::Key_P:
            if (state)
                z80io.ZXKeyboard.kdf &= (~0x1);
            else
                z80io.ZXKeyboard.kdf |= (0x1);
        break;
    case Qt::Key_O:
            if (state)
                z80io.ZXKeyboard.kdf &= (~0x2);
            else
                z80io.ZXKeyboard.kdf |= (0x2);
        break;
    case Qt::Key_I:
            if (state)
                z80io.ZXKeyboard.kdf &= (~0x4);
            else
                z80io.ZXKeyboard.kdf |= (0x4);
        break;
    case Qt::Key_U:
            if (state)
                z80io.ZXKeyboard.kdf &= (~0x8);
            else
                z80io.ZXKeyboard.kdf |= (0x8);
        break;
    case Qt::Key_Y:
            if (state)
                z80io.ZXKeyboard.kdf &= (~0x10);
            else
                z80io.ZXKeyboard.kdf |= (0x10);
        break;

    case Qt::Key_1:
            if (state)
                z80io.ZXKeyboard.kf7 &= (~0x1);
            else
                z80io.ZXKeyboard.kf7 |= (0x1);
        break;
    case Qt::Key_2:
            if (state)
                z80io.ZXKeyboard.kf7 &= (~0x2);
            else
                z80io.ZXKeyboard.kf7 |= (0x2);
        break;
    case Qt::Key_3:
            if (state)
                z80io.ZXKeyboard.kf7 &= (~0x4);
            else
                z80io.ZXKeyboard.kf7 |= (0x4);
        break;
    case Qt::Key_4:	//z80.debug=1;
            if (state)
                z80io.ZXKeyboard.kf7 &= (~0x8);
            else
                z80io.ZXKeyboard.kf7 |= (0x8);
        break;
    case Qt::Key_5:
            if (state)
                z80io.ZXKeyboard.kf7 &= (~0x10);
            else
                z80io.ZXKeyboard.kf7 |= (0x10);
        break;

    case Qt::Key_0:
            if (state)
                z80io.ZXKeyboard.kef &= (~0x1);
            else
                z80io.ZXKeyboard.kef |= (0x1);
        break;
    case Qt::Key_9:
            if (state)
                z80io.ZXKeyboard.kef &= (~0x2);
            else
                z80io.ZXKeyboard.kef |= (0x2);
        break;
    case Qt::Key_8:
            if (state)
                z80io.ZXKeyboard.kef &= (~0x4);
            else
                z80io.ZXKeyboard.kef |= (0x4);
        break;
    case Qt::Key_7:
            if (state)
                z80io.ZXKeyboard.kef &= (~0x8);
            else
                z80io.ZXKeyboard.kef |= (0x8);
        break;
    case Qt::Key_6:
            if (state)
                z80io.ZXKeyboard.kef &= (~0x10);
            else
                z80io.ZXKeyboard.kef |= (0x10);
        break;

    case Qt::Key_Space:
            if (state)
                z80io.ZXKeyboard.k7f &= (~0x1);
            else
                z80io.ZXKeyboard.k7f |= (0x1);
        break;
    case Qt::Key_Shift:
            if (state)
                z80io.ZXKeyboard.k7f &= (~0x2);
            else
                z80io.ZXKeyboard.k7f |= (0x2);
        break;
    case Qt::Key_M:
            if (state)
                z80io.ZXKeyboard.k7f &= (~0x4);
            else
                z80io.ZXKeyboard.k7f |= (0x4);
        break;
    case Qt::Key_N:
            if (state)
                z80io.ZXKeyboard.k7f &= (~0x8);
            else
                z80io.ZXKeyboard.k7f |= (0x8);
        break;
    case Qt::Key_B:
            if (state)
                z80io.ZXKeyboard.k7f &= (~0x10);
            else
                z80io.ZXKeyboard.k7f |= (0x10);
        break;
    case Qt::Key_Right: //right:
            if (state) {
                z80io.ZXKeyboard.kef &= (~0x4);
                z80io.ZXKeyboard.kfe &= (~0x1);
            } else {
                z80io.ZXKeyboard.kef |= (0x4);
                z80io.ZXKeyboard.kfe |= (0x1);
            }
        break;
    case Qt::Key_Up: //up:
            if (state) {
                z80io.ZXKeyboard.kef &= (~0x8);
                z80io.ZXKeyboard.kfe &= (~0x1);
            } else {
                z80io.ZXKeyboard.kef |= (0x8);
                z80io.ZXKeyboard.kfe |= (0x1);
            }
        break;
    case Qt::Key_Down: //down:
            if (state) {
                z80io.ZXKeyboard.kef &= (~0x10);
                z80io.ZXKeyboard.kfe &= (~0x1);
            } else {
                z80io.ZXKeyboard.kef |= (0x10);
                z80io.ZXKeyboard.kfe |= (0x1);
            }
        break;
    case Qt::Key_Left: //left:
            if (state) {
                z80io.ZXKeyboard.kf7 &= (~0x10);
                z80io.ZXKeyboard.kfe &= (~0x1);
            } else {
                z80io.ZXKeyboard.kf7 |= (0x10);
                z80io.ZXKeyboard.kfe |= (0x1);
            }
        break;
    case Qt::Key_Backspace: //backspace:
            if (state) {
                z80io.ZXKeyboard.kef &= (~0x1);
                z80io.ZXKeyboard.kfe &= (~0x1);

            } else {
                z80io.ZXKeyboard.kef |= (0x1);
                z80io.ZXKeyboard.kfe |= (0x1);
            }
        break;
    case Qt::Key_Apostrophe:
            if (state) {
                z80io.ZXKeyboard.kdf &= (~0x1);
                z80io.ZXKeyboard.k7f &= (~0x2);
            } else {
                z80io.ZXKeyboard.kdf |= (0x1);
                z80io.ZXKeyboard.k7f |= (0x2);
            }
        break;
    default:
        if (state) QWidget::keyPressEvent(event);
        else QWidget::keyReleaseEvent(event);
    }
}
