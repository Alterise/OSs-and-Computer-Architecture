// Host
#include <iostream>
#include <string>
#include <vector>
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

enum Cmds {Ready, Shoot, Miss, Hit, Destroyed, Pass, Win, Error, Quit};

struct ship_info
{
    int             size;
    int             durability;
    pair<int, int>  ship_bow_crd;
    pair<int, int>  ship_stern_crd;
    pair<int, int>  top_left_border;
    pair<int, int>  bottom_right_border;
};


bool    place_ship(int, int, int, int, int, int[10][10], vector<ship_info>&);
bool    is_correct_crd(int, int);
int     x_crd_parser(string);
int     y_crd_parser(string);
void    swap(int*, int*);
void    show_battlefield(int[10][10], int[10][10]);
void    destroyed_ship_explosion(int[10][10], int, int, int, int);


int main() {
    int                 h_sid, g_sid;
    socklen_t           g_size;
    sockaddr_in         h_addr, g_addr;
    Cmds                cmd_ready           = Ready;
    Cmds                cmd_shoot           = Shoot;
    Cmds                cmd_miss            = Miss; 
    Cmds                cmd_hit             = Hit; 
    Cmds                cmd_destroyed       = Destroyed; 
    Cmds                cmd_pass            = Pass; 
    Cmds                cmd_error           = Error; 
    Cmds                cmd_win             = Win; 
    Cmds                cmd_quit            = Quit; 
    Cmds                cmd_received;
    string              ship_bow_crd, ship_stern_crd; 
    string              crd_tmp; 
    int                 battlefield[10][10];
    int                 enemy_battlefield[10][10];
    int                 ship_count = 10;
    int                 x, y, xl, xr, yt, yb;
    vector<ship_info>   ship_collection;

    //Filling the socket struct
    h_addr.sin_family = AF_INET;
	h_addr.sin_port = htons(PORT1);
	inet_aton(IP_ADDR, &(h_addr.sin_addr));

    if((h_sid = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creating error");
        exit(-1);
    }

    //Binding socket
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

    //Opening socket for new connections
    if(listen(h_sid, MAX_CONNECTIONS) == -1)
    {
        perror("Listen error");
        exit(-3);
    }

    cout << "Waiting for second player." << endl2;

    g_sid = accept(h_sid, (struct sockaddr*)&g_addr, &g_size);
    cout << "Both players are connected." << endl2;
    sleep(1);

    //Checking connection
    cout << "Checking connection with other player." << endl2;
    sleep(1);

    send(g_sid, &cmd_ready, sizeof(Cmds), 0);
    if(!(recv(g_sid, &cmd_received, sizeof(Cmds), 0) > 0))
    {
        cout << "Socket messaging error(1)." << endl;
        cout << "Try again." << endl;
        sleep(3);
        shutdown(h_sid, SHUT_RDWR);
        close(h_sid);
        exit(-1);
    }
    if(cmd_received != Ready) 
    {
        cout << "Socket messaging error(2)." << endl;
        cout << "Try again." << endl;
        sleep(3);
        shutdown(h_sid, SHUT_RDWR);
        close(h_sid);
        exit(-2);
    }

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
        if(place_ship(x_crd_parser(ship_bow_crd), y_crd_parser(ship_bow_crd), x_crd_parser(ship_stern_crd), y_crd_parser(ship_stern_crd), 4, 
        battlefield, ship_collection)) break;
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
            if(place_ship(x_crd_parser(ship_bow_crd), y_crd_parser(ship_bow_crd), x_crd_parser(ship_stern_crd), y_crd_parser(ship_stern_crd), 
            3, battlefield, ship_collection)) break;
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
            if(place_ship(x_crd_parser(ship_bow_crd), y_crd_parser(ship_bow_crd), x_crd_parser(ship_stern_crd), y_crd_parser(ship_stern_crd), 
            2, battlefield, ship_collection)) break;
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
            if(place_ship(x_crd_parser(ship_bow_crd), y_crd_parser(ship_bow_crd), x_crd_parser(ship_stern_crd), y_crd_parser(ship_stern_crd), 
            1, battlefield, ship_collection)) break;
            else 
            {
                show_battlefield(battlefield, enemy_battlefield);
                cout << "You can't do that." << endl;
            }
        }
        show_battlefield(battlefield, enemy_battlefield);
        cout << endl;
    }

    //Checking willingness of players
    cout << "Waiting for another player to be ready." << endl2;
    sleep(2);

    send(g_sid, &cmd_ready, sizeof(Cmds), 0);
    if(!(recv(g_sid, &cmd_received, sizeof(Cmds), 0) > 0))
    {
        cout << "Something went wrong(1)." << endl;
        sleep(3);
        shutdown(h_sid, SHUT_RDWR);
        close(h_sid);
        exit(-1);
    }
    if(cmd_received != Ready) 
    {
        cout << "Something went wrong(2)." << endl;
        sleep(3);
        shutdown(h_sid, SHUT_RDWR);
        close(h_sid);
        exit(-2);
    }

    cout << "Starting the game." << endl2;
    sleep(2);
    
    //Game
    while(recv(g_sid, &cmd_received, sizeof(Cmds), 0) > 0)
    {
        if(cmd_received == Shoot)
        {
            recv(g_sid, &x, sizeof(int), 0);
            recv(g_sid, &y, sizeof(int), 0);
            if(battlefield[y][x] > 10) 
            {
                ship_collection[battlefield[y][x] - 11].durability--;
                if(ship_collection[battlefield[y][x] - 11].durability == 0)
                {
                    xl = ship_collection[battlefield[y][x] - 11].top_left_border.first;
                    yt = ship_collection[battlefield[y][x] - 11].top_left_border.second;
                    xr = ship_collection[battlefield[y][x] - 11].bottom_right_border.first;
                    yb = ship_collection[battlefield[y][x] - 11].bottom_right_border.second;
                    destroyed_ship_explosion(battlefield, xl, yt, xr, yb);
                    ship_count--;
                    show_battlefield(battlefield, enemy_battlefield);
                    cout << "Your opponent destroyed one of your ships." << endl2;
                    send(g_sid, &cmd_destroyed, sizeof(Cmds), 0);
                    send(g_sid, &x, sizeof(int), 0);
                    send(g_sid, &y, sizeof(int), 0);
                    send(g_sid, &xl, sizeof(int), 0);
                    send(g_sid, &yt, sizeof(int), 0);
                    send(g_sid, &xr, sizeof(int), 0);
                    send(g_sid, &yb, sizeof(int), 0);
                    if(ship_count) 
                    {
                        send(g_sid, &cmd_pass, sizeof(Cmds), 0);
                        cout << "Waiting for your opponent to shoot" << endl2;
                    }
                    else 
                    {
                        sleep(2);
                        cout << "~~~~~~~~~" << endl;
                        cout << "You lose." << endl;
                        cout << "~~~~~~~~~" << endl2;
                        send(g_sid, &cmd_win, sizeof(Cmds), 0);
                    }
                }
                else 
                {
                    battlefield[y][x] = 2;
                    show_battlefield(battlefield, enemy_battlefield);
                    cout << "Your opponent hitted one of your ships." << endl2;
                    send(g_sid, &cmd_hit, sizeof(Cmds), 0);
                    send(g_sid, &x, sizeof(int), 0);
                    send(g_sid, &y, sizeof(int), 0);
                    send(g_sid, &cmd_pass, sizeof(Cmds), 0);
                    cout << "Waiting for your opponent to shoot" << endl2;
                }
            }
            else if(battlefield[y][x] == 0)
            {
                battlefield[y][x] = 1;
                show_battlefield(battlefield, enemy_battlefield);
                cout << "Your opponent missed the shot." << endl2;
                send(g_sid, &cmd_miss, sizeof(Cmds), 0);
                send(g_sid, &x, sizeof(int), 0);
                send(g_sid, &y, sizeof(int), 0);
            }
            else 
            {
                send(g_sid, &cmd_error, sizeof(Cmds), 0);
                send(g_sid, &cmd_pass, sizeof(Cmds), 0);
            }
        }
        else if(cmd_received == Miss)
        {
            recv(g_sid, &x, sizeof(int), 0);
            recv(g_sid, &y, sizeof(int), 0);
            enemy_battlefield[y][x] = 1;
            show_battlefield(battlefield, enemy_battlefield);
            cout << "You missed the shot." << endl2;
            cout << "Waiting for your opponent to shoot" << endl2;
            send(g_sid, &cmd_pass, sizeof(Cmds), 0);
        }
        else if(cmd_received == Hit)
        {
            recv(g_sid, &x, sizeof(int), 0);
            recv(g_sid, &y, sizeof(int), 0);
            enemy_battlefield[y][x] = 2;
            show_battlefield(battlefield, enemy_battlefield);
            cout << "You hitted the ship." << endl2; 
        }
        else if(cmd_received == Destroyed)
        {
            recv(g_sid, &x, sizeof(int), 0);
            recv(g_sid, &y, sizeof(int), 0);
            enemy_battlefield[y][x] = 2;
            recv(g_sid, &xl, sizeof(int), 0);
            recv(g_sid, &yt, sizeof(int), 0);
            recv(g_sid, &xr, sizeof(int), 0);
            recv(g_sid, &yb, sizeof(int), 0);
            destroyed_ship_explosion(enemy_battlefield, xl, yt, xr, yb);
            show_battlefield(battlefield, enemy_battlefield);
            cout << "You destroyed the ship." << endl2;
        }
        else if(cmd_received == Pass)
        {
            cout << "Your turn to shoot" << endl2;
            while(1)
            {
                cout << "Input coordinates (Example: G6): ";
                cin >> crd_tmp;
                if(is_correct_crd(x_crd_parser(crd_tmp), y_crd_parser(crd_tmp)))
                {
                    x = x_crd_parser(crd_tmp);
                    y = y_crd_parser(crd_tmp);
                    send(g_sid, &cmd_shoot, sizeof(Cmds), 0);
                    send(g_sid, &x, sizeof(int), 0);
                    send(g_sid, &y, sizeof(int), 0);
                    break;    
                }
                else 
                {
                    show_battlefield(battlefield, enemy_battlefield);
                    cout << "Wrong input. Try again." << endl;
                }
            }            
        }
        else if(cmd_received == Win)
        {
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
            cout << "You won! Congratulations!" << endl;
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << endl2;
            send(g_sid, &cmd_quit, sizeof(Cmds), 0);
        }
        else if(cmd_received == Error)
        {
            show_battlefield(battlefield, enemy_battlefield);
            cout << "You can't do that." << endl2;
        }
        else if(cmd_received == Quit)
        {
            sleep(2);
            send(g_sid, &cmd_quit, sizeof(Cmds), 0);
            break;
        }
    }

    sleep(3);
    cout << "Ending the game." << endl2;
    shutdown(h_sid, SHUT_RDWR);
    close(h_sid);
}

