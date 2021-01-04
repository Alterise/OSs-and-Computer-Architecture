#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <unistd.h>

using namespace std;

long double f(long double x)
{
    return 4.0/(1.0 + x*x);
}

long double integral_calc(long double lb, long double rb, long double quantity, long double func(long double))
{
    long double sum = 0.0;
    long double step = (rb - lb) / quantity;

    for(long double i = 0.0; i < quantity; i++)
    {
        sum += func(lb + i * step);
    }
    return sum / quantity;
}

int main (int argc, char **argv)
{
    if(argc != 3)
    {
        perror("Console arguments error");
        exit(1);
    }

    int fifo[2];
    if(pipe(fifo) == -1)
    {
        perror("Pipe error");
        exit(2);
    }

    pid_t           pid;
    string          arg1 = argv[1];
    string          arg2 = argv[2];
    long double     tmp;
    long double     quantity_of_processes = atoi(arg1.c_str());
    long double     intervals_per_process = atoi(arg2.c_str()) / quantity_of_processes;
    long double     precission = arg2.length();
    long double     lb = 0;
    long double     rb = 1;
    long double     step = (rb - lb) / quantity_of_processes;
    long double     sum = 0;

    for(long double i = 0; i < quantity_of_processes; i++)
    {
        pid = fork();
        if(pid < 0)
        {
            perror("Fork error");
            exit(3);
        }
        else if(!pid)
        {
            tmp = integral_calc(i * step, (i + 1) * step - 1 / pow(10, precission), intervals_per_process, f);
            close(fifo[0]);
            write(fifo[1], &tmp, sizeof(long double));
            close(fifo[1]);
            exit(1);
        }
        else
        {
            read(fifo[0], &tmp, sizeof(long double));
            sum += tmp;
        }
    }
    sum /=  quantity_of_processes;
    sleep(1);
    cout << "Calculated: " << setprecision(precission) << sum << endl;
    cout << "Known:      " << setprecision(precission) << M_PI << endl;
}