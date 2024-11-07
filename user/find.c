#include "kernel/fcntl.h"
#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

char* fmtname(char* path)
{
    static char buf[DIRSIZ + 1];
    char* p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    // memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    // 这里是为了消除dir末尾的空格
    // 这里有点坑的是fmtname得到的name最后有DIRSIZ-strlen(p)个空格，导致我一直卡在open了
    // 最后debug发现还有空格
    buf[strlen(p)] = '\0'; // Null terminate the string
    return buf;
}

void find(char* path, char* file)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) {
    case T_DEVICE:
    case T_FILE:
        if (strcmp(fmtname(path), file) == 0) {
            printf("%s\n", path);
        }
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0)
                continue;
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
                continue;
            } 
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0) {
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            // printf("dir: %s()\n", dir);
            find(buf, file);
        }
        break;
    }
    close(fd);
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Usage: find path file\n");
    }
    find(argv[1], argv[2]);
    exit(0);
}