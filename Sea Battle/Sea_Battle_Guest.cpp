#include <iostream>
#include <string>
#include <cmath>
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

enum Cmds {Ready, Shoot, Miss, Hit, Quit};

bool place_ship(int, int, int, int, int, int[10][10]);
int x_crd_parser(string);
int y_crd_parser(string);
void swap(int*, int*);
void show_battlefield(int[10][10], int[10][10]);

int main() {
    int         h_sid;
    sockaddr_in h_addr;
    char        buff[BUFSIZ];
    Cmds        cmd_tmp;
    string      ship_bow_crd, ship_stern_crd; 
    int         battlefield[10][10];
    int         enemy_battlefield[10][10];

    //Filling the socket struct
    h_addr.sin_family = AF_INET;
	h_addr.sin_port = htons(PORT1);
	inet_aton(IP_ADDR, &(h_addr.sin_addr));

    if((h_sid = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creating error");
        exit(-1);
    }

    //Connecting
	if(connect(h_sid, (struct sockaddr*)&h_addr, sizeof(h_addr)))
    {
        cout << "Can't connect to main port:" << PORT1 << "." << endl;
        cout << "Trying to connect to reserve port:" << PORT2 << "." << endl2;
	    h_addr.sin_port = htons(PORT2);
        if(connect(h_sid, (struct sockaddr*)&h_addr, sizeof(h_addr)))
        {
            perror("Host isn't online");
            exit(-2);
        }
    }

    cout << "Both players are ready for the game." << endl2;

    //Creating battlefields and placing ships
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            battlefield[i][j] = 0;
        }
    }

    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            enemy_battlefield[i][j] = 0;
        }
    }
    
    show_battlefield(battlefield, enemy_battlefield);
    cout << "Place your ships (Example: A7 D10) [1st - ship bow coordinates, 2nd - ship stern coordinates]:" << endl;

    while(1)
    {
        cout << "Coordinates of your nuclear submarine (Size: 4): ";
        cin >> ship_bow_crd >> ship_stern_crd;
        if(place_ship(x_crd_parser(ship_bow_crd), y_crd_parser(ship_bow_crd), x_crd_parser(ship_stern_crd), y_crd_parser(ship_stern_crd), 4, battlefield)) break;
        else 
        {
            show_battlefield(battlefield, enemy_battlefield);
            cout << "You can't do that." << endl;
        }
    }
    show_battlefield(battlefield, enemy_battlefield);
    cout << endl;

    for(int i = 1; i <= 2; i++)
    {
        while(1)
        {
            cout << "Coordinates of your cruiser №" << i << " (Size: 3): ";
            cin >> ship_bow_crd >> ship_stern_crd;
            if(place_ship(x_crd_parser(ship_bow_crd), y_crd_parser(ship_bow_crd), x_crd_parser(ship_stern_crd), y_crd_parser(ship_stern_crd), 3, battlefield)) break;
            else 
            {
                show_battlefield(battlefield, enemy_battlefield);
                cout << "You can't do that." << endl;
            }
        }
        show_battlefield(battlefield, enemy_battlefield);
        cout << endl;
    }

    for(int i = 1; i <= 3; i++)
    {
        while(1)
        {
            cout << "Coordinates of your destroyer №" << i << " (Size: 2): ";
            cin >> ship_bow_crd >> ship_stern_crd;
            if(place_ship(x_crd_parser(ship_bow_crd), y_crd_parser(ship_bow_crd), x_crd_parser(ship_stern_crd), y_crd_parser(ship_stern_crd), 2, battlefield)) break;
            else 
            {
                show_battlefield(battlefield, enemy_battlefield);
                cout << "You can't do that." << endl;
            }
        }
        show_battlefield(battlefield, enemy_battlefield);
        cout << endl;
    }
    
    
    for(int i = 1; i <= 4; i++)
    {
        while(1)
        {
            cout << "Coordinates of your torpedo boat №" << i << " (Size: 1): ";
            cin >> ship_bow_crd >> ship_stern_crd;
            if(place_ship(x_crd_parser(ship_bow_crd), y_crd_parser(ship_bow_crd), x_crd_parser(ship_stern_crd), y_crd_parser(ship_stern_crd), 1, battlefield)) break;
            else 
            {
                show_battlefield(battlefield, enemy_battlefield);
                cout << "You can't do that." << endl;
            }
        }
        show_battlefield(battlefield, enemy_battlefield);
        cout << endl;
    }

    cout << "Waiting for another player to be ready." << endl2;
    
    //Game
    while(recv(h_sid, &cmd_tmp, sizeof(cmd_tmp), 0) > 0)
    {
        if(cmd_tmp == Quit)
        {
            cmd_tmp = Quit;
            send(h_sid, &cmd_tmp, sizeof(cmd_tmp), 0);
            break;
        }
        else if(cmd_tmp == Shoot)
        {
            // cout << "Shoted" << endl2;
            // cmd_tmp = Quit;
            // send(h_sid, &cmd_tmp, sizeof(cmd_tmp), 0);
        }
    }

    cout << "Ending the game.";
    sleep(1);
    close(h_sid);
}

