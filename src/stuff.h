#pragma once

struct xFile
{
	FILE* fp = 0;

	xFile(FILE* fp_) : fp(fp_) {}
	xFile(cch* name, cch* mode) { 
		fp = fopen(name, mode); }
	~xFile() { fclose(fp); }
		
	operator bool() { return fp; }
	operator FILE*() { return fp; }
	
	int read(void* data, int size) {
		return fread(data, size, 1, fp); }
};


TMPL(T)
struct xMem
{
	T* ptr;
	xMem() : ptr(0) {}
	xMem(T* p) : ptr(p) {}
	~xMem() { free(ptr); }
	
	operator T*() { return ptr; }
};
