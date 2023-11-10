#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>
#include <iomanip>
using namespace std;

// Struktur data untuk item menu
struct MenuItem
{
    int ID;
    string nama;
    double harga;
    int stok;
    MenuItem *next;
};

// Struktur data untuk pesanan
struct OrderItem
{
    string nama;
    int jumlah;
    int ID_order;
    double total_harga;
    double harga_item;
    string username;
    string verif;
    OrderItem *next;
};

// Struktur data untuk pengguna
struct User
{
    string nama;
    string username;
    string password;
    User *next;
};

// Struktur data untuk admin
struct Admin
{
    string username;
    string password;
};

struct OrderQueue {
    OrderItem* front;
    OrderItem* rear;
};

MenuItem *head;
OrderItem *pesanan; // Ini adalah deklarasi pointer ke tipe data MenuItem
int itemMenu = 0;
int ID_Increment_pesan = 0;
int ID_Increment_barang = 0;

// Fungsi untuk membuat jeda dalam program
void Delay(int milliseconds)
{
    // menunda eksekusi program selama milliseconds milidetik
    Sleep(milliseconds);
}

// fungsi untuk menampilkan pesan memuat pesanan dengan indikator persentase
void memuat()
{
    system("cls"); // menghapus layar konsol sebelum menampilkan pesan

    // loop untuk menampilkan pesan dan menggambarkan indikator persentase dengan jeda
    for (int i = 1; i <= 100; i++)
    {
        cout << "\rMemuat Pesanan[" << i << "%]";
        Delay(20);
    }
    cout << endl;
}

// fungsi untuk menampilkan pesan memuat item menu dengan efek warna latar belakang
void loading()
{
    system("cls");
    for (int i = 1; i <= 100; i++)
    {
        system("color 04"); // mengatur latar belakang konsol menjadi merah
        cout << "\rMemuat Item Menu[" << i << "%]";
        Delay(10);
        // loop untuk menampilkan pesan dan menggambarkan indikator persentase dengan jeda
    }
    cout << endl;
}

// Fungsi untuk pendaftaran pengguna
User *daftarUser(User *userList)
{
    string nama, username, password;
    system("cls");
    // Menampilkan header registrasi user
    cout << "--------------------------------------------------------" << endl;
    cout << "||                    REGISTRASI USER                 ||" << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << "   Nama lengkap : ";
    fflush(stdin);
    getline(cin, nama); // menerima input nama lengkap menggunakan getline untuk mengatasi spasi
    cout << "   Username     : ";
    cin >> username;
    cout << "   Password     : ";
    cin >> password;
    // Periksa apakah akun sudah terdaftar
    User *current = userList;
    while (current)
    {
        if (current->nama == nama && current->username == username && current->password == password)
        {
            cout << "Akun sudah terdaftar." << endl;
            system("pause");
            return userList; // Tidak perlu registrasi ulang
        }
        current = current->next;
    }
    // Membuat dan menginisialisasi node pengguna baru
    User *newUser = new User;
    newUser->nama = nama;
    newUser->username = username;
    newUser->password = password;
    newUser->next = userList; // Menyambungkan node baru ke daftar pengguna
    userList = newUser;       // Memperbarui daftar pengguna dengan node baru
    // Simpan data ke dalam file CSV
    ofstream file("akun.csv"); // Mode "append" untuk menambahkan data ke akhir file
    if (file)
    {
        file << nama << "," << username << "," << password << "\n"; // Menulis data ke file CSV
        file.close();
    }
    cout << "Akun berhasil terdaftar." << endl;
    system("pause");
    return userList;
}

// Fungsi untuk membaca data user dari file CSV
User *bacaAkunDariCSV()
{
    User *userList = nullptr;  // Inisialisasi daftar pengguna dengan nullptr
    ifstream file("akun.csv"); // Membuka file "akun.csv" untuk membaca data pengguna
    if (file)
    {
        string nama, username, password;

        // Loop membaca baris-baris dari file CSV dan mengisi daftar pengguna
        while (getline(file, nama, ',') &&
               getline(file, username, ',') &&
               getline(file, password))
        {
            User *newUser = new User;

            // Mengisi informasi pengguna dari data yang dibaca
            newUser->nama = nama;
            newUser->username = username;
            newUser->password = password;

            newUser->next = userList; // Menyambungkan node baru ke daftar pengguna yang ada
            userList = newUser;       // Memperbarui daftar pengguna dengan node baru
        }
        file.close();
    }
    return userList; // Mengembalikan daftar pengguna yang telah dibaca dari file CSV
}

