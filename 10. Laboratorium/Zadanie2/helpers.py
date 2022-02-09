def rand_test(num_tow=2, ind=(0, 10000), heights=(1, 100)):
	ind = random.sample(range(*ind), num_tow)
	print(num_tow)
	for i in ind:
		print(f'{random.randint(*heights)} {i}')
		
		
def check(s):
	nums = list(map(int, s.strip().split()))
	if nums[0] > 1:
		print('First wrong')
	for i in range(1, len(nums)):
		if nums[i] and nums[i - 1] or nums[i] > 1:
			print(f'A[{i}] = {nums[i]}, A[{i - 1}] = {A[i - 1]}')
			return False
	return True
	
