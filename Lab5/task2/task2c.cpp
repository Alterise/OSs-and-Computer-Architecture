// Client
#include <iostream>
#include <complex>
#include <vector>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <cmath>

using namespace std;

#define MSGKEY 44
#define MSGTYPE 88
const double Eps = 0.00000000001;

typedef struct
{
    double coef3;
    double coef2;
    double coef1;
    double coef0;
} cfbuf;

typedef struct
{
    complex<double> ans1;
    complex<double> ans2;
    complex<double> ans3;
} rtbuf;

typedef struct
{
    long mtype;
    long request_id;
    cfbuf coef_buffer;
    rtbuf root_buffer;
} mymsgbuf;

bool is_double(string);

int main ()
{   
    int msg_desc;
    if((msg_desc = msgget(MSGKEY, 0777)) == -1)
    {
        perror("Msgget fail");
        exit(1);
    }

    mymsgbuf mbuf;
    int passed;
    string coef3, coef2, coef1, coef0;

    passed = 0;
    while(!passed)
    {
        cout << "Input coefficients of cubic equation (Example: 1 2 3 4):" << endl;
        cin >> coef3 >> coef2 >> coef1 >> coef0;
        if(is_double(coef3) && is_double(coef2) && is_double(coef1) && is_double(coef0))
        {
            mbuf.coef_buffer.coef3 = strtod(coef3.c_str(), 0);
            mbuf.coef_buffer.coef2 = strtod(coef2.c_str(), 0);
            mbuf.coef_buffer.coef1 = strtod(coef1.c_str(), 0);
            mbuf.coef_buffer.coef0 = strtod(coef0.c_str(), 0);
            if(mbuf.coef_buffer.coef3 < 0 - Eps || mbuf.coef_buffer.coef3 > 0 + Eps)
            {
                passed = 1;
            }
            else
            {
                cout << "First coefficient can't be ZERO" << endl;
            }
        }
        else
        {
            cout << "Wrong input. Try again" << endl;
        }
        
    }
    mbuf.mtype = MSGTYPE;
    mbuf.request_id = getpid();
    msgsnd(msg_desc, &mbuf, sizeof(mbuf), 0);
    msgrcv(msg_desc, &mbuf, sizeof(mbuf), getpid(), MSG_NOERROR);
    cout << "Root №1: real part: "  << mbuf.root_buffer.ans1.real() << " , imaginary part: " << mbuf.root_buffer.ans1.imag() << endl;
    cout << "Root №2: real part: "  << mbuf.root_buffer.ans2.real() << " , imaginary part: " << mbuf.root_buffer.ans2.imag() << endl;
    cout << "Root №3: real part: "  << mbuf.root_buffer.ans3.real() << " , imaginary part: " << mbuf.root_buffer.ans3.imag() << endl;
}

bool is_double(string str)
{
    char *err_checker = nullptr;
    double value = strtod(str.c_str(), &err_checker);
    if(err_checker == str.c_str() || *err_checker != '\0') return 0;
    return 1;
}