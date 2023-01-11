#ifndef ZXKEYBOARD_H
#define ZXKEYBOARD_H

#include "zdefs.h"

struct TZXKeyboard {
    union {
        struct {
            byte kfe, kfd, kfb, kf7, kef, kdf, kbf, k7f, pfe;
        };
        byte krows[9];
    };
} __attribute__((__packed__));

enum kRows:byte { kfe, kfd, kfb, kf7, kef, kdf, kbf, k7f, pfe };

enum ZX_Key_Bits {
    ZX_KeyBit_CShift = 1,   ZX_KeyBit_A = 1,    ZX_KeyBit_Q = 1,    ZX_KeyBit_1 = 1,
    ZX_KeyBit_Z = 2,        ZX_KeyBit_S = 2,    ZX_KeyBit_W = 2,    ZX_KeyBit_2 = 2,
    ZX_KeyBit_X = 4,        ZX_KeyBit_D = 4,    ZX_KeyBit_E = 4,    ZX_KeyBit_3 = 4,
    ZX_KeyBit_C = 8,        ZX_KeyBit_F = 8,    ZX_KeyBit_R = 8,    ZX_KeyBit_4 = 8,
    ZX_KeyBit_V = 16,       ZX_KeyBit_G = 16,   ZX_KeyBit_T = 16,   ZX_KeyBit_5 = 16,

    ZX_KeyBit_Space = 1,    ZX_KeyBit_Enter = 1,ZX_KeyBit_P = 1,    ZX_KeyBit_0 = 1,
    ZX_KeyBit_SShift = 2,   ZX_KeyBit_L = 2,    ZX_KeyBit_O = 2,    ZX_KeyBit_9 = 2,
    ZX_KeyBit_M = 4,        ZX_KeyBit_K = 4,    ZX_KeyBit_I = 4,    ZX_KeyBit_8 = 4,
    ZX_KeyBit_N = 8,        ZX_KeyBit_J = 8,    ZX_KeyBit_U = 8,    ZX_KeyBit_7 = 8,
    ZX_KeyBit_B = 16,       ZX_KeyBit_H = 16,   ZX_KeyBit_Y = 16,   ZX_KeyBit_6 = 16,
};

enum ZX_Key_Rows {
    ZX_KeyRow_CShift = kfe, ZX_KeyRow_Z = kfe, ZX_KeyRow_X = kfe, ZX_KeyRow_C = kfe, ZX_KeyRow_V = kfe,
    ZX_KeyRow_A = kfd, ZX_KeyRow_S = kfd, ZX_KeyRow_D = kfd, ZX_KeyRow_F = kfd, ZX_KeyRow_G = kfd,
    ZX_KeyRow_Q = kfb, ZX_KeyRow_W = kfb, ZX_KeyRow_E = kfb, ZX_KeyRow_R = kfb, ZX_KeyRow_T = kfb,
    ZX_KeyRow_1 = kf7, ZX_KeyRow_2 = kf7, ZX_KeyRow_3 = kf7, ZX_KeyRow_4 = kf7, ZX_KeyRow_5 = kf7,

    ZX_KeyRow_Space = k7f, ZX_KeyRow_SShift = k7f, ZX_KeyRow_M = k7f, ZX_KeyRow_N = k7f, ZX_KeyRow_B = k7f,
    ZX_KeyRow_Enter = kbf, ZX_KeyRow_L = kbf, ZX_KeyRow_K = kbf, ZX_KeyRow_J = kbf, ZX_KeyRow_H = kbf,
    ZX_KeyRow_P = kdf, ZX_KeyRow_O = kdf, ZX_KeyRow_I = kdf, ZX_KeyRow_U = kdf, ZX_KeyRow_Y = kdf,
    ZX_KeyRow_0 = kef, ZX_KeyRow_9 = kef, ZX_KeyRow_8 = kef, ZX_KeyRow_7 = kef, ZX_KeyRow_6 = kef,
};

#endif // ZXKEYBOARD_H
