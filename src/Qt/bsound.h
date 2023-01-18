#ifndef BSOUND_H
#define BSOUND_H

#include <iostream>

#include <QMultimedia>
#include <QAudioDeviceInfo>
#include <QAudio>
#include <QAudioOutput>
#include <QBuffer>
#include <QThread>

extern QBuffer *buffer[];
extern QAudioOutput *a[];
extern QAudioFormat format;

extern char *data1;

extern void bsinit();

extern void bsplay();

#endif // BSOUND_H
