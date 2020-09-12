with open("reg-list2.txt") as f:
  lineList = [x.strip() for x in f.readlines()]
	
	
iter = iter(lineList)


while True:
	base = int(next(iter).replace('h', ''), 16)
	name = next(iter); desc = name
	size = 2
	
	print '{%d, %d, "%s", "%s"},' % (base, size, name, desc)
	




