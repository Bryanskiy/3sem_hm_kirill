#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void apply(int semid, int n, size_t id) {
    struct sembuf mybuf;
    mybuf.sem_op = n;
    mybuf.sem_flg = 0;
    mybuf.sem_num = id;

    if (semop(semid, &mybuf, 1) < 0)
    {
        fprintf(stderr, "Can\'t wait for condition\n");
        exit(-1);
    }
}

int main() {
    FILE* log = fopen("log.txt", "a");

    int semid;
    char pathname[] = "log.txt";

    key_t key;

    if ((key = ftok(pathname, 0)) < 0)
    {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((semid = semget(key, 2, 0666 | IPC_CREAT)) < 0)
    {
        printf("Can\'t get semid\n");
        exit(-1);
    }

    for(size_t i = 0; i < 1000; ++i) {
        sleep(2);
        apply(semid, -1, 1);
        fprintf(log, "[B] process\n");
        apply(semid, 1, 0);
    }
}