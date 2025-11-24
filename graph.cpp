#include <iostream>
using namespace std;

// ==============================
// NODE untuk adjacency list
// ==============================
struct Link {
    string tujuan;   // vertex tujuan
    int bobot;       // berat edge
    Link* next;      // pointer ke node berikutnya
};

// ==============================
// STRUCT GRAPH
// ==============================
struct Graph {
    string daftarNama[100];   // nama semua vertex
    Link* daftarSisi[100];    // adjacency list tiap vertex
    int jumlahVertex;         // berapa vertex yang ada
};

// ==============================
// INISIALISASI GRAPH
// ==============================
void inisialisasiGraph(Graph &g) {
    g.jumlahVertex = 0;
    for (int i = 0; i < 100; i++)
        g.daftarSisi[i] = nullptr;
}

// ==============================
// CARI INDEX VERTEX
// ==============================
int cariIndex(Graph &g, string nama) {
    for (int i = 0; i < g.jumlahVertex; i++) {
        if (g.daftarNama[i] == nama)
            return i;
    }
    return -1;
}

// ==============================
// TAMBAH VERTEX
// ==============================
void tambahVertex(Graph &g, string nama) {
    if (cariIndex(g, nama) != -1) {
        cout << "Vertex sudah ada!\n";
        return;
    }

    g.daftarNama[g.jumlahVertex] = nama;
    g.daftarSisi[g.jumlahVertex] = nullptr;
    g.jumlahVertex++;

    cout << "Vertex '" << nama << "' ditambahkan.\n";
    system("pause");
}

// ==============================
// TAMBAH EDGE (UNDIRECTED)
// ==============================
void tambahEdge(Graph &g, string dari, string ke, int bobot) {
    int idxDari = cariIndex(g, dari);
    int idxKe   = cariIndex(g, ke);

    if (idxDari == -1 || idxKe == -1) {
        cout << "Vertex tidak ditemukan!\n";
        return;
    }

    // dari -> ke
    Link* baru1 = new Link{ke, bobot, g.daftarSisi[idxDari]};
    g.daftarSisi[idxDari] = baru1;

    // ke -> dari
    Link* baru2 = new Link{dari, bobot, g.daftarSisi[idxKe]};
    g.daftarSisi[idxKe] = baru2;

    cout << "Edge ditambahkan.\n";
    system("pause");
}

// ==============================
// TAMPILKAN GRAPH
// ==============================
void tampilkanGraph(Graph &g) {
    cout << "\n=== GRAPH ===\n";
    for (int i = 0; i < g.jumlahVertex; i++) {
        cout << g.daftarNama[i] << " -> ";
        Link* jalan = g.daftarSisi[i];

        while (jalan != nullptr) {
            cout << jalan->tujuan << "(w=" << jalan->bobot << ") -> ";
            jalan = jalan->next;
        }
        cout << "NULL\n";
    }
    cout << endl;
}

// ==============================
// BFS
// ==============================
void bfs(Graph &g, string mulai) {
    int idxMulai = cariIndex(g, mulai);
    if (idxMulai == -1) {
        cout << "Vertex tidak ditemukan!\n";
        return;
    }

    bool sudahKunjung[100] = {false};
    string antrian[100];
    int depan = 0, belakang = 0;

    antrian[belakang++] = mulai;
    sudahKunjung[idxMulai] = true;

    cout << "BFS: ";

    while (depan < belakang) {
        string sekarang = antrian[depan++];
        cout << sekarang << " ";

        int idxSekarang = cariIndex(g, sekarang);
        Link* temp = g.daftarSisi[idxSekarang];

        while (temp != nullptr) {
            int idxTetangga = cariIndex(g, temp->tujuan);
            if (!sudahKunjung[idxTetangga]) {
                sudahKunjung[idxTetangga] = true;
                antrian[belakang++] = temp->tujuan;
            }
            temp = temp->next;
        }
    }
    cout << "\n\n";
}

// ==============================
// DFS
// ==============================
bool sudahDFS[100];

void dfsRekursif(Graph &g, string nama) {
    int idx = cariIndex(g, nama);
    if (sudahDFS[idx]) return;

    sudahDFS[idx] = true;
    cout << nama << " ";

    Link* temp = g.daftarSisi[idx];
    while (temp != nullptr) {
        dfsRekursif(g, temp->tujuan);
        temp = temp->next;
    }
}

void dfs(Graph &g, string mulai) {
    int idxMulai = cariIndex(g, mulai);
    if (idxMulai == -1) {
        cout << "Vertex tidak ditemukan!\n";
        return;
    }

    for (int i = 0; i < 100; i++)
        sudahDFS[i] = false;

    cout << "DFS: ";
    dfsRekursif(g, mulai);
    cout << "\n\n";
}

