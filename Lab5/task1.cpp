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

double f(double x)
{
    return 4/(1 + x*x);
}

double integral_calc(double lb, double rb, int quantity, double func(double))
{
    double sum = 0;
    double step = (rb - lb) / (double) quantity;
    double i = lb + step / 2; 

    for(int i = 0; i < quantity; i++)
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

    int     fifo[2];
    if(pipe(fifo) == -1)
    {
        perror("Pipe error");
        exit(2);
    }

    pid_t   pid;
    string  arg1 = argv[1];
    string  arg2 = argv[2];
    string  tmp;
    char    buf[BUFSIZ];
    int     quantity_of_processes = atoi(arg1.c_str());
    int     intervals_per_process = atoi(arg2.c_str()) / quantity_of_processes;
    int     precission = arg2.length();
    double  lb = 0;
    double  rb = 1;
    double  step = (rb - lb) / (double)quantity_of_processes;
    double  sum = 0;

    for(int i = 0; i < quantity_of_processes; i++)
    {
        pid = fork();
        if(pid < 0)
        {
            perror("Fork error");
            exit(3);
        }
        else if(!pid)
        {
            tmp = to_string(integral_calc((double)i * step, (double)(i + 1) * step - 1 / pow(10, precission), intervals_per_process, f));
            close(fifo[0]);
            write(fifo[1], tmp.c_str(), BUFSIZ);
            close(fifo[1]);
            exit(1);
        }
        else
        {
            waitpid(pid, 0, 0);
            read(fifo[0], buf, BUFSIZ);
            sum += atof(buf);
        }
    }
    sum /= (double) quantity_of_processes;
    cout << "Calculated: " << setprecision(precission) << sum << endl;
    cout << "Known:      " << setprecision(precission) << M_PI << endl;
}