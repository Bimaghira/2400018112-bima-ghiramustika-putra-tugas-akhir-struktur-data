#ifndef SMARTSPLIT_H
#define SMARTSPLIT_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring> 

using namespace std;

// ============================================================================
// KONSTANTA
// ============================================================================
#define MAX_NAMA 100
#define MAX_BARANG 50
#define MAX_PEMBELI 100
#define MAX_USERNAME 50
#define MAX_PASSWORD 50
#define MAX_ROLE 20
#define MAX_UNDO_STATES 50

enum StatusPembayaran { BELUM_BAYAR = 0, SEBAGIAN_BAYAR = 1, SUDAH_BAYAR = 2 };

// ============================================================================
// STRUKTUR DATA UTAMA
// ============================================================================

struct User {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char email[MAX_NAMA];
    char role[MAX_ROLE]; 
    char tanggal_daftar[30];
    bool aktif;
    User* next;
};

struct UserData {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char email[MAX_NAMA];
    char role[MAX_ROLE];
    char tanggal_daftar[30];
    bool aktif;
};

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

struct NodeVoucher {
    int id_voucher;
    char kode_voucher[20];
    double nominal_potongan;
    char deskripsi[100];
    NodeVoucher* next; 
    NodeVoucher* prev; 
};

struct NodeBarang {
    int id_barang;
    char nama[MAX_NAMA];
    double harga;
    double diskon_persen;
    double harga_setelah_diskon;
    NodeBarang* next;
};

struct RiwayatPembayaran {
    int id_pembayaran;
    char tanggal[30];
    double jumlah_bayar;
    char keterangan[MAX_NAMA];
    RiwayatPembayaran* next;
};

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
    char metode_pembayaran[20];
    NodeBarang* head_barang;
    RiwayatPembayaran* head_riwayat;
    Pembeli* next;
};

struct LogAktivitas {
    char timestamp[30];
    char username[MAX_USERNAME];
    char aktivitas[200];
    LogAktivitas* next;
};

struct StateStack {
    int state_id;
    char deskripsi[100];
    StateStack* next;
};

// --- STRUKTUR DATA BARU ---
struct StackNode {
    char deskripsi_tugas[100];
    StackNode* next;
};

struct QueueNode {
    char username_pemohon[50];
    char tipe_batal[50]; 
    int target_id;
    char alasan[100];
    QueueNode* next;
};
struct QueuePembatalan {
    QueueNode* front;
    QueueNode* rear;
};

struct HuffNode {
    char data;
    unsigned freq;
    HuffNode *left, *right;
};
struct MinHeap {
    unsigned size;
    unsigned capacity;
    HuffNode** array;
};

struct AVLNode {
    int id_pembeli;
    char nama[100];
    int height;
    AVLNode *left;
    AVLNode *right;
};

// ============================================================================
// PROTOTYPE FUNGSI (SUDAH DIPERBAIKI DENGAN CONST CHAR*)
// ============================================================================

void getCopyTanggal(char* buffer, size_t size);

// KTP
void createNodeKTP(NodeKTP*& baru, const char* nik, const char* nama, const char* ttl, const char* jk, const char* alamat, const char* agama, const char* status, const char* pekerjaan, const char* wni, const char* berlaku);
void insertLastKTP(NodeKTP*& head, NodeKTP*& tail, const char* nik, const char* nama, const char* ttl, const char* jk, const char* alamat, const char* agama, const char* status, const char* pekerjaan, const char* wni, const char* berlaku);
void displayForwardKTP(NodeKTP* head);
void hapusSemuaKTP(NodeKTP*& head, NodeKTP*& tail);

// Core
User* loadUsersFromFile();
void saveUsersToFile(User* head);
void menuLogin(User*& head, User*& loggedInUser);
void tambahLog(LogAktivitas*& h, const char* u, const char* a);
Pembeli* tambahPembeli(Pembeli*& h, int id, const char* n);
void tambahBarangKeLinkedList(NodeBarang*& h, int id, const char* n, double p, double d);
int hitungJumlahBarang(NodeBarang* h);
void tampilkanSemuaPembeli(Pembeli* h);
Pembeli* cariPembeliById(Pembeli* h, int id);
void hapusSemuaPembeli(Pembeli*& h);
void hitungDiskonTotal(Pembeli*& head, double d);
void catatPembayaran(Pembeli* p, double b, const char* k);

// Voucher
void tambahVoucher(NodeVoucher*& head, NodeVoucher*& tail, int id, const char* kode, double nominal, const char* deskripsi);
void tampilkanVoucherMaju(NodeVoucher* head);
void hapusSemuaVoucher(NodeVoucher*& head, NodeVoucher*& tail);

// Stack Tugas
void pushStackTugas(StackNode*& top, const char* deskripsi);
void popStackTugas(StackNode*& top);
void printStackTugas(StackNode* top);

// Queue Pembatalan
void initQueue(QueuePembatalan& q);
void enqueueBatal(QueuePembatalan& q, const char* user, const char* tipe, int target, const char* alasan);
void dequeueBatal(QueuePembatalan& q); 
void printQueue(QueuePembatalan q);

// Huffman
void demoHuffman(const char* text);

// AVL
AVLNode* insertAVL(AVLNode* node, int id, const char* nama);
AVLNode* searchAVL(AVLNode* root, int id);

// Menu
void menuUtama(const char* role);

void saveUsersToFile(User* head);

#endif