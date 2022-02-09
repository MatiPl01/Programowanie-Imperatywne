# Print expected results from an input (as a string - the same as will be passed to the C program input)
def test(s):
	arr = s.strip().splitlines()
	n, k, l = [int(v) for v in arr[0].split()]
	T = []
	for i in range(1, n + 1):
		row = [int(v) for v in arr[i].split()]
		T.append(row)
	mask = []
	for i in range(n + 1, n + 1 + k):
		row = [int(v) for v in arr[i].split()]
		mask.append(row)
	print('Values')
	print(*T, sep='\n')
	print('\nMask')
	print(*mask, sep='\n')
	max_sum = float('-inf')
	for i in range(n - k + 1):
		row = []
		for j in range(n - l + 1):
			curr_sum = 0
			for p in range(k):
				for r in range(l):
					if mask[p][r]:
						curr_sum += T[i + p][j + r]
			row.append(curr_sum)
			max_sum = max(max_sum, curr_sum)
		if not i % 2:
			print(*row, sep='\n')
		else:
			print(*row[::-1], sep='\n')
	print(max_sum)

# Generate test data
def random_data(n):
	arr = [[random.randint(-100, 100) for _ in range(n)] for _ in range(n)]
	k = random.randint(1, n)
	l = random.randint(1, n)
	mask = [[random.randint(0, 1) for _ in range(l)] for _ in range(k)]
	res = [f"{n} {k} {l}"]
	for row in arr:
		res.append(' '.join(map(str, row)))
	for row in mask:
		res.append(' '.join(map(str, row)))
	return '\n'.join(res)

# Print test data and expected results
def t(n):
	string = random_data(n)
	print(string)
	print('\n')
	test(string)

# Differentiate outputs (model output (from naive Python's solution) and program output)
def d(s1, s2):
	return all(x == y for x, y in zip(s1.strip().splitlines(), s2.strip().splitlines()))
