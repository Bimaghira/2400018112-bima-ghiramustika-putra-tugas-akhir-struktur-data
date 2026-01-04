#ifndef SMARTSPLIT_H
#define SMARTSPLIT_H

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <cstring>

using namespace std;

// ============================================================================
// KONSTANTA DAN ENUM
// ============================================================================

const int MAX_NAMA = 100;
const int MAX_BARANG = 50;
const int MAX_PEMBELI = 100;
const int MAX_USERNAME = 50;
const int MAX_PASSWORD = 50;
const int MAX_ROLE = 20;
const int MAX_UNDO_STATES = 50;

enum StatusPembayaran {
    BELUM_BAYAR = 0,
    SEBAGIAN_BAYAR = 1,
    SUDAH_BAYAR = 2
};

// ============================================================================
// STRUKTUR DATA
// ============================================================================

// Linked List untuk User
struct User {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char email[MAX_NAMA];
    char role[MAX_ROLE]; 
    char tanggal_daftar[30];
    bool aktif;
    User* next;
};

// Struktur bantu untuk File I/O
struct UserData {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char email[MAX_NAMA];
    char role[MAX_ROLE];
    char tanggal_daftar[30];
    bool aktif;
};

// Doubly Linked List untuk Voucher
struct NodeVoucher {
    int id_voucher;
    char kode_voucher[20];
    double nominal_potongan;
    char deskripsi[100];
    NodeVoucher* next; 
    NodeVoucher* prev; 
};

// Linked List untuk Barang
struct NodeBarang {
    int id_barang;
    char nama[MAX_NAMA];
    double harga;
    double diskon_persen;
    double harga_setelah_diskon;
    NodeBarang* next;
};

// Riwayat Pembayaran
struct RiwayatPembayaran {
    int id_pembayaran;
    char tanggal[30];
    double jumlah_bayar;
    char keterangan[MAX_NAMA];
    RiwayatPembayaran* next;
};

// Linked List Pembeli
struct Pembeli {
    int id_pembeli;
    char nama[MAX_NAMA];
    char email[MAX_NAMA];
    char telepon[20];
    double total_sebelum_diskon;
    double total_diskon_barang;
    double total_setelah_diskon;
    double bagian_diskon_total;
    double total_akhir;
    double jumlah_dibayar;
    double sisa_pembayaran;
    StatusPembayaran status_pembayaran;
    char tanggal_dibuat[30];
    char tanggal_deadline[30];
    char metode_pembayaran[20];
    NodeBarang* head_barang;
    RiwayatPembayaran* head_riwayat;
    Pembeli* next;
};

// Log Aktivitas
struct LogAktivitas {
    char timestamp[30];
    char username[MAX_USERNAME];
    char aktivitas[200];
    LogAktivitas* next;
};

// --- STRUKTUR SNAPSHOT (UNTUK UNDO STACK) ---
struct SnapshotBarang {
    int id_barang;
    char nama[MAX_NAMA];
    double harga;
    double diskon_persen;
    double harga_setelah_diskon;
    SnapshotBarang* next;
};

struct SnapshotRiwayat {
    int id_pembayaran;
    char tanggal[30];
    double jumlah_bayar;
    char keterangan[MAX_NAMA];
    SnapshotRiwayat* next;
};

struct SnapshotPembeli {
    int id_pembeli;
    char nama[MAX_NAMA];
    char email[MAX_NAMA];
    char telepon[20];
    double total_sebelum_diskon;
    double total_diskon_barang;
    double total_setelah_diskon;
    double bagian_diskon_total;
    double total_akhir;
    double jumlah_dibayar;
    double sisa_pembayaran;
    StatusPembayaran status_pembayaran;
    char tanggal_dibuat[30];
    char tanggal_deadline[30];
    char metode_pembayaran[20];
    SnapshotBarang* head_barang;
    SnapshotRiwayat* head_riwayat;
    SnapshotPembeli* next;
};

struct StateStack {
    int state_id;
    char timestamp[30];
    SnapshotPembeli* head_pembeli_snapshot;
    double diskon_total;
    int jumlah_pembeli;
    char deskripsi[100];
    StateStack* next;
};

// --- STRUKTUR DATA KTP LENGKAP (DOUBLY LINKED LIST) ---
struct DataKTP {
    char nik[50];
    char nama[100];
    char tempat_tgl_lahir[100];
    char jenis_kelamin[20];
    char alamat[200];
    char agama[20];
    char status_perkawinan[20];
    char pekerjaan[50];
    char kewarganegaraan[50];
    char berlaku_hingga[50];
};