//
void simpanAkunKeCSV(User *userList)
{
    // Membuka file "akun.csv" untuk menulis data pengguna
    ofstream file("akun.csv");
    User *current = userList;

    // Melakukan loop melalui daftar pengguna dan menulis informasi pengguna ke dalam file CSV
    while (current)
    {
        file << current->nama << "," << current->username << "," << current->password << "\n";
        current = current->next; // Beralih ke pengguna berikutnya dalam daftar
    }
    file.close(); // Menutup file setelah menulis data
}

// Fungsi untuk login pengguna
User *loginUser(User *userList)
{
    int percobaan = 1;
    while (percobaan <= 3)
    { // Maksimal 3 percobaan login
        string username, password;
        system("cls");
        cout << "--------------------------------------------------------" << endl;
        cout << "||                      LOGIN USER                    ||" << endl;
        cout << "--------------------------------------------------------" << endl;
        cout << "           Masukkan username: ";
        cin >> username;
        cout << "           Masukkan password: ";
        cin >> password;
        User *current = userList;

        // Loop melalui daftar pengguna dan mencocokkan username dan password yang dimasukkan
        while (current)
        {
            if (current->username == username && current->password == password)
            {
                return current; // Username dan password cocok, maka login berhasil, mengembalikan pointer ke pengguna yang berhasil login
            }
            current = current->next;
        }
        cout << "Username atau password salah." << endl;
        system("pause"); // Menunggu tindakan pengguna sebelum mencoba lagi
        percobaan++;
    }
    cout << "Percobaan melebihi 3 kali" << endl;
    system("pause");
    return nullptr; // Jika login gagal sebanyak 3 kali, mengembalikan nullptr
}

bool loginAdmin()
{
    const string adminUsername = "admin"; // Username admin yang diinginkan
    const string adminPassword = "admin"; // Password admin yang diinginkan
    string username, password;
    system("cls");
    for (int i = 1; i <= 3; i++)
    {
        cout << "--------------------------------------------------------" << endl;
        cout << "||                      LOGIN ADMIN                    ||" << endl;
        cout << "--------------------------------------------------------" << endl;
        cout << "   Masukkan username: ";
        cin >> username;
        cout << "   Masukkan password: ";
        cin >> password;
        return (username == adminUsername && password == adminPassword);
        for (int i = 0; i < 1; i++)
        {
            if (username == adminUsername && password == adminPassword)
            {
                cout << "Selamat datang " << adminUsername << "!\n\n";
                system("pause");
                return true; // Username dan password admin cocok, login berhasil, dan mengembalikan true
            }
        }
        cout << "Login gagal. Anda bukan admin." << endl;
        system("pause");
    }
    cout << "Anda telah mencapai batas maksimal 3x login! Silahkan masukkan username dan password anda yang benar!\n\n";
    return false;
}

void tambahItem(string nama, int harga, int stok, int ID)
{
    MenuItem *newItem = new MenuItem; // Membuat node baru untuk item menu
    newItem->nama = nama;
    newItem->harga = harga;
    newItem->stok = stok;
    newItem->ID = ID;
    newItem->next = nullptr; // Menjadikan newItem sebagai data terakhir

    if (head == nullptr)
    {
        // Jika menu masih kosong, maka newItem menjadi data pertama
        head = newItem;
    }
    else
    {
        // Jika menu sudah berisi data, cari data terakhir dan tambahkan newItem di bawahnya
        MenuItem *current = head;
        while (current->next != nullptr)
        {
            current = current->next;
        }
        current->next = newItem;
    }
}

void saveBarangToCSV(const string &barang, MenuItem *head)
{
    ofstream file("barang.csv");
    MenuItem *current = head;

    while (current)
    {
        // Membuat baris CSV dengan data barang
        stringstream csvLine;
        csvLine << current->nama << "," << current->harga << "," << current->stok << "," << current->ID << "\n";

        // Menulis baris ke file CSV
        file << csvLine.str();
        current = current->next;
    }
    file.close();
}

