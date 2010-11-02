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
#include <vector>
#include <fstream>
#include <cstdlib>

#include <stdio.h>      /* defines FILENAME_MAX */
#include <sys/stat.h>   // These should work with mingw, but may not with visual c
#include <sys/types.h>

using namespace std;

// Which platform is this being built on? (remark all wrong answers)
// If you are using the code::blocks project file, then this is selected using the "Build Target" Pull down menu
//#define WINDOWSCOMPATIBLEOS
//#define GNUCOMPATIBLEOS

//Some Error codes
#define E_BADSOURCEDATA 1
#define E_BADCOPFILECONTENTS 2
#define E_BADARGS 3
#define E_MISSINGDIRLIST 100
#define E_MISSINGFILELIST 101
#define E_FAILCOPYFILE 200
#define E_FAILDIRCREATION 201
#define E_FAILCOPYALL 202
#define E_FAILREADFILE 203
#define E_FAILWRITEFILE 204
#define E_FAILSPECIFICCOPY 205

// Globals aren't bad if they aren't used badly.
// If this tool gets over 1000 LOCs or sorted into multiple files this should change like maybe into a config file
string TargetDir("bin");                            //Has more data appended later
string RootSourceDir("data");                       //Has more data appended later
string SourceDir("");                               //is generated later
string DirList("makedirlist.txt");
string FileList("copyfilelist.txt");
string SpecificCopyScript("copyspecificfiles");     //Has extension appended later

#ifdef WINDOWSCOMPATIBLEOS
    #include <direct.h>
    #define GetCurrentDir _getcwd
    #define MKDIR(x)    mkdir(x)
    string Slash("\\");
    string ScriptExt(".bat");
#endif

#ifdef GNUCOMPATIBLEOS
    #include <unistd.h>
    #define GetCurrentDir getcwd
    #define MKDIR(x)    mkdir(x,777)
    string Slash("/");
    string ScriptExt(".sh");
#endif

//Utility functions
string Usage(string BaseName);
void Tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ");
string StringReplace(string Needle, string Haystack, string NewNeedle);
string FixSlashes(const string& FileName);

//File Manipulation Functions
bool FileDoesExistIsItADirectory(string Filename);
bool FileDoesExist(string Filename);
void CopyFile(string Source, string Destination);
void MakeDirTree(string Dirs);
void MakeDir(string OneDirDeep);

//Application Logic
void MakeTargetDirs();
void CopyAllFiles();
void RunSpecificCopyScript();

