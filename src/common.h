#pragma once
#ifndef DBGLOG_SHIFT
  #define DBGLOG_SHIFT 10
#endif

#pragma pack(push, 1)
struct DbgLog_Head { unsigned size, time;
	unsigned getSize() { return size>>8; }
	unsigned getType() { return size&0xFF; }
	
	void init(unsigned type, unsigned size) {
		this->size = type | (size << 8); }
};

struct DbgLog_Reg { char port; int data; };

#pragma pack(pop)
