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

1. Mounting FUSE & Penentuan Root Directory
   
   	```cpp
        int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <rootdir> <mountpoint>\n", argv[0]);
        return 1;
    }

    if (!realpath(argv[1], rootdir)) {
        perror("realpath");
        return 1;
    }

   char image_dir[PATH_MAX];
    snprintf(image_dir, PATH_MAX, "%s/image", rootdir);
    struct stat st;
    if (stat(image_dir, &st) == -1) {
        if (mkdir(image_dir, 0755) == -1) {
            perror("mkdir image");
            return 1;
        }
    }

    argv[1] = argv[2];
    argc--;
    }

    	char image_dir[PATH_MAX];
   
    	snprintf(image_dir, PATH_MAX, "%s/image", rootdir);
   
    	struct stat st;
   
    	if (stat(image_dir, &st) == -1) {
   
   		if (mkdir(image_dir, 0755) == -1) {
   
   		perror("mkdir image");
   
   		return 1;
        }
    }

    argv[1] = argv[2];
    argc--;

    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
	}

   	 ```

 2.  Menggabungkan root directory (rootdir) dan path relatif dari FUSE menjadi path absolut file di sistem lokal.

	```cpp
   	static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
        	 off_t offset, struct fuse_file_info *fi) {
    	DIR *dp;
    	struct dirent *de;
    	char fpath[PATH_MAX];
    	fullpath(fpath, path);

    	(void) offset;
    	(void) fi;

    	dp = opendir(fpath);
    	if (dp == NULL) return -errno;

    	while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
 	if (de->d_type == DT_REG) {
            const char *ext = strrchr(de->d_name, '.');
            if (ext && strcmp(ext, ".txt") == 0) {
                char filename_base[NAME_MAX];
                strncpy(filename_base, de->d_name, NAME_MAX);
                filename_base[strlen(filename_base) - 4] = '\0';

                if (!already_converted(filename_base)) {
                    char fullfile[PATH_MAX];
                    snprintf(fullfile, PATH_MAX, "%s/%s", fpath, de->d_name);
                    hex_to_png(fullfile, filename_base);
                }
            }
        }

        if (filler(buf, de->d_name, &st, 0)) break;
   	 }

   	 closedir(dp);
  	  return 0;

	```



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
