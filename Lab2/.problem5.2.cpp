#include <iostream>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <filesystem>
#include <time.h>
#include <vector>
#include <algorithm>
using namespace std;

bool comparator(const pair<string,time_t> &a, const pair<string, time_t> &b) {
    return (a.second < b.second);
}


int main() {
    char buff[BUFSIZ];
    time_t t = time(0);
    string dir_name;
    string file_name;
    vector<pair<string, time_t>> file_arr;
    cin >> dir_name;
    struct stat stats;
    struct timespec ts;
    DIR* dir = opendir(dir_name.c_str());
    struct dirent* directory;
    while((directory = readdir(dir)) != 0) {
        file_name = dir_name + directory->d_name;
        stat(file_name.c_str(), &stats);
        time_t mod_time = stats.st_mtim.tv_sec;
        file_arr.push_back(make_pair(file_name, mod_time));
        sort(file_arr.begin(), file_arr.end(), comparator);
        // strftime(buff, sizeof(buff), "%A %c", localtime((time_t)mod_time.tv_sec));
    }
    for(int i = 0; i < file_arr.size(); i++)
    {
        cout << file_arr[i].first << " " << file_arr[i].second << endl;
    }
} 