#ifndef MATAKULIAH_H
#define MATAKULIAH_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Satu mata kuliah
struct MataKuliah {
    string kode;
    string nama;
    int sks;
    int semester;
    vector<string> prasyarat;   // daftar kode prasyarat
};

// data awal 
vector<MataKuliah> dataAwal();

// input
int bacaAngka(const string& pesan);
string bacaTeks(const string& pesan);

// validasi
int cariIndeksKode(const vector<MataKuliah>& data, const string& kode);
bool kodeSudahAda(const vector<MataKuliah>& data, const string& kode);
bool sksValid(int sks);
bool semesterValid(int smt);
bool prasyaratValid(const vector<MataKuliah>& data, const vector<string>& pr);

//tampilkan
void tampilkanSatu(const MataKuliah& mk);
void tampilkanSemua(const vector<MataKuliah>& data);

// tambah / edit / hapus 
void tambahMK(vector<MataKuliah>& data);
void editMK(vector<MataKuliah>& data);
void hapusMK(vector<MataKuliah>& data);

// fitur tambahan
void menuSorting(vector<MataKuliah>& data);
void menuSearching(vector<MataKuliah>& data);
void menuGraph(vector<MataKuliah>& data);

#endif