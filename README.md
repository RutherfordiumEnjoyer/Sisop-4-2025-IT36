# Sisop-4-2025-IT36

Anggota Kelompok
|     NRP    |                Nama              |
|-----       | -----                            |
|5027241106  | Mohammad Abyan Ranuaji           |
|5027241078  | Naila Raniyah Hanan              |

- [Peraturan](#peraturan)
- [Soal](#soal)
- [Detail Tambahan](#detail-tambahan)
- [Penjelasan](#penjelasan)
  - [Soal 1](#soal-1)
  - [Soal 2](#soal-2)
  - [Soal 3](#soal-3)
  - [Soal 4](#soal-4)

---
## Peraturan
---

1. Waktu pengerjaan dimulai hari Senin (12 Mei 2025) setelah sesi lab hingga hari Sabtu (17 Mei 2025) pukul 23.59 WIB.
2. Praktikan diharapkan membuat laporan penjelasan dan penyelesaian soal dalam bentuk Readme (github).
3. Format nama repository github “Sisop-[Nomor Modul]-2025-[Nama Kelompok]” (contoh:Sisop-4-2025-IT01).
4. Struktur repository seperti berikut:
   - **soal_1:**
     - `hexed.c`
   - **soal_2:**
     - `baymax.c`
   - **soal_3:**
     - `Dockerfile`
     - `docker-compose.yml`
     - `antink.c`
   - **soal_4:**
     - `maimai_fs.c`
    
   - **assets**

Jika melanggar struktur repo akan dianggap sama dengan curang dan menerima konsekuensi sama dengan melakukan kecurangan. Folder assets bersifat opsional dan hanya diisi dengan dokumentasi pengerjaan yang akan diletakkan di readme. Tidak ada format khusus untuk penamaan file dalam assets.

5. Setelah pengerjaan selesai, semua script bash, awk, dan file yang berisi cron job ditaruh di github masing - masing kelompok, dan link github diletakkan pada form yang disediakan. Pastikan github di setting ke publik.
6. Commit terakhir maksimal 10 menit setelah waktu pengerjaan berakhir. Jika melewati maka akan dinilai berdasarkan commit terakhir.
7. Jika tidak ada pengumuman perubahan soal oleh asisten, maka soal dianggap dapat diselesaikan.
8. Jika ditemukan soal yang tidak dapat diselesaikan, harap menuliskannya pada Readme beserta permasalahan yang ditemukan.
9. Praktikan tidak diperbolehkan menanyakan jawaban dari soal yang diberikan kepada asisten maupun praktikan dari kelompok lainnya.
10. Jika ditemukan indikasi kecurangan dalam bentuk apapun di pengerjaan soal shift, maka nilai dianggap 0.
11. Pengerjaan soal shift sesuai dengan modul yang telah diajarkan.
12. Zip dari repository dikumpulkan di google form yang terdapat pada linktree.
13. Jika terdapat revisi soal akan dituliskan pada halaman terakhir.

---
##Soal
---
1. The Shorekeeper adalah sebuah entitas misterius yang memimpin dan menjaga Black Shores secara keseluruhan. Karena Shorekeeper hanya berada di Black Shores, ia biasanya berjalan - jalan di sekitar Black Shores untuk mencari anomali - anomali yang ada untuk mencegah adanya kekacauan ataupun krisis di Black Shores. Semenjak kemunculan Fallacy of No Return, ia semakin ketat dalam melakukan pencarian anomali - anomali yang ada di Black Shores untuk mencegah hal yang sama terjadi lagi.

Suatu hari, saat di Tethys' Deep, Shorekeeper menemukan sebuah anomali yang baru diketahui. Anomali ini berupa sebuah teks acak yang kelihatannya tidak memiliki arti. Namun, ia mempunyai ide untuk mencari arti dari teks acak tersebut.

   a. Pertama, Shorekeeper akan mengambil beberapa sampel anomali teks dari link berikut (https://drive.google.com/file/d/1hi_GDdP51Kn2JJMw02WmCOxuc3qrXzh5/view?usp=sharing). Pastikan file zip terhapus setelah proses unzip.

   b. Setelah melihat teks - teks yang didapatkan, ia menyadari bahwa format teks tersebut adalah hexadecimal. Dengan informasi tersebut, Shorekeeper mencoba untuk mencoba idenya untuk mencari makna dari teks - teks acak tersebut, yaitu dengan mengubahnya dari string hexadecimal menjadi sebuah file image. Bantulah Shorekeeper dengan membuat kode untuk FUSE yang dapat mengubah string hexadecimal menjadi sebuah gambar ketika file text tersebut dibuka di mount directory. Lalu, letakkan hasil gambar yang didapat ke dalam directory bernama “image”.

   c. Untuk penamaan file hasil konversi dari string ke image adalah [nama file string]_image_[YYYY-mm-dd]_[HH:MM:SS].

Contoh:
1_image_2025-05-11_18:35:26.png

   d. Catat setiap konversi yang ada ke dalam sebuah log file bernama conversion.log. Untuk formatnya adalah sebagai berikut.
[YYYY-mm-dd][HH:MM:SS]: Successfully converted hexadecimal text [nama file string] to [nama file image].

Contoh: 

   ![Image](https://github.com/user-attachments/assets/cd3a1703-d5e1-42bd-afd5-1239b7e3e378)

Contoh struktur akhir adalah sebagai berikut.

   ![Image](https://github.com/user-attachments/assets/3f8ba2e2-2204-4025-937f-8686fd7ca2e3)


2. Pada suatu hari, seorang ilmuwan muda menemukan sebuah drive tua yang tertanam di reruntuhan laboratorium robotik. Saat diperiksa, drive tersebut berisi pecahan data dari satu-satunya robot perawat legendaris yang dikenal dengan nama Baymax (https://drive.google.com/file/d/1MHVhFT57Wa9Zcx69Bv9j5ImHc9rXMH1c/view?usp=sharing). Sayangnya, akibat kerusakan sistem selama bertahun-tahun, file utuh Baymax telah terfragmentasi menjadi 14 bagian kecil, masing-masing berukuran 1 kilobyte, dan tersimpan dalam direktori bernama relics. Pecahan tersebut diberi nama berurutan seperti Baymax.jpeg.000, Baymax.jpeg.001, hingga Baymax.jpeg.013. Ilmuwan tersebut kini ingin membangkitkan kembali Baymax ke dalam bentuk digital yang utuh, namun ia tidak ingin merusak file asli yang telah rapuh tersebut.

   a. Sebagai asisten teknis, tugasmu adalah membuat sebuah sistem file virtual menggunakan FUSE (Filesystem in Userspace) yang dapat membantu sang ilmuwan. Buatlah sebuah direktori mount bernama bebas (misalnya mount_dir) yang merepresentasikan tampilan Baymax dalam bentuk file utuh Baymax.jpeg. File sistem tersebut akan mengambil data dari folder relics sebagai sumber aslinya.

   ![Image](https://github.com/user-attachments/assets/b0f70271-0291-42a9-a4ae-94ed183d88f2)

   b. Ketika direktori FUSE diakses, pengguna hanya akan melihat Baymax.jpeg seolah-olah tidak pernah terpecah, meskipun aslinya terdiri dari potongan .000 hingga .013. File Baymax.jpeg tersebut dapat dibaca, ditampilkan, dan disalin sebagaimana file gambar biasa, hasilnya merupakan gabungan sempurna dari keempat belas pecahan tersebut.

   c. Namun sistem ini harus lebih dari sekadar menyatukan. Jika pengguna membuat file baru di dalam direktori FUSE, maka sistem harus secara otomatis memecah file tersebut ke dalam potongan-potongan berukuran maksimal 1 KB, dan menyimpannya di direktori relics menggunakan format [namafile].000, [namafile].001, dan seterusnya.

   d. Ketika file tersebut dihapus dari direktori mount, semua pecahannya di relics juga harus ikut dihapus.

   e. Untuk keperluan analisis ilmuwan, sistem juga harus mencatat seluruh aktivitas pengguna dalam sebuah file log bernama activity.log yang disimpan di direktori yang sama. Aktivitas yang dicatat antara lain:
   	- Membaca file (misalnya membuka baymax.png)
	- Membuat file baru (termasuk nama file dan jumlah pecahan)
 	- Menghapus file (termasuk semua pecahannya yang terhapus)
	- Menyalin file (misalnya cp baymax.png /tmp/)
 	- Contoh Log:
	![Image](https://github.com/user-attachments/assets/de3e5e11-156c-4010-8995-34cbfe1fa3ea)


3. Nafis dan Kimcun merupakan dua mahasiswa anomali😱 yang paling tidak tahu sopan santun dan sangat berbahaya di antara angkatan 24. Maka dari itu, Pujo sebagai komting yang baik hati dan penyayang😍, memutuskan untuk membuat sebuah sistem pendeteksi kenakalan bernama Anti Napis Kimcun (AntiNK) untuk melindungi file-file penting milik angkatan 24. Pujo pun kemudian bertanya kepada Asisten bagaimana cara membuat sistem yang benar, para asisten pun merespon.

   a. Pujo harus membuat sistem AntiNK menggunakan Docker yang menjalankan FUSE dalam container terisolasi. Sistem ini menggunakan docker-compose untuk mengelola container antink-server (FUSE Func.) dan antink-logger (Monitoring Real-Time Log). Asisten juga memberitahu bahwa docker-compose juga memiliki beberapa komponen lain yaitu:
	- it24_host (Bind Mount -> Store Original File)
	- antink_mount (Mount Point)
	- antink-logs (Bind Mount -> Store Log)

   b. Sistem harus mendeteksi file dengan kata kunci "nafis" atau "kimcun" dan membalikkan nama file tersebut saat ditampilkan. Saat file berbahaya (kimcun atau nafis) terdeteksi, sistem akan mencatat peringatan ke dalam log.

Ex: "docker exec [container-name] ls /antink_mount" 

Output: 
test.txt  vsc.sifan  txt.nucmik

   c. Dikarenakan dua anomali tersebut terkenal dengan kelicikannya, Pujo mempunyai ide bahwa isi dari file teks normal akan di enkripsi menggunakan ROT13 saat dibaca, sedangkan file teks berbahaya tidak di enkripsi.
 
Ex: "docker exec [container-name] cat /antink_mount/test.txt" 

Output: 
enkripsi teks asli

   d. Semua aktivitas dicatat dengan ke dalam log file /var/log/it24.log yang dimonitor secara real-time oleh container logger.
   ![Image](https://github.com/user-attachments/assets/c74f2a75-0eba-4107-a359-6d10182524e3)

   e. Semua perubahan file hanya terjadi di dalam container server jadi tidak akan berpengaruh di dalam direktori host. 


### Soal 1



# Dokumentasi Program Konversi Hexadecimal ke Gambar

## Tujuan Program

- Mengambil sampel anomali teks dalam format hexadecimal dari file ZIP.
- Mengonversi setiap string hexadecimal menjadi gambar PNG.
- Menyimpan gambar dalam folder `image`.
- Mencatat setiap proses konversi dalam file `conversion.log`.

## Alur Program

1. **Mengunduh dan Mengekstrak File ZIP**
   - Menggunakan `wget` untuk mengunduh ZIP dari Google Drive.
   - Mengekstrak ZIP menggunakan `unzip`.
   - Menghapus file ZIP setelah ekstraksi.

2. **Identifikasi Format Teks**
   - Teks dalam file ZIP berada dalam format hexadecimal.

3. **Konversi Hexadecimal ke Gambar**
   - Menggunakan `strtol()` untuk mengubah string hexadecimal menjadi data biner.
   - Data biner disimpan sebagai file PNG.

4. **Penamaan File Gambar**
   - Format: `[nama file string]_image_[YYYY-mm-dd]_[HH:MM:SS].png`

5. **Penyimpanan Gambar**
   - Gambar disimpan di dalam folder `image`.

6. **Pencatatan ke dalam Log**
   - Format log: `[YYYY-mm-dd][HH:MM:SS]: Successfully converted hexadecimal text [nama file string] to [nama file image].`

## Fungsi Utama Program

### 1. `set_dirpath()`
Mengatur direktori kerja program ke `/anomali`.

### 2. `ensure_output_folder_exists()` dan `ensure_anomali_folder_exists()`
Memastikan folder output tersedia.

### 3. `get_current_time_str()`
Mengambil waktu saat ini dalam format tanggal dan waktu.

### 4. `hex_to_val()`
Mengubah karakter hexadecimal menjadi nilai integer.

### 5. `convert_hex_to_binary()`
Mengonversi string hexadecimal menjadi data biner.

### 6. `convert_binary_to_image()`
Menyimpan data biner sebagai file PNG.

### 7. `log_conversion()`
Mencatat proses konversi ke file `conversion.log`.

### 8. `xmp_getattr()` dan `xmp_readdir()`
Fungsi FUSE untuk mengambil atribut file dan membaca direktori.

## Contoh Output

### Nama File
`1_image_2025-05-11_18:35:26.png`

### Log
`[2025-05-11][18:35:26]: Successfully converted hexadecimal text 1.txt to 1_image_2025-05-11_18:35:26.png.`

## Penutup

Program ini memanfaatkan FUSE untuk menjalankan konversi file berbasis filesystem secara otomatis, memudahkan pengguna untuk melihat hasil konversi secara langsung dalam folder virtual.

## Output
	![Screenshot 2025-05-17 112924](https://github.com/user-attachments/assets/ed27f029-8347-477e-8cdc-1effb8600b87)
 	![Screenshot 2025-05-23 171145](https://github.com/user-attachments/assets/b730f98d-1b6c-4584-b19f-8d8dbfd20c47)
	![image](https://github.com/user-attachments/assets/54c6b0f0-385c-4fb9-8cf1-4ff2c8b5e569)


### Soal 2

  ## Script baymax.c

a.) Fungsi Pendukung
```
static int make_chunk_path(...);
void log_activity(...);
```
  - make_chunk_path(): Membuat nama file chunk, misal file.txt.000, file.txt.001, dst.
  - log_activity(): Mencatat aktivitas file ke dalam activity.log dengan timestamp.

b.) Fungsi Utama FUSE
```
static int baymax_getattr(...)
static int baymax_readdir(...)
static int baymax_open(...)
static int baymax_read(...)
static int baymax_create(...);
static int baymax_mknod(...);
static int baymax_write(...)
static int baymax_truncate(...);
static int baymax_ftruncate(...);
static int baymax_unlink(...)
```
  - Mengecek atribut file. Menggabungkan ukuran semua chunk menjadi satu ukuran file utuh.
  - Membaca isi direktori root / dan hanya menampilkan file yang punya chunk .000.
  - Mengecek apakah file .000 ada saat ingin dibuka. Juga mencatat aktivitas "OPEN".
  - Membaca isi file berdasarkan offset dan jumlah size. Membaca dari satu atau lebih chunk.
  - Membuat file baru dengan chunk pertama .000, dan mencatat "CREATE".
  - Menulis data ke chunk sesuai offset dan ukuran. Chunk dibuat otomatis jika perlu. Mencatat "WRITE".
  - Mengubah ukuran file. Menghapus chunk yang tidak dibutuhkan atau menyesuaikan ukuran chunk terakhir. Mencatat "TRUNCATE".
  - Menghapus semua chunk file dan mencatat "DELETE".

c.) Struktur FUSE Operation
```
static struct fuse_operations baymax_oper = {
    .getattr    = baymax_getattr,
    .readdir    = baymax_readdir,
    .open       = baymax_open,
    .read       = baymax_read,
    .create     = baymax_create,
    .write      = baymax_write,
    .unlink     = baymax_unlink,
    .mknod      = baymax_mknod,
    .truncate   = baymax_truncate,
    .ftruncate  = baymax_ftruncate,
};
```
  - Struktur ini menghubungkan nama operasi FUSE ke fungsi yang sudah didefinisikan.

d.) Fungsi main
```
int main(int argc, char *argv[]) {
    umask(0);
    return fuse_main(argc, argv, &baymax_oper, NULL);
}
```
  - Memulai filesystem FUSE dengan struktur operasi yang sudah dibuat.

### Soal 3

  ## Script Dockerfile

a.) Base & Install
```
FROM ubuntu:20.04
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential pkg-config libfuse-dev fuse \
    iproute2 procps nano vim curl sudo \
    && rm -rf /var/lib/apt/lists/*
```
  - Pakai Ubuntu 20.04.
  - Install tools build, FUSE, dan utilitas debugging.

b.) Setup Folder & Compile
```
RUN mkdir -p /it24_host /antink_mount /antink-logs
COPY antink.c /antink.c
RUN gcc -Wall -o /antink /antink.c `pkg-config fuse --cflags --libs`
```
  - Buat folder kerja.
  - Compile antink.c jadi binary /antink.

c.) Start Script
```
RUN echo '#!/bin/bash\n\
mkdir -p /it24_host /antink_mount /antink-logs\n\
touch /antink-logs/it24.log\n\
if [ "$1" = "server" ]; then\n\
  /antink /antink_mount -f -d\n\
elif [ "$1" = "logger" ]; then\n\
  tail -f /antink-logs/it24.log\n\
else\n\
  exec "$@"\n\
fi' > /start.sh && chmod +x /start.sh
```
  - Buat script start: bisa jalanin server, logger, atau perintah bebas.

d.) Entrypoint & CMD
```
ENTRYPOINT ["/start.sh"]
CMD ["server"]
```
  - Default-nya jalanin start.sh server.

  ## Script docker-compose.yml

a.) Versi & Struktur
```
version: '3'

services:
  antink-server:
    ...
  antink-logger:
    ...
volumes:
  antink_mount:
  antink-logs:
```
  - Gunakan Compose versi 3.
  - Definisikan 2 service: antink-server & antink-logger, plus 2 volume untuk share data.

b.) antink-server
```
  antink-server:
    build:
      context: .
      dockerfile: Dockerfile
    image: antink-server:latest
    container_name: antink-server
    command: server
    privileged: true
    volumes:
      - ./it24_host:/it24_host:rw
      - antink_mount:/antink_mount
      - antink-logs:/antink-logs
    cap_add:
      - SYS_ADMIN
    devices:
      - /dev/fuse:/dev/fuse
    security_opt:
      - apparmor:unconfined
    restart: unless-stopped
```
  - Build dari Dockerfile.
  - Butuh privileged, akses fuse, dan SYS_ADMIN untuk mount FUSE.
  - Volume untuk host folder dan log.

c.) antink-logger
```
  antink-logger:
    build:
      context: .
      dockerfile: Dockerfile
    image: antink-logger:latest
    container_name: antink-logger
    command: logger
    volumes:
      - antink-logs:/antink-logs
    depends_on:
      - antink-server
    restart: unless-stopped
```
  - Logger jalanin command: logger.
  - Ambil log dari volume antink-logs.
  - Start setelah antink-server.

d.) Volumes
```
volumes:
  antink_mount:
  antink-logs:
```
  - Volume shared antar container:
      - antink_mount: untuk hasil mount FUSE.
      - antink-logs: untuk file log it24.log.

  ## Script antink.c

a.) Bagian Konstanta dan Path
```
static const char *dirpath = "/it24_host";
static const char *logpath = "/antink-logs/it24.log";
```
  - dirpath: Direktori asli tempat file sebenarnya berada.
  - logpath: Lokasi file log semua aktivitas AntiNK.

b.) Logging Function
```
void write_log(const char *action, const char *path, const char *log_type);
```
  - Menulis log aktivitas ke logpath.
  - Abaikan log GETATTR dan READDIR jika log_type == NULL (biar log gak penuh).
  - Format log bervariasi tergantung log_type:
       - ALERT: untuk deteksi file berbahaya.
       - REVERSE: saat file dibalik namanya.
       - ENCRYPT: saat isi file dienkripsi.

c.) Deteksi File Berbahaya
```
int is_dangerous_file(const char *path);
```
  - Mengecek apakah nama file mengandung nafis atau kimcun (dalam huruf kecil).
  - Jika ya, tulis log ALERT dan return 1.

d.) Fungsi Reversing Nama
```
void get_reversed_name(const char *name, char *reversed);
void get_original_name(const char *reversed, char *original);
```
  - Membalik nama file atau mengembalikannya ke nama asli (digunakan di readdir, getattr, dll).

e.) Enkripsi ROT13
```
void rot13_encrypt(char *text) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            char base = islower(text[i]) ? 'a' : 'A';
            text[i] = (text[i] - base + 13) % 26 + base;
        }
    }
}
```
  - Mengubah karakter alfabet pada buffer dengan algoritma ROT13 (a↔n, b↔o, dll).
  - Digunakan untuk mengenkripsi isi file .txt biasa.

f.) Fungsi Utilitas Path
```
static void get_path_from_root(char *fullpath, const char *path) {
    strcpy(fullpath, dirpath);
    if (path[0] != '/' && strlen(path) > 0) {
        strcat(fullpath, "/");
    }
    strcat(fullpath, path);
}
```
  - Menggabungkan dirpath + path hasil mount agar bisa diakses langsung dari filesystem asli.

