#include <stdshit.h>
#include "wavwrite.h"

struct WaveHead {
	u32 riffID, riffSize;
	u32 waveId, fmtID, fmtSize;
	u16 wFormatTag;
	u16 nChannels;
	u32 nSamplesPerSec;
	u32 nAvgBytesPerSec;
	u16 nBlockAlign;
	u16 wBitsPerSample;
	u32 dataID, dataSize;
};

int WavWrite::open(cch* fileName)
{
	fp = fopen(fileName, "wb");
	if(!fp) return 1;
	fseek(fp, sizeof(WaveHead), 0);
	return 0;
}

int WavWrite::close(int rate, 
	int nchnl, int bits)
{
	SCOPE_EXIT(fclose(fp));

	// determin file size
	u32 fileSize = ftell(fp);
	if(fileSize < 44) return -1;
	u32 dataSize = fileSize-44;
	if(fileSize & 1) { fileSize++;
		if(!write(mem_zp4, 1)) return 1; }
		
	// write header
	int fmt = bits ? 1 : (bits=32, 3);
	int nBlockAlign = nchnl * (bits>>3);
	WaveHead wh = { 0x46464952, fileSize-8,
		0x45564157, 0x20746D66, 16, fmt, 
		nchnl, rate, rate*nBlockAlign, nBlockAlign, 
		bits ? bits : 32, 0x61746164, dataSize };
	fseek(fp, 0, 0); return write(&wh, sizeof(wh))^1;
}
