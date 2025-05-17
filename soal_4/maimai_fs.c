#define FUSE_USE_VERSION 31
#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <openssl/evp.h>
#include <zlib.h>

#define BASE_DIR "/tmp/chiho"
#define FUSE_DIR "/tmp/maimai_fuse"
#define MAX_PATH_LEN 1024

static const unsigned char AES_KEY[] = "thisisasecretencryptionkey12345678";

static const char *base_dir = BASE_DIR;

static char* build_real_path(const char *path) {
    char *real_path = malloc(MAX_PATH_LEN);
    if (strcmp(path, "/") == 0) {
        snprintf(real_path, MAX_PATH_LEN, "%s", BASE_DIR);
    } else {
        snprintf(real_path, MAX_PATH_LEN, "%s%s", BASE_DIR, path);
    }
    return real_path;
}

static int is_directory(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    return S_ISDIR(st.st_mode);
}

static char* starter_get_real_path(const char *path) {
    char *real_path = build_real_path(path);
    if (!is_directory(real_path) && strcmp(path, "/") != 0) {
        strcat(real_path, ".mai");
    }
    return real_path;
}

static int starter_getattr(const char *path, struct stat *stbuf) {
    char *real_path = starter_get_real_path(path);
    int res = lstat(real_path, stbuf);
    free(real_path);
    if (res == -1) return -errno;
    return 0;
}

static int starter_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi) {
    char *real_path = starter_get_real_path(path);

    DIR *dp;
    struct dirent *de;

    (void) offset;
    (void) fi;

    dp = opendir(real_path);
    if (dp == NULL) {
        free(real_path);
        return -errno;
    }

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        char *display_name = strdup(de->d_name);
        char *ext = strrchr(display_name, '.');
        if (ext && strcmp(ext, ".mai") == 0) {
            *ext = '\0';
        }

        if (filler(buf, display_name, &st, 0)) break;
        free(display_name);
    }

    closedir(dp);
    free(real_path);
    return 0;
}

static int starter_open(const char *path, struct fuse_file_info *fi) {
    char *real_path = starter_get_real_path(path);
    int res = open(real_path, fi->flags);
    free(real_path);
    if (res == -1) return -errno;
    fi->fh = res;
    return 0;
}

static int starter_read(const char *path, char *buf, size_t size, off_t offset,
                       struct fuse_file_info *fi) {
    int res = pread(fi->fh, buf, size, offset);
    if (res == -1) return -errno;
    return res;
}

static int starter_write(const char *path, const char *buf, size_t size,
                       off_t offset, struct fuse_file_info *fi) {
    int res = pwrite(fi->fh, buf, size, offset);
    if (res == -1) {
        fprintf(stderr, "Write failed: %s\n", strerror(errno));
        return -errno;
    }
    return res;
}

static int starter_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    char *real_path = starter_get_real_path(path);
    char *parent_dir = strdup(real_path);
    char *last_slash = strrchr(parent_dir, '/');
    if (last_slash) {
        *last_slash = '\0';
        mkdir(parent_dir, 0755);
    }
    free(parent_dir);

    int res = open(real_path, fi->flags, mode);
    free(real_path);
    if (res == -1) return -errno;

    fi->fh = res;
    return 0;
}

static int starter_unlink(const char *path) {
    char *real_path = starter_get_real_path(path);
    int res = unlink(real_path);
    free(real_path);
    if (res == -1) return -errno;
    return 0;
}

static char* metropolis_shift_name(const char *name, int direction) {
    char *shifted = strdup(name);
    for (int i = 0; shifted[i]; i++) {
        shifted[i] = (shifted[i] + (i % 256) * direction) % 256;
    }
    return shifted;
}

static char* metropolis_get_real_path(const char *path) {
    char *real_path = build_real_path(path);


    char *last_slash = strrchr(real_path, '/');
    if (last_slash && *(last_slash + 1)) {
        char *shifted = metropolis_shift_name(last_slash + 1, 1);
        strcpy(last_slash + 1, shifted);
        free(shifted);
    }

    return real_path;
}

