import graphs_2
import networkx as nx


graph = nx.MultiDiGraph()
graph.add_weighted_edges_from([(1, 3, 1.0)])
graph.add_weighted_edges_from([(1, 2, 0.5)])
graph.add_weighted_edges_from([(2, 3, 0.4)])
graph.add_weighted_edges_from([(2, 3, 0.3)])
graph.add_weighted_edges_from([(2, 4, 0.3)])
graph.add_weighted_edges_from([(2, 4, 0.3)])
graph.add_weighted_edges_from([(4, 5, 0.1)])
graph.add_weighted_edges_from([(5, 3, 0.1)])
graph.add_weighted_edges_from([(3, 5, 0.2)])
graph.add_weighted_edges_from([(5, 6, 0.2)])

for n in range(1, 7):
    for k in range(1, 7):
        if graph.has_edge(n, k):
            trail = graphs_2.find_min_trail(graph, n, k)
            weight_sum = 0.0
            for segment in trail:
                weight_sum += segment[3]
            if weight_sum == nx.dijkstra_path_length(graph, n, k):
                print('Test dla wierzchołków od {} do {} się powiódł'.format(n, k))
            else:
                print('Test dla wierzchołków od {} do {} się nie powiódł'.format(n, k))
