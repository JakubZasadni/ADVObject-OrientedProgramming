# <Jakub Zasadni>, <415741>
from typing import List, Dict


def adjmat_to_adjlist(adjmat: List[List[int]]) -> Dict[int, List[int]]:
    num_vertices = len(adjmat)
    adjlist = {}

    for i in range(num_vertices):
        neighbors = []

        for j in range(num_vertices):

            for k in range(adjmat[i][j]):
                neighbors.append(j + 1)
        adjlist[i + 1] = neighbors

    return adjlist


def DFS_recursive(G: Dict[int, List[int]], v: int, visited: List[int]) -> List[int]:
    visited.append(v)
    if v in G:
        for u in G[v]:
            if u not in visited:
                DFS_recursive(G, u, visited)
    return visited

def dfs_recursive(G: Dict[int, List[int]], s: int) -> List[int]:
    return DFS_recursive(G, s, [])


def dfs_iterative(G: Dict[int, List[int]], s: int) -> List[int]:
    visited = []
    stack = [s]

    while stack:
        v = stack.pop()

        if v not in visited:
            visited.append(v)
            if v in G:
                for u in G[v]:
                    if u not in visited:
                        stack.append(u)

    return visited


def cyclic(G: Dict[int, List[int]], v: int, visited: List[int]) -> bool:
    visited.append(v)

    if v in G:
        for vertex in G[v]:
            if vertex in visited:
                return True
            elif cyclic(G, vertex, visited[:]):
                return True

    return False


def is_acyclic(G: Dict[int, List[int]]) -> bool:
    for vertex in G.keys():
        if cyclic(G, vertex, []):
            return False
    return True