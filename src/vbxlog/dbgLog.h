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
#pragma pack(pop)


static void dbgLog_head(DbgHead* head, int type, int size)
{
  head->time = __rdtsc() >> 10;
  head->init(type, size);
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
