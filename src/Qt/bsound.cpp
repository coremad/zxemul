
#include "bsound.h"
#include "zxbeeper.h"

#include "zxemul.h"

QBuffer * buffer [2];
QAudioOutput *a[2];
QAudioFormat format;
QByteArray byteArray1, byteArray2;

extern QThread * thr1;
extern QThread * thr2;

int nosound = 1;

void bsinit() {
    format.setSampleRate(bSetSampleRate);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());

    if (!info.isFormatSupported(format)) {
        fprintf( stderr, "Format not supported" );
        nosound = 1;
        return;
    }

    a[0] = new QAudioOutput(format);
    a[1] = new QAudioOutput(format);
    a[0]->moveToThread(thr1);
    a[1]->moveToThread(thr2);

    byteArray1 =  QByteArray::fromRawData((const char*)&babuffer1, abufSize);
    buffer[0] = new QBuffer(&byteArray1);
    buffer[0]->open(QIODevice::ReadOnly | QIODevice::Unbuffered);
    byteArray2 =  QByteArray::fromRawData((const char*)&babuffer2, abufSize);
    buffer[1] = new QBuffer(&byteArray2);
    buffer[1]->open(QIODevice::ReadOnly | QIODevice::Unbuffered);

//    buffer[0]->seek(0);
    a[0]->start(buffer[0]);
    a[1]->start(buffer[1]);
    nosound = 0;
}

void bsplay() {
    if (nosound) return;
    baBuf->playBuffer ^= 1;
    buffer[baBuf->playBuffer]->seek(0);
    for(int i = 0; i < abufSize; i++) baBuf->aBuffer[baBuf->playBuffer ^ 1][i] = 128;
}