static int metropolis_getattr(const char *path, struct stat *stbuf) {
    char *real_path = metropolis_get_real_path(path);
    int res = lstat(real_path, stbuf);
    free(real_path);
    if (res == -1) return -errno;
    return 0;
}

static int metropolis_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                            off_t offset, struct fuse_file_info *fi) {
    char *real_path = metropolis_get_real_path(path);

    DIR *dp;
    struct dirent *de;

    (void) offset;
    (void) fi;

    dp = opendir(real_path);
    if (dp == NULL) {
        free(real_path);
        return -errno;
    }

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        char *display_name = metropolis_shift_name(de->d_name, -1);

        if (filler(buf, display_name, &st, 0)) break;
        free(display_name);
    }

    closedir(dp);
    free(real_path);
    return 0;
}

static int metropolis_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    char *real_path = metropolis_get_real_path(path);
    int res = open(real_path, fi->flags, mode);
    free(real_path);
    if (res == -1) return -errno;

    fi->fh = res;
    return 0;
}

static int metropolis_open(const char *path, struct fuse_file_info *fi) {
    char *real_path = metropolis_get_real_path(path);
    int res = open(real_path, fi->flags);
    free(real_path);
    if (res == -1) return -errno;

    fi->fh = res;
    return 0;
}

static int metropolis_read(const char *path, char *buf, size_t size, off_t offset,
                         struct fuse_file_info *fi) {
    int res = pread(fi->fh, buf, size, offset);
    if (res == -1) return -errno;
    return res;
}

static int metropolis_write(const char *path, const char *buf, size_t size,
                          off_t offset, struct fuse_file_info *fi) {
    int res = pwrite(fi->fh, buf, size, offset);
    if (res == -1) return -errno;
    return res;
}

static int metropolis_unlink(const char *path) {
    char *real_path = metropolis_get_real_path(path);
    int res = unlink(real_path);
    free(real_path);
    if (res == -1) return -errno;
    return 0;
}


static void dragon_rot13(char *str) {
    for (; *str; str++) {
        if (*str >= 'a' && *str <= 'z') {
            *str = 'a' + (*str - 'a' + 13) % 26;
        } else if (*str >= 'A' && *str <= 'Z') {
            *str = 'A' + (*str - 'A' + 13) % 26;
        }
    }
}

static int dragon_getattr(const char *path, struct stat *stbuf) {
    char *real_path = build_real_path(path);
    int res = lstat(real_path, stbuf);
    free(real_path);
    if (res == -1) return -errno;
    return 0;
}

static int dragon_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                        off_t offset, struct fuse_file_info *fi) {
    char *real_path = build_real_path(path);

    DIR *dp;
    struct dirent *de;

    (void) offset;
    (void) fi;

    dp = opendir(real_path);
    if (dp == NULL) {
        free(real_path);
        return -errno;
    }

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        if (filler(buf, de->d_name, &st, 0)) break;
    }

    closedir(dp);
    free(real_path);
    return 0;
}

static int dragon_open(const char *path, struct fuse_file_info *fi) {
    char *real_path = build_real_path(path);
    int res = open(real_path, fi->flags);
    free(real_path);
    if (res == -1) return -errno;

    fi->fh = res;
    return 0;
}

static int dragon_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi) {
    char *temp_buf = malloc(size);
    int res = pread(fi->fh, temp_buf, size, offset);
    if (res == -1) {
        free(temp_buf);
        return -errno;
    }

    dragon_rot13(temp_buf);
    memcpy(buf, temp_buf, res);
    free(temp_buf);
    return res;
}

