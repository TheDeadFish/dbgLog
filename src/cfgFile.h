#pragma once

struct CfgFile
{
	long long rdtsc;
	int shift;
};

extern CfgFile cfg;

void cfgFile_load();
