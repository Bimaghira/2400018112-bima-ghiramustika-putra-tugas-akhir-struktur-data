#include "pintar.h"

// ============================================================================
// 1. UTILITY & KTP
// ============================================================================
void getCopyTanggal(char* buffer, size_t size) {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    strftime(buffer, size, "%d-%m-%Y %H:%M:%S", timeinfo);
}

// Implementasi Create Node KTP dengan Data Lengkap
void createNodeKTP(NodeKTP*& baru, const char* nik, const char* nama, const char* ttl, const char* jk, const char* alamat, const char* agama, const char* status, const char* pekerjaan, const char* wni, const char* berlaku) {
    baru = new NodeKTP;
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
    baru->prev = NULL; 
    baru->next = NULL;
}

void insertLastKTP(NodeKTP*& head, NodeKTP*& tail, const char* nik, const char* nama, const char* ttl, const char* jk, const char* alamat, const char* agama, const char* status, const char* pekerjaan, const char* wni, const char* berlaku) {
    NodeKTP* baru;
    createNodeKTP(baru, nik, nama, ttl, jk, alamat, agama, status, pekerjaan, wni, berlaku);
    if (head == NULL) { head = tail = baru; } 
    else { tail->next = baru; baru->prev = tail; tail = baru; }
}

void displayForwardKTP(NodeKTP* head) {
    if (!head) { cout << "Data KTP Kosong.\n"; return; }
    cout << "\n============================================\n";
    cout << "            DATA KTP TERDAFTAR              \n";
    cout << "============================================\n";
    while (head) {
        cout << "NIK              : " << head->data.nik << "\n";
        cout << "Nama             : " << head->data.nama << "\n";
        cout << "Tempat/Tgl Lahir : " << head->data.tempat_tgl_lahir << "\n";
        cout << "Jenis Kelamin    : " << head->data.jenis_kelamin << "\n";
        cout << "Alamat           : " << head->data.alamat << "\n";
        cout << "Agama            : " << head->data.agama << "\n";
        cout << "Status           : " << head->data.status_perkawinan << "\n";
        cout << "Pekerjaan        : " << head->data.pekerjaan << "\n";
        cout << "Kewarganegaraan  : " << head->data.kewarganegaraan << "\n";
        cout << "Berlaku Hingga   : " << head->data.berlaku_hingga << "\n";
        cout << "--------------------------------------------\n";
        head = head->next;
    }
}

void hapusSemuaKTP(NodeKTP*& head, NodeKTP*& tail) {
    while (head) { NodeKTP* t = head; head = head->next; delete t; }
    tail = NULL;
}

// ============================================================================
// 2. VOUCHER & USER
// ============================================================================
void tambahVoucher(NodeVoucher*& head, NodeVoucher*& tail, int id, const char* kode, double nominal, const char* deskripsi) {
    NodeVoucher* baru = new NodeVoucher;
    baru->id_voucher = id; 
    strcpy(baru->kode_voucher, kode); 
    baru->nominal_potongan = nominal; 
    strcpy(baru->deskripsi, deskripsi);
    baru->next = NULL; baru->prev = NULL;
    if (!head) { head = tail = baru; } else { tail->next = baru; baru->prev = tail; tail = baru; }
    cout << "Voucher ditambahkan.\n";
}
void tampilkanVoucherMaju(NodeVoucher* head) {
    if(!head) cout<<"Kosong.\n"; else while(head){cout<<head->kode_voucher<<" (Rp"<<head->nominal_potongan<<")\n"; head=head->next;}
}
void hapusSemuaVoucher(NodeVoucher*& head, NodeVoucher*& tail) { while(head){NodeVoucher* t=head; head=head->next; delete t;} tail=NULL;}

void tambahUserToList(User*& head, const char* u, const char* p, const char* e, const char* r, const char* t, bool a) {
    User* baru = new User;
    strcpy(baru->username, u); strcpy(baru->password, p);
    strcpy(baru->email, e); strcpy(baru->role, r);
    strcpy(baru->tanggal_daftar, t); baru->aktif = a; baru->next = NULL;
    if(!head) head=baru; else {User* tmp=head; while(tmp->next) tmp=tmp->next; tmp->next=baru;}
}

