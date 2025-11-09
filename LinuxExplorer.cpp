#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
using namespace std;

void listDir(string path) {
    DIR *dir = opendir(path.c_str());
    dirent *entry;
    cout << "\nDirectory: " << path << endl;

    while ((entry = readdir(dir)) != NULL) {
        cout << " - " << entry->d_name << endl;
    }
    closedir(dir);
}

void searchFile(string base, string target) {
    DIR *dir = opendir(base.c_str());
    if (!dir) return;

    dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        string name = entry->d_name;
        string fullPath = base + "/" + name;

        if (name == target)
            cout << "Found: " << fullPath << endl;

        struct stat info;
        stat(fullPath.c_str(), &info);

        if (S_ISDIR(info.st_mode) && name != "." && name != "..")
            searchFile(fullPath, target);
    }
    closedir(dir);
}

int main() {
    string currentPath = ".";
    string cmd, arg1, arg2;

    while (true) {
        cout << "\n> ";
        cin >> cmd;

        if (cmd == "ls") {
            listDir(currentPath);
        }
        else if (cmd == "cd") {
            cin >> arg1;
            chdir(arg1.c_str());
            char buf[500];
            getcwd(buf, sizeof(buf));
            currentPath = buf;
        }
        else if (cmd == "new") {      // create file
            cin >> arg1;
            ofstream file(arg1);
        }
        else if (cmd == "del") {      // delete file
            cin >> arg1;
            remove(arg1.c_str());
        }
        else if (cmd == "ren") {      // rename
            cin >> arg1 >> arg2;
            rename(arg1.c_str(), arg2.c_str());
        }
        else if (cmd == "cp") {       // copy
            cin >> arg1 >> arg2;
            ifstream src(arg1, ios::binary);
            ofstream dest(arg2, ios::binary);
            dest << src.rdbuf();
        }
        else if (cmd == "search") {
            cin >> arg1;
            searchFile(currentPath, arg1);
        }
        else if (cmd == "chmod") {
            cin >> arg1 >> arg2;
            chmod(arg2.c_str(), stoi(arg1, 0, 8));  // convert "755" to octal
        }

        else if (cmd == "exit") break;
    }
}
