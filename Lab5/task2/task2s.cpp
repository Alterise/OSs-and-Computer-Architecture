// Server
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

int vieta_method(cfbuf*, rtbuf*);

int main ()
{   
    int msg_desc;
    if((msg_desc = msgget(MSGKEY, 0777)) != -1)
    {
        if (msgctl(msg_desc, IPC_RMID, 0) == -1)
        {
            perror("Msgctl fail");
            exit(1);
        }
    }

    if((msg_desc = msgget(MSGKEY, IPC_CREAT | 0777)) == -1)
    {
        perror("Msgget fail");
        exit(2);
    }

    pid_t pid = getpid();
    cfbuf coef_buffer = {1, -5, 3, 4};
    rtbuf root_buffer;
    vieta_method(&coef_buffer, &root_buffer);
    cout << root_buffer.ans1 << " " << root_buffer.ans2 << " " << root_buffer.ans3 << endl;

}

double sgn(double x)
{
    if(x < 0 - Eps) return -1;
    if(x > 0 + Eps) return 1;
    return 0;
}

int vieta_method(cfbuf *coefs, rtbuf *roots)
{

    if(0 - Eps < coefs->coef3 && coefs->coef3 < 0 + Eps) return -1;

    double a = coefs->coef2 / coefs->coef3; 
    double b = coefs->coef1 / coefs->coef3; 
    double c = coefs->coef0 / coefs->coef3;
    double a_2 = a * a;
    double a_3 = a * a * a;
    double Q = (a_2 - 3 * b) / 9;
    double Q_3 = Q * Q * Q;
    double R = (2 * a_3 - 9 * a * b + 27 * c) / 54;
    double R_2 = R * R;
    double S = Q_3 - R_2;
    double phi;
    double t;

    if(S >= (0 + Eps))
    {
        t = R / sqrt(Q_3);
        if(t < -1) t = -1;
        else if(t > 1) t = 1;
        phi = acos(t) / 3;
        roots->ans1.real(-2 * sqrt(Q) * cos(phi) - a / 3);
        roots->ans2.real(-2 * sqrt(Q) * cos(phi + 2 * M_PI / 3) - a / 3);
        roots->ans3.real(-2 * sqrt(Q) * cos(phi - 2 * M_PI / 3) - a / 3);
        return 1;
    }
    else if(S <= (0 - Eps))
    {
        if(0 - Eps < Q && Q < 0 + Eps)
        {
            roots->ans1.real(-cbrt(c - a_3 / 27) - a / 3);
            roots->ans2.real((- (a + roots->ans1.real())) / 2);
            roots->ans2.imag((sqrt(fabs((a - 3 * roots->ans1.real()) * (a + roots->ans1.real()) - 4 * b))) / 2);
            roots->ans3.real((- (a + roots->ans1.real())) / 2);
            roots->ans3.imag((sqrt(fabs((a - 3 * roots->ans1.real()) * (a + roots->ans1.real()) - 4 * b))) / 2);
        }
        else if (Q >= 0 + Eps)
        {
            t = fabs(R) / sqrt(Q_3);
            if(t < 1) t = 1;
            phi = acosh(t) / 3;
            roots->ans1.real(-2 * sgn(R) * sqrt(Q) * cosh(phi) - a / 3);
            roots->ans2.real(sgn(R) * sqrt(Q) * cosh(phi) - a / 3);
            roots->ans2.imag(sqrt(3) * sqrt(Q) * sinh(phi));
            roots->ans3.real(sgn(R) * sqrt(Q) * cosh(phi) - a / 3);
            roots->ans3.imag(-sqrt(3) * sqrt(Q) * sinh(phi));
        }
        else
        {
            t = fabs(R) / sqrt(fabs(Q_3));
            if(t < 1) t = 1;
            phi = asinh(t) / 3;
            roots->ans1.real(-2 * sgn(R) * sqrt(fabs(Q)) * sinh(phi) - a / 3);
            roots->ans2.real(sgn(R) * sqrt(fabs(Q)) * sinh(phi) - a / 3);
            roots->ans2.imag(sqrt(3) * sqrt(fabs(Q)) * cosh(phi));
            roots->ans3.real(sgn(R) * sqrt(fabs(Q)) * sinh(phi) - a / 3);
            roots->ans3.imag(-sqrt(3) * sqrt(fabs(Q)) * cosh(phi));
        }
        return 2;
    }
    else
    {
        roots->ans1.real(-2 * sgn(R) * sqrt(Q) - a / 3);
        roots->ans2.real(sgn(R) * sqrt(Q) - a / 3);
        roots->ans3.real(sgn(R) * sqrt(Q) - a / 3);
        return 0;
    }
}