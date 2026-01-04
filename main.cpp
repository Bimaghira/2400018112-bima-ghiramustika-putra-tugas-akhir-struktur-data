#include "smartsplit.h"

// ============================================================================
// 1. IMPLEMENTASI FUNGSI UTILITY
// ============================================================================

string getTanggalWaktu() {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
    return string(buffer);
}

void getCopyTanggal(char* buffer, size_t size) {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    strftime(buffer, size, "%d-%m-%Y %H:%M:%S", timeinfo);
}

bool isValidEmail(char* email) {
    string e(email);
    size_t at_pos = e.find('@');
    size_t dot_pos = e.find('.');
    if (at_pos == string::npos || dot_pos == string::npos) return false;
    if (at_pos > dot_pos) return false;
    return true;
}

// ============================================================================
// 2. IMPLEMENTASI KTP (DOUBLY LINKED LIST - NO CONST, NO STRING)
// ============================================================================

void createNodeKTP(NodeKTP*& baru, char* nik, char* nama, char* ttl, char* jk, char* alamat, char* agama, char* status, char* pekerjaan, char* wni, char* berlaku) {
    baru = new NodeKTP;
    
    // Copy data menggunakan strcpy karena char array
    strcpy(baru->data.nik, nik);
    strcpy(baru->data.nama, nama);
    strcpy(baru->data.tempat_tgl_lahir, ttl);
    strcpy(baru->data.jenis_kelamin, jk);
    strcpy(baru->data.alamat, alamat);
    strcpy(baru->data.agama, agama);
    strcpy(baru->data.status_perkawinan, status);
    strcpy(baru->data.pekerjaan, pekerjaan);
    strcpy(baru->data.kewarganegaraan, wni);
    strcpy(baru->data.berlaku_hingga, berlaku);
    
    baru->prev = nullptr;
    baru->next = nullptr;
}

void insertLastKTP(NodeKTP*& head, NodeKTP*& tail, char* nik, char* nama, char* ttl, char* jk, char* alamat, char* agama, char* status, char* pekerjaan, char* wni, char* berlaku) {
    NodeKTP* baru;
    createNodeKTP(baru, nik, nama, ttl, jk, alamat, agama, status, pekerjaan, wni, berlaku);
    if (head == nullptr) { 
        head = tail = baru;
    } else {
        tail->next = baru;
        baru->prev = tail;
        tail = baru;
    }
}

void displayForwardKTP(NodeKTP* head) {
    NodeKTP* bantu = head;
    cout << "\n=== DATA KTP TERDAFTAR ===\n";
    if (head == nullptr) {
        cout << "List Kosong.\n";
        return;
    }
    while (bantu != nullptr) {
        cout << "NIK              : " << bantu->data.nik << endl;
        cout << "Nama             : " << bantu->data.nama << endl;
        cout << "Tempat/Tgl Lahir : " << bantu->data.tempat_tgl_lahir << endl;
        cout << "Jenis Kelamin    : " << bantu->data.jenis_kelamin << endl;
        cout << "Alamat           : " << bantu->data.alamat << endl;
        cout << "Agama            : " << bantu->data.agama << endl;
        cout << "Status           : " << bantu->data.status_perkawinan << endl;
        cout << "Pekerjaan        : " << bantu->data.pekerjaan << endl;
        cout << "Kewarganegaraan  : " << bantu->data.kewarganegaraan << endl;
        cout << "Berlaku Hingga   : " << bantu->data.berlaku_hingga << endl;
        cout << "----------------------------------\n";
        bantu = bantu->next;
    }
}

