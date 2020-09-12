#pragma once

struct WavWrite
{
	FILE* fp; int open(cch* fileName);
	int close(int rate, int nchnl, int bits);
	int write(const void* data, int len) {
		return fwrite(data, len, 1, fp); }
};