void bacaBarangdariCSV(const string &barang, MenuItem *&head)
{
    ifstream file("barang.csv");
    if (!file.is_open())
    {
        cerr << "Gagal membaca file CSV." << endl;
        system("pause");
        return;
    }
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string nama;
        double harga;
        int stok;
        int ID;
        if (getline(ss, nama, ',') && ss >> harga && ss.ignore() && ss >> stok && ss.ignore() && ss >> ID)
        {
            // Menambahkan data yang dibaca ke linked list tanpa menghapus yang ada
            ID_Increment_barang = ID;
            tambahItem(nama, harga, stok, ID_Increment_barang);
        }
    }
    file.close(); // Menutup file setelah selesai membaca data barang
}

void tampilkanMenu()
{
    loading();
    system("cls");
    int i = 1;
    MenuItem *current = head;
    cout << "-----------------------------------------------------------------------" << endl;
    cout << "||                                                                   ||" << endl;
    cout << "||                           DAFTAR ITEM MENU                        ||" << endl;
    cout << "||                                                                   ||" << endl;
    cout << "-----------------------------------------------------------------------" << endl;
    if (current == NULL)
    {
        cout << "\n        ITEM MENU TIDAK ADA           " << endl;
        return;
    }
    else
    {
        cout << left << "||" << setw(1) << " " << setw(2) << "No"
             << " | " << setw(25) << "Nama Item"
             << " | " << setw(20) << "Harga"
             << " | " << setw(10) << "Stok"
             << "||" << endl;
        cout << "-----------------------------------------------------------------------" << endl;
        while (current != NULL)
        {
            cout << left << "||" << setw(1) << " " << setw(2) << i << " | " << setw(25) << current->nama << " | " << setw(20) << current->harga << " | " << setw(10) << current->stok << "||" << endl;
            current = current->next;
            i++;
        }
    }

    cout << "-----------------------------------------------------------------------" << endl;
    system("pause");
}

void hapusItem(const string &nama)
{
    MenuItem *current = head;
    MenuItem *prev = nullptr;
    while (current)
    {
        if (current->nama == nama)
        {
            // Jika elemen yang akan dihapus adalah elemen pertama dalam daftar
            if (prev == nullptr)
            {
                head = current->next;
            }
            // Jika elemen yang akan dihapus adalah elemen selain elemen pertama
            else
            {
                prev->next = current->next;
            }
            // Hapus elemen yang sudah tidak diperlukan
            delete current;
            system("cls");
            cout << "------------------------------------------------------" << endl;
            cout << "||           Sukses: Data berhasil dihapus          ||" << endl;
            cout << "------------------------------------------------------" << endl;
            system("pause");
            return; // Keluar dari metode setelah penghapusan berhasil
        }
        prev = current;
        current = current->next;
    }
    // Jika elemen dengan nama yang sesuai tidak ditemukan
    cout << "Item dengan nama '" << nama << "' tidak ditemukan." << endl;
}

void updateStockInCSV(const string &namaItem, int jumlah)
{
    // Baca data dari file CSV
    ifstream file("barang.csv");
    if (!file.is_open())
    {
        cout << "Gagal membuka file barang.csv" << endl;
        return;
    }

    ofstream tempFile("temp.csv");
    if (!tempFile.is_open())
    {
        cout << "Gagal membuka file sementara temp.csv" << endl;
        file.close();
        return;
    }

    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        string nama;
        double harga;
        int stok;
        if (iss >> nama >> harga >> stok)
        {
            if (nama == namaItem)
            {
                stok -= jumlah;
            }
            tempFile << nama << ' ' << harga << ' ' << stok << endl;
        }
    }

    file.close();
    tempFile.close();

    // Hapus file asli dan ganti dengan file sementara
    if (remove("barang.csv") != 0)
    {
        cout << "Gagal menghapus file barang.csv" << endl;
        return;
    }
    if (rename("temp.csv", "barang.csv") != 0)
    {
        cout << "Gagal mengganti nama file sementara" << endl;
    }
}

