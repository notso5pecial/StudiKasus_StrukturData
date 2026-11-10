#include "matakuliah.h"

//nanti ganti kalau udah punya kalian
void menuSorting(vector<MataKuliah>& data)   { cout << "Fitur sorting belum diimplementasi.\n"; }
void menuSearching(vector<MataKuliah>& data) { cout << "Fitur searching belum diimplementasi.\n"; }
void menuGraph(vector<MataKuliah>& data)     { cout << "Fitur graph belum diimplementasi.\n"; }

int main() {
    vector<MataKuliah> data = dataAwal();
    int pilihan;

    do {
        cout << "\n===== SISTEM PRASYARAT MATA KULIAH =====\n";
        cout << "1. Tampilkan semua mata kuliah\n";
        cout << "2. Tambah mata kuliah\n";
        cout << "3. Edit mata kuliah\n";
        cout << "4. Hapus mata kuliah\n";
        cout << "5. Sorting\n";
        cout << "6. Searching\n";
        cout << "7. Graph dan validasi prasyarat\n";
        cout << "0. Keluar\n";
        pilihan = bacaAngka("Pilih menu: ");

        switch (pilihan) {
            case 1: tampilkanSemua(data); break;
            case 2: tambahMK(data);       break;
            case 3: editMK(data);         break;
            case 4: hapusMK(data);        break;
            case 5: menuSorting(data);    break;
            case 6: menuSearching(data);  break;
            case 7: menuGraph(data);      break;
            case 0: cout << "Terima kasih.\n"; break;
            default: cout << "Menu tidak valid.\n";
        }
    } while (pilihan != 0);

    return 0;
}