struct NodeKTP {
    DataKTP data;
    NodeKTP* prev;
    NodeKTP* next;
};

// ============================================================================
// PROTOTYPE FUNGSI
// ============================================================================

// Utility
string getTanggalWaktu();
void getCopyTanggal(char* buffer, size_t size);
bool isValidEmail(char* email);

// Voucher
NodeVoucher* buatNodeVoucher(int id, char* kode, double nominal, char* deskripsi);
void tambahVoucher(NodeVoucher*& head, NodeVoucher*& tail, int id, char* kode, double nominal, char* deskripsi);
void tampilkanVoucherMaju(NodeVoucher* head);
void tampilkanVoucherMundur(NodeVoucher* tail);
void hapusSemuaVoucher(NodeVoucher*& head, NodeVoucher*& tail);

// User Management
User* buatNodeUser(char* user, char* pass, char* email, char* role, char* tgl, bool aktif);
void tambahUserToList(User*& head, char* user, char* pass, char* email, char* role, char* tgl, bool aktif);
User* loadUsersFromFile();
void saveUsersToFile(User* head);
bool userExists(User* head, char* username);
User* validateLogin(User* head, char* username, char* password);
void registerUser(User*& head);
User* loginSystem(User* head);
void menuLogin(User*& head, User*& loggedInUser);

// Log & Core SmartSplit Functions
LogAktivitas* buatNodeLog(char* username, char* aktivitas);
void tambahLog(LogAktivitas*& head, char* username, char* aktivitas);
void tampilkanLog(LogAktivitas* head);
void hapusSemuaLog(LogAktivitas*& head);

NodeBarang* buatNodeBarang(int id, char* nama, double harga, double diskon);
void tambahBarangKeLinkedList(NodeBarang*& head, int id, char* nama, double harga, double diskon);
int hitungJumlahBarang(NodeBarang* head);
void tampilkanBarangPembeli(NodeBarang* head);
void hapusSemuaBarang(NodeBarang*& head);

RiwayatPembayaran* buatNodeRiwayat(int id, double jumlah, char* ket);
void tambahRiwayatPembayaran(RiwayatPembayaran*& head, int id, double jumlah, char* keterangan);
int hitungJumlahPembayaran(RiwayatPembayaran* head);
void tampilkanRiwayatPembayaran(RiwayatPembayaran* head);
void hapusSemuaRiwayat(RiwayatPembayaran*& head);

Pembeli* buatNodePembeli(int id, char* nama);
Pembeli* tambahPembeli(Pembeli*& head, int id, char* nama);
Pembeli* cariPembeliByName(Pembeli* head, char* nama);
int hitungJumlahPembeli(Pembeli* head);
void tampilkanSemuaPembeli(Pembeli* head);
void hapusSemuaPembeli(Pembeli*& head);
void tampilkanDetailPembeli(Pembeli* pembeli);
void editDataPembeli(Pembeli* pembeli);

void hitungDiskonTotal(Pembeli*& head, double diskon_total_persen);
void catatPembayaran(Pembeli* pembeli, double jumlah_bayar, char* keterangan);
void setMetodePembayaran(Pembeli* pembeli);

// Undo / Stack
void pushStateToStack(StateStack*& stack, Pembeli* head_pembeli, double diskon_total, int& state_id, char* deskripsi, int& stack_count);
void undoPembagian(StateStack*& stack, Pembeli*& head_pembeli, double& diskon_total, int& stack_count, LogAktivitas*& head_log, char* username);
void tampilkanHistoryStack(StateStack* stack);
void hapusSemuaStateStack(StateStack*& stack);

// Laporan
void cetakLaporanTransparansiPembayaran(Pembeli* head);
void tampilkanNotifikasiPembayaran(Pembeli* head);
void cetakStruk(Pembeli* head, double diskon_total_persen);
void buatLaporanLengkap(Pembeli* head, double diskon_total_persen);
void tampilkanStatistik(Pembeli* head);

// KTP (Doubly Linked List) - FIXED (NO CONST, NO STRING)
void createNodeKTP(NodeKTP*& baru, char* nik, char* nama, char* ttl, char* jk, char* alamat, char* agama, char* status, char* pekerjaan, char* wni, char* berlaku);
void insertLastKTP(NodeKTP*& head, NodeKTP*& tail, char* nik, char* nama, char* ttl, char* jk, char* alamat, char* agama, char* status, char* pekerjaan, char* wni, char* berlaku);
void displayForwardKTP(NodeKTP* head);
void hapusSemuaKTP(NodeKTP*& head, NodeKTP*& tail);

void menuUtama(char* role);

#endif