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

    // Sorting edge pakai bubble sort (simple)
    for (int i = 0; i < jumlahEdge - 1; i++) {
        for (int j = 0; j < jumlahEdge - 1 - i; j++) {
            if (edges[j].bobot > edges[j+1].bobot) {
                Edge tmp = edges[j];
                edges[j] = edges[j+1];
                edges[j+1] = tmp;
            }
        }
    }

    cout << "Hasil MST Kruskal:\n";

    int total = 0;
    for (int i = 0; i < jumlahEdge; i++) {
        int a = edges[i].u;
        int b = edges[i].v;

        if (findSet(a) != findSet(b)) {
            unionSet(a, b);
            cout << nama[a] << " - " << nama[b]
                 << " : " << edges[i].bobot << endl;
            total += edges[i].bobot;
        }
    }

    cout << "Total bobot MST = " << total << endl;
}

int main() {
    string nama[5] = {"A","B","C","D","E"};
    int jumlahSimpul = 5;

    Edge edges[MAX];
    int jumlahEdge = 0;

    // Input graf UNDIRECTED
    auto addEdge = [&](string a, string b, int w) {
        int u=-1,v=-1;
        for(int i=0;i<jumlahSimpul;i++){
            if(nama[i]==a) u=i;
            if(nama[i]==b) v=i;
        }
        edges[jumlahEdge++] = {u,v,w};
    };

    addEdge("A","B",4);
    addEdge("A","C",3);
    addEdge("B","D",5);
    addEdge("C","D",2);
    addEdge("D","E",7);

    kruskal(edges, jumlahEdge, nama, jumlahSimpul);
}
