#ifndef ZXTAPE_H
#define ZXTAPE_H

#include "zxports.h"
#ifdef DEBUG
#include "fsnapshots.h"
#endif

#define mic_bit_mask 64
#define max_tape_size 256*1024
#define max_blocks 128

enum states { pton, sync, data, next, pause, stop };

struct TBlock {
    word bSize;
    byte bType;
    dword  bStart;
};

const dword pTicks = 2168;
const dword pHCount = 80001;

class TZXTape : public TZXport {
public:
    byte * tapeBuf;
    int state, fPos, tapeStarted, dataStarted;
    dword tapeSize = 0;
    TBlock blocks[max_blocks];
    int bCount, nBlock;
    int syncStarted;
    qword syncStart;
    qword syncEnd;
    int block = 0;
    int blockStart = 0;
    int blockPos = 0;
    int blockEnd = 0;
    byte val, bit;
    int await;

    //const dword pNCount = 81;
    //const dword pDCount = 30001;
    //const dword bpause = 50*1000;

    dword pCount = 0;
    dword sCount = 0;


    TZXTape (TZXmultiport * p, byte * tbuf) : TZXport ( p) {
        tapeBuf = tbuf;
        state = fPos = tapeStarted = syncStarted = dataStarted = 0;
    }

    byte pilotone() {
        if (state == pton && pCount && tapeStarted) {
            pCount--;
            return ((z80io->tickCounter / pTicks) & 1) * mic_bit_mask;
        }
        state = sync;
        return 64;
    }

    byte synctone() {
        if (tapeStarted && state == sync) {
            if (!syncStarted) {
                syncStarted = 1;
                syncStart = z80io->tickCounter;
                syncEnd = z80io->tickCounter + 667 + 735 + 11 + 7*2;
            }
            if (z80io->tickCounter < syncEnd ) {
                return 0;
            } else {
                state = data;
                syncStarted = 0;
                return mic_bit_mask;
            }
        }
        return 0;
    }

    byte dataBit() {
        if (tapeStarted && state == data) {
    //        fprintf(stderr, "wtf?!\n");
            if (!dataStarted) {
                dataStarted = 1;
                blockPos = 2;
                bit = 0;
    //            return mic_bit;
            }
            if (!bit) {
                val = tapeBuf[blocks[nBlock].bStart + blockPos];
                bit = 8;
                await = -1;
                blockPos++;
                if (blockPos > blocks[nBlock].bSize + 2) {
                    dataStarted = 0;
                    state = next;
    #ifdef DEBUG
                    fprintf(stderr, "\nbkock end %u\n", blockPos);
    #endif
                    return 0;
                }
    #ifdef DEBUG
                fprintf(stderr, "\n%02x %02x ", blockPos-2, val);
    #endif
            }
            int w = 14;
            if (!(val & 1) && await == -1) await = w;
            if ((val & 1) && await == -1) await = w*2;
    //        await--;
            if (!(await--)) {
    #ifdef DEBUG
                fprintf(stderr, "%i", val & 1);
    #endif
                bit--;
                val >>= 1;
                return mic_bit_mask;
            }
            return 0;
        }
        return 1;
    }

    byte nextBlock() {
        if(++nBlock >= bCount) {
            state = stop;
            nBlock = -1;
    #ifdef DEBUG
            fprintf(stderr, "tape stoped\n");
    #endif
            return 0;
        }
        dataStarted = bit = 0;
        pCount = pHCount;
        state = pton;
    #ifdef DEBUG
        fprintf(stderr, "block: %i type: %02x size: %04x start: %u \n", nBlock, blocks[nBlock].bType, blocks[nBlock].bSize, blocks[nBlock].bStart);
    #endif
        return 0;
    }

    virtual byte inData(word) {
        byte res = z80io->tickCounter;
    #ifdef DEBUG
        res = 0;
    #endif
        if (tapeStarted)
            switch (state) {
            case pton:
                res = pilotone();
                break;
            case sync:
                res = synctone();
                break;
            case data:
                res = dataBit();
                break;
            case next:
                res = nextBlock();
                break;
            default:
    #ifdef DEBUG
        fprintf(stderr, "state: [def]\n");
        res = 0;
    #endif
                tapeStarted = 0;
            }
        portdata = res & mic_bit_mask;
        return (portdata);
    }

    virtual void reset() {
        portdata = 0;
    }

    void initTape() {
    //    if (!tapeStarted) {
    #ifdef DEBUG
        loadSNA48k("roms/load.sna");

        FILE * tfile;
        tfile = fopen("roms/BigThings.tap", "r");
        tapeSize = fread(&tapeBuf[0], 1, max_tape_size, tfile);
        fclose(tfile);
        fprintf(stderr, "size: %u\n", tapeSize);
    #endif
        dword i = 0;
        bCount = 0;
        while (i < tapeSize) {
            blocks[bCount].bStart = i;
            blocks[bCount].bSize = tapeBuf[i] + tapeBuf[i+1]*256;
            blocks[bCount].bType = tapeBuf[i+2];
            i += (blocks[bCount].bSize + 2);
    #ifdef DEBUG
            fprintf(stderr, "block: %i type: %02x size: %04x start: %u \n", bCount, blocks[bCount].bType, blocks[bCount].bSize, blocks[bCount].bStart);
            if (i > tapeSize) fprintf(stderr, "bad tape size\n");
    #endif
            bCount++;
        }

        nBlock = -1;

        pCount = pHCount;
        block = -1;
        fPos = 0;
        blockEnd = 0;
        blockStart = 0;
        blockPos = 0;
        state = next;
        tapeStarted = 1;
    #ifdef DEBUG
            fprintf(stderr, "Tape size: %i, init done?\n", tapeSize);
    #endif
    //    }
    }

};

#endif // ZXTAPE_H
