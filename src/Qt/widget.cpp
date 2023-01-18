#include <QPainter>
#include <QTimer>

#include "widget.h"

#include "zxemul.h"

#include "bsound.h"

Widget::Widget(QWidget *parent) : QWidget(parent) {
    setFixedSize(640, 480);
    qi = new QImage(640, 480, QImage::Format_RGB32);
}

int emul_active = 0;

void Widget::animate() {
    if(!emul_active) {
        emul_active = 1;
        zx48.emul();
        zx48.ShowZXscreen();
        emul_active = 0;
        bsplay();
    } else fprintf(stderr, "skip frame");
    update();
}

void Widget::paintEvent(QPaintEvent *) {
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawImage(QPoint(0,0), *qi);
    painter.end();
}
