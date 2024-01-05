from graph import AdjacentVertex
from graph import Graph
import sys


class Graph2(Graph):
    def min_number_edges(self, start: str, end: str) -> int:
        """Returns the minimum number of edges from start to end."""

        if start not in self._vertices or end not in self._vertices or end == start:
            return 0

        # Dictionaries for dijkstra
        visited = {}
        for v in self._vertices.keys():
            visited[v] = False

        previous = {}
        for v in self._vertices.keys():
            previous[v] = None

        distances = {}
        for v in self._vertices.keys():
            distances[v] = sys.maxsize
        distances[start] = 0

        for x in range(len(self._vertices)):
            # This function picks the vertex with the minimum distance
            current_vertex = self.min_distance(distances, visited)

            # Marks the vertex as visited for not repeating it
            visited[current_vertex] = True

            # We visit all adjacent vertices and update its distance
            for adj in self._vertices[current_vertex]:
                i = adj.vertex
                w = adj.weight
                if visited[i] == False and distances[i] > distances[current_vertex] + w:
                    distances[i] = distances[current_vertex] + w
                    previous[i] = current_vertex

        # Checks first if the distance is infinite and changes it to 0
        if distances[end] == sys.maxsize:
            return 0
        else:
            return distances[end]

    def min_distance(self, distances, visited):
        """This functions returns the vertex whose distance is lower."""

        # Initialize minimum distance for next node
        min = sys.maxsize

        # Returns the vertex with minimum distance from the non-visited vertices
        for vertex in self._vertices.keys():
            if distances[vertex] <= min and visited[vertex] == False:
                min = distances[vertex]  # update the new smallest
                min_vertex = vertex  # update the index of the smallest

        return min_vertex

    def transpose(self) -> 'Graph2':
        """Returns a new graph that is the transpose of itself."""

        if not self._vertices.keys():
            return None

        if self._directed:
            # It creates the new graph and goes through all the vertices
            transpose = Graph2(self._vertices.keys())

            for v in transpose._vertices.keys():
                # Checks all the values of the different keys if they are equal to v
                for a in self._vertices[v]:
                    aux = a.vertex
                    transpose.add_edge(aux, v)
            return transpose

        return self

    def is_strongly_connected(self) -> bool:
        """This function checks if the graph is strongly connected.
        A directed graph is strongly connected when for any
        pair of vertices u and v, there is always a path from u to v.
        If the graph is undirected, the function returns True if the graph is
        connected, that is, there is a path from any vertex to any other vertex
        in the graph."""

        # Checks the option of a graph that isn't connected or is empty
        if not self._vertices.keys():
            return False

        for v in self._vertices.keys():
            if not self._vertices[v]:
                return False

        connected = True
        i = 0

        # Proves that all the vertices are connected
        while connected == True and i < len(self._vertices.keys()):
            vertices = list(self._vertices.keys())
            connected = self.bfs(vertices[i])
            i += 1

        if connected == 1:
            return True
        return False

    def bfs(self, v):
        # Dictionary for visited vertices
        visited = {}
        for a in self._vertices.keys():
            visited[a] = False

        queue = []
        visited[v] = True
        queue.append(v)

        # When the queue is empty, that means that we have check all the vertices
        while queue:
            for i in self._vertices[queue[0]]:
                if visited[i.vertex] == False:
                    queue.append(i.vertex)
                    visited[i.vertex] = True
            queue.pop(0)

        # If the dictionary visited is totally true, it returns true for the variable connected
        for v in self._vertices.keys():
            if visited[v] != True:
                return False

        return True
