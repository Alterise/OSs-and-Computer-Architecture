#include <iostream>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
using namespace std;

int main() {
    string file_path;
    cin >> file_path;
    string link_dir;
    cin >> link_dir;
    symlink(file_path.c_str(), link_dir.c_str());
    FILE* link_file;
    link_file = fopen(link_dir.c_str(), "r");
    char c;
    printf("\n");
    while((c = fgetc(link_file)) != EOF) {
        printf("%c", c);
    }
    fclose(link_file);
}