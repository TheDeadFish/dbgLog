#include "stdshit.h"
#include "dbgAc97.h"

namespace DbgAc97
{

DbgRegDef nabm_regs[] = {
	{0, 4, "PI_BDBAR", "PCM In Buffer Descriptor list Base Address Register"},
  {4, 1, "PI_CIV", "PCM In Current Index Value"},
  {5, 1, "PI_LVI", "PCM In Last Valid Index"},
  {6, 2, "PI_SR", "PCM In Status Register"},
  {8, 2, "PI_PICB", "PCM In Position In Current Buffer"},
  {10, 1, "PI_PIV", "PCM In Prefetched Index Value"},
  {11, 1, "PI_CR", "PCM In Control Register"},
  {16, 4, "PO_BDBAR", "PCM Out Buffer Descriptor list Base Address Register"},
  {20, 1, "PO_CIV", "PCM Out Current Index Value"},
  {21, 1, "PO_LVI", "PCM Out Last Valid Index"},
  {22, 2, "PO_SR", "PCM Out Status Register"},
  {24, 2, "PO_PICB", "PCM Out Position In Current Buffer"},
  {26, 1, "PO_PIV", "PCM Out Prefetched Index Value"},
  {27, 1, "PO_CR", "PCM Out Control Register"},
  {32, 4, "MC_BDBAR", "Mic. In Buffer Descriptor list Base Address Register"},
  {36, 1, "PM_CIV", "Mic. In Current Index Value"},
  {37, 1, "MC_LVI", "Mic. In Last Valid Index"},
  {38, 2, "MC_SR", "Mic. In Status Register"},
  {40, 2, "MC_PICB", "Mic In Position In Current Buffer"},
  {42, 1, "MC_PIV", "Mic. In Prefetched Index Value"},
  {43, 1, "MC_CR", "Mic. In Control Register"},
  {44, 4, "GLOB_CNT", "Global Control"},
  {48, 4, "GLOB_STA", "Global Status"},
  {52, 1, "ACC_SEMA", "Codec Write Semaphore Register"},
  {0,0,0,0}
};

DbgRegDef nam_regs[] = {
	{0, 2, "Reset", "Reset"},
	{2, 2, "Master Volume Mute", "Master Volume Mute"},
	{4, 2, "Headphone Volume Mute", "Headphone Volume Mute"},
	{6, 2, "Master Volume Mono Mute", "Master Volume Mono Mute"},
	{8, 2, "Master Tone (R & L)", "Master Tone (R & L)"},
	{10, 2, "PC_BEEP Volume Mute", "PC_BEEP Volume Mute"},
	{12, 2, "Phone Volume Mute", "Phone Volume Mute"},
	{14, 2, "Mic Volume Mute", "Mic Volume Mute"},
	{16, 2, "Line In Volume Mute", "Line In Volume Mute"},
	{18, 2, "CD Volume Mute", "CD Volume Mute"},
	{20, 2, "Video Volume Mute", "Video Volume Mute"},
	{22, 2, "Aux Volume Mute", "Aux Volume Mute"},
	{24, 2, "PCM Out Volume Mute", "PCM Out Volume Mute"},
	{26, 2, "Record Select", "Record Select"},
	{28, 2, "Record Gain Mute", "Record Gain Mute"},
	{30, 2, "Record Gain Mic Mute", "Record Gain Mic Mute"},
	{32, 2, "General Purpose", "General Purpose"},
	{34, 2, "3D Control", "3D Control"},
	{36, 2, "AC’97 RESERVED", "AC’97 RESERVED"},
	{38, 2, "Powerdown Ctrl/Stat", "Powerdown Ctrl/Stat"},
	{40, 2, "Extended Audio", "Extended Audio"},
	{42, 2, "Extended Audio Ctrl/Stat", "Extended Audio Ctrl/Stat"},
	{44, 2, "PCM Front DAC Rate", "PCM Front DAC Rate"},
	{46, 2, "PCM Surround DAC Rate", "PCM Surround DAC Rate"},
	{48, 2, "PCM LFE DAC Rate", "PCM LFE DAC Rate"},
	{50, 2, "PCM LR ADC Rate", "PCM LR ADC Rate"},
	{52, 2, "MIC ADC Rate", "MIC ADC Rate"},
	{54, 2, "6Ch Vol: C, LFE Mute", "6Ch Vol: C, LFE Mute"},
	{56, 2, "6Ch Vol: L, R Surround Mute", "6Ch Vol: L, R Surround Mute"},
};

int dumpAudio(DbgLog& log, cch* file) {
	return log.dumpWav(file, DBG_AUDIO, sizeof(DbgAudio)); }
	
int dumpDesc(DbgLog& log, cch* file, int type, int flags) {
	return log.dumpWav(file, type, sizeof(DbgBuffDesc), flags); }
	
	
void print_audio(DbgLog::ItemHead* item) {
	item->time_print("audio_Write");
	printf("%02X, %08X, %5d\n", 
			item->get<DbgAudio>().civ, item->get<DbgAudio>().addr, 
		item->size<DbgAudio>());	
}

void print_buffDesc(DbgLog::ItemHead* item, cch* name) {
	item->time_print(name);
	printf("%02X, %08X, %5d, %04X \n",
		item->get<DbgBuffDesc>().civ, item->get<DbgBuffDesc>().addr,
		item->size<DbgBuffDesc>(), item->get<DbgBuffDesc>().ctl_len >> 16 
	);
}

void print_all(DbgLog::ItemHead* item, int mask)
{
	if(!_BTST(mask, item->type)) return;

	switch(item->type) {
	case DBG_WRITE_NABM:
		item->reg_print(nabm_regs, "NABM"); break;
	case DBG_WRITE_NAM:
		item->reg_print(nam_regs, "NAM"); break;
	case DBG_AUDIO:
		print_audio(item); break;
	case DBG_BUFFDESC:
		print_buffDesc(item, "buff_Descri"); break;
	case DBG_BUFFGET:
		print_buffDesc(item, "buff_get   "); break;	
	case DBG_BUFFREL:
		print_buffDesc(item, "buff_rel   "); break;
	}
}

static 
void dump_xxx(DbgLog& log, cch* path, cch* name, int type, int page)
{
	xstr buff = pathCat(path, name);
		
	if(!page || (page & 4)) 
		dumpDesc(log, buff, type, 0);
	if(page & 1) dumpDesc(log, buff, type, 1);
	if(page & 2) dumpDesc(log, buff, type, 2);
}

void dump_all(DbgLog& log, cch* path, int mask, int page)
{
	if(_BTST(mask, DBG_AUDIO))
		dumpAudio(log, xstr(pathCat(path, "audio.wav")));
	if(_BTST(mask, DBG_BUFFDESC))
		dump_xxx(log, path, "desc.wav", DBG_BUFFDESC, page);
	if(_BTST(mask, DBG_BUFFGET))
		dump_xxx(log, path, "get.wav", DBG_BUFFGET, page);
	if(_BTST(mask, DBG_BUFFREL))
		dump_xxx(log, path, "rel.wav", DBG_BUFFREL, page);
}

}