void savePesananToCSV(OrderItem* currentOrderItem) {
    ofstream file("pesan.csv");
    while (currentOrderItem != NULL) {
        // Membuat baris CSV dengan data pesanan
        stringstream csvLine;
        csvLine << currentOrderItem->username << ",";
        csvLine << currentOrderItem->nama << ",";
        csvLine << currentOrderItem->jumlah << ",";
        csvLine << currentOrderItem->harga_item << ",";
        csvLine << currentOrderItem->total_harga << ",";
        csvLine << currentOrderItem->ID_order << ",";
        csvLine << currentOrderItem->verif << "\n";

        // Menulis baris ke file CSV
        file << csvLine.str();
        currentOrderItem = currentOrderItem->next;
    }
    file.close();
}

void bacaPesananFromCSV(OrderItem *&pesanan)
{
    ifstream file("pesan.csv");
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            istringstream iss(line);
            string namaPengguna, namaItem, verif;
            int jumlah, ID;
            double hargaPerItem, totalHarga;
            if (getline(iss, namaPengguna, ',') && getline(iss, namaItem, ',') && (iss >> jumlah) &&
                (iss.ignore()) && (iss >> hargaPerItem) && (iss.ignore()) && (iss >> totalHarga) && (iss.ignore()) && (iss >> ID) && (iss.ignore()) && (iss >> verif))
            {
                
            
                OrderItem *newOrderItem = new OrderItem;
                newOrderItem->nama = namaItem;
                newOrderItem->jumlah = jumlah;
                newOrderItem->ID_order = ID;
                newOrderItem->username = namaPengguna;
                newOrderItem->total_harga = totalHarga;
                newOrderItem->harga_item = hargaPerItem;
                newOrderItem->verif = verif;
                newOrderItem->next = NULL;
                if (pesanan == NULL)
                {
                    // Jika menu masih kosong, maka newItem menjadi data pertama
                    pesanan = newOrderItem;
                }
                else
                {
                    OrderItem *TEMP = pesanan;
                    // Jika menu sudah berisi data, cari data terakhir dan tambahkan newItem di bawahnya
                    while (TEMP->next != NULL)
                    {
                        TEMP = TEMP->next;
                    }
                    TEMP->next = newOrderItem;
                }
                
            }
        }
        file.close();
    }
    else
    {
        system("cls");
        cout << "----------------------------------------------------------------" << endl;
        cout << "||            Gagal: File pesan.csv tidak terbuka             ||" << endl;
        cout << "----------------------------------------------------------------" << endl;
    }
}

void pesanItem(OrderItem *currentOrderItem, string pengguna)
{
    int ID_pesan;
    int jumlah;
    system("cls");
    cout << "------------------------------------------------------" << endl;
    cout << "||                PEMESANAN ITEM                   ||" << endl;
    cout << "------------------------------------------------------" << endl;
    // Tampilkan menu sebelum memesan
    tampilkanMenu();
    cout << "\nMasukkan ID item yang ingin dipesan   : ";
    cin.ignore();
    while (!(cin >> ID_pesan))
    {
        cout << "\nMasukkan ID item yang ingin dipesan   : ";
        cin.clear();
        cin.ignore();
    }
    cout << "Masukkan jumlah item yang ingin dipesan : ";
    while (!(cin >> jumlah))
    {
        cout << "\nMasukkan jumlah item yang ingin dipesan : ";
        cin.clear();
        cin.ignore();
    }
    // Cari item di menu
    MenuItem *current = head;
    while (current != NULL)
    {
        if (current->ID == ID_pesan)
        {
            if (current->stok >= jumlah)
            {
                // Tambahkan item ke pesanan pengguna
                OrderItem *newOrderItem = new OrderItem;
                newOrderItem->ID_order = ID_Increment_pesan++;
                newOrderItem->nama = current->nama;
                newOrderItem->jumlah = jumlah;
                newOrderItem->total_harga = jumlah * current->harga;
                newOrderItem->harga_item = current->harga;
                newOrderItem->username = pengguna; // Ambil nama pengguna
                newOrderItem->verif = "belum bayar";
                newOrderItem->next = NULL;
                if (currentOrderItem == NULL)
                {
                    // Jika menu masih kosong, maka newItem menjadi data pertama
                    currentOrderItem = newOrderItem;
                }
                else
                {
                    OrderItem *TEMP = currentOrderItem;
                    // Jika menu sudah berisi data, cari data terakhir dan tambahkan newItem di bawahnya
                    while (TEMP->next != NULL)
                    {
                        TEMP = TEMP->next;
                    }
                    TEMP->next = newOrderItem;
                }
                // Perbarui stok item
                current->stok -= jumlah;
                system("cls");
                cout << "------------------------------------------------------" << endl;
                cout << "||              Sukses: Berhasil Memesan            ||" << endl;
                cout << "------------------------------------------------------" << endl;
                // Simpan pesanan ke file CSV
                savePesananToCSV(currentOrderItem);
                // Simpan juga pembaruan stok barang ke file CSV
                saveBarangToCSV("barang.csv", head);
            }
            else
            {
                cout << "------------------------------------------------------" << endl;
                cout << "||                Gagal: Stok tidak ada             ||" << endl;
                cout << "------------------------------------------------------" << endl;
            }
            system("pause");
            return;
        }
        current = current->next;
    }

    system("pause");
}

