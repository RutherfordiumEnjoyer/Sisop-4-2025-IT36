#define FUSE_USE_VERSION 26
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <ctype.h>

static const char *dirpath = "/it24_host";
static const char *logpath = "/antink-logs/it24.log";

void write_log(const char *action, const char *path, const char *log_type) {
    static const char *skip_operations[] = {"GETATTR", "READDIR", NULL};

    for (int i = 0; skip_operations[i] != NULL; i++) {
        if (strcmp(action, skip_operations[i]) == 0 && log_type == NULL) {
            return;
        }
    }

    FILE *log_file = fopen(logpath, "a");
    if (log_file) {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char timestamp[64];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);

        const char *display_path = path;
        const char *mount_point = "/antink_mount";
        if (strncmp(path, mount_point, strlen(mount_point)) == 0) {
            display_path = path + strlen(mount_point);
            if (display_path[0] == '\0') {
                display_path = "/";
            }
        }

        if (log_type) {
            if (strcmp(log_type, "ALERT") == 0) {
                fprintf(log_file, "[%s] %s %s: %s\n",
                       timestamp, log_type, action, display_path);
            }
            else if (strcmp(log_type, "REVERSE") == 0) {
                fprintf(log_file, "[%s] %s %s\n",
                       timestamp, log_type, action);
            }
            else if (strcmp(log_type, "ENCRYPT") == 0) {
                fprintf(log_file, "[%s] %s %s\n",
                       timestamp, log_type, action);
            }
        } else {
            fprintf(log_file, "[%s] %s: %s\n",
                   timestamp, action, display_path);
        }

        fclose(log_file);
    }
}

int is_dangerous_file(const char *path) {
    const char *filename = strrchr(path, '/');
    if (filename == NULL) filename = path;
    else filename++;

    char filename_lower[256];
    strcpy(filename_lower, filename);
    for (int i = 0; filename_lower[i]; i++) {
        filename_lower[i] = tolower(filename_lower[i]);
    }

    if (strstr(filename_lower, "nafis")) {
        write_log("Anomali Detected nafis in file", filename, "ALERT");
        return 1;
    } else if (strstr(filename_lower, "kimcun")) {
        write_log("Anomali Detected kimcun in file", filename, "ALERT");
        return 1;
    }
    return 0;
}

void get_reversed_name(const char *name, char *reversed) {
    strcpy(reversed, name);
    int len = strlen(reversed);
    for (int i = 0; i < len / 2; i++) {
        char temp = reversed[i];
        reversed[i] = reversed[len - i - 1];
        reversed[len - i - 1] = temp;
    }
}

void get_original_name(const char *reversed, char *original) {
    strcpy(original, reversed);
    int len = strlen(original);
    for (int i = 0; i < len / 2; i++) {
        char temp = original[i];
        original[i] = original[len - i - 1];
        original[len - i - 1] = temp;
    }
}

void rot13_encrypt(char *text) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            char base = islower(text[i]) ? 'a' : 'A';
            text[i] = (text[i] - base + 13) % 26 + base;
        }
    }
}

static void get_path_from_root(char *fullpath, const char *path) {
    strcpy(fullpath, dirpath);
    if (path[0] != '/' && strlen(path) > 0) {
        strcat(fullpath, "/");
    }
    strcat(fullpath, path);
}

static int xmp_getattr(const char *path, struct stat *stbuf) {
    char fullpath[1000];

    const char *filename = strrchr(path, '/');
    if (filename == NULL) filename = path;
    else filename++;

    char original_name[256];
    char modified_path[1000];

    if (strstr(path, "txt.sifan") || strstr(path, "txt.nucmik")) {
        strcpy(modified_path, path);
        char *filename_in_path = strrchr(modified_path, '/');
        if (filename_in_path == NULL) filename_in_path = modified_path;
        else filename_in_path++;

        get_original_name(filename_in_path, original_name);
        if (filename_in_path == modified_path) {
            strcpy(modified_path, original_name);
        } else {
            *(filename_in_path - 1) = '\0';
            strcat(modified_path, "/");
            strcat(modified_path, original_name);
        }

        get_path_from_root(fullpath, modified_path + 1);
    } else {
        get_path_from_root(fullpath, path);
    }

    int res = lstat(fullpath, stbuf);
    if (res == -1)
        return -errno;

    write_log("GETATTR", path, NULL);
    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                      off_t offset, struct fuse_file_info *fi) {
    char fullpath[1000];
    get_path_from_root(fullpath, path);

    DIR *dp = opendir(fullpath);
    if (dp == NULL)
        return -errno;

    struct dirent *de;
    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        char display_name[256];
        strcpy(display_name, de->d_name);

        if (de->d_type == DT_REG) {
            if (is_dangerous_file(de->d_name)) {
                char original_name[256];
                strcpy(original_name, de->d_name);
                get_reversed_name(de->d_name, display_name);

                char log_message[512];
                snprintf(log_message, sizeof(log_message),
                        "file %s has been reversed to %s",
                        original_name, display_name);
                write_log(log_message, path, "REVERSE");
            }
        }

        if (filler(buf, display_name, &st, 0))
            break;
    }

    closedir(dp);
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
                    struct fuse_file_info *fi) {
    char fullpath[1000];

    if (strstr(path, "txt.sifan") || strstr(path, "txt.nucmik")) {
        char modified_path[1000];
        strcpy(modified_path, path);

        char *filename = strrchr(modified_path, '/');
        if (filename == NULL) filename = modified_path;
        else filename++;

        char original_name[256];
        get_original_name(filename, original_name);

        if (filename == modified_path) {
            strcpy(modified_path, original_name);
        } else {
            *(filename - 1) = '\0';
            strcat(modified_path, "/");
            strcat(modified_path, original_name);
        }

        get_path_from_root(fullpath, modified_path + 1);
    } else {
        get_path_from_root(fullpath, path);
    }

    int fd = open(fullpath, O_RDONLY);
    if (fd == -1)
        return -errno;

    write_log("READ", path, NULL);

    int res = pread(fd, buf, size, offset);
    if (res == -1)
        res = -errno;

    if (res > 0) {
        int is_dangerous = 0;

        if (strstr(path, "txt.sifan") || strstr(path, "txt.nucmik")) {
            is_dangerous = 1;
        } else {
            is_dangerous = is_dangerous_file(path);
        }

        const char *ext = strrchr(fullpath, '.');
        if (ext != NULL && strcmp(ext, ".txt") == 0 && !is_dangerous) {
            char *temp_buf = malloc(res + 1);
            memcpy(temp_buf, buf, res);
            temp_buf[res] = '\0';
            rot13_encrypt(temp_buf);
            memcpy(buf, temp_buf, res);
            free(temp_buf);

            const char *filename = strrchr(path, '/');
            if (filename == NULL) filename = path;
            else filename++;

            char log_message[512];
            snprintf(log_message, sizeof(log_message), "file %s has been encrypted", filename);
            write_log(log_message, path, "ENCRYPT");
        }
    }

    close(fd);
    return res;
}

