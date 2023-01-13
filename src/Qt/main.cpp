#include <iostream>

#include "window.h"
#include "jevents.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QSurfaceFormat fmt;
    fmt.setSamples(4);
    QSurfaceFormat::setDefaultFormat(fmt);
    JEvents jevents;

    Window window;
    window.show();
    return app.exec();
}

