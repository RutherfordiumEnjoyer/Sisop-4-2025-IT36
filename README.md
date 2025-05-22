# Sisop-4-2025-IT36

Anggota Kelompok
|     Nrp    |                Nama              |
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
			—**soal_1:**
				— `hexed.c`
      —**soal_2:**
        — `baymax.c`
      —**soal_3:**	
	     — `Dockerfile`
	     — `docker-compose.yml`
	     — `antink.c`
     —soal_4:
      —`maimai_fs.c`
     —assets

   Jika melanggar struktur repo akan dianggap sama dengan curang dan menerima konsekuensi sama dengan melakukan kecurangan. Folder assets bersifat opsional dan hanya diisi dengan dokumentasi pengerjaan yang akan diletakkan di readme. Tidak ada format khusus untuk penamaan file dalam assets
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
