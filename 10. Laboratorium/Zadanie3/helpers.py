def directed_graph_list(E: 'array of edges', n: 'number of vertices'):
    G = [[] for _ in range(n)]
    for edge in E:
        G[edge[0]].append(edge[1])
    return G


def get_transposed_graph(G: 'graph represented using adjacency lists'):
    n = len(G)
    G2 = [[] for _ in range(n)]
    
    for u in range(n):
        for v in G[u]:
            G2[v].append(u)
            
    return G2


def g(s):
	ln = s.strip().splitlines()
	n = int(ln[0].split()[0])
	m = int(ln[0].split()[1])
	vert = [tuple(map(lambda v: v + n, map(int, e.split()))) for e in ln[1:]]
	E = [(vert[i][j], i + n + 1) for i in range(len(vert)) for j in range(2)]
	G = directed_graph_list(E, m + n + 1)
	print('Input graph:')
	print(*(f'{i}: {" ".join(map(str, lst))}' for i, lst in enumerate(G)), sep='\n')
	G2 = get_transposed_graph(G)
	print('\nTransposed graph:')
	print(*(f'{i}: {" ".join(map(str, lst))}' for i, lst in enumerate(G2)), sep='\n')
	

g('''3 6 2
6 6
3 4
1 5
-1 -2
-2 -2
-3 -3''')



from collections import deque


def directed_graph_list(E: 'array of edges', n: 'number of vertices'):
    G = [deque() for _ in range(n)]
    for edge in E:
        G[edge[0]].append(edge[1])
    return G


def transpose_graph(G: 'directed graph represented using adjacency lists'):
    n = len(G)
    # Store indices of the beginnings of new neighbours added to each vertex
    new_indices = [len(G[u]) for u in range(n)]
    
    # For each vertex remove its all old neighbours (vertices stored before the
    # first new added vertex) and for each of these removed neighbours append 
    # the current vertex to their new neighbours (this will be a reversed edge)
    for u in range(n):
        for _ in range(new_indices[u]):
            v = G[u].popleft()
            G[v].append(u)
        new_indices[u] = 0


def g2(s):
	ln = s.strip().splitlines()
	n = int(ln[0].split()[0])
	m = int(ln[0].split()[1])
	vert = [tuple(map(lambda v: v + n, map(int, e.split()))) for e in ln[1:]]
	E = [(vert[i][j], i + n + 1) for i in range(len(vert)) for j in range(2)]
	G = directed_graph_list(E, m + n + 1)
	print('Input graph:')
	print(*(f'{i}: {" ".join(map(str, lst))}' for i, lst in enumerate(G)), sep='\n')
	transpose_graph(G)
	print('\nTransposed graph:')
	print(*(f'{i}: {" ".join(map(str, lst))}' for i, lst in enumerate(G)), sep='\n')
	
	
def count(a, b, coefficients: 'string'):
	total = 0
	ind = [int(i) - 1 for i in '3 4 5 6 7 8 9 11 16 17 19 20 21 22 24 28 29 30'.split()]
	for i in range(a, b + 1):
		ones_count = 0
		for n in ind:
			ones_count += bool(i & (1 << n))
		total += ones_count % 2
	return total


def bin_sub(a, b):
	n = len(a)
	res = [int(d) for d in b]
	for i in range(n - 1, -1, -1):
		res[i] -= int(a[i])
		if res[i] < 0:
			res[i] += 2
			res[i - 1] -= 1
	return res
	