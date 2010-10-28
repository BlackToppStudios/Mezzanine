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

#include <sys/stat.h> // This should work with mingw, but may not with visual c

using namespace std;

// Which platform is this being built on? (remark all wrong answers)
// Make this an option by selecting the pulldown menu
//#define WINDOWS 10000
#define GNU 10000

//Some Error codes
#define E_BADSOURCEDATA 1
#define E_MISSINGDIRLIST 2
#define E_MISSINGFILELIST 3
#define E_BADARGS 65

// Globals aren't bad if they aren't used badly.
// If this tool gets over 1000 LOCs or sorted into multiple files this should change like maybe into a config file
string Usage(string BaseName);
string TargetDir("bin/");
string RootSourceDir("data/");
string SourceDir("");
string DirList("makedirlist.txt");
string FileList("copyfilelist.txt");

//File Manipulation Functions
bool FileDoesExistIsItADirectory(string Filename);
bool FileDoesExist(string Filename);
void CopyFile(string Source, string Destination);
void CopyAllFiles();
void MakeDirTree(string Dirs);

int main(int ArgCount, char** ArgValues)
{
    if(3 == ArgCount)
    {
        //Lets prep the values we will use the rest of the program
        stringstream Temp;
        Temp << TargetDir << ArgValues[1];
        TargetDir = Temp.str();
        Temp.str("");
        Temp << RootSourceDir << ArgValues[2];
        SourceDir = Temp.str();

        cout <<TargetDir<<endl<<SourceDir<<endl;
        //string Arg1(ArgValues[1]);
        //string Arg2(ArgValues[2]);
        if (FileDoesExistIsItADirectory(SourceDir))
        {
            if (!FileDoesExistIsItADirectory(SourceDir))
            {
                // If not present make target directory and continue
            }

            //check for filelist
            //check for dirlist
            // now that we know it is present copy the files

        }else{
            cout << "Source Directory \"" << SourceDir << " fails" << endl;
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
            << "CopyFromName is the set of source files to use. \"linux\", \"macosx\", \"windows\", ..." << endl;
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
    #ifdef WINDOWS
    //system()
    #endif

    #ifdef GNU

    #endif
}

void CopyAllFiles()
{
    #ifdef WINDOWS
    //system()
    #endif

    #ifdef GNU

    #endif
}


void CopyFile(string Source, string Destination)
{
    #ifdef WINDOWS
    //system()
    #endif

    #ifdef GNU

    #endif
}