User* loadUsersFromFile() {
    User* head = NULL;
    ifstream file("users_v2.dat", ios::binary);
    if (file.is_open()) {
        UserData u; 
        while (file.read((char*)&u, sizeof(UserData))) {
            tambahUserToList(head, u.username, u.password, u.email, u.role, u.tanggal_daftar, u.aktif);
        }
        file.close();
    } else {
        char tgl[30]; getCopyTanggal(tgl, 30);
        char u[] = "admin"; char p[] = "admin"; char e[] = "admin@sys.com"; char r[] = "admin";
        tambahUserToList(head, u, p, e, r, tgl, true);
        cout << "Akun default dibuat: admin / admin\n";
    }
    return head;
}

bool userExists(User* head, const char* username) {
    while(head) { if(strcmp(head->username, username)==0) return true; head=head->next; } return false;
}

void registerUser(User*& head) {
    cout << "\n=== REGISTER USER BARU ===\n";
    char username[MAX_USERNAME], password[MAX_PASSWORD], email[MAX_NAMA], role[MAX_ROLE];
    
    cout << "Username: "; cin.getline(username, MAX_USERNAME);
    if (userExists(head, username)) { cout << "Username sudah ada!\n"; return; }
    
    cout << "Password: "; cin.getline(password, MAX_PASSWORD);
    cout << "Email: "; cin.getline(email, MAX_NAMA);
    
    cout << "Role (1. User / 2. Admin): "; int r; cin >> r; cin.ignore();
    if (r == 2) strcpy(role, "admin"); else strcpy(role, "user");
    
    char tgl[30]; getCopyTanggal(tgl, 30);
    tambahUserToList(head, username, password, email, role, tgl, true);
    saveUsersToFile(head);
    cout << "Registrasi Berhasil! Silakan Login.\n";
}

User* validateLogin(User* h, const char* u, const char* p) { 
    while(h){if(strcmp(h->username,u)==0 && strcmp(h->password,p)==0) return h; h=h->next;} return NULL; 
}

User* loginSystem(User* head) {
    cout << "\n=== LOGIN SYSTEM ===\n";
    char username[MAX_USERNAME], password[MAX_PASSWORD];
    int attempt = 0;
    while(attempt < 3) {
        cout << "Username: "; cin.getline(username, MAX_USERNAME);
        cout << "Password: "; cin.getline(password, MAX_PASSWORD);
        User* u = validateLogin(head, username, password);
        if(u) return u;
        else { cout << "Login Gagal. Sisa percobaan: " << (2 - attempt) << "\n"; attempt++; }
    }
    return NULL;
}

void menuLogin(User*& head, User*& loggedInUser) {
    int pil = 0;
    while (loggedInUser == NULL) {
        cout << "\n================================\n";
        cout << "   SMARTSPLIT - AUTHENTICATION  \n";
        cout << "================================\n";
        cout << "1. Login\n";
        cout << "2. Register (Daftar Baru)\n"; 
        cout << "3. Keluar Aplikasi\n";
        cout << "Pilihan: "; 
        cin >> pil; cin.ignore();

        if (pil == 1) {
            loggedInUser = loginSystem(head);
        } else if (pil == 2) {
            registerUser(head);
        } else if (pil == 3) {
            cout << "Terima kasih.\n";
            exit(0);
        } else {
            cout << "Pilihan tidak valid.\n";
        }
    }
}

void saveUsersToFile(User* head) {
    ofstream file("users.txt");
    User* curr = head;

    while (curr != NULL) {
        file << curr->username << "|"
             << curr->email << "|"
             << curr->role << "|"
             << curr->aktif << endl;
        curr = curr->next;
    }

    file.close();
}


// ============================================================================
// 3. CORE LOGIC
// ============================================================================
void tambahLog(LogAktivitas*& h, const char* u, const char* a) {
    LogAktivitas* n = new LogAktivitas; getCopyTanggal(n->timestamp,30);
    strcpy(n->username, u); strcpy(n->aktivitas, a); n->next = NULL;
    if(!h) h=n; else { LogAktivitas* t=h; while(t->next) t=t->next; t->next=n; }
}

Pembeli* tambahPembeli(Pembeli*& h, int id, const char* n) {
    Pembeli* p = new Pembeli; p->id_pembeli=id; strcpy(p->nama,n);
    p->total_akhir=0; p->jumlah_dibayar=0; p->sisa_pembayaran=0; p->status_pembayaran=BELUM_BAYAR;
    p->head_barang=NULL; p->head_riwayat=NULL; p->next=NULL;
    if(!h) h=p; else { Pembeli* t=h; while(t->next) t=t->next; t->next=p; }
    return p;
}

