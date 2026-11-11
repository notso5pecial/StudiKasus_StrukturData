#include "matakuliah.h"
#include <iomanip>

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

// 3. SORT BY KODE (Untuk Binary Search)
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

static string gabungPr(const vector<string>& pr) {
    if (pr.empty()) return "-";
    string hasil;
    for (size_t i = 0; i < pr.size(); i++) {
        if (i > 0) hasil += ", ";
        hasil += pr[i];
    }
    return hasil;
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
             << setw(12) << gabungPr(mk.prasyarat) << endl;
    }
    cout << endl;
}

// Menu sorting: dipanggil dari main.cpp (menu 5).
// Yang diurutkan salinan data, jadi urutan data asli tidak berubah.
void menuSorting(vector<MataKuliah>& data) {
    int pil;
    do {
        cout << "\n--- MENU SORTING ---\n";
        cout << "1. Insertion Sort (Semester -> SKS)\n";
        cout << "2. Merge Sort (Semester -> SKS)\n";
        cout << "3. Urutkan berdasarkan kode\n";
        cout << "0. Kembali\n";
        pil = bacaAngka("Pilih: ");

        if (pil == 1) {
            vector<MataKuliah> salinan = data;
            insertionSortMultiKey(salinan);
            cout << "=== HASIL INSERTION SORT (Semester -> SKS) ===" << endl;
            cetakData(salinan);
        } else if (pil == 2) {
            vector<MataKuliah> salinan = data;
            if (!salinan.empty()) mergeSortMultiKey(salinan, 0, (int)salinan.size() - 1);
            cout << "=== HASIL MERGE SORT (Semester -> SKS) ===" << endl;
            cetakData(salinan);
        } else if (pil == 3) {
            vector<MataKuliah> salinan = data;
            sortKodeForBinarySearch(salinan);
            cout << "=== HASIL SORT BY KODE ===" << endl;
            cetakData(salinan);
        } else if (pil != 0) {
            cout << "Menu tidak valid.\n";
        }
    } while (pil != 0);
}
