#include "matakuliah.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <sstream>

struct HasilTopo {
    vector<string> urutan;   // urutan topologis (parsial kalau ada siklus)
    bool adaSiklus;
    vector<string> terlibat; // matkul yang tersangkut siklus
};

class Graph {
private:
    int n;
    vector<vector<int>> adj;   // adj[u]  = matkul yang butuh u sebagai prasyarat (u -> v)
    vector<vector<int>> radj;  // radj[v] = prasyarat v (graph terbalik, untuk Orang 3)
    vector<string> kode;
    unordered_map<string, int> idx;

public:
    explicit Graph(const vector<MataKuliah>& data) {
        n = (int)data.size();
        adj.assign(n, {});
        radj.assign(n, {});
        kode.resize(n);
        for (int i = 0; i < n; i++) {
            kode[i] = data[i].kode;
            idx[kode[i]] = i;
        }
        for (int v = 0; v < n; v++) {
            for (const string& p : data[v].prasyarat) {
                auto it = idx.find(p);
                if (it == idx.end()) continue;   // prasyarat tidak dikenal, abaikan
                int u = it->second;
                adj[u].push_back(v);             // edge: prasyarat -> matkul
                radj[v].push_back(u);
            }
        }
    }

    // Kahn's algorithm + deteksi siklus
    HasilTopo topologicalSort() const {
        vector<int> indeg(n, 0);
        for (int u = 0; u < n; u++)
            for (int v : adj[u]) indeg[v]++;

        queue<int> q;
        for (int i = 0; i < n; i++)
            if (indeg[i] == 0) q.push(i);

        HasilTopo h;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            h.urutan.push_back(kode[u]);
            for (int v : adj[u])
                if (--indeg[v] == 0) q.push(v);
        }
        h.adaSiklus = ((int)h.urutan.size() != n);
        if (h.adaSiklus) {
            for (int i = 0; i < n; i++)
                if (indeg[i] > 0) h.terlibat.push_back(kode[i]);
        }
        return h;
    }

    // Validasi rencana urutan pengambilan; alasan kegagalan dicetak
    bool validasiRencana(const vector<string>& rencana) const {
        unordered_map<string, int> posisi;
        for (int i = 0; i < (int)rencana.size(); i++) {
            if (!idx.count(rencana[i])) {
                cout << "Kode " << rencana[i] << " tidak ada di daftar.\n";
                return false;
            }
            if (posisi.count(rencana[i])) {
                cout << "Kode " << rencana[i] << " muncul lebih dari sekali.\n";
                return false;
            }
            posisi[rencana[i]] = i;
        }
        for (const string& k : rencana) {
            int v = idx.at(k);
            for (int u : radj[v]) {
                if (!posisi.count(kode[u])) {
                    cout << k << " butuh " << kode[u] << ", tapi belum ada di rencana.\n";
                    return false;
                }
                if (posisi[kode[u]] > posisi[k]) {
                    cout << k << " diambil sebelum prasyaratnya (" << kode[u] << ").\n";
                    return false;
                }
            }
        }
        return true;
    }

    // Untuk Orang 3: prasyarat langsung
    vector<string> prasyaratLangsung(const string& k) const {
        vector<string> hasil;
        auto it = idx.find(k);
        if (it == idx.end()) return hasil;
        for (int u : radj[it->second]) hasil.push_back(kode[u]);
        return hasil;
    }

    // Untuk Orang 3: semua prasyarat (langsung + tidak langsung), BFS di graph terbalik
    vector<string> semuaPrasyarat(const string& k) const {
        vector<string> hasil;
        auto it = idx.find(k);
        if (it == idx.end()) return hasil;
        vector<bool> visited(n, false);
        queue<int> q;
        visited[it->second] = true;
        q.push(it->second);
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (int u : radj[v]) {
                if (!visited[u]) {
                    visited[u] = true;       // bukan push(visited[u])!
                    q.push(u);
                    hasil.push_back(kode[u]);
                }
            }
        }
        return hasil;
    }
};

// ===== Fungsi bantu & menu =====

// Pakai di editMK: apakah perubahan prasyarat akan menimbulkan siklus?
inline bool editMenimbulkanSiklus(const vector<MataKuliah>& data,
                                  const string& kode,
                                  const vector<string>& prasyaratBaru) {
    vector<MataKuliah> salinan = data;
    for (MataKuliah& mk : salinan)
        if (mk.kode == kode) mk.prasyarat = prasyaratBaru;
    return Graph(salinan).topologicalSort().adaSiklus;
}

inline void menuTopologicalSort(const vector<MataKuliah>& data) {
    HasilTopo h = Graph(data).topologicalSort();
    if (h.adaSiklus) {
        cout << "Terdeteksi SIKLUS prasyarat! Matkul yang terlibat:\n";
        for (const string& k : h.terlibat) cout << "  " << k << "\n";
        return;
    }
    cout << "Urutan pengambilan yang valid:\n";
    for (size_t i = 0; i < h.urutan.size(); i++)
        cout << (i + 1) << ". " << h.urutan[i] << "\n";
}

inline void menuValidasiRencana(const vector<MataKuliah>& data) {
    cout << "Masukkan rencana urutan kode MK (pisah spasi): ";
    string baris;
    getline(cin, baris);
    stringstream ss(baris);
    vector<string> rencana;
    string k;
    while (ss >> k) rencana.push_back(k);

    if (Graph(data).validasiRencana(rencana))
        cout << "Rencana VALID.\n";
    else
        cout << "Rencana TIDAK valid.\n";
}

// Menu graph: dipanggil dari main.cpp (menu 7).
void menuGraph(vector<MataKuliah>& data) {
    int pil;
    do {
        cout << "\n--- MENU GRAPH DAN VALIDASI PRASYARAT ---\n";
        cout << "1. Urutan pengambilan (topological sort + deteksi siklus)\n";
        cout << "2. Validasi rencana urutan pengambilan\n";
        cout << "0. Kembali\n";
        pil = bacaAngka("Pilih: ");

        if (pil == 1) {
            menuTopologicalSort(data);
        } else if (pil == 2) {
            menuValidasiRencana(data);
        } else if (pil != 0) {
            cout << "Menu tidak valid.\n";
        }
    } while (pil != 0);
}