void tambahBarangKeLinkedList(NodeBarang*& h, int id, const char* n, double p, double d) {
    NodeBarang* b = new NodeBarang; b->id_barang=id; strcpy(b->nama,n); b->harga=p; b->diskon_persen=d;
    b->harga_setelah_diskon = p - (p*d/100); b->next=NULL;
    if(!h) h=b; else { NodeBarang* t=h; while(t->next)t=t->next; t->next=b; }
}

int hitungJumlahBarang(NodeBarang* h) { int c=0; while(h){c++; h=h->next;} return c; }

void tampilkanSemuaPembeli(Pembeli* h) { 
    cout<<"\n--- PEMBELI ---\n"; 
    while(h){ cout<<"ID:"<<h->id_pembeli<<" | "<<h->nama<<" | Total: "<<h->total_akhir<<"\n"; h=h->next;} 
}

Pembeli* cariPembeliById(Pembeli* h, int id) { while(h){ if(h->id_pembeli == id) return h; h=h->next; } return NULL; }

void hapusSemuaPembeli(Pembeli*& h) { while(h){ Pembeli* t=h; h=h->next; delete t;} }

void hitungDiskonTotal(Pembeli*& head, double d) {
    double grand=0; Pembeli* t=head; 
    while(t){ 
        NodeBarang* b=t->head_barang; t->total_setelah_diskon=0; 
        while(b){t->total_setelah_diskon+=(b->harga_setelah_diskon); b=b->next;} 
        grand+=t->total_setelah_diskon; t=t->next;
    }
    t=head; 
    while(t){ 
        if(grand>0) t->bagian_diskon_total=grand*(d/100)*(t->total_setelah_diskon/grand); 
        t->total_akhir=t->total_setelah_diskon-t->bagian_diskon_total; 
        t->sisa_pembayaran=t->total_akhir-t->jumlah_dibayar; 
        t=t->next; 
    }
}

void tambahRiwayatPembayaran(RiwayatPembayaran*& h, int id, double j, const char* k) {
    RiwayatPembayaran* r = new RiwayatPembayaran;
    r->id_pembayaran = id; getCopyTanggal(r->tanggal, 30);
    r->jumlah_bayar = j; strcpy(r->keterangan, k); r->next = NULL;
    if(!h) h=r; else { RiwayatPembayaran* t=h; while(t->next) t=t->next; t->next=r; }
}

void catatPembayaran(Pembeli* p, double b, const char* k) {
    if(p) {
        tambahRiwayatPembayaran(p->head_riwayat, 1, b, k); 
        p->jumlah_dibayar += b; 
        p->sisa_pembayaran = p->total_akhir - p->jumlah_dibayar;
        cout << "Pembayaran berhasil dicatat.\n";
    }
}

// ============================================================================
// 4. IMPLEMENTASI FITUR BARU
// ============================================================================

// --- A. STACK TUGAS ---
void pushStackTugas(StackNode*& top, const char* deskripsi) {
    StackNode* baru = new StackNode;
    strcpy(baru->deskripsi_tugas, deskripsi);
    baru->next = top;
    top = baru;
    cout << "[Stack] Tugas masuk.\n";
}
void popStackTugas(StackNode*& top) {
    if (!top) { cout << "[Stack] Kosong.\n"; return; }
    StackNode* temp = top;
    cout << "[Stack] Mengerjakan: " << temp->deskripsi_tugas << "\n";
    top = top->next;
    delete temp;
}
void printStackTugas(StackNode* top) {
    cout << "\n=== STACK (LIFO) ===\n";
    if(!top) cout<<"(Kosong)\n";
    while(top) { cout << ">> " << top->deskripsi_tugas << "\n"; top = top->next; }
}

