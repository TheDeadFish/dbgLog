#pragma once 
#include "dbgLog.h"

#pragma pack(push, 1)

struct DbgBuffDesc : DbgHead {  
    int addr; int ctl_len; 
    char civ; char data[65536];
};
    
    
struct DbgAudio : DbgHead { 
    int addr; char civ; };
    
DbgBuffDesc dbgBuffDesc;

#pragma pack(pop)

enum {
    DBG_WRITE_NAM, DBG_WRITE_NABM,
    DBG_AUDIO, DBG_BUFFDESC,
    DBG_BUFFGET, DBG_BUFFREL
};

static void dbgLog_buffdesc(int civ, int addr, int ctl_len, int size)
{
    dbgBuffDesc.civ = civ; dbgBuffDesc.addr = addr;
    dbgBuffDesc.ctl_len = ctl_len;
    if(size > 65536) size = 65536;
    dbgLog_write(dbgBuffDesc, DBG_BUFFDESC, size);
}

static void dbgLog_writeAudio(int civ, int addr, void* data, int size)
{
    DbgAudio dbgAudio;
    dbgAudio.civ = civ; dbgAudio.addr = addr;
    dbgLog_write(dbgAudio, DBG_AUDIO, data, size);
}

static void dbgLog_buffGetRel(int ctl_len, int size)
{
  dbgBuffDesc.civ = ctl_len>>16;
  dbgBuffDesc.ctl_len = ctl_len;
  int type = (ctl_len < 0) ? DBG_BUFFGET : DBG_BUFFREL;
  dbgLog_write(dbgBuffDesc, type, size);
}
