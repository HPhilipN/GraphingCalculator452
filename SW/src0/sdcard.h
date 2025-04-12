#ifndef SDCARD_H_
#define SDCARD_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "platform.h"
#include "xil_printf.h"

#include "interrupts.h"

#include "xil_types.h"
#include "xstatus.h"
#include "xscugic.h"
#include "xparameters.h"
#include "ff.h"
#include "main.h"

void throwFatalError(const char *func,const char *msg);
extern void playWavFile(int num);
//void playAudio();
void loadWavFiles();
void loadImageFiles();
void sd_card_initialization();

#define VOLUME 2

#define MAX_FILES 32
#define MAX_FILENAME_LENGTH 32

extern char wavFiles[MAX_FILES][MAX_FILENAME_LENGTH];
extern int wavFileCount;

extern u8 *buffers[MAX_FILES];
//extern fmtChunk_t fmtChunks[MAX_FILES];

#endif
