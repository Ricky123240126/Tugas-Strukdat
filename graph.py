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

# djikstra (mencari shortest path tetapi tidak dapat mengatasi bobot/weight negatif)
def dijkstra(graph, start, end=None):
    # Jika start tidak ada di graph
    if start not in graph.adj_list:
        return None

    # Inisialisasi jarak awal
    jarak = {v: float("inf") for v in graph.adj_list}
    jarak[start] = 0

    # Prioritas queue (minimal-heap)
    heap = [(0, start)]

    # Proses Dijkstra
    while heap:
        curr_dist, node = heapq.heappop(heap)

        # Skip node yang jaraknya sudah bukan paling kecil
        if curr_dist > jarak[node]:
            continue

        # Jika mencari 1 tujuan 
        if end is not None and node == end:
            return jarak[end]

        temp = graph.adj_list[node]
        while temp:
            new_dist = curr_dist + temp.weight

            if new_dist < jarak[temp.vertex]:
                jarak[temp.vertex] = new_dist
                heapq.heappush(heap, (new_dist, temp.vertex))

            temp = temp.next

    # Jika end = None 
    if end is None:
        return jarak

    # Jika end ada tetapi tidak terhubung
    return float("inf")

# prim (Mencari pohon rentang minimum / minimum spanning stree dari graph berbobot)
def prim(graph, start):
    if start not in graph.adj_list:
        return None
    
    visited = set([start])
    edges = []
    mst_edges = []
    total_weight = 0
    
    # Ambil semua edge dari start
    temp = graph.adj_list[start]
    while temp:
        heapq.heappush(edges, (temp.weight, start, temp.vertex))
        temp = temp.next
    
    while edges and len(visited) < len(graph.adj_list):
        weight, u, v = heapq.heappop(edges)
        
        if v not in visited:
            visited.add(v)
            mst_edges.append((u, v, weight))
            total_weight += weight
            
            # Tambahkan edge dari v
            temp = graph.adj_list[v]
            while temp:
                if temp.vertex not in visited:
                    heapq.heappush(edges, (temp.weight, v, temp.vertex))
                temp = temp.next
    
    return mst_edges, total_weight

# kruskal (Algoritma lain untuk minimum spanning tree menggunakan Union-Find)
def kruskal(graph):
    # Kumpulkan semua edge
    edges = []
    for u in graph.adj_list:
        temp = graph.adj_list[u]
        while temp:
            # Hindari duplikat untuk undirected graph
            if u < temp.vertex:
                edges.append((temp.weight, u, temp.vertex))
            temp = temp.next
    
    # Sort berdasarkan weight
    edges.sort()
    
    # Union-Find
    parent = {v: v for v in graph.adj_list}
    
    def find(x):
        if parent[x] != x:
            parent[x] = find(parent[x])
        return parent[x]
    
    def union(x, y):
        root_x = find(x)
        root_y = find(y)
        if root_x != root_y:
            parent[root_x] = root_y
            return True
        return False
    
    mst_edges = []
    total_weight = 0
    
    for weight, u, v in edges:
        if union(u, v):
            mst_edges.append((u, v, weight))
            total_weight += weight
    
    return mst_edges, total_weight

# bellman-ford (mencari shortest path tetapi dapat mengatasi bobot/weight negatif)
def bellman_ford(graph, start):
    if start not in graph.adj_list:
        return None
    
    # Inisialisasi jarak
    jarak = {v: float("inf") for v in graph.adj_list}
    jarak[start] = 0
    
    # Relaksasi |V|-1 kali
    for _ in range(len(graph.adj_list) - 1):
        for u in graph.adj_list:
            temp = graph.adj_list[u]
            while temp:
                if jarak[u] != float("inf") and jarak[u] + temp.weight < jarak[temp.vertex]:
                    jarak[temp.vertex] = jarak[u] + temp.weight
                temp = temp.next
    
    # Deteksi negative cycle
    for u in graph.adj_list:
        temp = graph.adj_list[u]
        while temp:
            if jarak[u] != float("inf") and jarak[u] + temp.weight < jarak[temp.vertex]:
                return None, "Negative cycle terdeteksi!"
            temp = temp.next
    
    return jarak, None

# Floyd-warshall (Mencari shortest path antara semua pasangan vertex)
def floyd_warshall(graph):
    vertices = list(graph.adj_list.keys())
    n = len(vertices)
    
    # Inisialisasi matrix jarak
    dist = {v: {u: float("inf") for u in vertices} for v in vertices}
    
    # Jarak ke diri sendiri = 0
    for v in vertices:
        dist[v][v] = 0
    
    # Isi jarak dari edge yang ada
    for u in graph.adj_list:
        temp = graph.adj_list[u]
        while temp:
            dist[u][temp.vertex] = temp.weight
            temp = temp.next
    
    # Floyd-Warshall
    for k in vertices:
        for i in vertices:
            for j in vertices:
                if dist[i][k] + dist[k][j] < dist[i][j]:
                    dist[i][j] = dist[i][k] + dist[k][j]
    
    return dist

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
8. Djikstra
9. Prim
10. Kruskal
11. Bellman-Ford
12. Floyd-warshall
0. Exit
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
            start = input("Mulai dari vertex: ")
            hasil = prim(g, start)
            if hasil is None:
                print("Vertex tidak ditemukan!\n")
            else:
                edges, total = hasil
                print("Prim's MST:", edges)
                print("Total weight:", total, "\n")

        elif pilihan == "10":
            hasil = kruskal(g)
            edges, total = hasil
            print("Kruskal's MST:", edges)
            print("Total weight:", total, "\n")

        elif pilihan == "11":
            start = input("Mulai dari vertex: ")
            hasil, error = bellman_ford(g, start)
            if error:
                print(error, "\n")
            elif hasil is None:
                print("Vertex tidak ditemukan!\n")
            else:
                print("Bellman-Ford:", hasil, "\n")

        elif pilihan == "12":
            hasil = floyd_warshall(g)
            print("\n=== Floyd-Warshall (All Pairs) ===")
            for u in hasil:
                print(f"Dari {u}: {hasil[u]}")
            print()

        elif pilihan == "0":
            print("Program selesai.")
            break

        else:
            print("Pilihan tidak valid!\n")


menu()
