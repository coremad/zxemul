#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class Window : public QWidget {
    Q_OBJECT

public:
    Window();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void checkKeys(QKeyEvent * event, int state);
};

#endif
