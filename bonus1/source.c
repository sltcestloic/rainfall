int main(int argc, char *argv[]) {
    int count;
    char buffer[40];

    count = atoi(argv[1]);

    if (count < 10) {
        memcpy(buffer, argv[2], count *  4);
        if (count == 0x574f4c46) {
            execl("/bin/sh", "sh", NULL);
        }
        return 0;
    }
    else {
        return 1;
    }
}