int main(int ArgCount, char** ArgValues)
{
    if(3 == ArgCount)
    {
        //Lets prep the values we will use the rest of the program
        cout << ArgValues[0] << endl ;
        char WorkingDirectory[FILENAME_MAX];
        cout << GetCurrentDir(WorkingDirectory, sizeof(WorkingDirectory)) <<endl ;
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

            // make directories in make list
            MakeTargetDirs();

            //Copy files in copylist
            CopyAllFiles();

            // Call
            RunSpecificCopyScript();

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

// This Function will return a string that has how to use this tool stored in it
// This functions requires the first commandline argument passed in, or the name of the binary to be called.
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

// This search for delimiters in str and puts the strings that are between the delimiters in str and puts them in tokens as their own strings.
// the actual return of this function is tokens and it will be a vector of strings
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

// This will search for each char in Needle in the HayStack and replace them with the entire string in NewNeedle) then returns the results
// Do not use the Multicharacter replacement in the Needle. Eventually this string will perform string replacement
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

//does the Filename exist if no return that; if yes, is it a directory, and return that
bool FileDoesExistIsItADirectory(string Filename)
{
    struct stat Results;
    //0 on success :( the old c api was really stupid
    if( 0==stat(Filename.c_str(),&Results) )
    {
        //File exists, now is it a Dir
        if( S_ISDIR(Results.st_mode) )
        {
            cout << Filename << " is a Directory" << endl;
            return true;
        }
        cout << Filename << " is not a Directory" << endl;
    }else{
        cout << Filename << " is nothing at all" << endl;
    }
    return false;
}

// return whether or not Filename exists
bool FileDoesExist(string Filename)
{
    struct stat Results;
    if( 0==stat(Filename.c_str(),&Results) )
        { return true; }
    return false;
}

// Make a subdirectory and all of it's parent directories
void MakeDirTree(string Dirs)
{
    Dirs = FixSlashes(Dirs);
    vector<string> Tokens;
    Tokenize(Dirs, Tokens, Slash);
    stringstream Depth;

    for ( vector<string>::iterator iter=Tokens.begin();  iter!=Tokens.end(); ++iter )
    {
        Depth << *iter;
        MakeDir(Depth.str());
        Depth << Slash;
    }
}

//Make a directory. All of the parent directories must be present
void MakeDir(string OneDirDeep)
{
    OneDirDeep = FixSlashes(OneDirDeep);
    if (FileDoesExistIsItADirectory(OneDirDeep))
        { return; } //No need to Fuss over an already existing folder


//    #ifdef WINDOWSCOMPATIBLEOS
    if ( 0== MKDIR(OneDirDeep.c_str()) ) //Success on 0
        { return; }
    else
        { cerr << "System Error: " << errno << " - ";}
/*    #endif

    #ifdef GNUCOMPATIBLEOS
    if (0==mkdir(OneDirDeep.c_str(),0777)) //Success on 0
        { return; }
    #endif*/

    //We expect the OS specific blocks to return
    cerr << "Could not make Directories: " << OneDirDeep << endl;
    exit(E_FAILDIRCREATION);
}

//Make all the directories required in the Make Directory Listing
void MakeTargetDirs()
{
    ifstream DirCreationList(DirList.c_str());
    while (!DirCreationList.eof())
    {
        char Line[FILENAME_MAX];
        DirCreationList.getline(Line,sizeof(Line));
        if(DirCreationList.fail())
        {
            if(DirCreationList.eof())
                { break; }
            cerr << "Creating Target Directories fails, add more code to find out why." << endl;
            exit(E_FAILDIRCREATION);
        }
        stringstream DirToMake;
        DirToMake << TargetDir << Slash << Line;
        MakeDirTree(DirToMake.str());
    }
}

// Copy all the files as instructe by the copy file list
void CopyAllFiles()
{
    ifstream CopyListFile(FileList.c_str());

    while (!CopyListFile.eof())
    {
        //get line from files
        char Line[FILENAME_MAX]; //anything longer than this is crazy
        CopyListFile.getline(Line,sizeof(Line));
        if(CopyListFile.fail())
        {   //in case it fucks up getting it from the files
            if(CopyListFile.eof())
                { break; }
            cerr << "Reading the copy file list mysteriously failed, add more code to copyallfiles to find out why." << endl;
            exit(E_FAILCOPYALL);
        }

        string AdjustedLine(FixSlashes(string(Line)));
        AdjustedLine = StringReplace("?", AdjustedLine, TargetDir);
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

// Read from the Source files contents and put it in the Destination file
void CopyFile(string Source, string Destination)
{
    if ( !FileDoesExist(Destination) )
    {
        cout << Source << " -> " << Destination << endl;
    }else{
        cout << Source << " Not copied, " << Destination << " Already exists" << endl;
    }

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

//Runs the script that should copy the system dependent files
void RunSpecificCopyScript()
{
    stringstream ScriptName;
    ScriptName << SourceDir << Slash << SpecificCopyScript << ScriptExt << " \"" << TargetDir << "\"";
    cout << ScriptName.str() <<endl;
    if ( system(ScriptName.str().c_str()) )
    {
        cerr << "Unknown Error Calling: " << ScriptName.str() << endl;
        exit(E_FAILSPECIFICCOPY);
    }
}

//Makes all of the slashes the system specific slashes for the filesystem
string FixSlashes(const string& FileName)
{
    string Results;
    Results=StringReplace(string("/"), FileName, Slash);
    Results=StringReplace(string("\\"), Results, Slash);
    return Results;
}