//Secondary functions
bool place_ship(int xb, int yb, int xs, int ys, int size, int battlefield[10][10], vector<ship_info>& ship_collection)
{
    if(xb - xs > 0) swap(&xb, &xs);
    if(yb - ys > 0) swap(&yb, &ys);

    if (xb >= 10 || xb < 0)                               return 0;
    if (xs >= 10 || xs < 0)                               return 0;
    if (yb >= 10 || yb < 0)                               return 0;
    if (ys >= 10 || ys < 0)                               return 0;
    if (abs(xb - xs) && abs(yb - ys))                     return 0;
    if ( abs(xb - xs) &&  (abs(xb - xs) + 1) != size)     return 0;
    if ( abs(yb - ys) &&  (abs(yb - ys) + 1) != size)     return 0;

    pair<int, int> top_left_border      (xb == 0 ? xb : xb - 1, yb == 0 ? yb : yb - 1);
    pair<int, int> bottom_right_border  (xs == 9 ? xs : xs + 1, ys == 9 ? ys : ys + 1);

    for (int i = top_left_border.second; i <= bottom_right_border.second; i++)
    {
        for (int j = top_left_border.first; j <= bottom_right_border.first; j++)
        {
            if(battlefield[i][j] > 0) return 0;
        }
    }    

    if(abs(xb - xs))
    {
        for(int i = xb; i <= xs; i++) battlefield[yb][i] = ship_collection.size() + 11;
    }
    else if(abs(yb - ys))
    {
        for(int i = yb; i <= ys; i++) battlefield[i][xb] = ship_collection.size() + 11;
    }
    else
    {
        battlefield[yb][xb] = ship_collection.size() + 11;
    }

    ship_info ship_info_tmp;
    ship_info_tmp.size                      = size;
    ship_info_tmp.durability                = size;
    ship_info_tmp.ship_bow_crd.first        = xb;
    ship_info_tmp.ship_bow_crd.second       = yb;
    ship_info_tmp.ship_stern_crd.first      = xs;
    ship_info_tmp.ship_stern_crd.second     = ys;
    ship_info_tmp.top_left_border           = top_left_border;
    ship_info_tmp.bottom_right_border       = bottom_right_border;
    ship_collection.push_back(ship_info_tmp);

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
            if(battlefield[i][j] == 0)          cout << "~";
            else if(battlefield[i][j] > 10)     cout << "@";
            else if(battlefield[i][j] == 1)     cout << "|";
            else if(battlefield[i][j] == 2)     cout << "o";
            else if(battlefield[i][j] == 3)     cout << "X";
            if(j != 9) cout << " ";
        }
        cout << "               ";

        if(i != 9) cout << " "; 
        cout << i + 1 << " ";

        for(int j = 0; j < 10; j++)
        {
            if(enemy_battlefield[i][j] == 0)        cout << "~";
            else if(enemy_battlefield[i][j] == 1)   cout << "|";
            else if(enemy_battlefield[i][j] == 2)   cout << "o";
            else if(enemy_battlefield[i][j] == 3)   cout << "X";
            if(j != 9) cout << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void destroyed_ship_explosion(int battlefield[10][10], int xl, int yt, int xr, int yb)
{
    for (int i = yt; i <= yb; i++)
    {
        for (int j = xl; j <= xr; j++)
        {
            if(battlefield[i][j] > 1)       battlefield[i][j] = 3;
            else                            battlefield[i][j] = 1;
        }
    }
}

bool is_correct_crd(int x, int y)
{
    if (x >= 10 || x < 0)   return 0;
    if (y >= 10 || y < 0)   return 0;
    return 1;
}