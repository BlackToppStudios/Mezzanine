// The DAGFrameScheduler is a Multi-Threaded lock free and wait free scheduling library.
// © Copyright 2010 - 2013 BlackTopp Studios Inc.
/* This file is part of The DAGFrameScheduler.

    The DAGFrameScheduler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The DAGFrameScheduler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The DAGFrameScheduler.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'doc' folder. See 'gpl.txt'
*/
/* We welcome the use of the DAGFrameScheduler to anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef _asynchronousfileloadingworkunit_cpp
#define _asynchronousfileloadingworkunit_cpp

#include "asynchronousfileloadingworkunit.h"
#include "atomicoperations.h"

#include <iterator>

/// @file
/// @brief The implemtation of the @ref Mezzanine::Threading::AsynchronousFileLoadWorkUnit "AsynchronousFileLoadWorkUnit" a workunit that loads a listing of files asynchronously.

namespace Mezzanine
{
    namespace Threading
    {
        void ThreadLoading(void* WU)
        {
            AsynchronousFileLoadWorkUnit& Unit = *((AsynchronousFileLoadWorkUnit*)WU);
            try{
                Unit.FilesRaw.reserve(Unit.Filenames.size());
            }catch (std::exception&){
                while(Unit.Status!=AtomicCompareAndSwap32(&(Unit.Status), Unit.Status, Failed));
                return;
            }
            for(std::vector<String>::iterator CurrentFileName = Unit.Filenames.begin(); CurrentFileName!=Unit.Filenames.end(); CurrentFileName++)
            {
                std::ifstream File(CurrentFileName->c_str(), std::ios::binary | std::ios::ate);
                if (File)
                {
                    MaxInt FileLength = File.tellg();
                    File.seekg(0,std::ios::beg);

                    RawFile* Loading;
                    try{
                        Loading = new RawFile(FileLength);
                    }catch (std::exception&){
                        while(Unit.Status!=AtomicCompareAndSwap32(&(Unit.Status), Unit.Status, Failed));
                        return;
                    }

                    File.read((char*)Loading->Data, FileLength);
                    Unit.FilesRaw.push_back(Loading);
                }else{
                    while(Unit.Status!=AtomicCompareAndSwap32(&(Unit.Status), Unit.Status, Failed));
                    break;
                }

            }
            while(Unit.Status!=AtomicCompareAndSwap32(&(Unit.Status), Unit.Status, Complete));
        }

        AsynchronousFileLoadWorkUnit::AsynchronousFileLoadWorkUnit() :
            LoadingThread(0),
            Status(NotStarted)
        {}

        AsynchronousFileLoadWorkUnit::~AsynchronousFileLoadWorkUnit()
            { DeleteLoadedFiles(); }

        RunningState AsynchronousFileLoadWorkUnit::BeginLoading(const std::vector<String>& Filenames_)
        {
            if(Running!=AtomicCompareAndSwap32(&Status, Status, Running))
            {
                //DeleteLoadedFiles();
                FilesRaw.clear();
                Filenames = Filenames_;
                LoadingThread = new Thread(ThreadLoading, this);
                return Starting;
            }else{
                //throw?
                return (RunningState)Status;
            }
        }

        void AsynchronousFileLoadWorkUnit::DoWork(DefaultThreadSpecificStorage::Type &)
        {
            if(Complete==Status&&0!=LoadingThread)
            {
                LoadingThread->join();
                delete LoadingThread;
                LoadingThread=0;
            }
        }

        RunningState AsynchronousFileLoadWorkUnit::IsWorkDone()
            { return (RunningState)Status; }

        RawFile* AsynchronousFileLoadWorkUnit::GetFile(const String& FileName) const
        {
            Whole Index=0;
            for(std::vector<String>::const_iterator CurrentFileName = Filenames.begin();
                CurrentFileName!=Filenames.end();
                CurrentFileName++, Index++)
            {
                if(*CurrentFileName==FileName)
                    { return GetFile(Index); }
            }
            return 0;
        }

        RawFile* AsynchronousFileLoadWorkUnit::GetFile(const Whole& Index) const
        {
            if(FilesRaw.size()<=Index)
                { return 0; }
            return FilesRaw[Index];
        }

        void AsynchronousFileLoadWorkUnit::DeleteLoadedFiles()
        {
            if(LoadingThread)
                { delete LoadingThread; }
            for(RawFileContainer::iterator Iter=FilesRaw.begin(); Iter!=FilesRaw.end(); ++Iter)
                { delete *Iter; }
        }
    } // \Threading
} // \Mezzanine
#endif
