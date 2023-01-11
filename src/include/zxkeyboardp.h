#ifndef ZXKEYBOARDP_H
#define ZXKEYBOARDP_H

#include "zxports.h"

class TZXKeyboardp : public TZXport {
public:
    TZXKeyboard * ZXKeyboard;

    TZXKeyboardp(TZXmultiport * p) : TZXport ( p) {
        ZXKeyboard = &p->z80io->ZXKeyboard;
    }

    virtual byte inData(word port) {
        byte keys;// = 191;
        switch	(port) {
        case 0xfefe:
            keys = ZXKeyboard->kfe;
            break;
        case 0xfdfe:
            keys = ZXKeyboard->kfd;
            break;
        case 0xfbfe:
            keys = ZXKeyboard->kfb;
            break;
        case 0xf7fe:
            keys = ZXKeyboard->kf7;
            break;
        case 0xeffe:
            keys = ZXKeyboard->kef;
            break;
        case 0xdffe:
            keys = ZXKeyboard->kdf;
            break;
        case 0xbffe:
            keys = ZXKeyboard->kbf;
            break;
        case 0x7ffe:
            keys = ZXKeyboard->k7f;
            break;
    //            case 0x00fe:
    //                keys = ((!(iTicksCounter & 64)) & 0xff);
    //                break;
        default:
            keys = ZXKeyboard->pfe;
        }
    //            keys |= getData(z80io->tickCounter);
    //            keys |= iTicksCounter & 64;
        portdata = keys;
        return portdata;
    }

    virtual void reset() {
        ZXKeyboard->kfe = ZXKeyboard->kfd = ZXKeyboard->kfb
            = ZXKeyboard->kf7 = ZXKeyboard->kef = ZXKeyboard->kdf
            = ZXKeyboard->kbf = ZXKeyboard->k7f = 0xbf;
        ZXKeyboard->pfe = 0;
    }

};

#endif // ZXKEYBOARDP_H
