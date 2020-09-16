import os

with open('dbgAc97.h', 'r') as f:
	ac97File = f.read()
with open('dbgLog.h', 'r') as f:
	dbgLog = f.read()
with open('..\common.h') as f:
	common = f.read();
	
path = os.path.abspath(os.getcwd()+'/../../bin/dbgLog.bin')

dbgLog = dbgLog.replace('%LOGFILE%', path.replace('\\', '\\\\'))
dbgLog = dbgLog.replace('#include "../common.h"', common)
ac97File = ac97File.replace('#include "dbgLog.h"', dbgLog)

with open('../../bin/dbgAc97.h', 'w') as f:
	f.write(ac97File)