static int dragon_write(const char *path, const char *buf, size_t size,
                       off_t offset, struct fuse_file_info *fi) {
    char *temp_buf = malloc(size);
    memcpy(temp_buf, buf, size);
    dragon_rot13(temp_buf);

    int res = pwrite(fi->fh, temp_buf, size, offset);
    free(temp_buf);
    if (res == -1) return -errno;
    return res;
}

static int dragon_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    char *real_path = build_real_path(path);
    int res = open(real_path, fi->flags, mode);
    free(real_path);
    if (res == -1) return -errno;

    fi->fh = res;
    return 0;
}

static int dragon_unlink(const char *path) {
    char *real_path = build_real_path(path);
    int res = unlink(real_path);
    free(real_path);
    if (res == -1) return -errno;
    return 0;
}


static int blackrose_getattr(const char *path, struct stat *stbuf) {
    char *real_path = build_real_path(path);
    int res = lstat(real_path, stbuf);
    free(real_path);
    if (res == -1) return -errno;
    return 0;
}

static int blackrose_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                           off_t offset, struct fuse_file_info *fi) {
    char *real_path = build_real_path(path);

    DIR *dp;
    struct dirent *de;

    (void) offset;
    (void) fi;

    dp = opendir(real_path);
    if (dp == NULL) {
        free(real_path);
        return -errno;
    }

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        if (filler(buf, de->d_name, &st, 0)) break;
    }

    closedir(dp);
    free(real_path);
    return 0;
}

static int blackrose_open(const char *path, struct fuse_file_info *fi) {
    char *real_path = build_real_path(path);
    int res = open(real_path, fi->flags);
    free(real_path);
    if (res == -1) return -errno;

    fi->fh = res;
    return 0;
}

static int blackrose_read(const char *path, char *buf, size_t size, off_t offset,
                        struct fuse_file_info *fi) {
    int res = pread(fi->fh, buf, size, offset);
    if (res == -1) return -errno;
    return res;
}

static int blackrose_write(const char *path, const char *buf, size_t size,
                         off_t offset, struct fuse_file_info *fi) {
    int res = pwrite(fi->fh, buf, size, offset);
    if (res == -1) return -errno;
    return res;
}

static int blackrose_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    char *real_path = build_real_path(path);
    int res = open(real_path, fi->flags, mode);
    free(real_path);
    if (res == -1) return -errno;

    fi->fh = res;
    return 0;
}

static int blackrose_unlink(const char *path) {
    char *real_path = build_real_path(path);
    int res = unlink(real_path);
    free(real_path);
    if (res == -1) return -errno;
    return 0;
}


static int heaven_encrypt_decrypt(const char *path, const char *in_buf, char *out_buf,
                                size_t size, int encrypt) {
    unsigned char key[32];
    memcpy(key, AES_KEY, 32);
    unsigned char iv[16];
    for (int i = 0; i < 16; i++) iv[i] = path[i % strlen(path)];

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return -1;

    if (encrypt) {
        EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    } else {
        EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    }

    int len, out_len;
    if (!EVP_CipherUpdate(ctx, out_buf, &len, in_buf, size)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    out_len = len;

    if (!EVP_CipherFinal_ex(ctx, out_buf + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    out_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return out_len;
}

static int heaven_getattr(const char *path, struct stat *stbuf) {
    char *real_path = build_real_path(path);
    int res = lstat(real_path, stbuf);
    free(real_path);
    if (res == -1) return -errno;
    return 0;
}

static int heaven_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                        off_t offset, struct fuse_file_info *fi) {
    char *real_path = build_real_path(path);

    DIR *dp;
    struct dirent *de;

    (void) offset;
    (void) fi;

    dp = opendir(real_path);
    if (dp == NULL) {
        free(real_path);
        return -errno;
    }

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        if (filler(buf, de->d_name, &st, 0)) break;
    }

    closedir(dp);
    free(real_path);
    return 0;
}

static int heaven_open(const char *path, struct fuse_file_info *fi) {
    char *real_path = build_real_path(path);
    int res = open(real_path, fi->flags);
    free(real_path);
    if (res == -1) return -errno;

    fi->fh = res;
    return 0;
}

static int heaven_read(const char *path, char *buf, size_t size, off_t offset,
                     struct fuse_file_info *fi) {

    char *enc_buf = malloc(size);
    int res = pread(fi->fh, enc_buf, size, offset);
    if (res <= 0) {
        free(enc_buf);
        return res;
    }


    heaven_encrypt_decrypt(path, enc_buf, buf, res, 0);
    free(enc_buf);
    return res;
}

static int heaven_write(const char *path, const char *buf, size_t size,
                      off_t offset, struct fuse_file_info *fi) {

    char *enc_buf = malloc(size);
    heaven_encrypt_decrypt(path, buf, enc_buf, size, 1);

    int res = pwrite(fi->fh, enc_buf, size, offset);
    free(enc_buf);
    if (res == -1) return -errno;
    return res;
}

static int heaven_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    char *real_path = build_real_path(path);
    int res = open(real_path, fi->flags, mode);
    free(real_path);
    if (res == -1) return -errno;

    fi->fh = res;
    return 0;
}