static int xmp_write(const char *path, const char *buf, size_t size,
                     off_t offset, struct fuse_file_info *fi) {
    char fullpath[1000];

    if (strstr(path, "txt.sifan") || strstr(path, "txt.nucmik")) {
        char modified_path[1000];
        strcpy(modified_path, path);

        char *filename = strrchr(modified_path, '/');
        if (filename == NULL) filename = modified_path;
        else filename++;

        char original_name[256];
        get_original_name(filename, original_name);

        if (filename == modified_path) {
            strcpy(modified_path, original_name);
        } else {
            *(filename - 1) = '\0';
            strcat(modified_path, "/");
            strcat(modified_path, original_name);
        }

        get_path_from_root(fullpath, modified_path + 1);
    } else {
        get_path_from_root(fullpath, path);
    }

    int fd = open(fullpath, O_WRONLY);
    if (fd == -1)
        return -errno;

    write_log("WRITE", path, NULL);

    int res = pwrite(fd, buf, size, offset);
    if (res == -1)
        res = -errno;

    close(fd);
    return res;
}

static int xmp_open(const char *path, struct fuse_file_info *fi) {
    char fullpath[1000];

    if (strstr(path, "txt.sifan") || strstr(path, "txt.nucmik")) {
        char modified_path[1000];
        strcpy(modified_path, path);

        char *filename = strrchr(modified_path, '/');
        if (filename == NULL) filename = modified_path;
        else filename++;

        char original_name[256];
        get_original_name(filename, original_name);

        if (filename == modified_path) {
            strcpy(modified_path, original_name);
        } else {
            *(filename - 1) = '\0';
            strcat(modified_path, "/");
            strcat(modified_path, original_name);
        }

        get_path_from_root(fullpath, modified_path + 1);
    } else {
        get_path_from_root(fullpath, path);
    }

    int res = open(fullpath, fi->flags);
    if (res == -1)
        return -errno;

    write_log("OPEN", path, NULL);

    close(res);
    return 0;
}

static int xmp_access(const char *path, int mask) {
    char fullpath[1000];

    if (strstr(path, "txt.sifan") || strstr(path, "txt.nucmik")) {
        char modified_path[1000];
        strcpy(modified_path, path);

        char *filename = strrchr(modified_path, '/');
        if (filename == NULL) filename = modified_path;
        else filename++;

        char original_name[256];
        get_original_name(filename, original_name);

        if (filename == modified_path) {
            strcpy(modified_path, original_name);
        } else {
            *(filename - 1) = '\0';
            strcat(modified_path, "/");
            strcat(modified_path, original_name);
        }

        get_path_from_root(fullpath, modified_path + 1);
    } else {
        get_path_from_root(fullpath, path);
    }

    int res = access(fullpath, mask);
    if (res == -1)
        return -errno;

    write_log("ACCESS", path, NULL);

    return 0;
}

static int xmp_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    char fullpath[1000];
    get_path_from_root(fullpath, path);

    int res = creat(fullpath, mode);
    if (res == -1)
        return -errno;

    write_log("CREATE", path, NULL);

    close(res);
    return 0;
}

static int xmp_mkdir(const char *path, mode_t mode) {
    char fullpath[1000];
    get_path_from_root(fullpath, path);

    int res = mkdir(fullpath, mode);
    if (res == -1)
        return -errno;

    write_log("MKDIR", path, NULL);

    return 0;
}

static int xmp_rmdir(const char *path) {
    char fullpath[1000];
    get_path_from_root(fullpath, path);

    int res = rmdir(fullpath);
    if (res == -1)
        return -errno;

    write_log("RMDIR", path, NULL);

    return 0;
}

static int xmp_unlink(const char *path) {
    char fullpath[1000];
    get_path_from_root(fullpath, path);

    int res = unlink(fullpath);
    if (res == -1)
        return -errno;

    write_log("UNLINK", path, NULL);

    return 0;
}

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

int main(int argc, char *argv[]) {
    mkdir("/antink-logs", 0755);

    FILE *log_file = fopen(logpath, "w");
    if (log_file) {
        fprintf(log_file, "=== AntiNK System Started ===\n");
        fclose(log_file);
    }

    return fuse_main(argc, argv, &xmp_oper, NULL);
}
