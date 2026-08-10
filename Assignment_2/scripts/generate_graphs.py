"""
Generates connected random graphs at the assignment's required sizes
(10, 100, 10000, 50000, 100000 vertices) for BFS/DFS (unweighted) and
SSSP (weighted, positive weights only).

Construction: build a random spanning tree first (guarantees every vertex
is reachable from the source, so BFS/DFS/SSSP results are meaningful and
not full of INF), then add extra random edges to reach the target average
degree. Graph is undirected: each edge is added to both endpoints'
adjacency lists, and E is reported as the number of distinct undirected
edges (per the assignment's counting rule).
"""
import random

SOURCE = 0

def generate_connected_graph(V, avg_degree, seed):
    rng = random.Random(seed)
    edges = set()

    # 1. Random spanning tree: guarantees connectivity from SOURCE.
    order = list(range(V))
    rng.shuffle(order)
    # make sure SOURCE participates in the tree from the start
    if order[0] != SOURCE:
        order.remove(SOURCE)
        order.insert(0, SOURCE)

    for i in range(1, V):
        u = order[i]
        v = order[rng.randint(0, i - 1)]
        a, b = min(u, v), max(u, v)
        edges.add((a, b))

    # 2. Extra random edges up to target average degree.
    target_edges = max(V - 1, (avg_degree * V) // 2)
    attempts = 0
    max_attempts = target_edges * 10 + 1000
    while len(edges) < target_edges and attempts < max_attempts:
        u = rng.randint(0, V - 1)
        v = rng.randint(0, V - 1)
        attempts += 1
        if u == v:
            continue
        a, b = min(u, v), max(u, v)
        edges.add((a, b))

    adj = [[] for _ in range(V)]
    for (a, b) in edges:
        adj[a].append(b)
        adj[b].append(a)

    return adj, len(edges)


def write_unweighted(path, V, adj, E, source=SOURCE):
    with open(path, "w") as f:
        f.write(f"{V} {E}\n")
        for u in range(V):
            nbrs = adj[u]
            f.write(f"{u} {len(nbrs)} " + " ".join(map(str, nbrs)) + "\n" if nbrs
                     else f"{u} 0\n")
        f.write(f"SOURCE {source}\n")


def write_weighted(path, V, adj, E, seed, source=SOURCE):
    rng = random.Random(seed + 1)
    with open(path, "w") as f:
        f.write(f"{V} {E}\n")
        for u in range(V):
            nbrs = adj[u]
            if not nbrs:
                f.write(f"{u} 0\n")
                continue
            parts = []
            for v in nbrs:
                w = rng.randint(1, 20)  # positive weight, 1..20
                parts.append(f"{v} {w}")
            f.write(f"{u} {len(nbrs)} " + " ".join(parts) + "\n")
        f.write(f"SOURCE {source}\n")


if __name__ == "__main__":
    import sys
    sizes = [10, 100, 10000, 50000, 100000]
    avg_degree = 4

    for V in sizes:
        seed = 1000 + V
        adj, E = generate_connected_graph(V, avg_degree, seed)

        write_unweighted(f"tests/bfs_{V}.txt", V, adj, E)
        write_unweighted(f"tests/dfs_{V}.txt", V, adj, E)
        write_weighted(f"tests/sssp_{V}.txt", V, adj, E, seed)

        print(f"V={V:>7}  E={E:>8}  -> bfs_{V}.txt, dfs_{V}.txt, sssp_{V}.txt")
