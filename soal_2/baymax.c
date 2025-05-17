#define FUSE_USE_VERSION 31
#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#define MAX_PATH_LEN 4096
#define CHUNK_SIZE 1024  // 1KB chunks
#define LOG_FILE "activity.log"
#define MAX_CHUNKS 1000

static char *relics_dir = "relics";

static int make_chunk_path(char *dest, size_t size, const char *base, int num) {
    if (num < 0 || num >= MAX_CHUNKS) return -1;
    return snprintf(dest, size, "%s.%03d", base, num) < (int)size ? 0 : -1;
}

void log_activity(const char *action, const char *details) {
    time_t now = time(NULL);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

    FILE *log = fopen(LOG_FILE, "a");
    if (log) {
        fprintf(log, "[%s] %s: %s\n", timestamp, action, details);
        fclose(log);
    }
}

static int baymax_getattr(const char *path, struct stat *stbuf) {
    memset(stbuf, 0, sizeof(struct stat));

    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        return 0;
    }

    char full_path[MAX_PATH_LEN];
    snprintf(full_path, sizeof(full_path), "%s/%s", relics_dir, path + 1);

    int chunk_count = 0;
    off_t total_size = 0;

    for (int i = 0; i < MAX_CHUNKS; i++) {
        char chunk_path[MAX_PATH_LEN];
        if (make_chunk_path(chunk_path, sizeof(chunk_path), full_path, i) != 0) break;

        struct stat chunk_stat;
        if (stat(chunk_path, &chunk_stat) == -1) break;

        chunk_count++;
        if (i == chunk_count - 1) {
            total_size += chunk_stat.st_size;
        } else {
            total_size += CHUNK_SIZE;
        }
    }

    if (chunk_count > 0) {
        stbuf->st_mode = S_IFREG | 0644;
        stbuf->st_nlink = 1;
        stbuf->st_size = total_size;
        return 0;
    }

    return -ENOENT;
}

static int baymax_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi) {
    (void) offset; (void) fi;

    if (strcmp(path, "/") != 0) return -ENOENT;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    DIR *dp = opendir(relics_dir);
    if (!dp) return -errno;

    struct dirent *de;
    while ((de = readdir(dp)) != NULL) {
        if (de->d_type == DT_REG) {
            char *dot = strrchr(de->d_name, '.');
            if (dot && !strcmp(dot, ".000")) {
                char name[MAX_PATH_LEN];
                strncpy(name, de->d_name, dot - de->d_name);
                name[dot - de->d_name] = '\0';
                filler(buf, name, NULL, 0);
            }
        }
    }

    closedir(dp);
    return 0;
}

static int baymax_open(const char *path, struct fuse_file_info *fi) {
    char full_path[MAX_PATH_LEN];
    snprintf(full_path, sizeof(full_path), "%s/%s.000", relics_dir, path + 1);

    if (access(full_path, F_OK) == -1) return -ENOENT;

    log_activity("OPEN", path + 1);
    return 0;
}

static int baymax_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi) {
    char base_path[MAX_PATH_LEN];
    snprintf(base_path, sizeof(base_path), "%s/%s", relics_dir, path + 1);

    size_t total = 0;
    int chunk_num = offset / CHUNK_SIZE;
    size_t chunk_off = offset % CHUNK_SIZE;

    while (size > 0) {
        char chunk_path[MAX_PATH_LEN];
        if (make_chunk_path(chunk_path, sizeof(chunk_path), base_path, chunk_num) != 0) break;

        FILE *chunk = fopen(chunk_path, "rb");
        if (!chunk) break;

        fseek(chunk, chunk_off, SEEK_SET);
        size_t to_read = CHUNK_SIZE - chunk_off;
        if (to_read > size) to_read = size;

        size_t read = fread(buf + total, 1, to_read, chunk);
        fclose(chunk);

        if (read == 0) break;

        total += read;
        size -= read;
        chunk_num++;
        chunk_off = 0;
    }

    return total;
}

static int baymax_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    char base_path[MAX_PATH_LEN];
    snprintf(base_path, sizeof(base_path), "%s/%s", relics_dir, path + 1);

    char chunk_path[MAX_PATH_LEN];
    if (make_chunk_path(chunk_path, sizeof(chunk_path), base_path, 0) != 0) {
        return -ENAMETOOLONG;
    }

    FILE *chunk = fopen(chunk_path, "wb");
    if (!chunk) return -errno;
    fclose(chunk);

    log_activity("CREATE", path + 1);
    return 0;
}

