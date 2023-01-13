#include "jevents.h"
#include <QtGamepad/QGamepad>

#include "zxemul.h"
#include "zxkempston.h"

JEvents::JEvents(QObject *parent) : QObject(parent), gamepad(0)
{
    auto gamepads = QGamepadManager::instance()->connectedGamepads();
    if (gamepads.isEmpty()) {
        return;
    }
    gamepad = new QGamepad(*gamepads.begin(), this);

    connect(gamepad, &QGamepad::axisLeftXChanged, this, [](double value) {
        if (value > 0.9) {
            z80io.setKempston(kLeft, 1);
            z80io.setKempston(kRigth, 0);
        } else if (value < -0.9) {
            z80io.setKempston(kRigth, 1);
            z80io.setKempston(kLeft, 0);
        } //else z80io.setKempston(kLeft | kRigth, 0);
    });
    connect(gamepad, &QGamepad::axisLeftYChanged, this, [](double value){
        if (value > 0.9) {
            z80io.setKempston(kUp, 1);
            z80io.setKempston(kDown, 0);
        } else if (value < -0.9) {
            z80io.setKempston(kDown, 1);
            z80io.setKempston(kUp, 0);
        } //else z80io.setKempston(kUp | kDown, 0);
    });
    connect(gamepad, &QGamepad::buttonDownChanged, this, [](bool pressed){
        z80io.setKempston(kUp, pressed);
    });
    connect(gamepad, &QGamepad::buttonUpChanged, this, [](bool pressed){
        z80io.setKempston(kDown, pressed);
    });
    connect(gamepad, &QGamepad::buttonRightChanged, this, [](bool pressed){
        z80io.setKempston(kLeft, pressed);
    });
    connect(gamepad, &QGamepad::buttonLeftChanged, this, [](bool pressed){
        z80io.setKempston(kRigth, pressed);
    });
    connect(gamepad, &QGamepad::buttonAChanged, this, [](bool pressed){
        z80io.setKempston(kFire, pressed);
    });
    connect(gamepad, &QGamepad::buttonBChanged, this, [](bool pressed){
        z80io.setKempston(kFire, pressed);
    });
    connect(gamepad, &QGamepad::buttonXChanged, this, [](bool pressed){
        z80io.setKempston(kFire, pressed);
    });
    connect(gamepad, &QGamepad::buttonYChanged, this, [](bool pressed){
        z80io.setKempston(kFire, pressed);
    });
}

JEvents::~JEvents()
{
    delete gamepad;
}
