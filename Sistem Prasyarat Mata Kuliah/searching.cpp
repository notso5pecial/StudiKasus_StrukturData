#include "matakuliah.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <set>

static string kecil(string s) {
    for (char& c : s) c = tolower((unsigned char)c);
    return s;
}

static string gabung(const vector<string>& v) {
    if (v.empty()) return "-";
    string hasil;
    for (size_t i = 0; i < v.size(); i++) {
        if (i > 0) hasil += ", ";
        hasil += v[i];
    }
    return hasil;
}

vector<MataKuliah> linearSearchNama(const vector<MataKuliah>& data,
                                    const string& kw, int& cmp) {
    vector<MataKuliah> hasil;
    cmp = 0;
    string k = kecil(kw);
    for (const MataKuliah& mk : data) {
        cmp++;
        if (kecil(mk.nama).find(k) != string::npos) hasil.push_back(mk);
    }
    return hasil;
}

vector<MataKuliah> salinanTerurut(const vector<MataKuliah>& data) {
    vector<MataKuliah> s = data;
    sort(s.begin(), s.end(),
         [](const MataKuliah& a, const MataKuliah& b) { return a.kode < b.kode; });
    return s;
}

int binarySearchKode(const vector<MataKuliah>& terurut, const string& kode, int& cmp) {
    int kiri = 0, kanan = (int)terurut.size() - 1;
    cmp = 0;
    while (kiri <= kanan) {
        int tengah = kiri + (kanan - kiri) / 2;
        cmp++;
        if (terurut[tengah].kode == kode) return tengah;
        if (terurut[tengah].kode < kode) kiri = tengah + 1;
        else kanan = tengah - 1;
    }
    return -1;
}

int linearSearchKode(const vector<MataKuliah>& data, const string& kode, int& cmp) {
    cmp = 0;
    for (int i = 0; i < (int)data.size(); i++) {
        cmp++;
        if (data[i].kode == kode) return i;
    }
    return -1;
}

static void dfsPrasyarat(const vector<MataKuliah>& data, const string& kode,
                         set<string>& visited) {
    int idx = cariIndeksKode(data, kode);
    if (idx == -1) return;
    for (const string& p : data[idx].prasyarat) {
        if (visited.count(p) == 0) {
            visited.insert(p);
            dfsPrasyarat(data, p, visited);
        }
    }
}

void tampilkanPrasyarat(const vector<MataKuliah>& data, const string& kode) {
    int idx = cariIndeksKode(data, kode);
    if (idx == -1) {
        cout << "Kode tidak ditemukan.\n";
        return;
    }

    vector<string> langsung = data[idx].prasyarat;

    set<string> semua;
    dfsPrasyarat(data, kode, semua);

    vector<string> tidakLangsung;
    for (const string& p : semua) {
        bool adaDiLangsung = find(langsung.begin(), langsung.end(), p) != langsung.end();
        if (!adaDiLangsung) tidakLangsung.push_back(p);
    }

    cout << "Prasyarat langsung      : " << gabung(langsung) << "\n";
    cout << "Prasyarat tidak langsung: " << gabung(tidakLangsung) << "\n";
}

void laporanPerbandingan(const vector<MataKuliah>& data) {
    vector<MataKuliah> terurut = salinanTerurut(data);
    long totalLinear = 0, totalBinary = 0;

    cout << left << setw(8) << "Kode"
         << setw(12) << "Linear" << setw(12) << "Binary" << "\n";
    cout << string(32, '-') << "\n";

    for (const MataKuliah& mk : data) {
        int cLin = 0, cBin = 0;
        linearSearchKode(data, mk.kode, cLin);
        binarySearchKode(terurut, mk.kode, cBin);
        totalLinear += cLin;
        totalBinary += cBin;
        cout << left << setw(8) << mk.kode
             << setw(12) << cLin << setw(12) << cBin << "\n";
    }

    cout << string(32, '-') << "\n";
    cout << left << setw(8) << "Total"
         << setw(12) << totalLinear << setw(12) << totalBinary << "\n";
    cout << "(Jumlah perbandingan untuk mencari setiap kode sekali)\n";
}

void menuSearching(vector<MataKuliah>& data) {
    int pil;
    do {
        cout << "\n--- MENU SEARCHING ---\n";
        cout << "1. Cari berdasarkan nama (linear, partial)\n";
        cout << "2. Cari berdasarkan kode (binary search)\n";
        cout << "3. Query prasyarat mata kuliah\n";
        cout << "4. Laporan perbandingan linear vs binary\n";
        cout << "0. Kembali\n";
        pil = bacaAngka("Pilih: ");

        if (pil == 1) {
            string kw = bacaTeks("Kata kunci nama: ");
            int cmp = 0;
            vector<MataKuliah> hasil = linearSearchNama(data, kw, cmp);
            if (hasil.empty()) cout << "Tidak ditemukan.\n";
            for (const MataKuliah& mk : hasil) tampilkanSatu(mk);
            cout << "Perbandingan: " << cmp << "\n";

        } else if (pil == 2) {
            string kode = bacaTeks("Kode MK: ");
            vector<MataKuliah> terurut = salinanTerurut(data);
            int cmp = 0;
            int idx = binarySearchKode(terurut, kode, cmp);
            if (idx == -1) cout << "Tidak ditemukan.\n";
            else tampilkanSatu(terurut[idx]);
            cout << "Perbandingan: " << cmp << "\n";

        } else if (pil == 3) {
            string kode = bacaTeks("Kode MK: ");
            tampilkanPrasyarat(data, kode);

        } else if (pil == 4) {
            laporanPerbandingan(data);

        } else if (pil != 0) {
            cout << "Menu tidak valid.\n";
        }
    } while (pil != 0);
}
