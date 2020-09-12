#include "stdshit.h"
#include "dbgLog.h"
#include "wavWrite.h"

#pragma pack(push, 1)
struct DbgHead { unsigned int time;
	unsigned short size; };
#pragma pack(pop)

int DbgLog::load(cch* file)
{
	FILE* fp = fopen(file, "rb");
	if(!fp) return errno;
	
	DbgHead head;
	__int64 timePos = 0;
	unsigned lastTime = 0xFFFFFFFF;
	
	while(fread(&head, sizeof(DbgHead), 1, fp))
	{
		int offset = ftell(fp);
			ItemHead* item = (ItemHead*)xmalloc(sizeof(ItemHead)+head.size);
		items.push_back(item);
		
		if(!fread(item+1, head.size, 1, fp)) break;
		item->type = head.time & 0x000000FF;
		item->size_ = head.size;
		
		// handle the time
		unsigned curTime = head.time & 0xFFFFFF00;
		if(lastTime == 0xFFFFFFFF) lastTime = curTime;
		timePos += curTime-lastTime; lastTime = curTime;
		item->time = timePos * timeScale;
	}
	
	int err = ferror(fp);
	fclose(fp); return 0;
}

void DbgLog::init(int shift, __int64 rate)
{
	timeScale = (1LL << shift) / double(rate);
}

void DbgLog::dump(FILE* fp, int type, int offset)
{
	for(auto* item : items) {
		if(item->type != type) continue;
		int size = item->size_-offset;
		if(size < 0) break;
		fwrite(item->data()+offset, 1, size, fp);
	}
}

int DbgLog::dump(cch* file, int type, int offset)
{
	FILE* fp = fopen(file, "wb");
	if(!fp) return errno;
	dump(fp, type, offset);
	fclose(fp); return 0;
}

int DbgLog::dumpWav(cch* file, int type, int offset)
{
	WavWrite ww;
	IFRET(ww.open(file));
	dump(ww.fp, type, offset);
	return ww.close(48000, 2, 16);
}

DbgRegDef* DbgLog::lookupReg(DbgRegDef* regs, int addr)
{
	for(;regs->name; regs++) {
		if(regs->chk(addr)) return regs;
	}
	
	return NULL;
}

void DbgLog::ItemHead::time_print(cch* type)
{
	printf("%.3f: %s, ", time, type);
}

void DbgLog::ItemHead::reg_print(DbgRegDef* regs, cch* name)
{
	time_print("RegWr");
	printf("%8X, ", reg_data());
	int port = reg_port(); int size = reg_size();
	DbgRegDef* reg = lookupReg(regs, port);
	
	if(reg) {
		printf("%s", reg->name);
		if(reg->ofs(port)) printf("+%d", reg->ofs(port));
		if(reg->size != size) printf(",%s", varDataName(size));
	} else {
		printf("%02X,%s", port, varDataName(size));
	}
	
	printf("\n");
	
}


int DbgLog::varData(void* data, unsigned len)
{
	int ret = 0;
	if(len > 4) len = 4;
	memcpy(&ret, data, len);
	return ret;
}

cch* DbgLog::varDataName(unsigned len)
{
	switch(len) {
	case 1: return "b";
	case 2: return "w";
	case 4: return "d";
	case 8: return "q";
	default: return "?";
	}
}
