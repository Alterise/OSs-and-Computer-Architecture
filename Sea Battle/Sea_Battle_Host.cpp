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
#define MAX_CONNECTIONS 1
#define endl2 endl << endl

using namespace std;

enum Cmds {Ready, Shoot, Quit};

int main() {
    int         h_sid, g_sid;
    socklen_t   g_size;
    sockaddr_in h_addr, g_addr;
    char        buff[BUFSIZ];
    Cmds        cmd_tmp;

    h_addr.sin_family = AF_INET;
	h_addr.sin_port = htons(PORT1);
	inet_aton(IP_ADDR, &(h_addr.sin_addr));

    if((h_sid = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creating error");
        exit(-1);
    }

    if(bind(h_sid, (sockaddr*)&h_addr, sizeof(h_addr)) == -1)
    {
        cout << "Can't bind to main port:" << PORT1 << "." << endl;
        cout << "Trying to bind to reserve port:" << PORT2 << "." << endl2;
	    h_addr.sin_port = htons(PORT2);
        if(bind(h_sid, (sockaddr*)&h_addr, sizeof(h_addr)) == -1)
        {   
            perror("Unsuccessful binding. Try again in a few minutes.");
            exit(-2);
        }
    }

    if(listen(h_sid, MAX_CONNECTIONS) == -1)
    {
        perror("Listen error");
        exit(-3);
    }

    cout << "First player is ready for game." << endl;
    cout << "Waiting for second player." << endl2;

    g_sid = accept(h_sid, (struct sockaddr*)&g_addr, &g_size);
    cout << "Second player is connected." << endl2;



    
    cmd_tmp = Ready;
    send(g_sid, &cmd_tmp, sizeof(cmd_tmp), 0);

    while(recv(g_sid, &cmd_tmp, sizeof(cmd_tmp), 0) > 0)
    {
        if(cmd_tmp == Quit)
        {
            cmd_tmp = Quit;
            send(g_sid, &cmd_tmp, sizeof(cmd_tmp), 0);
            break;
        }
        else if(cmd_tmp == Shoot)
        {
            cout << "Shoted" << endl2;
            send(h_sid, &cmd_tmp, sizeof(cmd_tmp), 0);
        }
        cmd_tmp = Shoot;
    }

    sleep(3);
    shutdown(h_sid, SHUT_RDWR);
    close(h_sid);
}