//Secondary functions
bool place_ship(int xh, int yh, int xs, int ys, int size, int battlefield[10][10])
{
    if(xh - xs > 0) swap(&xh, &xs);
    if(yh - ys > 0) swap(&yh, &ys);

    if (xh >= 10 || xh < 0)                               return 0;
    if (xs >= 10 || xs < 0)                               return 0;
    if (yh >= 10 || yh < 0)                               return 0;
    if (ys >= 10 || ys < 0)                               return 0;
    if (abs(xh - xs) && abs(yh - ys))                     return 0;
    if ( abs(xh - xs) &&  (abs(xh - xs) + 1) != size)     return 0;
    if ( abs(yh - ys) &&  (abs(yh - ys) + 1) != size)     return 0;

    pair<int, int> top_left_border      (xh == 0 ? xh : xh - 1, yh == 0 ? yh : yh - 1);
    pair<int, int> bottom_right_border  (xs == 9 ? xs : xs + 1, ys == 9 ? ys : ys + 1);

    for (int i = top_left_border.second; i <= bottom_right_border.second; i++)
    {
        for (int j = top_left_border.first; j <= bottom_right_border.first; j++)
        {
            if(battlefield[i][j] > 0) return 0;
        }
    }    

    if(abs(xh - xs))
    {
        for(int i = xh; i <= xs; i++) battlefield[yh][i] = size;
    }
    else if(abs(yh - ys))
    {
        for(int i = yh; i <= ys; i++) battlefield[i][xh] = size;
    }
    else
    {
        battlefield[yh][xh] = size;
    }
    return 1;
}

int x_crd_parser(string crd_str)
{
     if(crd_str.length() == 2 || crd_str.length() == 3)
    {
        if(!isalpha(crd_str[0])) return -1;
        return crd_str[0] - 'A';
    }
    else return -1;
}

int y_crd_parser(string crd_str)
{
    if(crd_str.length() == 2)
    {
        if(!isdigit(crd_str[1])) return -1;
        return (crd_str[1] - '0') - 1;
    }
    else if(crd_str.length() == 3)
    {
        if(!isdigit(crd_str[1])) return -1;
        if(!isdigit(crd_str[2])) return -1;
        return ((crd_str[1] - '0') * 10 + (crd_str[2] - '0')) - 1;
    }
    else return -1;
}

void swap(int *a, int *b)
{
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void show_battlefield(int battlefield[10][10], int enemy_battlefield[10][10])
{
    cout << endl2 << endl2 << endl2;
    cout << "     My Battlefield                      Enemy Battlefield" << endl2;
    cout << "   A B C D E F G H I J                  A B C D E F G H I J" << endl;
    for(int i = 0; i < 10; i++)
    {
        if(i != 9) cout << " "; 
        cout << i + 1 << " ";

        for(int j = 0; j < 10; j++)
        {
            if(battlefield[i][j] == 0) cout << "~";
            else if(battlefield[i][j] > 0) cout << "@";
            if(j != 9) cout << " ";
        }
        cout << "               ";

        if(i != 9) cout << " "; 
        cout << i + 1 << " ";

        for(int j = 0; j < 10; j++)
        {
            if(enemy_battlefield[i][j] == 0) cout << "~";
            else if(enemy_battlefield[i][j] > 0) cout << "@";
            if(j != 9) cout << " ";
        }
        cout << endl;
    }
    cout << endl;
}