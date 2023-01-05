#include <stdio.h>
#include "fsnapshots.h"

int loadSNA48k(const char * fname) {
    FILE * SNAFile;
    SNAFile = fopen(fname, "r");
	if (!SNAFile) {
	    fprintf(stderr, "Can`t open SNA file\n");
        return 0;
	}
    fread(&SNA, sizeof(SNA), 1, SNAFile);
    fread(&z80io.zxmem[16384], 48*1024, 1, SNAFile);
    fclose(SNAFile);
    initSNA48k();
    return 1;
}

int loadROM48k(const char * fname) {
    FILE * romfile = fopen(fname, "r");
    if (!romfile) {
        fprintf(stderr, "Can`t open ROM file\n");
        return 0;
    }
    if (!fread(&z80io.zxmem[0], 16384, 1, romfile)) {
        fprintf(stderr, "Can`t read ROM file\n");
        fclose(romfile);
        return 0;
    }
    fclose(romfile);
    return 1;
}