void tampilkanPesanan(OrderItem *pesanan, string pengguna)
{
    memuat();
    system("cls");
    int i = 1;
    OrderItem *current = pesanan;
    cout << "-------------------------------------------------------------------------------------" << endl;
    cout << "||                              DAFTAR PESANAN ANDA                                ||" << endl;
    cout << "-------------------------------------------------------------------------------------" << endl;
    if (current == nullptr)
    {
        cout << "\n------------------------------------------------------" << endl;
        cout << "||                Gagal: Pesanan kosong             ||" << endl;
        cout << "------------------------------------------------------" << endl;
    }
    else
    {
        // Tampilkan header tabel
        cout << left << "||" << setw(1) << " " << setw(2) << "No"
             << " | " << setw(20) << "Nama Item"
             << " | " << setw(15) << "Jumlah"
             << " | " << setw(15) << "Harga"
             << " | " << setw(15) << "Total Harga"
             << " | " << setw(15) << "Status"
             << " ||" << endl;
        cout << "-------------------------------------------------------------------------------------" << endl;
       while (current)
        {
            // Tambahkan kondisi untuk hanya menampilkan pesanan yang dimiliki oleh pengguna yang sedang login
            if (current->username == pengguna)
            {
                // Temukan harga per item dari daftar menu (disarankan menyimpannya dalam struktur OrderItem)
                MenuItem *currentItem = head;

                while (currentItem)
                {
                    if (currentItem->nama == current->nama)
                    {
                        // Tampilkan detail pesanan dalam bentuk tabel
                        cout << left << "||" << setw(1) << " " << setw(2) << i << " | " << setw(20) << current->nama << " | " << setw(15) << current->jumlah << " | " << setw(15) << current->harga_item << " | " << setw(15) << current->total_harga << "|" << setw(15) << current->verif << " ||" << endl;
                        i++;
                    }
                    currentItem = currentItem->next;
                }
            }
            current = current->next;
        }
    }

    cout << "-------------------------------------------------------------------------------------" << endl;
    system("pause");
}

void hapusPesanan(OrderItem *currentOrderItem, const int &ID_orderItem, string pengguna)
{
    OrderItem *TEMP = currentOrderItem;
    while (currentOrderItem != NULL)
    {
        if (currentOrderItem->ID_order == ID_orderItem)
        {
            string namaItem = currentOrderItem->nama;
            int ID_temp = currentOrderItem->ID_order;
            if (currentOrderItem->next == NULL)
            {
                TEMP->next = NULL;
            }
            else
            {
                TEMP->next = currentOrderItem->next;
            }
            // Hapus item dan bebaskan memori
            delete currentOrderItem;

            cout << "Pesanan '" << namaItem << "'dengan ID'" << ID_temp << "berhasil dihapus dari keranjang." << endl;
            system("pause");

            // Simpan pesanan ke file CSV setelah penghapusan
            savePesananToCSV(currentOrderItem);
            bacaPesananFromCSV(currentOrderItem);
            return;
        }
        TEMP = currentOrderItem;
        currentOrderItem = currentOrderItem->next;
    }
}


