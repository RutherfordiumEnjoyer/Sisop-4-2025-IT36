#define FUSE_USE_VERSION 28

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>
#include <limits.h>
#include <sys/stat.h>
#include <time.h>

static char rootdir[PATH_MAX];

static void fullpath(char fpath[PATH_MAX], const char *path) {
    snprintf(fpath, PATH_MAX, "%s%s", rootdir, path);
}

static int xmp_getattr(const char *path, struct stat *stbuf) {
    int res;
    char fpath[PATH_MAX];
    fullpath(fpath, path);

    res = lstat(fpath, stbuf);
    if (res == -1) return -errno;
    return 0;
}

static int hex_to_png(const char *filepath, const char *filename_base) {
    FILE *fp = fopen(filepath, "r");
    if (!fp) return -1;

    // Read hex data from file
    char *hexdata = NULL;
    size_t len = 0;
    ssize_t readlen = 0;
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    hexdata = malloc(len + 1);
    if (!hexdata) {
        fclose(fp);
        return -1;
    }
    readlen = fread(hexdata, 1, len, fp);
    fclose(fp);
    hexdata[readlen] = 0;

    // Remove newlines and spaces from hexdata (optional but safer)
    char *cleanhex = malloc(readlen + 1);
    if (!cleanhex) {
        free(hexdata);
        return -1;
    }
    int ci = 0;
    for (int i = 0; i < readlen; i++) {
        if ((hexdata[i] >= '0' && hexdata[i] <= '9') ||
            (hexdata[i] >= 'a' && hexdata[i] <= 'f') ||
            (hexdata[i] >= 'A' && hexdata[i] <= 'F')) {
            cleanhex[ci++] = hexdata[i];
        }
    }
    cleanhex[ci] = 0;
    free(hexdata);

    int bin_len = ci / 2;
    unsigned char *bin = malloc(bin_len);
    if (!bin) {
        free(cleanhex);
        return -1;
    }

    // Convert hex string to binary
    for (int i = 0; i < bin_len; i++) {
        unsigned int val;
        sscanf(&cleanhex[i * 2], "%2x", &val);
        bin[i] = (unsigned char)val;
    }
    free(cleanhex);

    // Prepare output path and filename with timestamp
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char outpath[PATH_MAX];
    snprintf(outpath, PATH_MAX, "%s/image/%s_image_%04d-%02d-%02d_%02d:%02d:%02d.png",
             rootdir, filename_base,
             tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
             tm.tm_hour, tm.tm_min, tm.tm_sec);

    // Write binary to PNG file
    FILE *out = fopen(outpath, "wb");
    if (!out) {
        free(bin);
        return -1;
    }
    fwrite(bin, 1, bin_len, out);
    fclose(out);
    free(bin);

    // Append log entry
    char logpath[PATH_MAX];
    snprintf(logpath, PATH_MAX, "%s/conversion.log", rootdir);
    FILE *log = fopen(logpath, "a");
    if (log) {
        fprintf(log, "[%04d-%02d-%02d %02d:%02d:%02d] Converted %s to %s\n",
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                tm.tm_hour, tm.tm_min, tm.tm_sec,
                filepath, outpath);
        fclose(log);
    }

    return 0;
}

static int already_converted(const char *filename_base) {
    // Check if any file in image folder starts with filename_base
    char image_dir[PATH_MAX];
    snprintf(image_dir, PATH_MAX, "%s/image", rootdir);

    DIR *dp = opendir(image_dir);
    if (!dp) return 0; // no image dir means not converted

    struct dirent *de;
    size_t base_len = strlen(filename_base);
    int found = 0;
    while ((de = readdir(dp)) != NULL) {
        if (strncmp(de->d_name, filename_base, base_len) == 0) {
            found = 1;
            break;
        }
    }
    closedir(dp);
    return found;
}

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

        // Jika file txt, cek dan convert jika belum pernah
        if (de->d_type == DT_REG) {
            const char *ext = strrchr(de->d_name, '.');
            if (ext && strcmp(ext, ".txt") == 0) {
                char filename_base[NAME_MAX];
                strncpy(filename_base, de->d_name, NAME_MAX);
                filename_base[strlen(filename_base) - 4] = '\0'; // hilangkan ".txt"

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
}

static int xmp_read(const char *path, char *buf, size_t size,
                    off_t offset, struct fuse_file_info *fi) {
    int fd;
    int res;
    char fpath[PATH_MAX];
    fullpath(fpath, path);

    (void) fi;

    fd = open(fpath, O_RDONLY);
    if (fd == -1) return -errno;

    res = pread(fd, buf, size, offset);
    if (res == -1) res = -errno;

    close(fd);
    return res;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
};

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <rootdir> <mountpoint>\n", argv[0]);
        return 1;
    }

    // Simpan rootdir dari argumen pertama
    if (!realpath(argv[1], rootdir)) {
        perror("realpath");
        return 1;
    }

    // Pastikan folder image ada
    char image_dir[PATH_MAX];
    snprintf(image_dir, PATH_MAX, "%s/image", rootdir);
    struct stat st;
    if (stat(image_dir, &st) == -1) {
        if (mkdir(image_dir, 0755) == -1) {
            perror("mkdir image");
            return 1;
        }
    }

    // Geser argumen agar sesuai untuk fuse_main
    argv[1] = argv[2];
    argc--;

    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