static int heaven_unlink(const char *path) {
    char *real_path = build_real_path(path);
    int res = unlink(real_path);
    free(real_path);
    if (res == -1) return -errno;
    return 0;
}


static int youth_compress_data(const char *in_buf, size_t in_size, char *out_buf, size_t *out_size) {
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    if (deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 | 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        return -1;
    }

    strm.next_in = (Bytef *)in_buf;
    strm.avail_in = in_size;
    strm.next_out = (Bytef *)out_buf;
    strm.avail_out = *out_size;

    if (deflate(&strm, Z_FINISH) != Z_STREAM_END) {
        deflateEnd(&strm);
        return -1;
    }

    *out_size = strm.total_out;
    deflateEnd(&strm);
    return 0;
}

static int youth_decompress_data(const char *in_buf, size_t in_size, char *out_buf, size_t *out_size) {
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = (Bytef *)in_buf;
    strm.avail_in = in_size;

    if (inflateInit2(&strm, 15 | 16) != Z_OK) {
        return -1;
    }

    int ret;
    do {
        strm.next_out = (Bytef *)out_buf;
        strm.avail_out = *out_size;

        ret = inflate(&strm, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR) break;

        switch (ret) {
            case Z_NEED_DICT:
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                inflateEnd(&strm);
                return -1;
        }
    } while (ret != Z_STREAM_END);

    *out_size = strm.total_out;
    inflateEnd(&strm);
    return ret == Z_STREAM_END ? 0 : -1;
}

static int youth_getattr(const char *path, struct stat *stbuf) {
    char *real_path = build_real_path(path);
    int res = lstat(real_path, stbuf);
    free(real_path);
    if (res == -1) return -errno;
    return 0;
}

static int youth_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                       off_t offset, struct fuse_file_info *fi) {
    char *real_path = build_real_path(path);

    DIR *dp;
    struct dirent *de;

    (void) offset;
    (void) fi;

    dp = opendir(real_path);
    if (dp == NULL) {
        free(real_path);
        return -errno;
    }

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        if (filler(buf, de->d_name, &st, 0)) break;
    }

    closedir(dp);
    free(real_path);
    return 0;
}

static int youth_open(const char *path, struct fuse_file_info *fi) {
    char *real_path = build_real_path(path);
    int res = open(real_path, fi->flags);
    free(real_path);
    if (res == -1) return -errno;

    fi->fh = res;
    return 0;
}