void melakukanPembayaran(OrderItem *currentOrderItem, string pengguna)
{
    double totalHarga = 0.0;
    system("cls");
    cout << "-----------------------------------------------------------------" << endl;
    cout << "||                     PEMBAYARAN PESANAN                      ||" << endl;
    cout << "-----------------------------------------------------------------" << endl;

    while ((currentOrderItem != NULL) && currentOrderItem->username == pengguna)
    {
        string namaItem = currentOrderItem->nama;
        MenuItem *currentMenuItem = head;
        while (currentMenuItem)
        {
            if (currentMenuItem->nama == namaItem)
            {
                totalHarga += currentMenuItem->harga * currentOrderItem->jumlah;
                break;
            }
            currentMenuItem = currentMenuItem->next;
        }
        currentOrderItem = currentOrderItem->next;
    }

    if (totalHarga > 0)
    {
        cout << " " << endl;
        cout << "                   Total Harga Pesanan: Rp. " << totalHarga << endl;
        cout << " " << endl;
        cout << "-----------------------------------------------------------------" << endl;

        // Tambahkan kondisi untuk membayar sekarang atau nanti
        cout << "Apakah Anda ingin membayar sekarang? (y/n): ";
        char bayarSekarang;
        cin >> bayarSekarang;

        if (bayarSekarang == 'y' || bayarSekarang == 'Y')
        {
            // Tampilkan struk pembayaran
            cout << "===== Struk Pembayaran =====" << endl;
            cout << "|| Total Harga: Rp." << totalHarga << "||" << endl;
            cout << "============================" << endl;

            cout << "Terima kasih atas pembayarannya!" << endl;
            currentOrderItem = nullptr;
        }
        else
        {
            cout << "Silakan membayar nanti. Terima kasih atas pesanannya!" << endl;
        }

        system("pause");
    }
    else
    {
        cout << "Pesanan Anda kosong. Tidak ada yang perlu dibayar." << endl;
        system("pause");
    }
}

// void konfirmasiPesananUser(User* pengguna) {
//     // Implementasi fungsi untuk mengkonfirmasi pesanan pengguna
//     tampilkanPesanan(pengguna->pesanan);

//     double totalHarga = 0.0;  // Menyimpan total harga pesanan

//     // Lakukan logika konfirmasi pesanan di sini, seperti mengurangi stok item yang dipesan.
//     OrderItem* pesanan = pengguna->pesanan;
//     while (pesanan) {
//         // Cari item di dalam daftar barang
//         MenuItem* item = head;
//         while (item) {
//             if (item->nama == pesanan->nama) {
//                 // Periksa apakah stok cukup
//                 if (item->stok >= pesanan->jumlah) {
//                     // Mengurangi stok
//                     item->stok -= pesanan->jumlah;
//                     // Menghitung total harga
//                     totalHarga += (item->harga * pesanan->jumlah);
//                 } else {
//                     cout << "Stok " << item->nama << " tidak mencukupi." << endl;
//                 }
//                 break; // Item ditemukan, keluar dari loop
//             }
//             item = item->next;
//         }
//         pesanan = pesanan->next;
//     }
//     cout << "Pesanan pengguna telah dikonfirmasi. Total Harga: Rp" << totalHarga << endl;
//     // Hapus pesanan pengguna setelah dikonfirmasi (jika diperlukan)
//     // Implementasi ini tergantung pada aturan bisnis Anda.
//     // Misalnya, jika pesanan harus tetap ada setelah dikonfirmasi, Anda dapat menghapus baris berikut:
//     pengguna->pesanan = nullptr;
//     system("pause");
// }

const char *FILENAME = "pesan.csv";

// Fungsi untuk menampilkan pesanan pertama dalam antrian
void displayFirstOrder(OrderQueue& q) {
    if (q.front != nullptr) {
        OrderItem* firstOrder = q.front;
        cout << "Pesanan Pertama Masuk:" << endl;
        cout << "Nama: " << firstOrder->nama << endl;
        cout << "Jumlah: " << firstOrder->jumlah << endl;
        cout << "ID Order: " << firstOrder->ID_order << endl;
        cout << "Total Harga: " << firstOrder->total_harga << endl;
        cout << "Harga Item: " << firstOrder->harga_item << endl;
        cout << "Username: " << firstOrder->username << endl;
        cout << "Verifikasi: " << firstOrder->verif << endl;

        delete firstOrder; // Membebaskan memori
        q.front = q.front->next;

        if (q.front == nullptr) {
            q.rear = nullptr;
        }
    } else {
        cout << "Tidak ada pesanan dalam antrian." << endl;
    }

    system("pause");
}

