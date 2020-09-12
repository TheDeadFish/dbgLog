with open("reg-list.txt") as f:
  lineList = [x.strip() for x in f.readlines()]
	
	
iter = iter(lineList)


while True:
	base = int(next(iter).replace('h', ''), 16)
	name = next(iter); desc = next(iter)
	size = len(next(iter).replace('h', ''))/2
	next(iter)
	
	print '{%d, %d, "%s", "%s"},' % (base, size, name, desc)
	




