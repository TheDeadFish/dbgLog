#pragma once
#include "common.h"

struct DbgRegDef {
	int base, size; cch *name, *desc;
	int end() { return base+size; }
	bool chk(int addr) { return inRng1(addr, base, end()); }
	int ofs(int addr) { return addr-base; }
};
	
	
	

struct DbgLog
{
	typedef DbgLog_Reg RegItem;

	static int varData(void* data, unsigned len);
	static cch* varDataName(unsigned len);
	














	struct ItemHead {
		double time; 
		int type, size_;
		
		TMPL(T) int size() { return size_-sizeof(T); }
			
		TMPL(T) T& get() { return *(T*)(this+1); }
		char* data_() { return (char*)(this+1); }
		TMPL(T) char* data() { return data_() + sizeof(T); }
		char* end() { return data_() + size_; }
		
		void time_print(cch* type);
		
		
		int& addr() { return *(int*)data_(); }
		
		// register interface
		void reg_print(DbgRegDef* regs, cch* name);
		int reg_size() { return size_-1; }
		int reg_port() { return get<RegItem>().port; }
		int reg_data() { return varData(&get<RegItem>().data, reg_size()); }
	};
	
	xArray<ItemHead*> items;
	int load(cch* file);
	
	void init(int shift, __int64 rate);
	
	

	void dump(FILE* fp, int type, int offset, int flags=0);
	int dump(cch* file, int type, int offset, int flags=0);
	int dumpWav(cch* file, int type, int offset, int flags=0);
	
	
	
	static DbgRegDef* lookupReg(DbgRegDef* regs, int addr);
	
	
	
	
private:
	double timeScale;
	
	struct Parse;

	//double getTime(int time);
};
