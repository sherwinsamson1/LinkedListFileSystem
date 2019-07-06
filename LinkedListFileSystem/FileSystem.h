
#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
using namespace std;

struct DATA
{
    bool isdir;
    string name;
    string path;
    DATA *parent;
    DATA *next;
    DATA *child;
};

class DuplicateNameException: public exception
{
public:
    DuplicateNameException():exception(){}
};

class DirDoesNotExistException: public exception
{
public:
    DirDoesNotExistException():exception(){}
};

class FileDoesNotExistException: public exception
{
public:
    FileDoesNotExistException():exception(){}
};

class FileSystem
{
protected:
    DATA *root;
    DATA *cwdpointer;
    string cwd;
    bool nameCheck(string name);
    void dataInsert(DATA *d);
    void clearHelper(DATA *e);
    void lsrHelper(ostream &out, DATA *p, int tabcounter);
    bool dirCheck(string name);
public:
    FileSystem();
    ~FileSystem();
    void mkdir(string name);
    void rmdir(string name);
    void chdir(string name);
    void ls(ostream &out);
    void lsr(ostream &out);
    void mkfile(string name);
    void rmfile(string name);
    void pwd(ostream &out);
};
#endif // FILESYSTEM_H

