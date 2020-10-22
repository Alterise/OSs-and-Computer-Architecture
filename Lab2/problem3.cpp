#include <iostream>
#include <dirent.h>
#include <string>
#include <unistd.h>
#include <algorithm>
using namespace std;

int main() {
    string file_path;
    cin >> file_path;
    FILE* file;
    if(!fopen(file_path.c_str(), "r")) {
        file = fopen(file_path.c_str(), "w");
        char c;
        while((c = fgetc(stdin)) != EOF) {
            fputc(c, file);
        }
        fclose(file);
        return 0;
    }
    string file_name, ext, tmp, file_name_tmp;
    string dir_path = file_path;
    while(dir_path.back() != '/' && dir_path.length()) {
        tmp.push_back(dir_path.back());
        dir_path.pop_back();
    }
    while(tmp.length()) {
        file_name.push_back(tmp.back());
        tmp.pop_back();
    }
    file_name_tmp = file_name;
    while(file_name_tmp.back() != '.' && file_name_tmp.length()) {
        ext.push_back(file_name_tmp.back());
        file_name_tmp.pop_back();
    }
    if(file_name_tmp.back() == '.')
    {
        file_name_tmp.pop_back();
    }

    if(file_name_tmp.length())
    {
        file_name = file_name_tmp;
    }
    else
    {
        ext.clear();
    }

    reverse(ext.begin(), ext.end());

    int num = 0;
    string dir_path_fin = dir_path + file_name + "." + to_string(num) + "." + ext;
            if(!ext.length())
        {
            dir_path_fin.pop_back();
        }
    FILE* link_file;
    if(!dir_path.length())
    {
        dir_path.push_back('.');
    }
    link_file = fopen(dir_path.c_str(), "r");
    while(link_file) {
        fclose(link_file);
        num++;
        dir_path_fin = dir_path + file_name + "." + to_string(num) + "." + ext;
        if(!ext.length())
        {
            dir_path_fin.pop_back();
        }
        link_file = fopen(dir_path_fin.c_str(), "r");
    }
    link(file_path.c_str(), dir_path_fin.c_str());
} 