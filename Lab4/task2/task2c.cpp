// Client
#include <iostream>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string>

using namespace std;

#define SEMKEY 88
#define SHMKEY1 95
#define SHMKEY2 96
#define SHMKEY3 97
#define SEMSIZE 2

union semun
{
    int val;
    semid_ds *mbuf;
    ushort *array;
    seminfo *ibuf;
} arg;

bool is_double(string str)
{
    char *err_checker = nullptr;
    double value = strtod(str.c_str(), &err_checker);
    if(err_checker == str.c_str() || *err_checker != '\0') return 0;
    return 1;
}

bool is_operation(string str)
{
    if(str.length() != 1) return 0;
    if(str[0] != '+' && str[0] != '-' && str[0] != '/' && str[0] != '*') return 0;
    return 1;
}


int main ()
{   
    // semid_ds mbuf;
    // arg.mbuf = &mbuf;
    // arg.val = 2;
    ushort arr[BUFSIZ];
    arg.array = arr;
    
    
    sembuf sop1[2] {{0, 0, 0}, {1, -1, 0}};
    sembuf sop2[2] {{0, 1, 0 | IPC_NOWAIT}, {1, 0, 0 | IPC_NOWAIT}};
    sembuf sop3[2] {{0, -1, 0}, {1, -1, 0}};
    sembuf sop4[2] {{0, 0, 0 | IPC_NOWAIT}, {1, 1, 0 | IPC_NOWAIT}};

    shmid_ds sbuf;
    double *p_opd1, *p_opd2;
    char *p_opt, *err_checker;
    string tmpd1, tmpd2, tmpc;

    int shm_desk1, shm_desk2, shm_desk3, passed;

    if((shm_desk1 = shmget(SHMKEY1, sizeof(double), 0777 | IPC_CREAT)) == -1)
    {
        perror("Shmget fail");
        exit(20);
    }

    if((shm_desk2 = shmget(SHMKEY2, sizeof(char), 0777 | IPC_CREAT)) == -1)
    {
        perror("Shmget fail");
        exit(21);
    }

    if((shm_desk3 = shmget(SHMKEY3, sizeof(double), 0777 | IPC_CREAT)) == -1)
    {
        perror("Shmget fail");
        exit(22);
    }

    int sem_desk;

    if((sem_desk = semget(SEMKEY, SEMSIZE, 0777)) == -1)
    {
        perror("Semget fail");
        exit(1);
    }

    if (semop(sem_desk, sop1, 2) == -1) 
    {
        perror("Semop fail");
        exit(5);
    }

    if (shmctl(shm_desk1, SHM_UNLOCK, &sbuf) == -1)
    {
        perror("Shmctl fail");
        exit(50);
    }
    
    if (shmctl(shm_desk2, SHM_UNLOCK, &sbuf) == -1)
    {
        perror("Shmctl fail");
        exit(51);
    }

    if (shmctl(shm_desk3, SHM_UNLOCK, &sbuf) == -1)
    {
        perror("Shmctl fail");
        exit(52);
    }

    p_opd1 = (double*)shmat(shm_desk1, 0, 0);
    if(p_opd1 == ((double*)-1))
    {
        perror("Shmat fail");
        exit(30);
    }
    p_opt = (char*)(shmat(shm_desk2, 0, 0));
    if(p_opt == ((char*)-1))
    {
        perror("Shmat fail");
        exit(31);
    }
    p_opd2 = (double*)(shmat(shm_desk3, 0, 0));
    if(p_opd2 == ((double*)-1))
    {
        perror("Shmat fail");
        exit(32);
    }

    passed = 0;
    while(!passed)
    {
        cout << "Input expression (Example: 1 + 2):" << endl;
        cin >> tmpd1 >> tmpc >> tmpd2;
        if(is_double(tmpd1) && is_operation(tmpc) && is_double(tmpd2)) passed = 1;
        if(!passed) cout << "Wrong input, try again" << endl;
    }
    *p_opd1 = stod(tmpd1); 
    *p_opt = tmpc[0];
    *p_opd2 = stod(tmpd2);

    if(shmdt(p_opd1) == -1)
    {
        perror("Shmdt fail(1)");
        exit(40);
    }
    if(shmdt(p_opt) == -1)
    {
        perror("Shmdt fail(2)");
        exit(41);
    }
    if(shmdt(p_opd2) == -1)
    {
        perror("Shmdt fail(3)");
        exit(42);
    }

    if (shmctl(shm_desk1, SHM_LOCK, &sbuf) == -1)
    {
        perror("Shmctl fail");
        exit(53);
    }
    
    if (shmctl(shm_desk2, SHM_LOCK, &sbuf) == -1)
    {
        perror("Shmctl fail");
        exit(54);
    }

    if (shmctl(shm_desk3, SHM_LOCK, &sbuf) == -1)
    {
        perror("Shmctl fail");
        exit(55);
    }

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

    if (shmctl(shm_desk1, SHM_UNLOCK, &sbuf) == -1)
    {
        perror("Shmctl fail");
        exit(56);
    }

    p_opd1 = (double*)shmat(shm_desk1, 0, 0);
    if(p_opd1 == ((double*)-1))
    {
        perror("Shmat fail");
        exit(30);
    }

    cout << *p_opd1 << endl;

    if(shmdt(p_opd1) == -1)
    {
        perror("Shmdt fail(1)");
        exit(40);
    }
    
    if (shmctl(shm_desk1, SHM_LOCK, &sbuf) == -1)
    {
        perror("Shmctl fail");
        exit(57);
    }

    if (semop(sem_desk, sop4, 2) == -1) 
    {
        perror("Semop fail");
        exit(8);
    }
}