// --- B. QUEUE PEMBATALAN ---
void initQueue(QueuePembatalan& q) { q.front = q.rear = NULL; }
void enqueueBatal(QueuePembatalan& q, const char* user, const char* tipe, int target, const char* alasan) {
    QueueNode* baru = new QueueNode;
    strcpy(baru->username_pemohon, user); strcpy(baru->tipe_batal, tipe);
    baru->target_id = target; strcpy(baru->alasan, alasan);
    baru->next = NULL;
    if (q.rear == NULL) { q.front = q.rear = baru; return; }
    q.rear->next = baru; q.rear = baru;
    cout << "[Queue] Request dikirim.\n";
}
void dequeueBatal(QueuePembatalan& q) {
    if (q.front == NULL) { cout << "[Queue] Tidak ada request.\n"; return; }
    QueueNode* temp = q.front;
    
    cout << "\n>>> APPROVAL ADMIN <<<\n";
    cout << "User: " << temp->username_pemohon << " | Tipe: " << temp->tipe_batal << "\n";
    cout << "ID: " << temp->target_id << " | Alasan: " << temp->alasan << "\n";
    cout << "Setujui? (y/n): "; char ans; cin >> ans; cin.ignore();

    if (ans == 'y' || ans == 'Y') cout << "Disetujui.\n";
    else cout << "Ditolak.\n";

    q.front = q.front->next;
    if (q.front == NULL) q.rear = NULL;
    delete temp;
}
void printQueue(QueuePembatalan q) {
    cout << "\n=== QUEUE (FIFO) ===\n";
    QueueNode* t = q.front;
    if(!t) cout << "(Kosong)\n";
    while(t) {
        cout << ">> " << t->username_pemohon << ": " << t->tipe_batal << " ID " << t->target_id << "\n";
        t = t->next;
    }
}

// --- C. HUFFMAN TREE ---
HuffNode* newNode(char data, unsigned freq) {
    HuffNode* temp = new HuffNode;
    temp->left = temp->right = NULL;
    temp->data = data; temp->freq = freq;
    return temp;
}
MinHeap* createMinHeap(unsigned capacity) {
    MinHeap* minHeap = new MinHeap;
    minHeap->size = 0; minHeap->capacity = capacity;
    minHeap->array = new HuffNode*[capacity];
    return minHeap;
}
void swapMinHeapNode(HuffNode** a, HuffNode** b) { HuffNode* t = *a; *a = *b; *b = t; }
void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx; int left = 2 * idx + 1; int right = 2 * idx + 2;
    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq) smallest = left;
    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq) smallest = right;
    if (smallest != idx) { swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]); minHeapify(minHeap, smallest); }
}
HuffNode* extractMin(MinHeap* minHeap) {
    HuffNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1]; --minHeap->size;
    minHeapify(minHeap, 0); return temp;
}
void insertMinHeap(MinHeap* minHeap, HuffNode* minHeapNode) {
    ++minHeap->size; int i = minHeap->size - 1;
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2]; i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}
void buildMinHeap(MinHeap* minHeap) { int n = minHeap->size - 1; for (int i = (n - 1) / 2; i >= 0; --i) minHeapify(minHeap, i); }
HuffNode* buildHuffmanTree(const char* data, int freq[], int size) {
    HuffNode *left, *right, *top;
    MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i) minHeap->array[i] = newNode(data[i], freq[i]);
    minHeap->size = size; buildMinHeap(minHeap);
    while (minHeap->size != 1) {
        left = extractMin(minHeap); right = extractMin(minHeap);
        top = newNode('$', left->freq + right->freq);
        top->left = left; top->right = right; insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}
void printCodes(HuffNode* root, int arr[], int top, int& totalBits) {
    if (root->left) { arr[top] = 0; printCodes(root->left, arr, top + 1, totalBits); }
    if (root->right) { arr[top] = 1; printCodes(root->right, arr, top + 1, totalBits); }
    if (!root->left && !root->right) {
        cout << root->data << ": ";
        for (int i = 0; i < top; ++i) cout << arr[i];
        cout << "\n";
        totalBits += (top * root->freq);
    }
}
void demoHuffman(const char* text) {
    cout << "\n=== DEMO HUFFMAN TREE ===\n";
    int freq[256] = {0}; 
    int n = strlen(text); 
    for (int i = 0; i < n; i++) freq[(unsigned char)text[i]]++;
    char data[256]; int f[256]; int size = 0;
    for (int i = 0; i < 256; i++) { if (freq[i] > 0) { data[size] = (char)i; f[size] = freq[i]; size++; } }
    
    HuffNode* root = buildHuffmanTree(data, f, size);
    int arr[100], top = 0, compressedBits = 0;
    printCodes(root, arr, top, compressedBits);
    
    int originalBits = n * 8;
    cout << "Size Asli: " << originalBits << " bits | Kompresi: " << compressedBits << " bits\n";
}

// --- D. AVL TREE ---
int height(AVLNode *N) { if (N == NULL) return 0; return N->height; }
int max(int a, int b) { return (a > b)? a : b; }
AVLNode* newAVLNode(int id, const char* nama) {
    AVLNode* node = new AVLNode;
    node->id_pembeli = id; strcpy(node->nama, nama);
    node->left = NULL; node->right = NULL; node->height = 1;
    return(node);
}
AVLNode* rightRotate(AVLNode *y) {
    AVLNode *x = y->left; AVLNode *T2 = x->right;
    x->right = y; y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}
