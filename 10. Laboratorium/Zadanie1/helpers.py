""""
Poni¿sza funkcja oblicza, jak¹ kwotê otrzymamy, po¿yczaj¹c i oddaj¹c
pieni¹dze w kolejnych bankomatach, przekazanych w postaci ci¹gu tekstowego
numerów bankomatów, oddzielonych spacjami.
"""
def check(s):
	total = 0
	ind = list(map(int, s.strip().split()))
	for i in ind:
		total += (-1 if i % 2 else 1) * 2 ** i
	return total
	

# Przyk³ad:
check('1 3 5 6 7 9 10 11 12 14 16 22 24 25 28 29 30 32 33 37 38 41 47 50 52 53 54 55 56 57 62 63 64 65 66 69 70 72 73 74 75 76 77')  # -98528765376195985323626