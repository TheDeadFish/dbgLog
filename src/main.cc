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
		switch(item->type) {
		case DbgAc97::DBG_WRITE_NABM:
			item->reg_print(DbgAc97::nabm_regs, "NABM"); break;
		case DbgAc97::DBG_WRITE_NAM:
			item->reg_print(DbgAc97::nam_regs, "NAM"); break;
		case DbgAc97::DBG_AUDIO:
			DbgAc97::print_audio(item); break;
		case DbgAc97::DBG_BUFFDESC:
			DbgAc97::print_buffDesc(item); break;
		}
	}
	
	
	DbgAc97::dumpDesc(dbgLog, "desc.wav", DbgAc97::DBG_BUFFDESC);
	DbgAc97::dumpAudio(dbgLog, "audio.wav");
	DbgAc97::dumpDesc(dbgLog, "get.wav", DbgAc97::DBG_BUFFGET, 1);
	DbgAc97::dumpDesc(dbgLog, "get2.wav", DbgAc97::DBG_BUFFGET, 2);
	DbgAc97::dumpDesc(dbgLog, "rel.wav", DbgAc97::DBG_BUFFREL);

}

