//© Copyright 2010 BlackTopp Studios Inc.
// This file is part of The PhysGame Engine and is used during the build process to copy files.
//
//    The PhysGame Engine is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    The PhysGame Engine is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
//
// The original authors have included a copy of the license specified above in the
//   'Docs' folder. See 'gpl.txt'
//
// We welcome the use of The PhysGame anyone, including companies who wish to
//   Build professional software and charge for their product.
//
//   However there are some practical restrictions, so if your project involves
//   any of the following you should contact us and we will try to work something
//   out:
//    - DRM or Copy Protection of any kind(except Copyrights)
//    - Software Patents You Do Not Wish to Freely License
//    - Any Kind of Linking to Non-GPL licensed Works
//    - Are Currently In Violation of Another Copyright Holder's GPL License
//    - If You want to change our code and not add a few hundred MB of stuff to
//        your distribution
//
//   These and other limitations could cause serious legal problems if you ignore
//   them, so it is best to simply contact us or the Free Software Foundation, if
//   you have any questions.
//
//   Joseph Toppi - toppij@gmail.com
//   John Blackwood - makoenergy02@gmail.com

#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include <vector>
#include <fstream>
#include <cstdlib>

#include <sys/stat.h> // These should work with mingw, but may not with visual c
#include <sys/types.h>

using namespace std;

// Which platform is this being built on? (remark all wrong answers)
// If you are using the code::blocks project file, then this is selected using the "Build Target" Pull down menu
//#define WINDOWSCOMPATIBLEOS
//#define GNUCOMPATIBLEOS

//Some Error codes
#define E_BADSOURCEDATA 1
#define E_MISSINGDIRLIST 2
#define E_MISSINGFILELIST 3
#define E_COPYFAILED 4
#define E_BADARGS 65

// Globals aren't bad if they aren't used badly.
// If this tool gets over 1000 LOCs or sorted into multiple files this should change like maybe into a config file
string TargetDir("bin");
string RootSourceDir("data");
string SourceDir("");
string DirList("makedirlist.txt");
string FileList("copyfilelist.txt");

#ifdef WINDOWSCOMPATIBLEOS
string Slash("\\");
#endif

#ifdef GNUCOMPATIBLEOS
string Slash("/");
#endif

//Utility functions
string Usage(string BaseName);
void Tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ");

//File Manipulation Functions
bool FileDoesExistIsItADirectory(string Filename);
bool FileDoesExist(string Filename);
void CopyFile(string Source, string Destination);
void CopyAllFiles();
void MakeDirTree(string Dirs);
void MakeDir(string OneDirDeep);


int main(int ArgCount, char** ArgValues)
{
    if(3 == ArgCount)
    {
        //Lets prep the values we will use the rest of the program
        stringstream Temp;
        Temp << TargetDir << Slash << ArgValues[1];
        TargetDir = Temp.str();
        Temp.str("");

        Temp << RootSourceDir << Slash << ArgValues[2];
        SourceDir = Temp.str();
        Temp.str("");

        Temp << RootSourceDir << Slash << DirList;
        DirList = Temp.str();
        Temp.str("");

        Temp << RootSourceDir << Slash << FileList;
        FileList = Temp.str();
        Temp.str("");

        if (FileDoesExistIsItADirectory(SourceDir))
        {
            // If the Target Directory does no exist make it
            if (!FileDoesExistIsItADirectory(TargetDir))
                { MakeDirTree(TargetDir); }

            if (!FileDoesExist(DirList))
            {
                cerr << "Directory creation list missing \"" << DirList << "\" must exist" << endl;
                return E_MISSINGDIRLIST;
            }

            if (!FileDoesExist(FileList))
            {
                cerr << "File copy list missing \"" << FileList << "\" must exist" << endl;
                return E_MISSINGFILELIST;
            }

            // TODO Need to create all dirs here

            // now that we know everything we need is present copy the files
            CopyAllFiles();
        }else{
            cerr << "Source Directory \"" << SourceDir << "\" fails" << endl;
            return E_BADSOURCEDATA;
        }

    }else{
        cout << Usage(ArgValues[0]);
        return E_BADARGS;
    }

    return 0;
}

string Usage(string BaseName)
{
    stringstream Results;
    Results << "Usage: " << BaseName << " BuildName CopyFromName" << endl
            << "BuildName is the name of the Build Process \"LinuxDebug\", \"LinuxRelease\", \"WinDebug\", ..."<< endl
            << "CopyFromName is the set of source files to use. \"linux\", \"macosx\", \"windows\", ..." << endl
            << "Notes: Since this tool is designed to be run as part of a build process and not as a standalone tool it makes several assumptions. This tool expects no slashes in the filenames passed, this will cause problems. See the source code for more details." << endl;

    return Results.str();
}

// used with permission from
// http://oopweb.com/CPP/Documents/CPPHOWTO/Volume/C++Programming-HOWTO-7.html
// Author wrote "Here is a couple of suggestions, use what suits your best." just before:
void Tokenize(const string& str, vector<string>& tokens, const string& delimiters)
{
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}

bool FileDoesExistIsItADirectory(string Filename)
{
    struct stat Results;
    //0 on success :( the old c api was really stupid
    if( 0==stat(Filename.c_str(),&Results) )
    {
        //File exists, now is it a Dir
        if( S_ISDIR(Results.st_mode) )
            { return true; }
    }
    return false;
}

bool FileDoesExist(string Filename)
{
    struct stat Results;
    if( 0==stat(Filename.c_str(),&Results) )
        { return true; }
    return false;
}

void MakeDirTree(string Dirs)
{
    vector<string> Tokens;
    Tokenize(Dirs, Tokens, Slash);
    stringstream Depth;

    for ( vector<string>::iterator iter=Tokens.begin();  iter!=Tokens.end(); ++iter )
    {
        Depth << *iter << Slash;
        MakeDir(Depth.str());
    }
}

void MakeDir(string OneDirDeep)
{
    if (FileDoesExistIsItADirectory(OneDirDeep))
        { return; } //No need to Fuss over an already exisitng folder

    // Begin OS specific code for Directory creation
    #ifdef WINDOWS

    #endif

    #ifdef GNUCOMPATIBLEOS
    if (0==mkdir(OneDirDeep.c_str(),0777)) //Success on 0
        { return; }
    #endif

    //We expect the OS specific blocks to return
    cerr << "Could not make Directories: " << OneDirDeep << endl;
    throw exception();
}

void CopyAllFiles()
{
    ifstream CopyListFile(FileList.c_str());

    while (!CopyListFile.eof())
    {
        char Line[256]; //anything longer than this is crazy
        CopyListFile.getline(Line,256);
        cout << Line << endl;

        // TODO tokenize string using Target, then re-serialize replace the target.

        if(CopyListFile.fail())
        {
            if(CopyListFile.eof())
                { break; }
            cerr << "Copying Files mysteriously failed, add more code to copy to find out why." << endl;
            exit(E_COPYFAILED);
        }
    }

    CopyListFile.close();
}

void CopyFile(string Source, string Destination)
{
    ifstream sfs(Source.c_str(), std::ios::binary);
    ofstream dfs(Destination.c_str(), std::ios::binary);
    dfs << sfs.rdbuf();
    sfs.close();
    dfs.close();
}
