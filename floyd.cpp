#include <iostream>
using namespace std;

const int MAX = 50;
const int INF = 1e9;

void printPath(int next[][MAX], string nama[], int i, int j) {
    if (next[i][j] == -1) {
        cout << "Tidak ada jalur";
        return;
    }
    
    int path[MAX];
    int pathLen = 0;
    int curr = i;
    path[pathLen++] = curr;
    
    while (curr != j) {
        curr = next[curr][j];
        path[pathLen++] = curr;
    }
    
    for (int k = 0; k < pathLen; k++) {
        cout << nama[path[k]];
        if (k < pathLen - 1) cout << " -> ";
    }
}

void floydWarshall(string nama[], int jumlah, int dist[][MAX]) {
    int next[MAX][MAX];
    
    // Inisialisasi next matrix untuk path reconstruction
    for (int i = 0; i < jumlah; i++) {
        for (int j = 0; j < jumlah; j++) {
            if (i != j && dist[i][j] != INF)
                next[i][j] = j;
            else
                next[i][j] = -1;
        }
    }

    // Algoritma Floyd-Warshall
    cout << "\n=== PROSES FLOYD-WARSHALL ===\n";
    cout << "Mencari shortest path untuk semua pasangan vertex...\n";
    
    for (int k = 0; k < jumlah; k++) {
        for (int i = 0; i < jumlah; i++) {
            for (int j = 0; j < jumlah; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        next[i][j] = next[i][k];
                    }
                }
            }
        }
    }

    // Tampilkan hasil lengkap semua pasangan
    cout << "\n=== HASIL FLOYD-WARSHALL ===\n";
    cout << "Shortest path untuk SEMUA pasangan vertex:\n";
    cout << "============================================\n";
    
    for (int i = 0; i < jumlah; i++) {
        for (int j = 0; j < jumlah; j++) {
            if (i == j) continue;
            
            cout << "\n" << nama[i] << " -> " << nama[j] << " : ";
            
            if (dist[i][j] == INF) {
                cout << "TIDAK TERHUBUNG";
            } else {
                cout << "jarak = " << dist[i][j];
                cout << "\n  Jalur: ";
                printPath(next, nama, i, j);
            }
        }
        cout << "\n--------------------------------------------";
    }

    // Pilih pasangan vertex tertentu untuk ditampilkan detail
    cout << "\n\n=== CARI SHORTEST PATH SPESIFIK ===\n";
    string start, end;
    cout << "Masukkan vertex awal: ";
    cin >> start;
    cout << "Masukkan vertex tujuan: ";
    cin >> end;

    int s = -1, t = -1;
    for (int i = 0; i < jumlah; i++) {
        if (nama[i] == start) s = i;
        if (nama[i] == end) t = i;
    }

    if (s == -1 || t == -1) {
        cout << "\nVertex tidak ditemukan!\n";
        return;
    }

    cout << "\n=== HASIL PENCARIAN ===\n";
    cout << "Dari: " << start << "\n";
    cout << "Ke  : " << end << "\n";
    
    if (dist[s][t] == INF) {
        cout << "Status: TIDAK TERHUBUNG\n";
    } else {
        cout << "Jarak total: " << dist[s][t] << "\n";
        cout << "Jalur: ";
        printPath(next, nama, s, t);
        cout << "\n";
    }
}

int main() {
    string nama[MAX];
    int jumlah;
    int dist[MAX][MAX];

    cout << "================================================\n";
    cout << "     ALGORITMA FLOYD-WARSHALL                  \n";
    cout << "     (All Pairs Shortest Path)                 \n";
    cout << "================================================\n";
    
    cout << "\nMasukkan jumlah vertex: ";
    cin >> jumlah;

    cout << "\nMasukkan nama setiap vertex:\n";
    for (int i = 0; i < jumlah; i++) {
        cout << "  Vertex ke-" << (i + 1) << ": ";
        cin >> nama[i];
    }

    // Inisialisasi matrix jarak
    cout << "\nMenginisialisasi matrix jarak...\n";
    for (int i = 0; i < jumlah; i++) {
        for (int j = 0; j < jumlah; j++) {
            if (i == j) 
                dist[i][j] = 0;
            else 
                dist[i][j] = INF;
        }
    }

    int jumlahEdge;
    cout << "\nMasukkan jumlah edge: ";
    cin >> jumlahEdge;

    cout << "\nMasukkan data edge (format: vertex1 vertex2 bobot)\n";
    for (int i = 0; i < jumlahEdge; i++) {
        string a, b;
        int w;
        cout << "  Edge ke-" << (i + 1) << ": ";
        cin >> a >> b >> w;

        int u = -1, v = -1;
        for (int j = 0; j < jumlah; j++) {
            if (nama[j] == a) u = j;
            if (nama[j] == b) v = j;
        }

        if (u == -1 || v == -1) {
            cout << "  ERROR: Vertex tidak ditemukan! Silakan input ulang.\n";
            i--;
            continue;
        }

        dist[u][v] = w;
        dist[v][u] = w; // undirected graph
    }

    floydWarshall(nama, jumlah, dist);

    cout << "\n================================================\n";
    return 0;
}