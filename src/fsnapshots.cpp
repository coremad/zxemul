#include <stdio.h>

#include "z80.h"
#include "fsnapshots.h"

extern Tz80 z80;

int loadSNA48k(const char * fname) {
    FILE * SNAFile = fopen(fname, "r");
    if (!(SNAFile)) {
	    fprintf(stderr, "Can`t open SNA file\n");
        return false;
	}
    if(fread(&SNA, sizeof(SNA), 1, SNAFile)) {
        if(fread(&z80.z80io->zxmem[16384], 48*1024, 1, SNAFile)) {
            fclose(SNAFile);
            initSNA48k();
            return true;
        }
    }
    fprintf(stderr, "Can`t read SNA file\n");
    fclose(SNAFile);
    return false;
}

int loadROM48k(const char * fname) {
    FILE * ROMFile = fopen(fname, "r");
    if (!ROMFile) {
        fprintf(stderr, "Can`t open ROM file\n");
        return false;
    }
    if (!fread(&z80.z80io->zxmem[0], 16384, 1, ROMFile)) {
        fprintf(stderr, "Can`t read ROM file\n");
        fclose(ROMFile);
        return false;
    }
    fclose(ROMFile);
    return true;
}
