import os

with open('dbgAc97.h', 'r') as f:
	ac97File = f.read()
with open('dbgLog.h', 'r') as f:
	dbgLog = f.read()
	
path = os.path.abspath(os.getcwd()+'/../../bin/dbgLog.bin')
print path
dbgLog = dbgLog.replace('%LOGFILE%', path.replace('\\', '\\\\'))

ac97File = ac97File.replace('#include "dbgLog.h"', dbgLog)
with open('../../bin/dbgAc97.h', 'w') as f:
	f.write(ac97File)
	