static int youth_read(const char *path, char *buf, size_t size, off_t offset,
                    struct fuse_file_info *fi) {
    char *comp_buf = malloc(size);
    int res = pread(fi->fh, comp_buf, size, offset);
    if (res <= 0) {
        free(comp_buf);
        return res;
    }

    size_t uncompressed_size = size * 4;
    char *uncompressed_buf = malloc(uncompressed_size);

    while (youth_decompress_data(comp_buf, res, uncompressed_buf, &uncompressed_size) != 0) {
        free(uncompressed_buf);
        uncompressed_size *= 2;
        uncompressed_buf = malloc(uncompressed_size);
    }

    memcpy(buf, uncompressed_buf, uncompressed_size);
    free(comp_buf);
    free(uncompressed_buf);
    return uncompressed_size;
}

static int youth_write(const char *path, const char *buf, size_t size,
                     off_t offset, struct fuse_file_info *fi) {
    size_t compressed_size = size + (size / 100) + 12;
    char *compressed_buf = malloc(compressed_size);

    if (youth_compress_data(buf, size, compressed_buf, &compressed_size) != 0) {
        free(compressed_buf);
        return -EIO;
    }

    int res = pwrite(fi->fh, compressed_buf, compressed_size, offset);
    free(compressed_buf);
    if (res == -1) return -errno;
    return size;
}

static int youth_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    char *real_path = build_real_path(path);
    int res = open(real_path, fi->flags, mode);
    free(real_path);
    if (res == -1) return -errno;

    fi->fh = res;
    return 0;
}

static int youth_unlink(const char *path) {
    char *real_path = build_real_path(path);
    int res = unlink(real_path);
    free(real_path);
    if (res == -1) return -errno;
    return 0;
}

static int prism_translate_path(const char *path, char *real_path) {
    const char *prefix = "/7sref/";
    if (strncmp(path, prefix, strlen(prefix)) != 0) {
        return -1;
    }

    const char *filename = path + strlen(prefix);
    char *underscore = strchr(filename, '_');
    if (!underscore) return -1;

    char area[32];
    strncpy(area, filename, underscore - filename);
    area[underscore - filename] = '\0';

    snprintf(real_path, MAX_PATH_LEN, "%s/%s/%s", base_dir, area, underscore + 1);
    return 0;
}

static int prism_getattr(const char *path, struct stat *stbuf) {
    if (strcmp(path, "/7sref") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        return 0;
    }

    char real_path[MAX_PATH_LEN];
    if (prism_translate_path(path, real_path) == -1) {
        return -ENOENT;
    }

    return lstat(real_path, stbuf);
}

static int prism_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                       off_t offset, struct fuse_file_info *fi) {
    if (strcmp(path, "/7sref") != 0) {
        return -ENOENT;
    }

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    filler(buf, "starter_", NULL, 0);
    filler(buf, "metro_", NULL, 0);
    filler(buf, "dragon_", NULL, 0);
    filler(buf, "blackrose_", NULL, 0);
    filler(buf, "heaven_", NULL, 0);
    filler(buf, "youth_", NULL, 0);

    return 0;
}

static int prism_open(const char *path, struct fuse_file_info *fi) {
    char real_path[MAX_PATH_LEN];
    if (prism_translate_path(path, real_path) == -1) {
        return -ENOENT;
    }

    int res = open(real_path, fi->flags);
    if (res == -1) return -errno;

    fi->fh = res;
    return 0;
}

static int prism_read(const char *path, char *buf, size_t size, off_t offset,
                    struct fuse_file_info *fi) {
    const char *area_start = strchr(path + 1, '/') + 1;
    const char *underscore = strchr(area_start, '_');
    char area[32];
    strncpy(area, area_start, underscore - area_start);
    area[underscore - area_start] = '\0';

    if (strcmp(area, "starter") == 0) {
        return starter_read(path, buf, size, offset, fi);
    } else if (strcmp(area, "metro") == 0) {
        return metropolis_read(path, buf, size, offset, fi);
    } else if (strcmp(area, "dragon") == 0) {
        return dragon_read(path, buf, size, offset, fi);
    } else if (strcmp(area, "blackrose") == 0) {
        return blackrose_read(path, buf, size, offset, fi);
    } else if (strcmp(area, "heaven") == 0) {
        return heaven_read(path, buf, size, offset, fi);
    } else if (strcmp(area, "youth") == 0) {
        return youth_read(path, buf, size, offset, fi);
    }

    return -ENOENT;
}

