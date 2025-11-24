#include <iostream>
using namespace std;

struct Edge {
    int u, v, bobot;
};

const int MAX = 50;

void bellmanFord(Edge edges[], int jumlahEdge, string nama[],
                 int jumlahSimpul, string start) {

    int s = -1;
    for (int i = 0; i < jumlahSimpul; i++)
        if (nama[i] == start) s = i;

    int jarak[MAX];

    for (int i = 0; i < jumlahSimpul; i++)
        jarak[i] = 1e9;

    jarak[s] = 0;

    // Relax edges sebanyak (V-1) kali
    for (int i = 1; i < jumlahSimpul; i++) {
        for (int j = 0; j < jumlahEdge; j++) {
            int u = edges[j].u;
            int v = edges[j].v;
            int w = edges[j].bobot;

            if (jarak[u] != 1e9 && 
                jarak[u] + w < jarak[v]) {
                jarak[v] = jarak[u] + w;
            }
        }
    }

    cout << "Jarak dari " << start << ":\n";
    for (int i = 0; i < jumlahSimpul; i++) {
        cout << start << " -> " << nama[i] 
             << " = " << jarak[i] << endl;
    }
}

int main() {
    string nama[5] = {"A","B","C","D","E"};
    int jumlahSimpul = 5;

    Edge edges[MAX];
    int jumlahEdge = 0;

    auto addEdge = [&](string a, string b, int w) {
        int u=-1,v=-1;
        for(int i=0;i<jumlahSimpul;i++){
            if(nama[i]==a) u=i;
            if(nama[i]==b) v=i;
        }
        edges[jumlahEdge++] = {u,v,w};
        edges[jumlahEdge++] = {v,u,w}; // UNDIRECTED
    };

    addEdge("A","B",4);
    addEdge("A","C",3);
    addEdge("B","D",5);
    addEdge("C","D",2);
    addEdge("D","E",7);

    bellmanFord(edges, jumlahEdge, nama, jumlahSimpul, "A");
}