void lihatSemuaPesananAdmin() {
    ifstream file("pesan.csv");
    if (!file.is_open()) {
        cout << "Gagal membuka file pesan.csv" << endl;
        return;
    }

    OrderQueue orderQueue{nullptr, nullptr};

    string line;
    while (getline(file, line)) {
        OrderItem* newOrder = new OrderItem;
        istringstream iss(line);
        iss >> newOrder->nama >> newOrder->jumlah >> newOrder->ID_order >> newOrder->total_harga
            >> newOrder->harga_item >> newOrder->username >> newOrder->verif;

        newOrder->next = nullptr;

        if (orderQueue.front == nullptr) {
            orderQueue.front = newOrder;
            orderQueue.rear = newOrder;
        } else {
            orderQueue.rear->next = newOrder;
            orderQueue.rear = newOrder;
        }
    }

    file.close();

    // Menampilkan pesanan pertama
    displayFirstOrder(orderQueue);
}



void lihatDaftarUser(User *userList)
{
    system("cls");
    cout << "-----------------------------------------------------------" << endl;
    cout << "||                        DATA USER                      ||" << endl;
    cout << "-----------------------------------------------------------" << endl;

    User *current = userList;

    if (current == nullptr)
    {
        cout << "DATA USER KOSONG " << endl;
    }
    else
    {
        int index = 1;
        cout << left << "||" << setw(1) << " " << setw(2) << "NO"
             << " | " << setw(25) << "NAMA"
             << " | " << setw(20) << "USERNAME"
             << " || " << endl;
        cout << "-----------------------------------------------------------" << endl;
        while (current)
        {
            cout << left << "||" << setw(1) << " " << setw(2) << index << " | " << setw(25) << current->nama << " | " << setw(20) << current->username << " || " << endl;
            current = current->next;
            index++;
        }
    }

    cout << "-----------------------------------------------------------" << endl;
    system("pause");
}

void menuUser(string pengguna, OrderItem *pesanan)
{
    string namaItem;
    int ID_order;
    bacaPesananFromCSV(pesanan);
    User *currentUser;
    int pilihanUser;
    while (pilihanUser != 6)
    {
        system("cls");
        cout << "------------------------------------------------------" << endl;
        cout << "||                      MENU USER                   ||" << endl;
        cout << "------------------------------------------------------" << endl;
        cout << "|| [1] Lihat Menu                                   ||" << endl;
        cout << "|| [2] Pesan                                        ||" << endl;
        cout << "|| [3] Tampilkan pesanan                            ||" << endl;
        cout << "|| [4] Hapus pesanan                                ||" << endl;
        cout << "|| [5] Melakukan pembayaran                         ||" << endl;
        cout << "|| [6] Kembali ke Menu Utama                        ||" << endl;
        cout << "------------------------------------------------------" << endl;
        cout << " >> ";
        cin >> pilihanUser;
        switch (pilihanUser)
        {
        case 1:
            tampilkanMenu();
            break;
        case 2:
            pesanItem(pesanan, pengguna);
            break;
        case 3:
            tampilkanPesanan(pesanan, pengguna);
            break;
        case 4:
            tampilkanPesanan(pesanan, pengguna);
            // Implementasi menu untuk menghapus pesanan
            cout << "Masukkan no pesanan yang ingin dihapus: ";
            cin.ignore();
            getline(cin, namaItem);
            hapusPesanan(pesanan, ID_order, pengguna);
            break;
        case 5:
            tampilkanPesanan(pesanan, pengguna);
            melakukanPembayaran(pesanan, pengguna);
            break;
        case 6:
            // Implementasikan menu untuk melakukan pembayaran
            // ...
            break;
        default:
            cout << "Pilihan tidak valid.\n";
            system("pause");
            break;
        }
    }
}