static int prism_write(const char *path, const char *buf, size_t size,
                     off_t offset, struct fuse_file_info *fi) {
    const char *area_start = strchr(path + 1, '/') + 1;
    const char *underscore = strchr(area_start, '_');
    char area[32];
    strncpy(area, area_start, underscore - area_start);
    area[underscore - area_start] = '\0';

    if (strcmp(area, "starter") == 0) {
        return starter_write(path, buf, size, offset, fi);
    } else if (strcmp(area, "metro") == 0) {
        return metropolis_write(path, buf, size, offset, fi);
    } else if (strcmp(area, "dragon") == 0) {
        return dragon_write(path, buf, size, offset, fi);
    } else if (strcmp(area, "blackrose") == 0) {
        return blackrose_write(path, buf, size, offset, fi);
    } else if (strcmp(area, "heaven") == 0) {
        return heaven_write(path, buf, size, offset, fi);
    } else if (strcmp(area, "youth") == 0) {
        return youth_write(path, buf, size, offset, fi);
    }

    return -ENOENT;
}

static int prism_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    char real_path[MAX_PATH_LEN];
    if (prism_translate_path(path, real_path) == -1) {
        return -ENOENT;
    }

    const char *area_start = strchr(path + 1, '/') + 1;
    const char *underscore = strchr(area_start, '_');
    char area[32];
    strncpy(area, area_start, underscore - area_start);
    area[underscore - area_start] = '\0';

    if (strcmp(area, "starter") == 0) {
        return starter_create(real_path, mode, fi);
    } else if (strcmp(area, "metro") == 0) {
        return metropolis_create(real_path, mode, fi);
    } else if (strcmp(area, "dragon") == 0) {
        return dragon_create(real_path, mode, fi);
    } else if (strcmp(area, "blackrose") == 0) {
        return blackrose_create(real_path, mode, fi);
    } else if (strcmp(area, "heaven") == 0) {
        return heaven_create(real_path, mode, fi);
    } else if (strcmp(area, "youth") == 0) {
        return youth_create(real_path, mode, fi);
    }

    return -ENOENT;
}

static int prism_unlink(const char *path) {
    char real_path[MAX_PATH_LEN];
    if (prism_translate_path(path, real_path) == -1) {
        return -ENOENT;
    }

    const char *area_start = strchr(path + 1, '/') + 1;
    const char *underscore = strchr(area_start, '_');
    char area[32];
    strncpy(area, area_start, underscore - area_start);
    area[underscore - area_start] = '\0';

    if (strcmp(area, "starter") == 0) {
        return starter_unlink(real_path);
    } else if (strcmp(area, "metro") == 0) {
        return metropolis_unlink(real_path);
    } else if (strcmp(area, "dragon") == 0) {
        return dragon_unlink(real_path);
    } else if (strcmp(area, "blackrose") == 0) {
        return blackrose_unlink(real_path);
    } else if (strcmp(area, "heaven") == 0) {
        return heaven_unlink(real_path);
    } else if (strcmp(area, "youth") == 0) {
        return youth_unlink(real_path);
    }

    return -ENOENT;
}

static int maimai_getattr(const char *path, struct stat *stbuf) {
    memset(stbuf, 0, sizeof(struct stat));

    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        return 0;
    }

    char *real_path = build_real_path(path);
    int res = lstat(real_path, stbuf);
    free(real_path);

    if (res == -1) return -errno;
    return 0;
}

