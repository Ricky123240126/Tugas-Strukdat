#include <iostream>
using namespace std;

const int MAX = 50;
const int INF = 1e9;

// ==============================
// NODE untuk adjacency list
// ==============================
struct Link {
    int tetangga;
    int bobot;
    Link* next;
};

// ==============================
// STRUCT GRAPH
// ==============================
struct Graph {
    string nama[MAX];
    Link* sisi[MAX];
    int jumlah;
};

// ==============================
// INISIALISASI
// ==============================
void initGraph(Graph &g) {
    g.jumlah = 0;
    for (int i = 0; i < MAX; i++)
        g.sisi[i] = nullptr;
}

// ==============================
// CARI INDEX VERTEX
// ==============================
int getIndex(Graph &g, string v) {
    for (int i = 0; i < g.jumlah; i++)
        if (g.nama[i] == v) return i;
    return -1;
}

// ==============================
// TAMBAH VERTEX
// ==============================
void tambahVertex(Graph &g, string v) {
    if (getIndex(g, v) != -1) {
        cout << "Vertex sudah ada!\n";
        return;
    }
    g.nama[g.jumlah] = v;
    g.sisi[g.jumlah] = nullptr;
    g.jumlah++;
}

// ==============================
// TAMBAH EDGE (UNDIRECTED)
// ==============================
void tambahEdge(Graph &g, string a, string b, int w) {
    int u = getIndex(g, a);
    int v = getIndex(g, b);

    if (u == -1 || v == -1) {
        cout << "Vertex tidak ditemukan!\n";
        return;
    }

    // a -> b
    Link* baru1 = new Link{v, w, g.sisi[u]};
    g.sisi[u] = baru1;

    // b -> a
    Link* baru2 = new Link{u, w, g.sisi[v]};
    g.sisi[v] = baru2;
}

// ==============================
// PRIM
// ==============================
void prim(Graph &g, string mulai) {
    int start = getIndex(g, mulai);
    if (start == -1) {
        cout << "Vertex tidak ditemukan!\n";
        return;
    }

    int key[MAX];
    bool sudah[MAX] = {false};
    int parent[MAX];

    for (int i = 0; i < g.jumlah; i++) {
        key[i] = INF;
        parent[i] = -1;
    }

    key[start] = 0;

    struct MSTEdge {
        int u, v, bobot;
    };
    MSTEdge mstEdges[MAX];
    int mstCount = 0;

    for (int step = 0; step < g.jumlah; step++) {
        // pilih vertex dengan key terkecil
        int u = -1;
        int minKey = INF;
        for (int i = 0; i < g.jumlah; i++) {
            if (!sudah[i] && key[i] < minKey) {
                minKey = key[i];
                u = i;
            }
        }

        if (u == -1) break;

        sudah[u] = true;

        if (parent[u] != -1) {
            mstEdges[mstCount++] = {parent[u], u, key[u]};
        }

        Link* temp = g.sisi[u];
        while (temp != nullptr) {
            int v = temp->tetangga;
            int w = temp->bobot;

            if (!sudah[v] && w < key[v]) {
                key[v] = w;
                parent[v] = u;
            }

            temp = temp->next;
        }
    }

    cout << "\n=== MST (PRIM) dimulai dari " << mulai << " ===\n";

    int total = 0;
    for (int i = 0; i < g.jumlah; i++) {
        if (parent[i] != -1) {
            cout << g.nama[parent[i]] << " - " << g.nama[i]
                 << " : " << key[i] << endl;
            total += key[i];
        }
    }

    cout << "\nTotal bobot MST = " << total << "\n";

    // Shortest path dalam MST
    cout << "\n=== SHORTEST PATH DALAM MST ===\n";
    string endVertex;
    cout << "Masukkan vertex tujuan: ";
    cin >> endVertex;

    int t = getIndex(g, endVertex);
    if (t == -1) {
        cout << "Vertex tidak ditemukan!\n";
        return;
    }

    // BFS untuk cari path di MST
    int queue[MAX];
    int front = 0, rear = 0;
    bool visited[MAX] = {false};
    int pathParent[MAX];
    
    for (int i = 0; i < g.jumlah; i++)
        pathParent[i] = -1;

    queue[rear++] = start;
    visited[start] = true;

    while (front < rear) {
        int curr = queue[front++];

        if (curr == t) break;

        for (int i = 0; i < mstCount; i++) {
            int next = -1;
            if (mstEdges[i].u == curr && !visited[mstEdges[i].v]) {
                next = mstEdges[i].v;
            } else if (mstEdges[i].v == curr && !visited[mstEdges[i].u]) {
                next = mstEdges[i].u;
            }

            if (next != -1) {
                visited[next] = true;
                pathParent[next] = curr;
                queue[rear++] = next;
            }
        }
    }

    if (!visited[t]) {
        cout << "Tidak ada jalur dari " << mulai << " ke " << endVertex << " dalam MST\n";
        return;
    }

    // Reconstruct path
    int path[MAX];
    int pathLen = 0;
    int curr = t;
    
    while (curr != -1) {
        path[pathLen++] = curr;
        curr = pathParent[curr];
    }

    // Hitung total bobot
    int pathWeight = 0;
    cout << "Jalur: ";
    for (int i = pathLen - 1; i >= 0; i--) {
        cout << g.nama[path[i]];
        if (i > 0) {
            cout << " -> ";
            // Cari bobot edge
            for (int j = 0; j < mstCount; j++) {
                if ((mstEdges[j].u == path[i] && mstEdges[j].v == path[i-1]) ||
                    (mstEdges[j].v == path[i] && mstEdges[j].u == path[i-1])) {
                    pathWeight += mstEdges[j].bobot;
                    break;
                }
            }
        }
    }
    cout << "\nTotal jarak: " << pathWeight << "\n";
}

// ==============================
// MAIN
// ==============================
int main() {
    Graph g;
    initGraph(g);

    int jumlahVertex, jumlahEdge;

    cout << "=== ALGORITMA PRIM (MST) ===\n";
    cout << "Masukkan jumlah vertex: ";
    cin >> jumlahVertex;

    cout << "Masukkan nama vertex:\n";
    for (int i = 0; i < jumlahVertex; i++) {
        string nama;
        cout << "Vertex " << (i + 1) << ": ";
        cin >> nama;
        tambahVertex(g, nama);
    }

    cout << "\nMasukkan jumlah edge: ";
    cin >> jumlahEdge;

    cout << "Masukkan edge (format: vertex1 vertex2 bobot):\n";
    for (int i = 0; i < jumlahEdge; i++) {
        string a, b;
        int w;
        cout << "Edge " << (i + 1) << ": ";
        cin >> a >> b >> w;
        tambahEdge(g, a, b, w);
    }

    string start;
    cout << "\nMasukkan vertex awal: ";
    cin >> start;

    prim(g, start);

    return 0;
}