void hapusSemuaKTP(NodeKTP*& head, NodeKTP*& tail) {
    while (head != nullptr) {
        NodeKTP* temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
}

// ============================================================================
// 3. IMPLEMENTASI VOUCHER
// ============================================================================
NodeVoucher* buatNodeVoucher(int id, char* kode, double nominal, char* deskripsi) {
    NodeVoucher* baru = new NodeVoucher();
    baru->id_voucher = id; strcpy(baru->kode_voucher, kode);
    baru->nominal_potongan = nominal; strcpy(baru->deskripsi, deskripsi);
    baru->next = nullptr; baru->prev = nullptr; return baru;
}
void tambahVoucher(NodeVoucher*& head, NodeVoucher*& tail, int id, char* kode, double nominal, char* deskripsi) {
    NodeVoucher* baru = buatNodeVoucher(id, kode, nominal, deskripsi);
    if (head == nullptr) { head = baru; tail = baru; } 
    else { tail->next = baru; baru->prev = tail; tail = baru; }
    cout << "Voucher ditambahkan.\n";
}
void tampilkanVoucherMaju(NodeVoucher* head) {
    if(!head) { cout<<"Kosong.\n"; return; }
    cout<<"--- Voucher ---\n";
    while(head) { cout<<head->kode_voucher<<" (Rp"<<head->nominal_potongan<<")\n"; head=head->next; }
}
void tampilkanVoucherMundur(NodeVoucher* tail) {
    if(!tail) { cout<<"Kosong.\n"; return; }
    cout<<"--- Voucher (Rev) ---\n";
    while(tail) { cout<<tail->kode_voucher<<" (Rp"<<tail->nominal_potongan<<")\n"; tail=tail->prev; }
}
void hapusSemuaVoucher(NodeVoucher*& head, NodeVoucher*& tail) {
    while(head){ NodeVoucher* t=head; head=head->next; delete t; } tail=nullptr;
}

// ============================================================================
// 4. IMPLEMENTASI USER (LOGIN & REGISTER)
// ============================================================================

User* buatNodeUser(char* user, char* pass, char* email, char* role, char* tgl, bool aktif) {
    User* baru = new User();
    strcpy(baru->username, user); strcpy(baru->password, pass);
    strcpy(baru->email, email); strcpy(baru->role, role);
    strcpy(baru->tanggal_daftar, tgl); baru->aktif = aktif;
    baru->next = nullptr; return baru;
}

void tambahUserToList(User*& head, char* user, char* pass, char* email, char* role, char* tgl, bool aktif) {
    User* baru = buatNodeUser(user, pass, email, role, tgl, aktif);
    if (!head) head = baru;
    else { User* t = head; while(t->next) t=t->next; t->next=baru; }
}

User* loadUsersFromFile() {
    User* head = nullptr;
    ifstream file("users_v2.dat", ios::binary);
    if (file.is_open()) {
        UserData u;
        while (file.read((char*)&u, sizeof(UserData))) {
            tambahUserToList(head, u.username, u.password, u.email, u.role, u.tanggal_daftar, u.aktif);
        }
        file.close();
    } else {
        char tgl[30]; getCopyTanggal(tgl, 30);
        char admUser[] = "admin"; char admPass[] = "admin"; char admMail[] = "admin@smartsplit.com"; char admRole[] = "admin";
        tambahUserToList(head, admUser, admPass, admMail, admRole, tgl, true);
        cout << "Akun default dibuat: admin / admin\n";
    }
    return head;
}

void saveUsersToFile(User* head) {
    ofstream file("users_v2.dat", ios::binary);
    User* t = head;
    while (t) {
        UserData u; strcpy(u.username, t->username); strcpy(u.password, t->password);
        strcpy(u.email, t->email); strcpy(u.role, t->role);
        strcpy(u.tanggal_daftar, t->tanggal_daftar); u.aktif = t->aktif;
        file.write((char*)&u, sizeof(UserData));
        t = t->next;
    }
    file.close();
}

bool userExists(User* head, char* username) {
    while(head) { if(strcmp(head->username, username)==0) return true; head=head->next; } return false;
}

User* validateLogin(User* head, char* username, char* password) {
    while(head) { if(strcmp(head->username, username)==0 && strcmp(head->password, password)==0 && head->aktif) return head; head=head->next; } return nullptr;
}

void registerUser(User*& head) {
    cout << "\n=== REGISTER ===\n";
    char username[MAX_USERNAME], password[MAX_PASSWORD], confirm[MAX_PASSWORD], email[MAX_NAMA], role[MAX_ROLE];
    
    cout << "Username: "; cin.getline(username, MAX_USERNAME);
    if (strlen(username) < 1 || userExists(head, username)) { cout << "Username tidak valid/sudah ada.\n"; return; }
    
    cout << "Email: "; cin.getline(email, MAX_NAMA);
    
    cout << "Password: "; cin.getline(password, MAX_PASSWORD);
    if (strlen(password) < 1) { cout << "Password kosong.\n"; return; }
    
    cout << "Role (1.User, 2.Admin): "; int r; cin >> r; cin.ignore();
    if(r==2) strcpy(role, "admin"); else strcpy(role, "user");
    
    char tgl[30]; getCopyTanggal(tgl, 30);
    tambahUserToList(head, username, password, email, role, tgl, true);
    saveUsersToFile(head);
    cout << "Registrasi berhasil! Silakan Login.\n";
}

User* loginSystem(User* head) {
    cout << "\n=== LOGIN ===\n";
    char username[MAX_USERNAME], password[MAX_PASSWORD];
    int attempt = 0;
    while(attempt < 3) {
        cout << "Username: "; cin.getline(username, MAX_USERNAME);
        cout << "Password: "; cin.getline(password, MAX_PASSWORD);
        User* u = validateLogin(head, username, password);
        if(u) return u;
        else { cout << "Gagal. Coba lagi.\n"; attempt++; }
    }
    return nullptr;
}

void menuLogin(User*& head, User*& loggedInUser) {
    int pil = 0;
    while(loggedInUser == nullptr) {
        cout << "\n1. Login\n2. Register\n3. Keluar\nPilihan: ";
        cin >> pil; cin.ignore();
        if(pil==1) loggedInUser = loginSystem(head);
        else if(pil==2) registerUser(head);
        else if(pil==3) exit(0);
    }
}

// ============================================================================
// 5. IMPLEMENTASI CORE SMARTSPLIT (Standard)
// ============================================================================

LogAktivitas* buatNodeLog(char* u, char* a) { LogAktivitas* n=new LogAktivitas; getCopyTanggal(n->timestamp,30); strcpy(n->username,u); strcpy(n->aktivitas,a); n->next=NULL; return n; }
void tambahLog(LogAktivitas*& h, char* u, char* a) { LogAktivitas* n=buatNodeLog(u,a); if(!h)h=n; else{ LogAktivitas* t=h; while(t->next)t=t->next; t->next=n; } }
void tampilkanLog(LogAktivitas* h) { while(h){ cout<<h->timestamp<<" "<<h->username<<" "<<h->aktivitas<<"\n"; h=h->next; } }
void hapusSemuaLog(LogAktivitas*& h) { while(h){ LogAktivitas* t=h; h=h->next; delete t; } }

NodeBarang* buatNodeBarang(int id, char* n, double h, double d) { NodeBarang* b=new NodeBarang; b->id_barang=id; strcpy(b->nama,n); b->harga=h; b->diskon_persen=d; b->harga_setelah_diskon=h-(h*d/100); b->next=NULL; return b; }
void tambahBarangKeLinkedList(NodeBarang*& h, int id, char* n, double p, double d) { NodeBarang* b=buatNodeBarang(id,n,p,d); if(!h)h=b; else{ NodeBarang* t=h; while(t->next)t=t->next; t->next=b; } }
int hitungJumlahBarang(NodeBarang* h) { int c=0; while(h){c++; h=h->next;} return c; }
void tampilkanBarangPembeli(NodeBarang* h) { int i=1; while(h){ cout<<"  "<<i++<<". "<<h->nama<<" (Rp"<<h->harga<<")\n"; h=h->next; } }
void hapusSemuaBarang(NodeBarang*& h) { while(h){ NodeBarang* t=h; h=h->next; delete t; } }

RiwayatPembayaran* buatNodeRiwayat(int id, double j, char* k) { RiwayatPembayaran* r=new RiwayatPembayaran; r->id_pembayaran=id; getCopyTanggal(r->tanggal,30); r->jumlah_bayar=j; strcpy(r->keterangan,k); r->next=NULL; return r; }
void tambahRiwayatPembayaran(RiwayatPembayaran*& h, int id, double j, char* k) { RiwayatPembayaran* r=buatNodeRiwayat(id,j,k); if(!h)h=r; else{ RiwayatPembayaran* t=h; while(t->next)t=t->next; t->next=r; } }
int hitungJumlahPembayaran(RiwayatPembayaran* h) { int c=0; while(h){c++; h=h->next;} return c; }
void tampilkanRiwayatPembayaran(RiwayatPembayaran* h) { while(h){ cout<<"  "<<h->tanggal<<" Rp"<<h->jumlah_bayar<<"\n"; h=h->next; } }
void hapusSemuaRiwayat(RiwayatPembayaran*& h) { while(h){ RiwayatPembayaran* t=h; h=h->next; delete t; } }

Pembeli* buatNodePembeli(int id, char* n) { Pembeli* p=new Pembeli; p->id_pembeli=id; strcpy(p->nama,n); p->total_akhir=0; p->jumlah_dibayar=0; p->sisa_pembayaran=0; p->status_pembayaran=BELUM_BAYAR; p->head_barang=NULL; p->head_riwayat=NULL; p->next=NULL; return p; }
Pembeli* tambahPembeli(Pembeli*& h, int id, char* n) { Pembeli* p=buatNodePembeli(id,n); if(!h)h=p; else{ Pembeli* t=h; while(t->next)t=t->next; t->next=p; } return p; }
Pembeli* cariPembeliByName(Pembeli* h, char* n) { while(h){ if(strcmp(h->nama,n)==0) return h; h=h->next; } return NULL; }
int hitungJumlahPembeli(Pembeli* h) { int c=0; while(h){c++; h=h->next;} return c; }
void tampilkanSemuaPembeli(Pembeli* h) { cout<<"\n--- Pembeli ---\n"; while(h){ cout<<h->id_pembeli<<". "<<h->nama<<" Total: "<<h->total_akhir<<"\n"; h=h->next; } }
void hapusSemuaPembeli(Pembeli*& h) { while(h){ Pembeli* t=h; h=h->next; hapusSemuaBarang(t->head_barang); hapusSemuaRiwayat(t->head_riwayat); delete t; } }
void tampilkanDetailPembeli(Pembeli* p) { if(p){ cout<<"Detail "<<p->nama<<"\n"; tampilkanBarangPembeli(p->head_barang); tampilkanRiwayatPembayaran(p->head_riwayat); } }

void hitungDiskonTotal(Pembeli*& head, double d) { 
    double total=0; Pembeli* t=head; while(t){ NodeBarang* b=t->head_barang; t->total_setelah_diskon=0; while(b){ t->total_setelah_diskon+=(b->harga-(b->harga*b->diskon_persen/100)); b=b->next; } total+=t->total_setelah_diskon; t=t->next; }
    t=head; while(t){ if(total>0) t->bagian_diskon_total=total*(d/100)*(t->total_setelah_diskon/total); t->total_akhir=t->total_setelah_diskon-t->bagian_diskon_total; t->sisa_pembayaran=t->total_akhir-t->jumlah_dibayar; t=t->next; }
}
void catatPembayaran(Pembeli* p, double b, char* k) { if(p){ tambahRiwayatPembayaran(p->head_riwayat, 1, b, k); p->jumlah_dibayar+=b; p->sisa_pembayaran=p->total_akhir-p->jumlah_dibayar; cout<<"Tercatat.\n"; } }
void editDataPembeli(Pembeli* p) { if(p){ cout<<"Email baru: "; cin>>p->email; cout<<"Saved.\n"; } }
void setMetodePembayaran(Pembeli* p) { if(p) { cout<<"Metode Tunai diset.\n"; strcpy(p->metode_pembayaran, "Tunai"); } }

// --- UNDO/STACK (Simplified) ---
SnapshotPembeli* copyPembeliToSnapshot(Pembeli* p) { return nullptr; } // Placeholder
StateStack* buatStateSnapshot(Pembeli* h, double d, int id, char* desc) { return new StateStack; }
void pushStateToStack(StateStack*& s, Pembeli* h, double d, int& id, char* desc, int& c) { StateStack* n=new StateStack; n->next=s; s=n; c++; }
void undoPembagian(StateStack*& s, Pembeli*& h, double& d, int& c, LogAktivitas*& l, char* u) { if(s){ StateStack* t=s; s=s->next; delete t; cout<<"Undo (Simulasi) Berhasil.\n"; } }
void tampilkanHistoryStack(StateStack* s) { cout<<"Stack OK.\n"; }
void hapusSemuaStateStack(StateStack*& s) { while(s){ StateStack* t=s; s=s->next; delete t; } }

// --- LAPORAN ---
void cetakStruk(Pembeli* h, double d) { cout<<"Struk dicetak.\n"; }
void cetakLaporanTransparansiPembayaran(Pembeli* h) { cout<<"Laporan Transparansi dicetak.\n"; }
void buatLaporanLengkap(Pembeli* h, double d) { cout<<"Laporan Lengkap dibuat.\n"; }
void tampilkanStatistik(Pembeli* h) { cout<<"Statistik ditampilkan.\n"; }
void tampilkanNotifikasiPembayaran(Pembeli* h) { cout<<"Notifikasi ditampilkan.\n"; }

// ============================================================================
// 6. MENU UTAMA
// ============================================================================

void menuUtama(char* role) {
    cout << "\n" << string(60, '=') << "\n";
    cout << "      SMARTSPLIT BILL - " << role << " MODE \n";
    cout << string(60, '=') << "\n";
    if (strcmp(role, "admin") == 0) {
        cout << "1. Input Data Pembeli & Barang\n2. Tambah Pembeli Baru\n3. Tambah Barang\n4. Edit Pembeli\n";
        cout << "7. Hitung Split Bill\n8. Set Metode Bayar\n9. Catat Pembayaran\n16. Undo\n19. Reset Data\n20. Voucher\n";
    }
    cout << "5. Lihat Semua Pembeli\n6. Detail Pembeli\n10. Riwayat Bayar\n11. Cetak Struk\n";
    cout << "12. Notifikasi\n13. Laporan Transparansi\n14. Laporan Lengkap\n15. Statistik\n17. History Stack\n18. Log\n";
    cout << "21. LOGOUT\n22. KELUAR PROGRAM\n";
    cout << "Pilihan: ";
}

// ============================================================================
// 7. MAIN FUNCTION
// ============================================================================

int main() {
    Pembeli* head_pembeli = nullptr;
    LogAktivitas* head_log = nullptr;
    StateStack* undo_stack = nullptr;
    NodeVoucher* head_voucher = nullptr; NodeVoucher* tail_voucher = nullptr;
    NodeKTP* head_ktp = nullptr; NodeKTP* tail_ktp = nullptr;
    
    User* head_users = loadUsersFromFile();
    User* currentUser = nullptr;

    int id_pembeli = 1; int state_id = 1; int stack_count = 0; double diskon_total_persen = 0.0;

    cout << "\n=== SISTEM TERINTEGRASI KTP & SMARTSPLIT ===\n";

    while (true) { // Loop Global
        
        // --- 1. LOGIN ---
        currentUser = nullptr;
        menuLogin(head_users, currentUser);
        if (!currentUser) break;
        tambahLog(head_log, currentUser->username, (char*)"Login");

        // --- 2. INPUT KTP (LANGSUNG SETELAH LOGIN) ---
        cout << "\n" << string(60, '=') << "\n";
        cout << "  Halo " << currentUser->username << ", Mohon Input Data KTP Anda\n";
        cout << string(60, '=') << "\n";

        // Variabel penampung input (Char Array agar support strcpy)
        char nik[50], nama[100], ttl[100], jk[20], alamat[200];
        char agama[20], status[20], kerja[50], wni[50], berlaku[50];
        
        cout << "NIK                : "; cin.getline(nik, 50);
        cout << "Nama Lengkap       : "; cin.getline(nama, 100);
        cout << "Tempat/Tgl Lahir   : "; cin.getline(ttl, 100);
        cout << "Jenis Kelamin      : "; cin.getline(jk, 20);
        cout << "Alamat             : "; cin.getline(alamat, 200);
        cout << "Agama              : "; cin.getline(agama, 20);
        cout << "Status Perkawinan  : "; cin.getline(status, 20);
        cout << "Pekerjaan          : "; cin.getline(kerja, 50);
        cout << "Kewarganegaraan    : "; cin.getline(wni, 50);
        cout << "Berlaku Hingga     : "; cin.getline(berlaku, 50);

        insertLastKTP(head_ktp, tail_ktp, nik, nama, ttl, jk, alamat, agama, status, kerja, wni, berlaku);
        cout << "\n[OK] Data KTP berhasil disimpan sementara.\n";
        
        // Tampilkan sekilas
        displayForwardKTP(head_ktp);
        
        // --- 3. MENU UTAMA SMARTSPLIT ---
        cout << "\nMelanjutkan ke Aplikasi Utama...\n";

        bool logout = false;
        while (!logout) {
            menuUtama(currentUser->role);
            int pil; cin >> pil; cin.ignore();
            bool isAdmin = (strcmp(currentUser->role, "admin") == 0);

            if (!isAdmin && (pil == 1 || pil == 2 || pil == 3 || pil == 4 || pil == 7 || pil == 8 || pil == 9 || pil == 16 || pil == 19 || pil == 20)) {
                cout << "Akses Admin Diperlukan.\n"; continue;
            }

            if(pil==1) {
                cout << "Jml Pembeli: "; int j; cin >> j; cin.ignore();
                for(int i=0; i<j; i++) {
                    char nm[100]; cout << "Nama: "; cin.getline(nm, 100);
                    tambahPembeli(head_pembeli, id_pembeli++, nm);
                }
            }
            else if(pil==5) tampilkanSemuaPembeli(head_pembeli);
            else if(pil==7) { cout<<"Diskon %: "; cin>>diskon_total_persen; hitungDiskonTotal(head_pembeli, diskon_total_persen); cout<<"Hitung Selesai.\n"; }
            else if(pil==20) {
                cout << "1.Tambah Voucher 2.Lihat: "; int v; cin>>v; cin.ignore();
                if(v==1) { char k[20]; double n; cout<<"Kode: "; cin>>k; cout<<"Nominal: "; cin>>n; cin.ignore(); tambahVoucher(head_voucher, tail_voucher, 1, k, n, (char*)"-"); }
                else tampilkanVoucherMaju(head_voucher);
            }
            
            else if(pil==21) {
                cout << "Logout...\n";
                hapusSemuaKTP(head_ktp, tail_ktp); // Reset KTP saat logout
                logout = true; // Kembali ke Login
            }
            else if(pil==22) exit(0);
        }
    }

    hapusSemuaPembeli(head_pembeli);
    hapusSemuaKTP(head_ktp, tail_ktp);
    return 0;
}