static int maimai_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi) {
    (void) offset;
    (void) fi;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    if (filler(buf, "starter", NULL, 0) == 1) return 0;
    if (filler(buf, "metro", NULL, 0) == 1) return 0;
    filler(buf, "starter", NULL, 0);
    filler(buf, "metro", NULL, 0);
    filler(buf, "dragon", NULL, 0);
    filler(buf, "blackrose", NULL, 0);
    filler(buf, "heaven", NULL, 0);
    filler(buf, "youth", NULL, 0);
    filler(buf, "7sref", NULL, 0);

    return 0;
}

static int maimai_open(const char *path, struct fuse_file_info *fi) {
    char *real_path = build_real_path(path);
    int res = open(real_path, fi->flags);
    free(real_path);
    if (res == -1) return -errno;

    fi->fh = res;
    return 0;
}

static int maimai_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi) {
    int res = pread(fi->fh, buf, size, offset);
    if (res == -1) return -errno;
    return res;
}

static int maimai_write(const char *path, const char *buf, size_t size,
                       off_t offset, struct fuse_file_info *fi) {
    int res = pwrite(fi->fh, buf, size, offset);
    if (res == -1) return -errno;
    return res;
}

static int maimai_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    char *real_path = build_real_path(path);
    int res = open(real_path, fi->flags, mode);
    free(real_path);
    if (res == -1) return -errno;

    fi->fh = res;
    return 0;
}

static int maimai_unlink(const char *path) {
    if (strncmp(path, "/starter", 8) == 0) {
        return starter_unlink(path + 8);
    } else if (strncmp(path, "/metro", 6) == 0) {
        return metropolis_unlink(path + 6);
    } else if (strncmp(path, "/dragon", 7) == 0) {
        return dragon_unlink(path + 7);
    } else if (strncmp(path, "/blackrose", 10) == 0) {
        return blackrose_unlink(path + 10);
    } else if (strncmp(path, "/heaven", 7) == 0) {
        return heaven_unlink(path + 7);
    } else if (strncmp(path, "/youth", 6) == 0) {
        return youth_unlink(path + 6);
    } else if (strncmp(path, "/7sref", 6) == 0) {
        return prism_unlink(path);
    }

    return -ENOENT;
}

static int maimai_mkdir(const char *path, mode_t mode) {
    char *real_path = build_real_path(path);
    int res = mkdir(real_path, mode);
    free(real_path);
    if (res == -1) return -errno;
    return 0;
}

static int maimai_rmdir(const char *path) {
    char *real_path = build_real_path(path);
    int res = rmdir(real_path);
    free(real_path);
    if (res == -1) return -errno;
    return 0;
}

static int maimai_release(const char *path, struct fuse_file_info *fi) {
    if (fi->fh != 0) {
        close(fi->fh);
        fi->fh = 0;
    }
    return 0;
}

static int maimai_truncate(const char *path, off_t size) {
    char *real_path = build_real_path(path);
    int res = truncate(real_path, size);
    free(real_path);
    if (res == -1) return -errno;
    return 0;
}

static struct fuse_operations maimai_oper = {
    .getattr    = maimai_getattr,
    .readdir    = maimai_readdir,
    .mkdir      = maimai_mkdir,
    .rmdir      = maimai_rmdir,
    .create     = maimai_create,
    .open       = maimai_open,
    .read       = maimai_read,
    .write      = maimai_write,
    .release    = maimai_release,
    .unlink     = maimai_unlink,
    .truncate   = maimai_truncate,
    .chmod      = NULL,
    .chown      = NULL,
    .utimens    = NULL,
    .statfs     = NULL,
};

int main(int argc, char *argv[]) {

    mkdir(BASE_DIR, 0755);
    mkdir(BASE_DIR "/starter", 0755);
    mkdir(BASE_DIR "/metro", 0755);
    mkdir(BASE_DIR "/dragon", 0755);
    mkdir(BASE_DIR "/blackrose", 0755);
    mkdir(BASE_DIR "/heaven", 0755);
    mkdir(BASE_DIR "/youth", 0755);
    mkdir(FUSE_DIR, 0755);

    return fuse_main(argc, argv, &maimai_oper, NULL);
}
