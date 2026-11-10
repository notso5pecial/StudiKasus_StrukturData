#include "matakuliah.h"   
#include <sstream>       

vector<MataKuliah> dataAwal() {
    return {
        {"IF101", "Algoritma dan Pemrograman",      3, 1, {}},
        {"IF102", "Matematika Diskrit",             3, 1, {}},
        {"IF103", "Kalkulus",                       3, 1, {}},
        {"IF104", "Pengantar Teknologi Informasi",  2, 1, {}},
        {"IF201", "Struktur Data",                  3, 2, {"IF101"}},
        {"IF202", "Basis Data",                     3, 2, {"IF101"}},
        {"IF203", "Aljabar Linear",                 3, 2, {"IF103"}},
        {"IF204", "Sistem Digital",                 2, 2, {}},
        {"IF301", "Algoritma Lanjut",               3, 3, {"IF201"}},
        {"IF302", "Pemrograman Berorientasi Objek", 3, 3, {"IF201"}},
        {"IF303", "Sistem Operasi",                 3, 3, {"IF201"}},
        {"IF304", "Statistika",                     3, 3, {"IF103"}},
        {"IF401", "Analisis Algoritma",             3, 4, {"IF301"}},
        {"IF402", "Jaringan Komputer",              3, 4, {"IF303"}},
        {"IF403", "Rekayasa Perangkat Lunak",       3, 4, {"IF302"}}
    };
}

int bacaAngka(const string& pesan) {
    int x;
    while (true) {
        cout << pesan;
        if (cin >> x) {              
            cin.ignore(1000, '\n');  
            return x;
        }
        cin.clear();                 
        cin.ignore(1000, '\n');     
        cout << "Input harus angka, coba lagi.\n";
    }
}

string bacaTeks(const string& pesan) {
    string s;
    cout << pesan;
    getline(cin, s);
    return s;
}

//  VALIDASI
int cariIndeksKode(const vector<MataKuliah>& data, const string& kode) {
    for (int i = 0; i < (int)data.size(); i++) {
        if (data[i].kode == kode) return i;
    }
    return -1;
}

bool kodeSudahAda(const vector<MataKuliah>& data, const string& kode) {
    return cariIndeksKode(data, kode) != -1;
}

bool sksValid(int sks)      { return sks >= 1 && sks <= 6; }

bool semesterValid(int smt) { return smt >= 1 && smt <= 8; }

bool prasyaratValid(const vector<MataKuliah>& data, const vector<string>& pr) {
    for (const string& p : pr) {
        if (!kodeSudahAda(data, p)) {
            cout << "Prasyarat " << p << " tidak ada di daftar mata kuliah.\n";
            return false;
        }
    }
    return true;
}

//  TAMPILAN
static string gabungPrasyarat(const vector<string>& pr) {
    if (pr.empty()) return "-";
    string hasil;
    for (size_t i = 0; i < pr.size(); i++) {
        if (i > 0) hasil += ", ";   // koma hanya di antara item
        hasil += pr[i];
    }
    return hasil;
}

void tampilkanSatu(const MataKuliah& mk) {
    cout << mk.kode << " | " << mk.nama
         << " | " << mk.sks << " SKS"
         << " | Smt " << mk.semester
         << " | Prasyarat: " << gabungPrasyarat(mk.prasyarat) << "\n";
}

void tampilkanSemua(const vector<MataKuliah>& data) {
    if (data.empty()) {
        cout << "Data kosong.\n";
        return;
    }
    cout << "\n=== DAFTAR MATA KULIAH (" << data.size() << ") ===\n";
    for (const MataKuliah& mk : data) {
        tampilkanSatu(mk);
    }
}

//  INPUT PRASYARAT
static vector<string> bacaPrasyarat() {
    string baris = bacaTeks("Prasyarat (pisah spasi, ketik - jika tidak ada): ");
    stringstream ss(baris);
    vector<string> hasil;
    string kode;
    while (ss >> kode) {            
        if (kode != "-") hasil.push_back(kode);
    }
    return hasil;
}
//  TAMBAH MK
void tambahMK(vector<MataKuliah>& data) {
    MataKuliah mk;

    mk.kode = bacaTeks("Kode MK: ");
    if (mk.kode.empty()) {
        cout << "Kode tidak boleh kosong.\n";
        return;
    }
    if (kodeSudahAda(data, mk.kode)) {      // cegah kode ganda
        cout << "Kode " << mk.kode << " sudah ada.\n";
        return;
    }

    mk.nama = bacaTeks("Nama MK: ");
    if (mk.nama.empty()) {
        cout << "Nama tidak boleh kosong.\n";
        return;
    }

    mk.sks = bacaAngka("SKS (1-6): ");
    if (!sksValid(mk.sks)) {
        cout << "SKS harus 1 sampai 6.\n";
        return;
    }

    mk.semester = bacaAngka("Semester (1-8): ");
    if (!semesterValid(mk.semester)) {
        cout << "Semester harus 1 sampai 8.\n";
        return;
    }

    mk.prasyarat = bacaPrasyarat();
    if (!prasyaratValid(data, mk.prasyarat)) return;   

    data.push_back(mk);                  
    cout << "Mata kuliah berhasil ditambahkan.\n";
}

//  EDIT MK
void editMK(vector<MataKuliah>& data) {
    string kode = bacaTeks("Kode MK yang diedit: ");
    int idx = cariIndeksKode(data, kode);
    if (idx == -1) {
        cout << "Kode tidak ditemukan.\n";
        return;
    }

    cout << "Data sekarang: ";
    tampilkanSatu(data[idx]);
    cout << "Masukkan data baru (kode tidak bisa diubah).\n";

    string nama = bacaTeks("Nama MK baru: ");
    if (nama.empty()) {
        cout << "Nama tidak boleh kosong.\n";
        return;
    }

    int sks = bacaAngka("SKS baru (1-6): ");
    if (!sksValid(sks)) {
        cout << "SKS harus 1 sampai 6.\n";
        return;
    }

    int smt = bacaAngka("Semester baru (1-8): ");
    if (!semesterValid(smt)) {
        cout << "Semester harus 1 sampai 8.\n";
        return;
    }

    vector<string> pr = bacaPrasyarat();
    if (!prasyaratValid(data, pr)) return;

    for (const string& p : pr) {
        if (p == kode) {
            cout << "Mata kuliah tidak boleh jadi prasyarat dirinya sendiri.\n";
            return;
        }
    }

    data[idx].nama = nama;
    data[idx].sks = sks;
    data[idx].semester = smt;
    data[idx].prasyarat = pr;
    cout << "Data berhasil diubah.\n";
}

//  HAPUS MK
void hapusMK(vector<MataKuliah>& data) {
    string kode = bacaTeks("Kode MK yang dihapus: ");
    int idx = cariIndeksKode(data, kode);
    if (idx == -1) {
        cout << "Kode tidak ditemukan.\n";
        return;
    }

    for (const MataKuliah& mk : data) {
        for (const string& p : mk.prasyarat) {
            if (p == kode) {
                cout << "Tidak bisa dihapus: " << kode
                     << " adalah prasyarat " << mk.kode << ".\n";
                return;
            }
        }
    }

    data.erase(data.begin() + idx);
    cout << "Mata kuliah berhasil dihapus.\n";
}