AVLNode* leftRotate(AVLNode *x) {
    AVLNode *y = x->right; AVLNode *T2 = y->left;
    y->left = x; x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}
int getBalance(AVLNode *N) { if (N == NULL) return 0; return height(N->left) - height(N->right); }
AVLNode* insertAVL(AVLNode* node, int id, const char* nama) {
    if (node == NULL) return(newAVLNode(id, nama));
    if (id < node->id_pembeli) node->left = insertAVL(node->left, id, nama);
    else if (id > node->id_pembeli) node->right = insertAVL(node->right, id, nama);
    else return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && id < node->left->id_pembeli) return rightRotate(node);
    if (balance < -1 && id > node->right->id_pembeli) return leftRotate(node);
    if (balance > 1 && id > node->left->id_pembeli) { node->left = leftRotate(node->left); return rightRotate(node); }
    if (balance < -1 && id < node->right->id_pembeli) { node->right = rightRotate(node->right); return leftRotate(node); }
    return node;
}
AVLNode* searchAVL(AVLNode* root, int id) {
    if (root == NULL || root->id_pembeli == id) return root;
    if (root->id_pembeli < id) return searchAVL(root->right, id);
    return searchAVL(root->left, id);
}

// ============================================================================
// 6. MENU & MAIN
// ============================================================================
void menuUtama(const char* role) {
    char admin[] = "admin";
    cout << "\n================================\n";
    cout << "      SMARTSPLIT - " << role << " MODE \n";
    cout << "================================\n";
    if (strcmp(role, admin) == 0) {
        cout << "1. Input Data Pembeli\n";
        cout << "7. Hitung Split Bill\n";
        cout << "30. [Queue] Approve Pembatalan\n";
        cout << "31. [Stack] Proses Tugas\n";
    } else {
        cout << "40. [Queue] Request Pembatalan\n";
    }
    cout << "5. Lihat Semua Pembeli\n";
    cout << "50. Cari Pembeli (AVL Tree)\n";
    cout << "51. Demo Kompresi Huffman\n";
    cout << "20. Voucher\n";
    cout << "21. LOGOUT\n";
    cout << "Pilihan: ";
}
void tampilkanHasilSplitBill(Pembeli* head) {
    cout << "\n===== HASIL SPLIT BILL =====\n";

    if (!head) {
        cout << "Tidak ada data pembeli.\n";
        return;
    }

    while (head) {
        cout << "Nama Pembeli        : " << head->nama << endl;
        cout << "Total Barang        : Rp" << head->total_setelah_diskon << endl;
        cout << "Bagian Diskon       : Rp" << head->bagian_diskon_total << endl;
        cout << "Total Akhir         : Rp" << head->total_akhir << endl;
        cout << "Sisa Pembayaran     : Rp" << head->sisa_pembayaran << endl;
        cout << "------------------------------\n";
        head = head->next;
    }
}

