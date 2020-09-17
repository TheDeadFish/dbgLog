#include "stdshit.h"
#include "cfgFile.h"

CfgFile cfg;
static char* progDir;

ALWAYS_INLINE wxstr widen(cch* path, cch* name) {
	return xstrfmt(L"%J%K", progDir, "config.cfg"); }

long long cfgFile_number(cchw* name)
{
	WCHAR buff[32];
	GetPrivateProfileStringW(L"config", name, NULL,
		buff, 32, widen(progDir, "config.cfg"));
	return wtoll(buff);
}

void cfgFile_load()
{
	progDir = getProgramDir();
	cfg.rdtsc = cfgFile_number(L"rdtsc");
	cfg.shift = cfgFile_number(L"shift");
}
