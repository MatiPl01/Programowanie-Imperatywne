def check(S, X):
	'''Check if S can be obtained by summing X in the way described'''
	if S == 0 and X != -1: return False
	if X > S: return False
	while X and S >= 0:
		S -= X
		X //= 10
	if S:
		return False
	return True


def res(n, m=None, full=True):
	''''Prints sums S and X values which correspond to them'''
	low = 0
	def update_lower_bound(num):
		nonlocal low
		if num < 100: return
		else:
			if not low: low = 91
			next_low = (low - 1) * 10 + 1
			if next_low <= num: low = next_low
	if m is None: m = n
	max_len = len(str(m))
	for S in range(n, m + 1):
		update_lower_bound(S)
		for X in range(low, S + 1):
			if check(S, X):
				if full: print(f'S: {str(S).ljust(max_len)} -> X: {X}')
				else: print(X)
				break
		else:
			if full: print(f'S: {str(S).ljust(max_len)} -> X: -1')
			else: print(-1)
			
			
def cmp(good, tested):
	for a, b in zip(good.strip().splitlines(), tested.strip().splitlines()):
		if a != b:
			print('WRONG!')
			print(f'Expected: {a}, Got: {b}')
			
			
def gen_res(n, m=None):
	''''Prints sums S and X values which correspond to them'''
	low = 0 if n < 100 else int(f'9{"0" * (len(str(n)) - 3)}1')
	result = []
	if m is None: m = n
	max_len = len(str(m))
	for S in range(n, m + 1):
		for X in range(low, S + 1):
			if check(S, X):
				low = X
				result.append(X)
				break
		else:
			result.append(-1)
	return result
			

def cmp2(n, m, tested):
	good = gen_res(n, m)
	is_ok = True
	for num, a, b in zip(range(n, m + 1), good, map(int, tested.strip().splitlines())):
		if a != b:
			print(f'WRONG! (for S = {num})')
			print(f'Expected: {a}, Got: {b}')
			is_ok = False
	if is_ok: print('All ok')
	
	
def yield_res(n, m=None):
	''''Prints sums S and X values which correspond to them'''
	low = 0 if n < 100 else int(f'9{"0" * (len(str(n)) - 3)}1')
	if m is None: m = n
	max_len = len(str(m))
	for S in range(n, m + 1):
		for X in range(low, S + 1):
			if check(S, X):
				low = X + 1
				yield X
				break
		else:
			yield -1


def res_file(n, m=None, fname='test.txt', *, path=r'F:\Edukacja\Studia\2. semestr\Programowanie Imperatywne\Laboratoria\8. Laboratorium\Zadanie1'):
	import os
	os.chdir(path)
	with open(fname, 'w') as f:
		f.write(f"{n} {m if m is not None else n}\n")
		for S in yield_res(n, m):
			f.write(f"{S}\n")


"""W drug¹ stronê - czytanie wyników z C i sprawdzanie w Pythonie"""

def check_results(filename='results.txt', *, dirpath=r'F:\Edukacja\Studia\2. semestr\Programowanie Imperatywne\Laboratoria\8. Laboratorium\Zadanie1', step=1_000):
	low = 0
	
	def check(S, X):
		nonlocal low
		if X == -1:
			if S == 0: return True
			if S >= 100: low = max(low, int(f"9{'0' * (len(str(S)) - 3)}1"))
			for i in range(low, S + 1):
				total = 0
				while i:
					total += i
					i //= 10
					if total > S: break
				else:
					if total == S: return False  # Wrong result as we can get this sum
			return True
		else:
			total = 0
			while X:
				total += X
				X //= 10
			return total == S
			
	import os
	os.chdir(dirpath)
	with open(filename, 'r') as f:
		print('Opened a file')
		while True:
			line = f.readline()
			if not line or line.isspace(): break
			S, X = map(int, line.strip().split())
			if not S % step:
				print('Now processing', S)
			if not check(S, X):
				print(f'WRONG! For S={S}: Got X={X}')
