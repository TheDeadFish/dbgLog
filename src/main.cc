#include "stdshit.h"
#include "cfgFile.h"
#include "dbgAc97.h"

const char progName[] = "test";

int main(int argc, char** argv)
{
	cfgFile_load();

	DbgLog dbgLog;
	dbgLog.init(cfg.shift, cfg.rdtsc);
	int ec = dbgLog.load("dbgLog.bin");
	if(ec) { printf("%d\n", ec); return ec; }
	

	for(auto* item : dbgLog.items) {
		DbgAc97::print_all(item);
	}
	
	DbgAc97::dump_all(dbgLog, "", -1);
}

