#pragma once

static 
void regWrite8(unsigned short port, unsigned char data) {
	asm volatile ("out %%al, %%dx" :: "a"(data), "d"(port)); }
static
void regWrite16(unsigned short port, unsigned short data) {
	asm volatile ("out %%ax, %%dx" :: "a"(data), "d"(port)); }
static
void regWrite32(unsigned short port, int data) {
	asm volatile ("out %%eax, %%dx" :: "a"(data), "d"(port)); }
static
int regRead32(unsigned short port) { int ret;
	asm volatile ("in %%dx, %%eax" : "=a"(ret) : "d"(port));
	return ret; }

static 
void log_bytes(int port, void* data, int size)
{
	char* pos = (char*)data; 
	while((size -= 4) >= 0) {
		regWrite32(port, *(int*)pos); pos += 4; }
	if(size & 2) { regWrite16(port, *(short*)pos); pos += 2; }
	if(size & 1) { regWrite8(port, *(char*)pos); }
}

static
void log_raw(int type, void* data, int size)
{
	int hItem = regRead32(0xd1ff);
	log_bytes(hItem, data, size);
	regWrite32(0xd1ff, (hItem<<16)|type);
}
