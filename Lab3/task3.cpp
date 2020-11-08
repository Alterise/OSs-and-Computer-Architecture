#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <vector>
using namespace std;

int factor(int num)
{
    if(num <= 0) return 1;
    return num * factor(num - 1); 
}

int main (int argc, char** argv)
{   
    if(argc == 1)
    {
        perror("No input\n");
        exit(-1);
    }
    else if(argc > 6)
    {        
        perror("Wrong input\n");
        exit(-2);
    }

    int count = 1;
    bool out_in_file = 0, need_cleaning = 0;
    FILE *fout, *ftmp;
    string tmp, tmp2, filename, fntmp;
    
    for (size_t i = 1; i < argc; i++)
    {
        tmp = argv[i];

        if(tmp == "-n")
        {
            if(i + 1 <= argc)
            {
                tmp2 = argv[i + 1];
                if (stoi(tmp2) > 0) count = stoi(tmp2);
                else
                {
                    perror("Wrong count\n");
                    exit(-5);
                }
            }
            else
            {
                perror("Wrong input\n");
                exit(-3);
            }
        }
        else if (tmp == "-o")
        {
            if(i + 1 <= argc)
            {
                filename = argv[i + 1];
                out_in_file = 1;
            }
            else
            {
                perror("Wrong input\n");
                exit(-4);
            }
        }
        else if (tmp =="-c")
        {
            need_cleaning = 1;
        }
    }
    
    if (out_in_file)
    {
        
        if(!(fout = fopen(filename.c_str(), "w")))
        {
            perror("Fileopening error\n");
            exit(-4);
        }
    }
    
    vector<string> files;
    pid_t pid;
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
            fntmp = "outfile.";
            fntmp += to_string(getpid());
            fntmp += ".txt";
            if((ftmp = fopen(fntmp.c_str(), "w")))
            {
                int factorial = factor(abs(getpid() - getppid()));
                fprintf(ftmp, "Терексей Алешков Чебурекович\n");
                fprintf(ftmp, "%d\n", factorial);
                fclose(ftmp);
            }
            return 0;
        }
        else
        {
            fntmp = "outfile.";
            fntmp += to_string(pid);
            fntmp += ".txt";
            files.push_back(fntmp);
        }
    }
    sleep(3);
    

    if (out_in_file)
    {
        while(files.size() > 0)
        {
            char buff[BUFSIZ];
            fntmp = files.back();
            files.pop_back();
            if((ftmp = fopen(fntmp.c_str(), "r")))
            {
                while (fgets(buff, BUFSIZ, ftmp))
                {
                    fputs(buff, fout);
                }
                fclose(ftmp);
                if(need_cleaning) remove(fntmp.c_str());
            }
        }
        fclose(fout);
    }
    else
    {
        while(files.size() > 0)
        {
            char buff[BUFSIZ];
            fntmp = files.back();
            files.pop_back();
            if((ftmp = fopen(fntmp.c_str(), "r")))
            {
                while (fgets(buff, BUFSIZ, ftmp))
                {
                    printf("%s", buff);
                }
                fclose(ftmp);
                if(need_cleaning) remove(fntmp.c_str());
            }
        }
    }
}