void menuAdmin()
{
    User *userHead = bacaAkunDariCSV();
    User *pengguna = NULL; // Deklarasi variabel pengguna
    string namaItem;
    User *currentUser;
    int pilihanAdmin;
    while (pilihanAdmin != 7)
    {
        system("cls");
        cout << "------------------------------------------------------" << endl;
        cout << "||                     MENU ADMIN                   ||" << endl;
        cout << "------------------------------------------------------" << endl;
        cout << "|| [1] Lihat stok menu                              ||" << endl;
        cout << "|| [2] Tambah item menu                             ||" << endl;
        cout << "|| [3] Hapus item menu                              ||" << endl;
        cout << "|| [4] Lihat pesanan user                           ||" << endl;
        cout << "|| [5] Konfirmasi pesanan user                      ||" << endl;
        cout << "|| [6] Lihat daftar user                            ||" << endl;
        cout << "|| [7] Kembali ke menu utama                        ||" << endl;
        cout << "------------------------------------------------------" << endl;
        cout << "Pilih opsi: ";
        cin >> pilihanAdmin;
        switch (pilihanAdmin)
        {
        case 1:
            // Implementasikan menu untuk menampilkan item menu
            tampilkanMenu();
            break;
        case 2:
        {

            string nama;
            double harga;
            int stok;

            system("cls");
            cout << "\nMasukkan nama item yang akan ditambahkan : ";
            cin.ignore();
            getline(cin, nama);
            cout << "Masukkan harga item                      : ";
            cin >> harga;
            cout << "Masukkan stok item                       : ";
            cin >> stok;
            ID_Increment_barang += 1;
            tambahItem(nama, harga, stok, ID_Increment_barang);

            system("cls");
            cout << "------------------------------------------------------" << endl;
            cout << "||        Sukses: Data berhasil ditambahkan         ||" << endl;
            cout << "------------------------------------------------------" << endl;
            system("pause");
        }
        break;
        case 3:
        {
            tampilkanMenu();
            string hapus;
            cout << "\nMasukkan nama item yang akan dihapus : ";
            cin.ignore();
            getline(cin, hapus);
            hapusItem(hapus);
        }
        break;
        case 4:
            bacaPesananFromCSV(pesanan);
            lihatSemuaPesananAdmin();
            break;
        case 5:
            // bacaPesananFromCSV(pengguna->pesanan, pengguna);
            // konfirmasiPesananUser(pengguna);
            break;
        case 6:
            lihatDaftarUser(userHead);
            break;
        case 7:
            // Kembali ke menu utama
            break;
        default:
            cout << "Pilihan tidak valid." << endl;
            system("pause");
            break;
        }
        saveBarangToCSV("barang.csv", head);
    }
}

int main()
{
    bacaBarangdariCSV("barang.csv", head);
    User *userHead = bacaAkunDariCSV();
    Admin admin = {"admin", "admin"};
    int peran;
    User *currentUser = nullptr;
    bool isLoggedIn = false;
    bool menuDibaca = false;

    do
    {
        system("cls");
        system("color 01");
        cout << "--------------------------------------------------------" << endl;
        cout << "||                     KOPIRIA SHOP                   ||" << endl;
        cout << "--------------------------------------------------------" << endl;
        cout << "|| [1] USER Regis                                     ||" << endl;
        cout << "|| [2] USER Login                                     ||" << endl;
        cout << "|| [3] ADMIN (kasir)                                  ||" << endl;
        cout << "|| [4] KELUAR                                         ||" << endl;
        cout << "--------------------------------------------------------" << endl;
        cout << " >> ";
        cin >> peran;
        switch (peran)
        {
        case 1: // jika memilih 1 maka akan dilakukan user regis
            if (!isLoggedIn)
            {
                userHead = daftarUser(userHead);
                simpanAkunKeCSV(userHead);
            }
            break;
        case 2: // jika memilih 2 maka akan dilakukan user login
            currentUser = loginUser(userHead);
            if (currentUser == nullptr)
            {
                cout << "\nPercobaan melebihi 3 kali" << endl;
                system("pause");
            }
            else
            {
                string pengguna = currentUser->username;
                menuUser(pengguna, pesanan);
            }
            break;
        case 3: // jika memilih 3 maka akan dilakukan login admin
            if (loginAdmin())
            {
                menuAdmin();
            }
            break;
        case 4: // jika memilih 4 maka akan keluar dari
            break;

        default: // jika salah memasukkan inputan maka menu akan di tampilkan kembali
            cout << "Pilihan tidak valid." << endl;
            system("pause");
        }
        // jika memilih lebih dari angka yang ada maka kodingan akan berhenti
    } while (peran != 4);
    return 0;
}