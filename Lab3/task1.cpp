#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <string>
using namespace std;

int main (int argc, char** argv)
{
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
    pid_t pid;
    int count = stoi(argv[1]);
    for(int i = 0; i < count; i++)
    {
        pid = fork();
        if(pid == -1)
        {
            perror("Fork Error\n");
            exit(1);
        }
        else if(pid == 0)
        {
            cout << "Терексей Алешков Чебурекович\n" << "Process PID: " << getpid() << endl;
            return 0;
        }
    }
}