g.) xmp_getattr
```
static int xmp_getattr(const char *path, struct stat *stbuf);
```
  - Mendapatkan atribut file.
  - Jika nama file sudah dibalik (misal .sifan), kembalikan dulu ke nama asli.
  - Lanjutkan lstat.

h.) xmp_readdir
```
static int xmp_readdir(...);
```
  - Digunakan untuk membaca isi direktori.
  - Jika file termasuk berbahaya, nama file akan dibalik sebelum ditampilkan.
  - Menulis log REVERSE saat proses ini terjadi.

i.) xmp_read
```
static int xmp_read(...);
```
  - Buka file dari path asli.
  - Enkripsi isi file menggunakan ROT13
  - Log ENCRYPT ditulis bila enkripsi dijalankan.

j.) xmp_write, xmp_open, xmp_access
```
static int xmp_write(...);
static int xmp_open(...);
static int xmp_access(...);
```
  - Sama-sama mengubah path jika file telah dibalik.
  - Menulis log WRITE, OPEN, dan ACCESS.

k.) xmp_create, xmp_mkdir, xmp_rmdir, xmp_unlink
```
static int xmp_create(...);
static int xmp_mkdir(...);
static int xmp_rmdir(...);
static int xmp_unlink(...);
```
  - Operasi dasar filesystem (buat file/dir, hapus, dll).
  - Menyusun full path lalu menjalankan operasi asli.
  - Log ditulis sesuai aksi (CREATE, MKDIR, dst).

l.) Operasi FUSE
```
static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .write = xmp_write,
    .open = xmp_open,
    .access = xmp_access,
    .create = xmp_create,
    .mkdir = xmp_mkdir,
    .rmdir = xmp_rmdir,
    .unlink = xmp_unlink,
};
```
  - Mendaftarkan semua handler untuk fungsi-fungsi FUSE seperti read, write, readdir, dll.

m.) Fungsi main
```
int main(int argc, char *argv[]) {
    mkdir("/antink-logs", 0755);

    FILE *log_file = fopen(logpath, "w");
    if (log_file) {
        fprintf(log_file, "=== AntiNK System Started ===\n");
        fclose(log_file);
    }

    return fuse_main(argc, argv, &xmp_oper, NULL);
}
```
  - Membuat direktori /antink-logs jika belum ada.
  - Menulis "AntiNK System Started" ke file log.
  - Memulai FUSE menggunakan fuse_main.
