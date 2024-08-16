from typing import List, Set, Dict, NamedTuple
import networkx as nx

VertexID = int
AdjList = Dict[VertexID, List[VertexID]]
Distance = int
EdgeID = int


class TrailSegmentEntry(NamedTuple):
    vertex_start: VertexID
    vertex_end: VertexID
    edge_id: EdgeID
    weight: float


Trail = List[TrailSegmentEntry]


def neighbors(adjlist: AdjList, start_vertex_id: VertexID, max_distance: Distance) -> Set[VertexID]:
    visited = set()
    queue = [(start_vertex_id, 0)]
    while queue:
        vertex_u, distance = queue.pop(0)
        if distance >= max_distance:
            return visited
        elif vertex_u in adjlist.keys():
            for vertex_v in adjlist[vertex_u]:
                if vertex_v not in visited:
                    visited.add(vertex_v)
                    queue.append((vertex_v, distance + 1))
    return visited


def load_multigraph_from_file(filepath: str) -> nx.MultiDiGraph:
    """Stwórz multigraf na podstawie danych o krawędziach wczytanych z pliku.

    :param filepath: względna ścieżka do pliku (wraz z rozszerzeniem)
    :return: multigraf
    """

    graph = nx.MultiDiGraph()
    with open(filepath) as data:
        for line in data:
            split = line.split()
            if split:
                graph.add_weighted_edges_from([(int(split[0]), int(split[1]), float(split[2]))])
    return graph


def find_min_trail(g: nx.MultiDiGraph, v_start: VertexID, v_end: VertexID) -> Trail:
    """Znajdź najkrótszą ścieżkę w grafie pomiędzy zadanymi wierzchołkami.

    :param g: graf
    :param v_start: wierzchołek początkowy
    :param v_end: wierzchołek końcowy
    :return: najkrótsza ścieżka
    """

    trail = []
    way = nx.dijkstra_path(g, v_start, v_end)
    connections = 0
    while connections < len(way) - 1:
        current_trail = []
        current_weight = 1.7e308
        for edge in range(len(g[way[connections]][way[connections + 1]])):
            if g[way[connections]][way[connections + 1]][edge]['weight'] < current_weight:
                current_weight = g[way[connections]][way[connections + 1]][edge]['weight']
                current_trail = TrailSegmentEntry(way[connections], way[connections + 1], edge, current_weight)
        trail.append(current_trail)
        connections += 1
    return trail


def trail_to_str(trail: Trail) -> str:
    """Wyznacz reprezentację tekstową ścieżki.

    :param trail: ścieżka
    :return: reprezentacja tekstowa ścieżki
    """

    weight_sum = 0.
    text = ''
    for step in trail:
        text += '{} -[{}: {}]-> '.format(step[0], step[2], step[3])
        weight_sum += step[3]
        if step == trail[-1]:
            text += '{}  (total = {})'.format(step[1], weight_sum)
    return text
