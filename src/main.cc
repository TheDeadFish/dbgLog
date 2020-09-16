#include "stdshit.h"
#include "dbgAc97.h"

const char progName[] = "test";

int main()
{
	DbgLog dbgLog;
	dbgLog.init(10, 3060000000);
	int ec = dbgLog.load("dbgLog.bin");
	if(ec) { printf("%d\n", ec); return ec; }
	

	for(auto* item : dbgLog.items) {
		DbgAc97::print_all(item);
	}
	
	DbgAc97::dump_all(dbgLog, "", -1);
}

