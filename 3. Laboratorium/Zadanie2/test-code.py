import random
import os


os.chdir(r"F:\Edukacja\Studia\2. semestr\Programowanie Imperatywne\Laboratoria\3. Laboratorium\Zadanie2")


def generate_samples(length, k, range_=(0, 10_000)):
    lst = [random.randint(*range_) for _ in range(length)]
    counts = {}
    for val in lst:
        counts.setdefault(val, 0)
        counts[val] += 1
    total_pairs = 0
    matching = []
    for val, reps in counts.items():
        for i in range(val - k, val + k + 1):
            if i != val and i in counts:
                matching.extend([val] * reps)
                total_pairs += reps
                break
    with open('test-case.txt', 'w') as f:
        f.write(f'{length} {k}\n')
        f.write(' '.join(map(str, lst)))

    with open('test-case--sorted.txt', 'w') as f:
        f.write('=== Values: ===\n')
        for key in sorted(list(counts.keys())):
            f.write(f'{key}: {counts[key]}\n')
        f.write('\n')
        f.write(f'\nExpected result: {total_pairs}\n')
        f.write('\n\n\n=== Matching numbers (with pairs): ===\n')
        f.write(' '.join(map(str, sorted(matching))))
    return total_pairs


def comp(s1, s2):
    s1_vals = s1.strip().split()
    s2_vals = s2.strip().split()
    for v1, v2 in zip(s1_vals, s2_vals):
        if v1 != v2:
            print(f'WRONG: {v1}, {v2}')
            break
