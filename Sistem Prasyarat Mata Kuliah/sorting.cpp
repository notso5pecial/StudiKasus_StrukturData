#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

struct MataKuliah {
    string kode;
    string nama;
    int sks;
    int semester;
    string prasyarat;
};

// Return true kalau a harus di depan b
bool compareMultiKey(const MataKuliah& a, const MataKuliah& b) {
    if (a.semester != b.semester) {
        return a.semester < b.semester; // Utama: Semester kecil ke besar
    }
    return a.sks < b.sks; // Tie-breaker: SKS kecil ke besar
}

// 1. INSERTION SORT (Multi-Key: Semester -> SKS)
void insertionSortMultiKey(vector<MataKuliah>& data) {
    int n = data.size();
    for (int i = 1; i < n; i++) {
        MataKuliah key = data[i];
        int j = i - 1;
        while (j >= 0 && compareMultiKey(key, data[j])) {
            data[j + 1] = data[j];
            j--;
        }
        data[j + 1] = key;
    }
}

// 2. MERGE SORT (Multi-Key: Semester -> SKS)
void merge(vector<MataKuliah>& data, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<MataKuliah> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = data[left + i];
    for (int j = 0; j < n2; j++) R[j] = data[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (compareMultiKey(L[i], R[j])) {
            data[k] = L[i];
            i++;
        } else {
            data[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        data[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        data[k] = R[j];
        j++;
        k++;
    }
}

void mergeSortMultiKey(vector<MataKuliah>& data, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortMultiKey(data, left, mid);
        mergeSortMultiKey(data, mid + 1, right);
        merge(data, left, mid, right);
    }
}

// 3. SORT BY KODE (Untuk Binary Search Orang 3)
void sortKodeForBinarySearch(vector<MataKuliah>& data) {
    int n = data.size();
    for (int i = 1; i < n; i++) {
        MataKuliah key = data[i];
        int j = i - 1;
        while (j >= 0 && data[j].kode > key.kode) {
            data[j + 1] = data[j];
            j--;
        }
        data[j + 1] = key;
    }
}

void cetakData(const vector<MataKuliah>& data) {
    cout << left << setw(8) << "Kode" 
         << setw(35) << "Nama Mata Kuliah" 
         << setw(6) << "SKS" 
         << setw(10) << "Semester" 
         << setw(12) << "Prasyarat" << endl;
    cout << string(71, '-') << endl;
    for (const auto& mk : data) {
        cout << left << setw(8) << mk.kode 
             << setw(35) << mk.nama 
             << setw(6) << mk.sks 
             << setw(10) << mk.semester 
             << setw(12) << (mk.prasyarat.empty() ? "-" : mk.prasyarat) << endl;
    }
    cout << endl;
}

int main() {
    vector<MataKuliah> dataset = {
        {"IF101", "Algoritma dan Pemrograman", 3, 1, ""},
        {"IF102", "Matematika Diskrit", 3, 1, ""},
        {"IF103", "Kalkulus", 3, 1, ""},
        {"IF104", "Pengantar Teknologi Informasi", 2, 1, ""},
        {"IF201", "Struktur Data", 3, 2, "IF101"},
        {"IF202", "Basis Data", 3, 2, "IF101"},
        {"IF203", "Aljabar Linear", 3, 2, "IF103"},
        {"IF204", "Sistem Digital", 2, 2, ""},
        {"IF301", "Algoritma Lanjut", 3, 3, "IF201"},
        {"IF302", "Pemrograman Berorientasi Objek", 3, 3, "IF201"},
        {"IF303", "Sistem Operasi", 3, 3, "IF201"},
        {"IF304", "Statistika", 3, 3, "IF103"},
        {"IF401", "Analisis Algoritma", 3, 4, "IF301"},
        {"IF402", "Jaringan Komputer", 3, 4, "IF303"},
        {"IF403", "Rekayasa Perangkat Lunak", 3, 4, "IF302"}
    };

    cout << "=== DATA SEBELUM SORTING ===" << endl;
    cetakData(dataset);

    // uji Insertion Sort
    vector<MataKuliah> dataInsertion = dataset;
    insertionSortMultiKey(dataInsertion);
    cout << "=== HASIL INSERTION SORT (Semester -> SKS) ===" << endl;
    cetakData(dataInsertion);

    // uji Merge Sort
    vector<MataKuliah> dataMerge = dataset;
    mergeSortMultiKey(dataMerge, 0, dataMerge.size() - 1);
    cout << "=== HASIL MERGE SORT (Semester -> SKS) ===" << endl;
    cetakData(dataMerge);

    // Sorting berdasarkan kode untuk Binary Search
    vector<MataKuliah> dataKode = dataset;
    sortKodeForBinarySearch(dataKode);
    cout << "=== HASIL SORT BY KODE (Untuk Binary Search Orang 3) ===" << endl;
    cetakData(dataKode);

    return 0;
}