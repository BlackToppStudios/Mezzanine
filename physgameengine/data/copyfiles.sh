#!/bin/bash
#© Copyright 2010 BlackTopp Studios Inc.
# This file is part of The PhysGame Engine and is used during the build process to copy files.
#
#    The PhysGame Engine is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    The PhysGame Engine is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
#
# The original authors have included a copy of the license specified above in the
#   'Docs' folder. See 'gpl.txt'
#
# We welcome the use of The PhysGame anyone, including companies who wish to
#   Build professional software and charge for their product.
#
#   However there are some practical restrictions, so if your project involves
#   any of the following you should contact us and we will try to work something
#   out:
#    - DRM or Copy Protection of any kind(except Copyrights)
#    - Software Patents You Do Not Wish to Freely License
#    - Any Kind of Linking to Non-GPL licensed Works
#    - Are Currently In Violation of Another Copyright Holder's GPL License
#    - If You want to change our code and not add a few hundred MB of stuff to
#        your distribution
#
#   These and other limitations could cause serious legal problems if you ignore
#   them, so it is best to simply contact us or the Free Software Foundation, if
#   you have any questions.
#
#   Joseph Toppi - toppij@gmail.com
#   John Blackwood - makoenergy02@gmail.com

if [ ! -f "data/copyfiles" ]
then
    g++ -DGNUCOMPATIBLEOS ../tools/CopyFilesTool/main.cpp -o data/copyfiles
fi
data/copyfiles $1 $2
exit 0

#Define some error codes
E_BADSOURCEDATA=1
E_MISSINGDIRLIST=2
E_MISSINGFILELIST=3
E_BADARGS=65

#Assign some values we may want to change in the future
DirList="makedirlist.txt"
FileList="copyfilelist.txt"

TargetDir="bin/$1/"
RootSourceDir="data/"
SourceDir="$RootSourceDir$2/"

#check that we have exactly 2 arguments
# The first argument is the name of the Build Process "LinuxDebug", "LinuxRelease", "WinDebug", ...
if [ ! -n "$1" ]
then
	echo "Usage: `basename $0` BuildName CopyFromName"
	exit $E_BADARGS
else
	mkdir "$TargetDir" -p
fi

# The second argument is the set of source files to use. "linux", "macosx", "windows", ...
if [ ! -n "$2" ]
then
	echo "Usage: `basename $0` BuildName CopyFromName"
	exit $E_BADARGS
else
	if [ ! -d "$SourceDir" ]
	then
		echo "Directory $SourceDir does no exist no place to get data from."
		exit $E_BADSOURCEDATA
	else
		mkdir "$TargetDir$SourceDir" -p
	fi
fi

# Check that we have the file listings we need
if [ ! -f "$RootSourceDir$DirList" ]
then
	echo "Missing Directory Listing to Create: $RootSourceDir$DirList"
	exit $E_MISSINGDIRLIST
fi

if [ ! -f "$RootSourceDir$FileList" ]
then
	echo "Missing Directory Listing to Create: $RootSourceDir$FileList"
	exit $E_MISSINGFILELIST
fi

#Make all the required directories to copy files too
while read Line
do
	mkdir "$TargetDir$Line" -p
done <"$RootSourceDir$DirList"

#Make all the planned file copies and do the work
while read Line
do
	cp ${Line/Target\//$TargetDir}
done <"$RootSourceDir$FileList"

#run the next platform specific file copy.

"$SourceDir"copyspecificfiles.sh $1
