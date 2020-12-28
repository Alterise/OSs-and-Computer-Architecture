// Server
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>

#define IP_ADDR "127.0.0.1"
#define PORT1 1337
#define PORT2 1338
#define PORT3 1339
#define PORT4 1340
#define MAX_CONNECTIONS 1

using namespace std;

int main() {
    int         s1_sid, s2_sid, c1_sid, c2_sid;
    socklen_t   c1_size, c2_size;
    sockaddr_in s1_addr, s2_addr, c1_addr, c2_addr;

    s1_addr.sin_family = AF_INET;
	s1_addr.sin_port = htons(PORT1);
	inet_aton(IP_ADDR, &(s1_addr.sin_addr));

    s2_addr.sin_family = AF_INET;
	s2_addr.sin_port = htons(PORT3);
	inet_aton(IP_ADDR, &(s2_addr.sin_addr));

    if((s1_sid = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creating error");
        exit(-1);
    }

    if((s2_sid = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creating error");
        exit(-1);
    }

    if(bind(s1_sid, (sockaddr*)&s1_addr, sizeof(s1_addr)) == -1)
    {
        cout << "Can't bind(1) to main port:" << PORT1 << ". Trying to bind to reserve port:" << PORT2 << "." << endl;
	    s1_addr.sin_port = htons(PORT2);
        if(bind(s1_sid, (sockaddr*)&s1_addr, sizeof(s1_addr)) != -1)
        {   
            cout << "Successful binding." << endl;
        }
        else
        {
            cout << "Unsuccessful binding. Try again in a few minutes." << endl;
            exit(-2);
        }
    }

    if(bind(s2_sid, (sockaddr*)&s2_addr, sizeof(s2_addr)) == -1)
    {
        cout << "Can't bind(2) to main port:" << PORT3 << ". Trying to bind to reserve port:" << PORT4 << "." << endl;
	    s2_addr.sin_port = htons(PORT4);
        if(bind(s2_sid, (sockaddr*)&s2_addr, sizeof(s2_addr)) != -1)
        {   
            cout << "Successful binding." << endl;
        }
        else
        {
            cout << "Unsuccessful binding. Try again in a few minutes." << endl;
            exit(-3);
        }
    }

    if(listen(s1_sid, MAX_CONNECTIONS) == -1)
    {
        perror("Listen error");
        exit(-4);
    }

    if(listen(s2_sid, MAX_CONNECTIONS) == -1)
    {
        perror("Listen error");
        exit(-5);
    }

    cout << "Server is ready for game." << endl;

    c1_sid = accept(s1_sid, (struct sockaddr*)&c1_addr, &c1_size);
    cout << "1st player is connected." << endl;
    // write(c1_sid, "Waiting for another player.", 30);
    sleep(1);
    send(c1_sid, "Waiting for another player.", 30, 0);

    c2_sid = accept(s2_sid, (struct sockaddr*)&c2_addr, &c2_size);
    cout << "2nd player is connected." << endl;
    // write(c1_sid, "Both players have been connected.", 35);
    // write(c2_sid, "Both players have been connected.", 35);
    send(c1_sid, "Both players have been connected.", 35, 0);
    sleep(1);
    send(c2_sid, "Both players have been connected.", 35, 0);

    sleep(5);
    send(c2_sid, "Goodbye.", 10, 0);
    shutdown(c2_sid, SHUT_RDWR);
    sleep(2);
    send(c1_sid, "Goodbye.", 10, 0);
    shutdown(c1_sid, SHUT_RDWR);
    shutdown(s1_sid, SHUT_RDWR);
    shutdown(s2_sid, SHUT_RDWR);
    close(s1_sid);
    close(s2_sid);
}