#ifndef JEVENTS_H
#define JEVENTS_H

#include <QtCore/QObject>

QT_BEGIN_NAMESPACE
class QGamepad;
QT_END_NAMESPACE

class JEvents : public QObject
{
    Q_OBJECT
public:
    explicit JEvents(QObject *parent = 0);
    ~JEvents();

private:
    QGamepad * gamepad;
};

#endif // JEVENTS_H