// ==============================
// CETAK PATH DIJKSTRA
// ==============================
void tampilkanPath(int parent[], int start, int end, Graph &g) {
    string jalur[100];
    int jumlah = 0;

    int sekarang = end;

    while (sekarang != -1) {
        jalur[jumlah++] = g.daftarNama[sekarang];
        if (sekarang == start) break;
        sekarang = parent[sekarang];
    }

    cout << "Shortest Path : ";

    for (int i = jumlah - 1; i >= 0; i--) {
        cout << jalur[i];
        if (i > 0) cout << " -> ";
    }
    cout << endl;
}

// ==============================
// DIJKSTRA + PATH
// ==============================
void dijkstra(Graph &g, string mulai) {
    int idxMulai = cariIndex(g, mulai);
    if (idxMulai == -1) {
        cout << "Vertex tidak ditemukan!\n";
        return;
    }

    const int INF = 1e9;
    int jarak[100];
    int parent[100];
    bool sudah[100] = {false};

    for (int i = 0; i < g.jumlahVertex; i++) {
        jarak[i] = INF;
        parent[i] = -1;
    }

    jarak[idxMulai] = 0;

    for (int i = 0; i < g.jumlahVertex; i++) {

        int minJarak = INF;
        int u = -1;

        for (int j = 0; j < g.jumlahVertex; j++) {
            if (!sudah[j] && jarak[j] < minJarak) {
                minJarak = jarak[j];
                u = j;
            }
        }

        if (u == -1) break;
        sudah[u] = true;

        Link* temp = g.daftarSisi[u];

        while (temp != nullptr) {
            int idxTetangga = cariIndex(g, temp->tujuan);
            int kemungkinan = jarak[u] + temp->bobot;

            if (kemungkinan < jarak[idxTetangga]) {
                jarak[idxTetangga] = kemungkinan;
                parent[idxTetangga] = u;    // INI PENAMBAHAN TERPENTING
            }

            temp = temp->next;
        }
    }

    cout << "\n=== HASIL DIJKSTRA dari " << mulai << " ===\n";
    for (int i = 0; i < g.jumlahVertex; i++) {
        cout << mulai << " -> " << g.daftarNama[i]
             << " = " << jarak[i] << endl;
    }

    cout << "\nIngin lihat jalur ke vertex mana? ";
    string tujuan;
    cin >> tujuan;

    int idxTujuan = cariIndex(g, tujuan);

    if (idxTujuan == -1) {
        cout << "Vertex tidak ditemukan!\n";
        return;
    }

    cout << "Cost total     : " << jarak[idxTujuan] << endl;
    tampilkanPath(parent, idxMulai, idxTujuan, g);

    cout << endl;
}

// ==============================
// MENU PROGRAM
// ==============================
int main() {
    Graph g;
    inisialisasiGraph(g);

    while (true) {
        system("cls");   // bersihkan layar menu utama

        cout << R"(
=============================
        MENU GRAPH
=============================
1. Tambah Vertex
2. Tambah Edge
3. Tampilkan Graph
4. BFS
5. DFS
6. Dijkstra (Cost + Shortest Path)
7. Exit
=============================
)";
        cout << "Pilih menu: ";
        string pilihanInput;
        cin >> pilihanInput;

        int pilihan = stoi(pilihanInput);

        switch (pilihan) {

        case 1: {
            system("cls");
            string nama;
            cout << "Masukkan nama vertex: ";
            cin >> nama;
            tambahVertex(g, nama);
            break;
        }

        case 2: {
            system("cls");
            string dari, ke;
            int bobot;
            cout << "Vertex asal  : ";
            cin >> dari;
            cout << "Vertex tujuan: ";
            cin >> ke;
            cout << "Bobot        : ";
            cin >> bobot;
            tambahEdge(g, dari, ke, bobot);
            break;
        }

        case 3:
            system("cls");
            tampilkanGraph(g);
            system("pause");
            break;

        case 4: {
            system("cls");
            string mulai;
            cout << "Mulai dari vertex: ";
            cin >> mulai;
            bfs(g, mulai);
            system("pause");
            break;
        }

        case 5: {
            system("cls");
            string mulai;
            cout << "Mulai dari vertex: ";
            cin >> mulai;
            dfs(g, mulai);
            system("pause");
            break;
        }

        case 6: {
            system("cls");
            string mulai;
            cout << "Mulai dari vertex: ";
            cin >> mulai;
            dijkstra(g, mulai);
            system("pause");
            break;
        }

        case 7:
            system("cls");
            cout << "Program selesai.\n";
            return 0;

        default:
            system("cls");
            cout << "Pilihan tidak valid!\n";
            system("pause");
        }
    }

    return 0;
}