static int baymax_mknod(const char *path, mode_t mode, dev_t rdev) {
    (void)mode; (void)rdev;
    return baymax_create(path, mode, NULL);
}

static int baymax_write(const char *path, const char *buf, size_t size, off_t offset,
                       struct fuse_file_info *fi) {
    char base_path[MAX_PATH_LEN];
    snprintf(base_path, sizeof(base_path), "%s/%s", relics_dir, path + 1);

    int chunk_num = offset / CHUNK_SIZE;
    size_t chunk_off = offset % CHUNK_SIZE;
    size_t written = 0;

    while (size > 0) {
        char chunk_path[MAX_PATH_LEN];
        if (make_chunk_path(chunk_path, sizeof(chunk_path), base_path, chunk_num) != 0) {
            return -ENAMETOOLONG;
        }

        FILE *chunk = fopen(chunk_path, chunk_num == 0 && chunk_off == 0 ? "wb" : "r+b");
        if (!chunk) {
            chunk = fopen(chunk_path, "wb");
            if (!chunk) return -errno;
        }

        if (chunk_off > 0) {
            fseek(chunk, chunk_off, SEEK_SET);
        }

        size_t to_write = CHUNK_SIZE - chunk_off;
        if (to_write > size) to_write = size;

        size_t res = fwrite(buf + written, 1, to_write, chunk);
        fclose(chunk);

        if (res != to_write) return -errno;

        written += res;
        size -= res;
        chunk_num++;
        chunk_off = 0;
    }

    char log_msg[256];
    snprintf(log_msg, sizeof(log_msg), "%s (%d chunks)", path + 1, chunk_num);
    log_activity("WRITE", log_msg);

    return written;
}

static int baymax_truncate(const char *path, off_t size) {
    char base_path[MAX_PATH_LEN];
    snprintf(base_path, sizeof(base_path), "%s/%s", relics_dir, path + 1);

    int needed_chunks = (size + CHUNK_SIZE - 1) / CHUNK_SIZE;

    for (int i = needed_chunks; i < MAX_CHUNKS; i++) {
        char chunk_path[MAX_PATH_LEN];
        if (make_chunk_path(chunk_path, sizeof(chunk_path), base_path, i) != 0) break;

        if (unlink(chunk_path) == -1) {
            if (errno == ENOENT) break;
            return -errno;
        }
    }

    if (size > 0) {
        int last_chunk = needed_chunks - 1;
        char chunk_path[MAX_PATH_LEN];
        if (make_chunk_path(chunk_path, sizeof(chunk_path), base_path, last_chunk) != 0) {
            return -ENAMETOOLONG;
        }

        FILE *chunk = fopen(chunk_path, "r+b");
        if (!chunk) {
            chunk = fopen(chunk_path, "wb");
            if (!chunk) return -errno;
        }

        off_t last_chunk_size = size % CHUNK_SIZE;
        if (last_chunk_size == 0) last_chunk_size = CHUNK_SIZE;

        if (ftruncate(fileno(chunk), last_chunk_size) == -1) {
            fclose(chunk);
            return -errno;
        }

        fclose(chunk);
    }

    log_activity("TRUNCATE", path + 1);
    return 0;
}

static int baymax_ftruncate(const char *path, off_t size, struct fuse_file_info *fi) {
    return baymax_truncate(path, size);
}

static int baymax_unlink(const char *path) {
    char base_path[MAX_PATH_LEN];
    snprintf(base_path, sizeof(base_path), "%s/%s", relics_dir, path + 1);

    int count = 0;
    for (int i = 0; i < MAX_CHUNKS; i++) {
        char chunk_path[MAX_PATH_LEN];
        if (make_chunk_path(chunk_path, sizeof(chunk_path), base_path, i) != 0) break;

        if (unlink(chunk_path) == -1) {
            if (errno == ENOENT) break;
            return -errno;
        }
        count++;
    }

    if (count > 0) {
        char log_msg[256];
        snprintf(log_msg, sizeof(log_msg), "%s (%d chunks)", path + 1, count);
        log_activity("DELETE", log_msg);
    }

    return count > 0 ? 0 : -ENOENT;
}

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

int main(int argc, char *argv[]) {
    umask(0);
    return fuse_main(argc, argv, &baymax_oper, NULL);
}
