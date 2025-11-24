#include <iostream>
using namespace std;

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
    string nama[50];
    Link* sisi[50];
    int jumlah;
};

// ==============================
// INISIALISASI
// ==============================
void initGraph(Graph &g) {
    g.jumlah = 0;
    for (int i = 0; i < 50; i++)
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

    const int INF = 1e9;
    int key[50];
    bool sudah[50] = {false};
    int parent[50];

    for (int i = 0; i < g.jumlah; i++) {
        key[i] = INF;
        parent[i] = -1;
    }

    key[start] = 0;

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

    cout << "\nMST (Prim) dimulai dari " << mulai << ":\n";

    int total = 0;
    for (int i = 0; i < g.jumlah; i++) {
        if (parent[i] != -1) {
            cout << g.nama[parent[i]] << " - " << g.nama[i]
                 << " : " << key[i] << endl;
            total += key[i];
        }
    }

    cout << "Total bobot MST = " << total << "\n\n";
}

// ==============================
// MAIN
// ==============================
int main() {
    Graph g;
    initGraph(g);

    // TAMBAH VERTEX
    tambahVertex(g, "A");
    tambahVertex(g, "B");
    tambahVertex(g, "C");
    tambahVertex(g, "D");
    tambahVertex(g, "E");

    // TAMBAH EDGE UNDIRECTED
    tambahEdge(g, "A", "B", 4);
    tambahEdge(g, "A", "C", 3);
    tambahEdge(g, "B", "D", 5);
    tambahEdge(g, "C", "D", 2);
    tambahEdge(g, "D", "E", 7);

    // JALANKAN PRIM
    prim(g, "A");

    return 0;
}
