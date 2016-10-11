import random

max = 1000
maxn = 1000000
vec = []
for i in range(max):
	vec.append(random.randint(0,maxn))

print max,max
for i in range(max):
	print vec[i],

for i in range(max):
	a = random.randint(0,max-1)
	b = random.randint(a+1,max)
	print a,b