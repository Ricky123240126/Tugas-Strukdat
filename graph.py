from collections import deque
import heapq

class Node:
    def __init__(self, vertex, weight=1):
        self.vertex = vertex
        self.weight = weight
        self.next = None

class Graph:
    def __init__(self):
        self.adj_list = {}

    def add_vertex(self, vertex):
        if vertex not in self.adj_list:
            self.adj_list[vertex] = None

    def add_edge(self, u, v, w=1, undirected=True):
        self.add_vertex(u)
        self.add_vertex(v)

        new_node = Node(v, w)
        new_node.next = self.adj_list[u]
        self.adj_list[u] = new_node

        if undirected:
            new_node2 = Node(u, w)
            new_node2.next = self.adj_list[v]
            self.adj_list[v] = new_node2

    def delete_from_list(self, src, target):
        head = self.adj_list.get(src)

        if head is None:
            return
        
        # Jika yang dihapus adalah node pertama (head)
        if head.vertex == target:
            self.adj_list[src] = head.next   
            return
        
        # Jika node berada di tengah/belakang
        prev = head
        current = head.next
        while current:
            if current.vertex == target:
                prev.next = current.next
                return
            prev = current
            current = current.next

        
    def delete_edge(self, u, v, undirected = True):
        self.delete_from_list(u, v)

        if undirected:
            self.delete_from_list(v, u)

    def delete_vertex(self, v):

        if v not in self.adj_list:
            print("Vertex tidak ditemukan!")
            return

        # Hapus vertex sebagai key
        del self.adj_list[v]

        # Hapus vertex dari adjacency list lain
        for u in self.adj_list:
            self.delete_from_list(u, v)   

        print(f"Vertex '{v}' berhasil dihapus.\n")



    def display(self):
        print("\n=== GRAPH (Linked List) ===")
        for vertex in self.adj_list:
            print(f"{vertex} -> ", end="") 
            temp = self.adj_list[vertex]
            while temp:
                print(f"{temp.vertex}(w={temp.weight}) , ", end="")
                temp = temp.next
            print("NULL")
        print()


# BFS
def bfs(graph, start):
    if start not in graph.adj_list:
        return None

    visited = set()
    queue = deque([start])
    hasil = []

    while queue:
        node = queue.popleft()
        if node not in visited:
            visited.add(node)
            hasil.append(node)

            temp = graph.adj_list[node]
            while temp:
                if temp.vertex not in visited:
                    queue.append(temp.vertex)
                temp = temp.next

    return hasil


# DFS
def dfs(graph, start):
    if start not in graph.adj_list:
        return None

    visited = set()
    hasil = []

    def _dfs(node):
        if node not in visited:
            visited.add(node)
            hasil.append(node)

            temp = graph.adj_list[node]
            while temp:
                _dfs(temp.vertex)
                temp = temp.next

    _dfs(start)
    return hasil


# Dijkstra
def dijkstra(graph, start):
    if start not in graph.adj_list:
        return None

    jarak = {v: float("inf") for v in graph.adj_list}
    jarak[start] = 0

    heap = [(0, start)]

    while heap:
        curr_dist, node = heapq.heappop(heap)

        if curr_dist > jarak[node]:
            continue

        temp = graph.adj_list[node]
        while temp:
            new_dist = curr_dist + temp.weight
            if new_dist < jarak[temp.vertex]:
                jarak[temp.vertex] = new_dist
                heapq.heappush(heap, (new_dist, temp.vertex))
            temp = temp.next

    return jarak


def menu():
    g = Graph()

    while True:
        print("""
=============================
        MENU GRAPH
=============================
1. Tambah Vertex
2. Tambah Edge
3. Hapus Vertex
4. Hapus Edge
5. Tampilkan Graph
6. BFS
7. DFS
8. Dijkstra
9. Exit
=============================
""")

        pilihan = input("Pilih menu: ")

        if pilihan == "1":
            v = input("Masukkan nama vertex: ")
            g.add_vertex(v)
            print(f"Vertex '{v}' berhasil ditambahkan.\n")

        elif pilihan == "2":
            u = input("Vertex asal: ")
            v = input("Vertex tujuan: ")
            w = int(input("Bobot (weight): "))
            g.add_edge(u, v, w)
            print("Edge berhasil ditambahkan!\n")

        elif pilihan == "3":
            v = input("vertex yang ingin dihapus: ")
            g.delete_vertex(v)

        elif pilihan == "4":
            u = input("hapus edge dari vertex: ")
            v = input("menuju vertex: ")
            g.delete_edge(u, v)
            print(f"edge antara vertex '{u} dan vertex '{v}' berhasil dihapus\n")

        elif pilihan == "5":
            g.display()

        elif pilihan == "6":
            start = input("Mulai dari vertex: ")
            hasil = bfs(g, start)
            if hasil is None:
                print("Vertex tidak ditemukan!\n")
            else:
                print("BFS:", hasil, "\n")

        elif pilihan == "7":
            start = input("Mulai dari vertex: ")
            hasil = dfs(g, start)
            if hasil is None:
                print("Vertex tidak ditemukan!\n")
            else:
                print("DFS:", hasil, "\n")

        elif pilihan == "8":
            start = input("Mulai dari vertex: ")
            hasil = dijkstra(g, start)
            if hasil is None:
                print("Vertex tidak ditemukan!\n")
            else:
                print("Dijkstra:", hasil, "\n")

        elif pilihan == "9":
            print("Program selesai.")
            break

        else:
            print("Pilihan tidak valid!\n")


menu()
