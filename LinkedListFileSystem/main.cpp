//
//  main.cpp
//  LinkedListFileSystem
//
//  Created by Sherwin Samson on 7/5/19.
//  Copyright © 2019 Sherwin Samson. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "FileSystem.h"

using namespace std;


void runFileSystem(ifstream &in, ostream &out)
{
    FileSystem sys;
    
    // table possible commands
    string commands[9] = {"mkdir", "rmdir", "chdir", "ls",
        "lsr", "mkfile", "rmfile", "pwd", "Q"};
    string buffer = "", arg = "";
    int index, pos;
    
    while(!in.eof() && buffer[0] != 'Q')    // while not end of file or 'Q'
    {
        out << "\n";
        sys.pwd(out);            // just prints out
        out << "> " << flush;    // current_working_directory>
        getline(in, buffer);
        out << "*" << buffer << endl;
        
        pos = buffer.find_first_of(' ', 0); // find ' ' to separate out arguments
        arg = "";
        if (pos == -1) // for ls or lsr
        {
            pos = buffer.length();
        }
        else
        {
            arg = buffer.substr(pos+1, buffer.length()-1);
        }
        for (int i = 0; i < 9; i++) // find command from commands table
        {
            if (strcmp(buffer.substr(0, pos).c_str(),
                       commands[i].c_str()) == 0)
            {
                index = i;
            }
        }
        try
        {
            switch (index)  // case = commands
            {
                case 0:
                    sys.mkdir(arg);
                    break;
                case 1:
                    sys.rmdir(arg);
                    break;
                case 2:
                    sys.chdir(arg);
                    break;
                case 3:
                    sys.ls(out);
                    break;
                case 4:
                    sys.lsr(out);
                    break;
                case 5:
                    sys.mkfile(arg);
                    break;
                case 6:
                    sys.rmfile(arg);
                    break;
                case 7:
                    sys.pwd(out);
                    out << endl;
                    break;
                case 8:
                    sys.chdir("/");
                    sys.lsr(out);       // do lsr if 'Q'
                    break;
                default:
                    out << "UNKNOWN COMMAND: " << buffer << endl;
            }
        }
        catch(DuplicateNameException)
        {
            out << "This name already exists: " << arg << endl;
        }
        catch(DirDoesNotExistException)
        {
            out << "This directory does not exists: " << arg << endl;
        }
        catch(FileDoesNotExistException)
        {
            out << "This file does not exists: " << arg << endl;
        }
    }
    return;
}




int main(int argc, char* argv[])
{
    cout << "Starting progream...." << endl;
    
    /*
    if (argc < 3)   // check for command line arguments
    {
        cout << "Insufficient arguments!" << endl;
        return -1;
    }
    
    ifstream fin(argv[1]);
    ofstream fout(argv[2]);
    */
    
    ifstream fin("input.txt");  // input.txt must be in same folder as executable...
    ofstream fout("output.txt");
    
    
    // check input and output files
    if (!fin)
    {
        cout << "Unable to open input file." << endl;
        return -2;
    }
    if (!fout)
    {
        cout << "Unable to open output file." << endl;
        return -2;
    }
    
    runFileSystem(fin, fout); // run file system
    
    fin.close();
    fout.close();
    cout << endl << endl << "Program finished..." << endl;
    return 0;
}
