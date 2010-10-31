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
#define E_FAILCOPYFILE 4
#define E_FAILDIRCREATIO 5
#define E_FAILCOPYALL 6
#define E_FAILREADFILE 8
#define E_FAILWRITEFILE 9
#define E_BADCOPFILECONTENTS 10
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
string StringReplace(string Needle, string Haystack, string NewNeedle);

//File Manipulation Functions
bool FileDoesExistIsItADirectory(string Filename);
bool FileDoesExist(string Filename);
void CopyFile(string Source, string Destination);
void CopyAllFiles();
void MakeDirTree(string Dirs);
void MakeDir(string OneDirDeep);
void MakeTargetDirs();

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

            MakeTargetDirs();
            // TODO Need to create all dirs here

            // now that we know everything we need is present copy the files
            CopyAllFiles();
        }else{
            cerr << "Source Directory \"" << SourceDir << "\" fails" << endl;
            return E_BADSOURCEDATA;
        }

    }else{
        cerr << Usage(ArgValues[0]);
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

string StringReplace(string Needle, string Haystack, string NewNeedle)
{
    vector<string> Tokens;
    Tokenize(Haystack, Tokens, Needle);
    stringstream Results;
    //ut << "Needle: " << Needle << endl << "Haystack: " << Haystack << endl << "NewNeedle: " << NewNeedle << endl;
    bool FirstTime=true;
    for ( vector<string>::iterator iter=Tokens.begin();  iter!=Tokens.end(); ++iter )
    {
        if(FirstTime)
        {
            Results << *iter;
            FirstTime=false;
        }else{
            Results << NewNeedle << *iter;
        }
    }

    return Results.str();
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
        { return; } //No need to Fuss over an already existing folder

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

void MakeTargetDirs()
{
    ifstream DirCreationList(DirList.c_str());
    while (!DirCreationList.eof())
    {
        char Line[256]; //anything longer than this is crazy
        DirCreationList.getline(Line,256);
        if(DirCreationList.fail())
        {
            if(DirCreationList.eof())
                { break; }
            cerr << "Copying Files mysteriously failed, add more code to copy to find out why." << endl;
            exit(E_FAILDIRCREATIO);
        }
        stringstream DirToMake;
        DirToMake << TargetDir << Slash << Line;
        MakeDirTree(DirToMake.str());
    }
}

void CopyAllFiles()
{
    ifstream CopyListFile(FileList.c_str());

    while (!CopyListFile.eof())
    {
        //get line from files
        char Line[256]; //anything longer than this is crazy
        CopyListFile.getline(Line,256);
        if(CopyListFile.fail())
        {   //in case it fucks up getting it from the files
            if(CopyListFile.eof())
                { break; }
            cerr << "Copying Files mysteriously failed, add more code to copyallfiles to find out why." << endl;
            exit(E_FAILCOPYALL);
        }
        //Clean up the lines
        string AdjustedLine(StringReplace(string("?"), string(Line), TargetDir));
        AdjustedLine=StringReplace(string("/"), AdjustedLine, Slash);
        AdjustedLine=StringReplace(string("\\"), AdjustedLine, Slash);

        vector<string> Files;
        Tokenize(AdjustedLine, Files, "|");
        if(Files.size() == 2)
        {
//            stringstream DestStaging;
//            DestStaging << Files[1] << Slash;
            CopyFile(Files[0],Files[1]);
        }else{
            cerr << "Bad " << FileList << "too many pipes in filename";
            exit(E_BADCOPFILECONTENTS);
        }
    }

    CopyListFile.close();
}

void CopyFile(string Source, string Destination)
{
    cout<<Source << "||||" <<Destination<<endl;
    ifstream sfs(Source.c_str(), std::ios::binary);
    ofstream dfs(Destination.c_str(), std::ios::binary);
    dfs << sfs.rdbuf();
    if( sfs.fail() )
    {
        cerr << "Error reading Source file: " << Source << endl;
        exit(E_FAILREADFILE);
    }

    if( dfs.fail() )
    {
        cerr << "Error writing Destination file: " << Destination << endl;
        exit(E_FAILWRITEFILE);
    }
    //sfs.close();
    //dfs.close();
}
