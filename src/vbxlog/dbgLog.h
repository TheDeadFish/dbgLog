// binary debuggin logger

#pragma once
#include <stdio.h>
#include <intrin.h>
#include <stdarg.h>
#include <stddef.h>
#include <conio.h>

//extern "C"
//void __stdcall AllocConsole();

static FILE* fpDump;
static void dbgLog_open() { if(fpDump) fclose(fpDump);
   // AllocConsole();
	fpDump = fopen("%LOGFILE%", "wb"); }
static void dbgLog_write_(void* data, int len) { fwrite(data, 1, len, fpDump); }

#pragma pack(push, 1)
struct DbgHead { unsigned int time; unsigned short size; };
struct DbgReg : DbgHead { char port; int data; };
template <int n>
struct DbgRaw : DbgHead { char data[n]; };
#pragma pack(pop)


static void dbgLog_head(DbgHead* head, int type, int size)
{
	head->time = type | ((__rdtsc() >> 10) & 0xFFFFFF00);
    head->size = size;
}

template <class T>
static void dbgLog_write(T& head, int type, int size)
{
    size += offsetof(T, data)-sizeof(DbgHead);
    dbgLog_head(&head, type, size);
    dbgLog_write_(&head, size+sizeof(DbgHead));
}

static void dbgLog_reg(int type, int port, int size, int data) {
    DbgReg reg; reg.port = port; reg.data = data;
    dbgLog_write(reg, type, size);
}

static void dbgLog_msg(const char* fmt, ...)
{
	DbgRaw<2048> msg;
	va_list args; va_start (args, fmt);
    int len = vsprintf (msg.data,fmt, args);
	dbgLog_write(msg, 255, len);
}


template <class T>
static void dbgLog_write(T& head, int type, void* data, int size)
{
    dbgLog_head(&head, type, sizeof(head)-sizeof(DbgHead)+size);
    dbgLog_write_(&head, sizeof(head));
    dbgLog_write_(data, size);
}