int main() {
    Pembeli* head_pembeli = NULL;
    LogAktivitas* head_log = NULL;
    NodeVoucher* head_voucher = NULL; NodeVoucher* tail_voucher = NULL;
    NodeKTP* head_ktp = NULL; NodeKTP* tail_ktp = NULL;
    
    StackNode* task_stack = NULL; 
    QueuePembatalan cancel_queue; initQueue(cancel_queue); 
    AVLNode* avl_root = NULL; 

    User* head_users = loadUsersFromFile();
    User* currentUser = NULL;
    int id_pembeli = 1; double diskon_total_persen = 0.0;

    while (true) { 
        currentUser = NULL;
        menuLogin(head_users, currentUser);
        if (!currentUser) break;
        
        char l[]="Login";
        tambahLog(head_log, currentUser->username, l);

        // --- INPUT KTP (LENGKAP) ---
        cout << "\n======================================\n";
        cout << "   Halo, " << currentUser->username << "! Mohon Lengkapi Data KTP\n";
        cout << "======================================\n";
        
        char nik[50], nama[100], ttl[100], jk[20], alamat[200], agama[20], status[20], kerja[50], wni[50], berlaku[50];
        
        cout << "1. NIK                : "; cin.getline(nik, 50);
        cout << "2. Nama Lengkap       : "; cin.getline(nama, 100);
        cout << "3. Tempat/Tgl Lahir   : "; cin.getline(ttl, 100);
        cout << "4. Jenis Kelamin      : "; cin.getline(jk, 20);
        cout << "5. Alamat             : "; cin.getline(alamat, 200);
        cout << "6. Agama              : "; cin.getline(agama, 20);
        cout << "7. Status Perkawinan  : "; cin.getline(status, 20);
        cout << "8. Pekerjaan          : "; cin.getline(kerja, 50);
        cout << "9. Kewarganegaraan    : "; cin.getline(wni, 50);
        cout << "10. Berlaku Hingga    : "; cin.getline(berlaku, 50);
        
        insertLastKTP(head_ktp, tail_ktp, nik, nama, ttl, jk, alamat, agama, status, kerja, wni, berlaku);
        cout << "\n[SUKSES] Data KTP Tersimpan.\n";
        
        // Tampilkan Data yang baru diinput
        displayForwardKTP(head_ktp);

        bool logout = false;
        while (!logout) {
            menuUtama(currentUser->role);
            int pil; cin >> pil; cin.ignore();
            
            char admin[] = "admin";
            bool isAdmin = (strcmp(currentUser->role, admin) == 0);

            if (pil == 1 && isAdmin) {
    		cout << "Jml Pembeli: ";
    int j; cin >> j; cin.ignore();

    for(int i=0; i<j; i++) {
        char nm[100];
        cout << "Nama Pembeli: ";
        cin.getline(nm, 100);

        Pembeli* p = tambahPembeli(head_pembeli, id_pembeli, nm);
        avl_root = insertAVL(avl_root, id_pembeli, nm);

        // STACK
        char task[100]; 
        strcpy(task, "Verifikasi: ");
        strcat(task, nm);
        pushStackTugas(task_stack, task);

        // === INPUT BARANG ===
        cout << "Jumlah Barang: ";
        int jb; cin >> jb; cin.ignore();

        for(int b=0; b<jb; b++) {
            char nb[100];
            double harga, diskon;

            cout << "  Nama Barang   : ";
            cin.getline(nb, 100);
            cout << "  Harga         : ";
            cin >> harga;
            cout << "  Diskon (%)    : ";
            cin >> diskon;
            cin.ignore();

            tambahBarangKeLinkedList(
                p->head_barang,
                b+1,
                nb,
                harga,
                diskon
            );
        }

        id_pembeli++;
    }
}

            else if (pil == 5) tampilkanSemuaPembeli(head_pembeli);
            else if (pil == 7 && isAdmin) {
    cout << "Diskon %: ";
    cin >> diskon_total_persen;
    cin.ignore();

    hitungDiskonTotal(head_pembeli, diskon_total_persen);
    tampilkanHasilSplitBill(head_pembeli);
}

            else if (pil == 20) {
                cout << "1.Tambah Voucher 2.Lihat: "; int v; cin>>v; cin.ignore();
                char dashV[]="-";
                if(v==1) { char k[20]; double n; cout<<"Kode: "; cin>>k; cout<<"Nominal: "; cin>>n; cin.ignore(); tambahVoucher(head_voucher, tail_voucher, 1, k, n, dashV); }
                else tampilkanVoucherMaju(head_voucher);
            }
            else if (pil == 30 && isAdmin) {
                printQueue(cancel_queue);
                dequeueBatal(cancel_queue);
            }
            else if (pil == 31 && isAdmin) {
                printStackTugas(task_stack);
                cout << "Proses tugas teratas? (y/n): "; char y; cin>>y; cin.ignore();
                if(y=='y') popStackTugas(task_stack);
            }
            else if (pil == 40 && !isAdmin) {
                char reason[100]; char type[]="Hapus Pembeli";
                int target; cout << "ID Target: "; cin >> target; cin.ignore();
                cout << "Alasan: "; cin.getline(reason, 100);
                enqueueBatal(cancel_queue, currentUser->username, type, target, reason);
            }
            else if (pil == 50) { 
                cout << "Masukkan ID Pembeli: "; int searchId; cin >> searchId; cin.ignore();
                AVLNode* res = searchAVL(avl_root, searchId);
                if(res) cout << "DITEMUKAN: " << res->nama << "\n";
                else cout << "TIDAK DITEMUKAN.\n";
            }
            else if (pil == 51) { 
                char txt[100]; cout << "Masukkan teks: "; cin.getline(txt, 100);
                demoHuffman(txt);
            }
            else if (pil == 21) { logout = true; hapusSemuaKTP(head_ktp, tail_ktp); }
            else exit(0);
        }
    }
    return 0;
}