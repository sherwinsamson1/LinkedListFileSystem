//
//  FileSystem.cpp
//  LinkedListFileSystem
//
//  Created by Sherwin Samson on 7/5/19.
//  Copyright © 2019 Sherwin Samson. All rights reserved.
//


#include "FileSystem.h"

// default constructor
// also creates root node
FileSystem::FileSystem()
{
    DATA *r = new DATA;
    r->name = "/";
    r->isdir = true;
    r->child = nullptr;
    r->path = "/";
    r->next = nullptr;
    root = r;
    r->parent = r;
    cwdpointer = r; // make current working directory pointer
    cwd = "/";      // point to root
}
// destructor calls clearHelper
FileSystem::~FileSystem()
{
    DATA *p = root;
    if (p->child != nullptr)
    {
        clearHelper(p->child); // if root has child, call helper function
    }
    delete root;
}
///////////////////////////////////////////////////////////

// clearHelper helps destructor delete nodes recursively
void FileSystem::clearHelper(DATA *p)
{
    DATA *q = nullptr;
    while (p != nullptr)
    {
        if (p->child != nullptr)
        {
            clearHelper(p->child); // recursive call if directory has content
        }
        q = p;
        p = p->next;
        delete q;
    }
    return;
}

// for inserting nodes in lexicographical order
void FileSystem::dataInsert(DATA *d)
{
    DATA *p = cwdpointer->child, *q = nullptr;
    // traverse until end of list or p->name is greater than d->name
    while(p != nullptr && (d->name.compare(p->name) > 0))
    {
        q = p;
        p = p->next;
    }
    if (q == nullptr) // if list is empty
    {
        cwdpointer->child = d;
    }
    else
    {
        q->next = d;
    }
    d->next = p;
    return;
}

// recursive lsrHelper
void FileSystem::lsrHelper(ostream &out, DATA *p, int tabcounter)
{
    while(p != nullptr)
    {
        // print spaces depending on how deep the recursive call
        for(int i = 0; i < (tabcounter*3); i++)
        {
            out << ' ' << flush;
        }
        out << p->name << flush;
        if (p->isdir == 0)
        {
            out << ".txt" << flush;
        }
        else
        {
            out << ".dir" << flush;
        }
        out << endl;
        // if p has content, call recursive call and add tabcounter
        if (p->child != nullptr)
        {
            lsrHelper(out, p->child, tabcounter+1);
        }
        p = p->next;
    }
    return;
}

// checks if directory with name exists
bool FileSystem::dirCheck(string name)
{
    DATA *p = cwdpointer->child;
    while(p != nullptr)
    {
        if (name.compare(p->name) == 0)
        {
            if (p->isdir == 1)
            {
                return true;
            }
        }
        p = p->next;
    }
    return false;
}

// checks if name exists
bool FileSystem::nameCheck(string name)
{
    DATA *p = cwdpointer->child;
    while(p != nullptr)
    {
        if (name.compare(p->name) == 0)
        {
            return true;
        }
        p = p->next;
    }
    return false;
}
////////////////////////////////////////////////////////////


void FileSystem::mkdir(string name)
{
    if (nameCheck(name))    // if name already exists
    {
        throw DuplicateNameException();
    }
    else
    {
        DATA *d = new DATA;
        d->name = name;
        d->isdir = true;
        d->parent = cwdpointer;
        d->child = nullptr;
        d->path = cwd;
        d->path.append(d->name);
        d->path.append("/");
        d->next = nullptr;
        dataInsert(d);
    }
    return;
}


void FileSystem::rmdir(string name)
{
    if (!dirCheck(name))    // if directory does not exist
    {
        throw DirDoesNotExistException();
    }
    else
    {
        DATA *p = cwdpointer->child, *q = nullptr;
        // find directory to  remove
        while(p != nullptr)
        {
            if (name.compare(p->name) == 0)
            {
                if (p->isdir == 1)
                {
                    break;
                }
            }
            q = p;
            p = p->next;
        }
        // q is null if list is empty or dir is in front of list
        if (q == nullptr)
        {
            q = p->next;
            if (q != nullptr)
            {
                q->parent->child = q;
            }
            else
            {
                cwdpointer->child = nullptr;
            }
        }
        else
        {
            q->next = p->next;
        }
        // call clearHelper is dir has content
        clearHelper(p->child);
        delete p;
    }
    
    return;
}


void FileSystem::chdir(string name)
{
    char buffer[100] = "";
    // copy name into char string to use strtok
    strcpy(buffer, name.c_str());
    char *token;
    DATA *p = nullptr, *q = nullptr;
    
    // if first char is '/' go to root
    if (buffer[0] == '/')
    {
        cwdpointer = root;
    }
    token = strtok(buffer, "/");
    q = cwdpointer;
    
    while (token != NULL)
    {
        if (strcmp(token, "..") == 0)
        {
            q = q->parent;
        }
        // if '.' do nothing
        else if (strcmp(token, ".") == 0) {}
        else
        {
            p = q->child;
            // look for directory
            while(p != nullptr)
            {
                if (strcmp(token, p->name.c_str()) == 0)
                {
                    if (p->isdir == 1)
                    {
                        q = p;
                        break;
                    }
                }
                p = p->next;
            }
            // if list is exhausted then dir is not there
            if (p == nullptr)
            {
                throw DirDoesNotExistException();
            }
        }
        token = strtok(NULL, "/\n");
    }
    
    cwdpointer = q;
    cwd = cwdpointer->path;
    return;
}


void FileSystem::ls(ostream &out)
{
    DATA *p = cwdpointer->child;
    while (p != nullptr)
    {
        out << p->name << flush;
        if (p->isdir == 0)
        {
            out << ".txt" << flush;
        }
        else
        {
            out << ".dir" << flush;
        }
        out << endl;
        p = p->next;
    }
    out << endl;
    return;
}


void FileSystem::lsr(ostream &out)
{
    DATA *p = cwdpointer->child;
    while (p != nullptr)
    {
        out << p->name << flush;
        if (p->isdir == 0)
        {
            out << ".txt" << flush;
        }
        else
        {
            out << ".dir" << flush;
        }
        out << endl;
        // call recursive helper if dir has content
        if (p->child != nullptr)
        {
            lsrHelper(out, p->child, 1);
        }
        p = p->next;
    }
    out << endl;
    return;
}

// similar to mkdir
void FileSystem::mkfile(string name)
{
    if (nameCheck(name))
    {
        throw DuplicateNameException();
    }
    else
    {
        DATA *d = new DATA;
        d->name = name;
        d->isdir = false;
        d->parent = cwdpointer;
        d->path = cwd;
        d->path.append("/");
        d->path.append(d->name);
        d->next = nullptr;
        d->child = nullptr;
        dataInsert(d);
    }
    
    return;
}

// similar to rmdir
void FileSystem::rmfile(string name)
{
    if (!nameCheck(name) || dirCheck(name))
    {
        throw FileDoesNotExistException();
    }
    else
    {
        DATA *p = cwdpointer->child, *q = nullptr;
        while(p != nullptr)
        {
            if (name.compare(p->name) == 0)
            {
                if (p->isdir == 0)
                {
                    break;
                }
            }
            q = p;
            p = p->next;
        }
        if (q == nullptr)
        {
            q = p->next;
            if (q != nullptr)
            {
                q->parent->child = q;
            }
            else
            {
                cwdpointer->child = nullptr;
            }
        }
        else
        {
            q->next = p->next;
        }
        delete p;
    }
    return;
}


void FileSystem::pwd(ostream &out)
{
    out << cwd << flush;
    return;
}
