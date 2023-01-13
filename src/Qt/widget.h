#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class Widget : public QWidget {
    Q_OBJECT

public:
    Widget(QWidget *parent);
    QImage * qi;

public slots:
    void animate();

protected:
    void paintEvent(QPaintEvent *event) override;

};

#endif
