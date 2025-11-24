#include <iostream>
using namespace std;

const int MAX = 20;

int main() {
    string nama[5] = {"A","B","C","D","E"};
    int jumlah = 5;

    int INF = 1e9;

    int dist[MAX][MAX];

    // Inisialisasi jarak
    for (int i = 0; i < jumlah; i++) {
        for (int j = 0; j < jumlah; j++) {
            if (i == j) dist[i][j] = 0;
            else dist[i][j] = INF;
        }
    }

    auto addEdge = [&](string a, string b, int w) {
        int u=-1,v=-1;
        for (int i=0; i<jumlah;i++){
            if(nama[i]==a) u=i;
            if(nama[i]==b) v=i;
        }
        dist[u][v] = w;
        dist[v][u] = w; // undirected
    };

    addEdge("A","B",4);
    addEdge("A","C",3);
    addEdge("B","D",5);
    addEdge("C","D",2);
    addEdge("D","E",7);

    // Floyd-Warshall
    for (int k = 0; k < jumlah; k++) {
        for (int i = 0; i < jumlah; i++) {
            for (int j = 0; j < jumlah; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }

    cout << "Hasil Floyd-Warshall:\n";
    for (int i = 0; i < jumlah; i++) {
        for (int j = 0; j < jumlah; j++) {
            cout << nama[i] << " -> " << nama[j]
                 << " = " << dist[i][j] << endl;
        }
        cout << endl;
    }
}
