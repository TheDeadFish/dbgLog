#pragma once
#include "dbgLog.h"

namespace DbgAc97
{

enum {
	DBG_WRITE_NAM, DBG_WRITE_NABM,
	DBG_AUDIO, DBG_BUFFDESC,
	DBG_BUFFGET, DBG_BUFFREL
};

#pragma pack(push, 1)

struct DbgBuffDesc  { int addr; 
	u32 ctl_len; char civ; char data[]; };
struct DbgAudio { int addr; char civ; };

#pragma pack(pop)

int dumpAudio(DbgLog& log, cch* file);
int dumpDesc(DbgLog& log, cch* file, int type, int flags=0);
  
extern DbgRegDef nabm_regs[];
extern DbgRegDef nam_regs[];

void print_all(DbgLog::ItemHead* item, int mask=-1);
void dump_all(DbgLog& log, cch* path, int mask=-1, int page=0);


}
