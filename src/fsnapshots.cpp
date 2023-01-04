#include <stdio.h>
#include "fsnapshots.h"

void loadSNA48k(const char * fname) {
    FILE * SNAFile;
    SNAFile = fopen(fname,"r");
	if (!SNAFile) {
	    fprintf(stderr, "Can`t open ROM file\n");
	    return;
	}
    fread(&SNA, sizeof(SNA), 1, SNAFile);
    fread(&z80io.zxmem[16384], 48*1024, 1, SNAFile);
    fclose(SNAFile);
    initSNA48k();
}
