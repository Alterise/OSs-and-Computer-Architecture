#include <iostream>
#include <dirent.h>
#include <string.h>
using namespace std;

int main() {
    string dir_name;
    cin >> dir_name;
    DIR* dir = opendir(dir_name.c_str());
    struct dirent* directory;
    while((directory = readdir(dir)) != 0) {
        if(directory->d_type != DT_DIR) {
            puts(directory->d_name);
        }
    }
} 