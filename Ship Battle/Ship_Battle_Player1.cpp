// Server
#include <iostream>
#include <string>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>

#define IP_ADDR "127.0.0.1"
#define PORT1 1337
#define PORT2 1338
#define endl2 endl << endl

using namespace std;

int main() {
    int         sid;
    sockaddr_in s_addr;
    char        buff[BUFSIZ];

    s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT1);
	inet_aton(IP_ADDR, &(s_addr.sin_addr));

    if((sid = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creating error");
        exit(-1);
    }

	if(!connect(sid, (struct sockaddr*)&s_addr, sizeof(s_addr)))
    {   
        cout << "Successful connection." << endl2;
    }
    else
    {
        cout << "Can't connect to main port:" << PORT1 << ". Trying to connect to reserve port:" << PORT2 << "." << endl2;
	    s_addr.sin_port = htons(PORT2);
        if(!connect(sid, (struct sockaddr*)&s_addr, sizeof(s_addr)))
        {   
            cout << "Successful connection." << endl2;
        }
        else
        {
            cout << "Unsuccessful connection. Try again in a few minutes." << endl2;
        }        
    }
    

    // while(read(sid, &buff, BUFSIZ) > 0)
    while(recv(sid, &buff, BUFSIZ, 0) > 0)
    {
	    cout << buff << endl2;
        sleep(1);
    }

    close(sid);
}