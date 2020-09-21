// binary debuggin logger

#pragma once
#include <stdio.h>
#include <intrin.h>
#include <stdarg.h>
#include <stddef.h>
#include <conio.h>
#include "../common.h"

//extern "C"
//void __stdcall AllocConsole();

static FILE* fpDump;
static void dbgLog_open() { if(fpDump) fclose(fpDump);
   // AllocConsole();
	fpDump = fopen("%LOGFILE%", "wb"); }
static void dbgLog_write_(void* data, int len) { fwrite(data, 1, len, fpDump); }

#pragma pack(push, 1)
typedef DbgLog_Head DbgHead;
struct DbgReg : DbgLog_Head, DbgLog_Reg {};
template <int n>
struct DbgRaw : DbgHead { char data[n]; };
struct DbgChunk : DbgHead, DbgLog_Chunk { 
  char data[4096]; };

#pragma pack(pop)

static void dbgLog_head(DbgHead* head, int type, int size)
{
  head->time = __rdtsc() >> 10;
  head->init(type, size);
}

static void dbgLog_write_(DbgHead* head, int type, int size)
{
  dbgLog_head(head, type, size);
  dbgLog_write_(head, size);
}

template <class T>
static void dbgLog_write(T& head, int type, int size)
{
    size += offsetof(T, data);
    dbgLog_head(&head, type, size);
    dbgLog_write_(&head, size);
}

static void dbgLog_reg(int type, int port, int size, int data) {
    DbgReg reg; reg.port = port; reg.data = data;
    dbgLog_write(reg, type, size);
}

#if 0
static void dbgLog_msg(const char* fmt, ...)
{
	DbgRaw<2048> msg;
	va_list args; va_start (args, fmt);
    int len = vsprintf (msg.data,fmt, args);
	dbgLog_write(msg, 255, len);
}
#endif

template <class T>
static void dbgLog_write(T& head, int type, void* data, int size)
{
    dbgLog_head(&head, type, sizeof(head)+size);
    dbgLog_write_(&head, sizeof(head));
    dbgLog_write_(data, size);
}

static DbgChunk dbgLog_chunk;
static char dbgLog_slot[256];

static 
void dbgLog_itemFlush(int type)
{
  // write the block
  dbgLog_chunk.tItem = type;
  dbgLog_write_(&dbgLog_chunk, 255, dbgLog_chunk.size);
  dbgLog_chunk.size = offsetof(DbgChunk, data);
}

static
void dbgLog_itemSwitch(int hItem)
{
  if(dbgLog_chunk.hItem != hItem) {
    dbgLog_itemFlush(-1); 
    dbgLog_chunk.hItem = hItem;
  }
}

static 
int dbgLog_itemBegin(int min, int max)
{
  for(int i = min; i < max; i++) {
    if(dbgLog_slot[i] == 0) { dbgLog_slot[i] = 1;
      if(dbgLog_chunk.hItem) { dbgLog_itemFlush(-1); }
      dbgLog_chunk.hItem = i; 
      dbgLog_chunk.size = offsetof(DbgChunk, data);
      dbgLog_itemFlush(-1);
      return i;
    }
  }

  return -1;
}

static
void dbgLog_itemEnd(int hItem, int type)
{
  dbgLog_itemSwitch(hItem);
  dbgLog_itemFlush(type);
  dbgLog_slot[hItem] = 0;
  dbgLog_chunk.hItem = 0;
}

static
void dbgLog_itemWrite(int hItem, void* data, int size)
{
  dbgLog_itemSwitch(hItem);
  
  // check the size
TRY_AGAIN:
  int oldSize = dbgLog_chunk.size;
  int newSize = dbgLog_chunk.size+size;
  if(newSize > sizeof(dbgLog_chunk)) {
    dbgLog_itemFlush(-1); goto TRY_AGAIN; }
  
  // copy the data
  char* dst = ((char*)&dbgLog_chunk)+oldSize;
  dbgLog_chunk.size = newSize; 
  memcpy(dst, data, size);
}
