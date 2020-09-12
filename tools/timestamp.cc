#include <stdio.h>
#include <intrin.h>
#include <windows.h>
#include <limits.h>

u_int64 getTimerRate()
{
	u_int64 time1 = __rdtsc();
	Sleep(1000);
	u_int64 time2 = __rdtsc();
	return time2-time1;
}

struct getShift_t { int shift; u_int64 pSec; };

getShift_t getShift(u_int64 rate, int size, unsigned seconds)
{
	int shift = 0; rate *= seconds;
	u_int64 rateMax = (-1ULL >> (64-size));
	while(rate > rateMax) {rate >>=1; shift++; }
	return {shift, rate / seconds};
}


int main()
{
	u_int64 rate = getTimerRate();
	printf("rate: %I64u\n", rate);
	for(int i = 1; i < 48; i *= 2) {
		printf(" mins: %d\n", i);
		for(int j = 1; j <= 8; j++) {
			auto x = getShift(rate, j*8, 60*i);
			printf("  size: %d, shift: %d, rate %I64u\n", 
				j, x.shift, x.pSec);
		}
	}
	return 0;
}
