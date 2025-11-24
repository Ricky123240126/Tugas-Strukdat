#include <iostream>
using namespace std;

struct Edge {
    int u, v, bobot;
};

const int MAX = 50;

// ----------- UNION FIND ------------
int parentUF[MAX];

int findSet(int x) {
    if (parentUF[x] == x) return x;
    return parentUF[x] = findSet(parentUF[x]); 
}

void unionSet(int a, int b) {
    a = findSet(a);
    b = findSet(b);
    if (a != b) parentUF[b] = a;
}

// ----------- KRUSKAL -----------
void kruskal(Edge edges[], int jumlahEdge, string nama[], int jumlahSimpul) {
    // Inisialisasi union-find
    for (int i = 0; i < jumlahSimpul; i++)
        parentUF[i] = i;

    // Sorting edge dengan bubble sort (ascending berdasarkan bobot)
    for (int i = 0; i < jumlahEdge - 1; i++) {
        for (int j = 0; j < jumlahEdge - 1 - i; j++) {
            if (edges[j].bobot > edges[j+1].bobot) {
                Edge tmp = edges[j];
                edges[j] = edges[j+1];
                edges[j+1] = tmp;
            }
        }
    }

    cout << "\n=== HASIL MST (KRUSKAL) ===\n";
    cout << "Edge yang dipilih untuk MST:\n";

    Edge mstEdges[MAX];
    int mstCount = 0;
    int totalBobot = 0;

    // Proses Kruskal: pilih edge dengan bobot terkecil yang tidak membentuk cycle
    for (int i = 0; i < jumlahEdge; i++) {
        int a = edges[i].u;
        int b = edges[i].v;

        // Cek apakah menambahkan edge ini akan membentuk cycle
        if (findSet(a) != findSet(b)) {
            unionSet(a, b);
            mstEdges[mstCount] = edges[i];
            mstCount++;
            
            cout << nama[a] << " - " << nama[b]
                 << " (bobot: " << edges[i].bobot << ")" << endl;
            totalBobot += edges[i].bobot;
        }
    }

    cout << "\n>>> Total bobot MST = " << totalBobot << " <<<\n";

    // ========== SHORTEST PATH DALAM MST ==========
    cout << "\n=== SHORTEST PATH DALAM MST ===\n";
    string start, end;
    cout << "Masukkan vertex awal: ";
    cin >> start;
    cout << "Masukkan vertex tujuan: ";
    cin >> end;

    int s = -1, t = -1;
    for (int i = 0; i < jumlahSimpul; i++) {
        if (nama[i] == start) s = i;
        if (nama[i] == end) t = i;
    }

    if (s == -1 || t == -1) {
        cout << "Vertex tidak ditemukan!\n";
        return;
    }

    // BFS untuk mencari path dari s ke t dalam MST
    int queue[MAX];
    int front = 0, rear = 0;
    bool visited[MAX] = {false};
    int parent[MAX];
    
    for (int i = 0; i < jumlahSimpul; i++)
        parent[i] = -1;

    queue[rear++] = s;
    visited[s] = true;

    bool found = false;

    while (front < rear) {
        int curr = queue[front++];

        if (curr == t) {
            found = true;
            break;
        }

        // Cek semua edge dalam MST yang terhubung dengan curr
        for (int i = 0; i < mstCount; i++) {
            int next = -1;
            
            if (mstEdges[i].u == curr && !visited[mstEdges[i].v]) {
                next = mstEdges[i].v;
            } else if (mstEdges[i].v == curr && !visited[mstEdges[i].u]) {
                next = mstEdges[i].u;
            }

            if (next != -1) {
                visited[next] = true;
                parent[next] = curr;
                queue[rear++] = next;
            }
        }
    }

    if (!found) {
        cout << "Tidak ada jalur dari " << start << " ke " << end << " dalam MST!\n";
        return;
    }

    // Rekonstruksi path dari t ke s
    int path[MAX];
    int pathLen = 0;
    int curr = t;
    
    while (curr != -1) {
        path[pathLen++] = curr;
        curr = parent[curr];
    }

    // Tampilkan path dan hitung total bobot
    int pathWeight = 0;
    cout << "\nJalur terpendek: ";
    
    for (int i = pathLen - 1; i >= 0; i--) {
        cout << nama[path[i]];
        
        if (i > 0) {
            cout << " -> ";
            
            // Cari bobot edge antara path[i] dan path[i-1]
            for (int j = 0; j < mstCount; j++) {
                if ((mstEdges[j].u == path[i] && mstEdges[j].v == path[i-1]) ||
                    (mstEdges[j].v == path[i] && mstEdges[j].u == path[i-1])) {
                    pathWeight += mstEdges[j].bobot;
                    break;
                }
            }
        }
    }
    
    cout << "\nTotal jarak: " << pathWeight << endl;
}

int main() {
    string nama[MAX];
    int jumlahSimpul;

    cout << "================================================\n";
    cout << "       ALGORITMA KRUSKAL (MST)                 \n";
    cout << "================================================\n";
    
    cout << "\nMasukkan jumlah vertex: ";
    cin >> jumlahSimpul;

    cout << "\nMasukkan nama setiap vertex:\n";
    for (int i = 0; i < jumlahSimpul; i++) {
        cout << "  Vertex ke-" << (i + 1) << ": ";
        cin >> nama[i];
    }

    Edge edges[MAX];
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
        for (int j = 0; j < jumlahSimpul; j++) {
            if (nama[j] == a) u = j;
            if (nama[j] == b) v = j;
        }

        if (u == -1 || v == -1) {
            cout << "  ERROR: Vertex tidak ditemukan! Silakan input ulang.\n";
            i--;
            continue;
        }

        edges[i].u = u;
        edges[i].v = v;
        edges[i].bobot = w;
    }

    kruskal(edges, jumlahEdge, nama, jumlahSimpul);

    cout << "\n================================================\n";
    return 0;
}