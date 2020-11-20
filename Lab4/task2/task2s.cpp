// Server
#include <iostream>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

using namespace std;

#define SEMKEY 88
#define SHMKEY 97
#define SEMSIZE 2

union semun
{
    int val;
    semid_ds *mbuf;
    ushort *array;
    seminfo *ibuf;
} arg;


int main ()
{   
    // semid_ds mbuf;
    // arg.mbuf = &mbuf;
    // arg.val = 2;
    ushort arr[BUFSIZ];
    arg.array = arr;
    
    sembuf sopS[1] = {{1, 1, 0}};
    sembuf sop1[2] {{0, -1, 0}, {1, 0, 0}};
    sembuf sop2[2] {{0, 0, 0 | IPC_NOWAIT}, {1, 0, 0 | IPC_NOWAIT}};
    sembuf sop3[2] {{0, 1, 0 | IPC_NOWAIT}, {1, 1, 0 | IPC_NOWAIT}};

    shmid_ds sbuf;
    int *p_opd1, *p_opd2;
    char *p_opt;

    int shm_desk;

    if((shm_desk = shmget(SHMKEY, sizeof(int) * 2 + sizeof(char), 0777)) != -1)
    {
        if (shmctl(shm_desk, IPC_RMID, &sbuf) == -1)
        {
            perror("Shmctl fail");
            exit(10);
        }
    }

    if((shm_desk = shmget(SHMKEY, sizeof(int) * 2 + sizeof(char), 0777 | IPC_CREAT)) == -1)
    {
        perror("Shmget fail");
        exit(11);
    }



    int sem_desk;

    if((sem_desk = semget(SEMKEY, SEMSIZE, 0777)) != -1)
    {
        if (semctl(sem_desk, 0, IPC_RMID, 0) == -1)
        {
            perror("Semctl fail");
            exit(1);
        }
    }

    if((sem_desk = semget(SEMKEY, SEMSIZE, IPC_CREAT | 0777)) == -1)
    {
        perror("Semget fail");
        exit(2);
    }

    if (semop(sem_desk, sopS, 1) == -1) 
    {
        perror("Semop fail");
        exit(4);
    }

    while (1)
    {
        if (semop(sem_desk, sop1, 2) == -1) 
        {
            perror("Semop fail");
            exit(5);
        }

        p_opd1 = (int*)shmat(shm_desk, 0, 0);
        if(p_opd1 == ((int*)-1))
        {
            perror("Shmat fail");
            exit(20);
        }
        p_opt = (char*)(shmat(shm_desk, 0, 0) + sizeof(int));
        if(p_opt == ((char*)-1))
        {
            perror("Shmat fail");
            exit(21);
        }
        p_opd2 = (int*)(shmat(shm_desk, 0, 0) + sizeof(char) + sizeof(int));
        if(p_opd2 == ((int*)-1))
        {
            perror("Shmat fail");
            exit(22);
        }

        if(*p_opt == '+') *p_opd1 = *p_opd1 + *p_opd2;
        else if(*p_opt == '*') *p_opd1 = *p_opd1 * *p_opd2;
        else if(*p_opt == '-') *p_opd1 = *p_opd1 - *p_opd2;
        else if(*p_opt == '/') *p_opd1 = *p_opd1 / *p_opd2;
        else if(*p_opt == '%') *p_opd1 = *p_opd1 % *p_opd2;
        else *p_opd1 = -1;

        // if(shmdt((void*)p_opd1) == -1)
        // {
        //     perror("Shmdt fail(1)");
        //     exit(30);
        // }
        // if(shmdt((void*)p_opt) == -1)
        // {
        //     perror("Shmdt fail(2)");
        //     exit(31);
        // }
        // if(shmdt((void*)p_opd2) == -1)
        // {
        //     perror("Shmdt fail(3)");
        //     exit(32);
        // }
        ////

        if (semop(sem_desk, sop2, 2) == -1) 
        {
            perror("Semop fail");
            exit(6);
        }
        if (semop(sem_desk, sop3, 2) == -1) 
        {
            perror("Semop fail");
            exit(7);
        }
        sleep(1);
    }
    
}