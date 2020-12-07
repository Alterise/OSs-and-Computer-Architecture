#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string>
#include <vector>

using namespace std;

void recursive_fork(int count)
{
    if( count <= 0) return;
    pid_t pid;
    vector<int> pids;
    for(int i = 0; i < count; i++)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("Fork Error\n");
            exit(1);
        }
        else if(pid == 0)
        {
            sleep(1);
            FILE *fout = fopen("tmp.txt", "a");
            fprintf(fout, "\n");
            fclose(fout);
            recursive_fork(count - 1);
            sleep(10 + count);
            return;
        }
        else 
        {
            pids.push_back(pid);
        }
    }
    cout << getpid() << ": ";
    for (size_t i = 0; i < count; i++)
    {
        cout << pids[i] << " ";
    }
    cout << endl;
}

int main (int argc, char** argv)
{   
    FILE *fin;
    if(!(fin = fopen("tmp.txt", "w")))
    {
        perror("File opening error");
        exit(1);
    }
    fclose(fin);
    if(argc == 1)
    {
        perror("No input\n");
        exit(-1);
    }
    else if(argc > 2)
    {        
        perror("Wrong input\n");
        exit(-2);
    }

    pid_t startpid = getpid();
    recursive_fork(5);
    if (getpid() == startpid) 
    {
        sleep(6);
        FILE *fin;
        if(!(fin = fopen("tmp.txt", "r")))
        {
            perror("File opening error");
            exit(1);
        }
        char tmp;
        int count = 0;
        while(fgetc(fin) != EOF) count++;
        fclose(fin);
        cout << endl << "Processes executed: " << count << endl;
        string command = "pstree -p > ";
        command+= argv[1];
        system(command.c_str());
        remove("tmp.txt");
    }
}