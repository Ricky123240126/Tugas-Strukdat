#include <iostream>
using namespace std;

struct Edge {
    int u, v, bobot; //u = asal, v = tujuan
};

const int MAX = 50;

void bellmanFord(Edge edges[], int jumlahEdge, string nama[],
                 int jumlahSimpul, string start, string end) {

    int s = -1, t = -1;
    for (int i = 0; i < jumlahSimpul; i++) {
        if (nama[i] == start) s = i;
        if (nama[i] == end) t = i;
    }

    if (s == -1 || t == -1) {
        cout << "Vertex tidak ditemukan!\n";
        return;
    }

    int jarak[MAX];
    int parent[MAX];

    for (int i = 0; i < jumlahSimpul; i++) {
        jarak[i] = 1e9;
        parent[i] = -1;
    }

    jarak[s] = 0;

    // Relax edges sebanyak (V-1) kali
    for (int i = 1; i < jumlahSimpul; i++) {
        for (int j = 0; j < jumlahEdge; j++) {
            int u = edges[j].u;
            int v = edges[j].v;
            int w = edges[j].bobot;

            if (jarak[u] != 1e9 && jarak[u] + w < jarak[v]) {
                jarak[v] = jarak[u] + w;
                parent[v] = u;
            }
        }
    }

    // Tampilkan jarak ke semua vertex
    cout << "\n=== JARAK DARI " << start << " ===\n";
    for (int i = 0; i < jumlahSimpul; i++) {
        cout << start << " -> " << nama[i] << " = ";
        if (jarak[i] == 1e9)
            cout << "TIDAK TERHUBUNG\n";
        else
            cout << jarak[i] << endl;
    }

    // Tampilkan shortest path ke vertex tujuan
    if (jarak[t] == 1e9) {
        cout << "\nTidak ada jalur dari " << start << " ke " << end << "\n";
        return;
    }

    cout << "\n=== SHORTEST PATH: " << start << " -> " << end << " ===\n";
    cout << "Total jarak: " << jarak[t] << "\n";
    cout << "Jalur: ";

    // Reconstruct path (tanpa vector)
    int path[MAX];
    int pathLen = 0;
    int curr = t;
    
    while (curr != -1) {
        path[pathLen++] = curr;
        curr = parent[curr];
    }

    // Print path (reversed)
    for (int i = pathLen - 1; i >= 0; i--) {
        cout << nama[path[i]];
        if (i > 0) cout << " -> ";
    }
    cout << "\n\n";
}

int main() {
    string nama[MAX];
    int jumlahSimpul;

    cout << "=== ALGORITMA BELLMAN-FORD ===\n";
    cout << "Masukkan jumlah vertex: ";
    cin >> jumlahSimpul;

    cout << "Masukkan nama vertex:\n";
    for (int i = 0; i < jumlahSimpul; i++) {
        cout << "Vertex " << (i + 1) << ": ";
        cin >> nama[i];
    }

    Edge edges[MAX];
    int jumlahEdge;
    
    cout << "\nMasukkan jumlah edge: ";
    cin >> jumlahEdge;

    cout << "Masukkan edge (format: vertex1 vertex2 bobot):\n";
    for (int i = 0; i < jumlahEdge; i++) {
        string a, b;
        int w;
        cout << "Edge " << (i + 1) << ": ";
        cin >> a >> b >> w;

        int u = -1, v = -1;
        for (int j = 0; j < jumlahSimpul; j++) {
            if (nama[j] == a) u = j;
            if (nama[j] == b) v = j;
        }

        if (u == -1 || v == -1) {
            cout << "Vertex tidak ditemukan! Ulangi input.\n";
            i--;
            continue;
        }

        edges[i].u = u;
        edges[i].v = v;
        edges[i].bobot = w;
    }

    string start, end;
    cout << "\nMasukkan vertex awal: ";
    cin >> start;
    cout << "Masukkan vertex tujuan: ";
    cin >> end;

    bellmanFord(edges, jumlahEdge, nama, jumlahSimpul, start, end);

    return 0;
}