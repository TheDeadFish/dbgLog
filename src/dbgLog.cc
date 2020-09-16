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

void DbgLog::dump(FILE* fp, int type, int offset, int flags)
{
	for(auto* item : items) {
		if(item->type != type) continue;
		int size = item->size_-offset;
		if(size < 0) break;
		char* data = item->data()+offset;
		
		// boundary checks
		if(flags & 3) {
			
		
		
		
			unsigned page = ALIGN_PAGE(item->addr());
			unsigned end = item->addr() + size;
			if(page > end) page = end;
			
			if(flags & 1) { end = page; }
			if(flags & 2) { item->addr() = page; }
			size = end-item->addr();
		}
		
		fwrite(item->data()+offset, 1, size, fp);
	}
}

static
char* fix_name(cch* name, int flags)
{
	if(flags == 0) return xstrdup(name);
	cch* ext = getExt(name);
	return xstrfmt("%v%c%s", cstr(name, ext),
		flags & 1 ? 'O' : 'X', ext);
}

int DbgLog::dump(cch* file, int type, int offset, int flags)
{
	
	FILE* fp = fopen(xstr(fix_name(file, flags)), "wb");
	if(!fp) return errno;
	dump(fp, type, offset, flags);
	fclose(fp); return 0;
}

int DbgLog::dumpWav(cch* file, int type, int offset, int flags)
{
	WavWrite ww;
	IFRET(ww.open(xstr(fix_name(file, flags))));
	dump(ww.fp, type, offset, flags);
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
