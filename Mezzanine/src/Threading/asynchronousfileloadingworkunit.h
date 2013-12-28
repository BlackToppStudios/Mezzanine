// The DAGFrameScheduler is a Multi-Threaded lock free and wait free scheduling library.
// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _asynchronousfileloadingworkunit_h
#define _asynchronousfileloadingworkunit_h

#if !defined(SWIG) || defined(SWIG_THREADING) // Do not read when in swig and not in the threading module
#include "asynchronousworkunit.h"
#endif

/// @file
/// @brief The declaration of the @ref Mezzanine::Threading::AsynchronousFileLoadWorkUnit "AsynchronousFileLoadWorkUnit" a workunit that loads a listing of files asynchronously.
namespace Mezzanine
{
    namespace Threading
    {
#ifndef SWIG
        /// @brief An Internal helper function for the @ref AsynchronousFileLoadWorkUnit. This is the function that AsynchronousFileLoadWorkUnit instances will use to load data.
        /// @details There is a 1 to 1 relationship between this function and its associated @ref Mezzanine::Threading::AsynchronousFileLoadWorkUnit "AsynchronousFileLoadWorkUnit".
        /// This is the function run in another thread to allow loading asynchrously.
        /// @param WU A pointer to the @ref Mezzanine::Threading::AsynchronousFileLoadWorkUnit "AsynchronousFileLoadWorkUnit" that is passed to the asynchronous thread to be used as metadata.
        void ThreadLoading(void* WU);

        /// @brief A simple in memory representation of a file.
        class MEZZ_LIB RawFile
        {
            public:
                /// @brief How big is the file in bytes?
                MaxInt Size;

                /// @brief A pointer to a block of memory as it was loaded raw from the file.
                UInt8* Data;

                /// @brief Constructor
                /// @details Allocates space of the amount passed. In the @ref Mezzanine::Threading::AsynchronousFileLoadWorkUnit "AsynchronousFileLoadWorkUnit"
                /// this is the size of the file to be loaded.
                /// @param BufferSize The size of the buffer to allocate.
                RawFile(const MaxInt BufferSize) : Size(BufferSize)
                    { Data = new UInt8[BufferSize]; }

                /// @brief De-allocates the memory for the file.  This make deleting Data manually a bad idea.
                ~RawFile()
                {
                    if(Data)
                        { delete[] Data; }
                }

            private:
                /// @brief Copying this class is a bad idea, private copy constructor
                RawFile(RawFile&) {}

                /// @brief Assigning this class is bad, private operator=.
                /// @param Unused Like it says it is unused in any real sense, it is returned to prevent compiler warnings.
                /// @return Returns *this.
                RawFile& operator=(RawFile& Unused) { return Unused; }
        };//RawFile
#endif

        /// @brief This is intended to load files asynchronously and continue doing so whether or not other the @ref FrameScheduler is running or paused.
        /// @details The goal is to trigger lower level mechanisms to use DMA to load directly without CPU intervention. In some operating systems this
        /// is handled in the standard library, and in others special mechanisms must be used to make this happen. The current state of this allows
        /// testing to determine how good automatic mechanisms work.
        class MEZZ_LIB AsynchronousFileLoadWorkUnit : public iAsynchronousWorkUnit
        {
#ifndef SWIG
            /// @cond false
            // If only there was a way to pass member function to the thread constructor.
            friend void ThreadLoading(void* WU);
            /// @endcond
#endif
            protected:
                /// @brief The names of the files this batch of loading will retrieve.
                std::vector<String> Filenames;

                /// @brief Used to make referencing the the type of @ref FilesRaw easier.
                typedef std::vector<
                            RawFile*
                        > RawFileContainer;

                /// @brief The contents of the files once they have been loaded. No partial files will exists here, either the whole file is added or nothing is added.
                RawFileContainer FilesRaw;

                /// @brief This is either 0 or the other thread that is loading.
                Thread* LoadingThread;

                /// @brief This stores the current RunningState.
                Int32 Status;

            public:
                /// @brief Default constructor.
                AsynchronousFileLoadWorkUnit();

                /// @brief Destructor, deletes all loaded files.
                virtual ~AsynchronousFileLoadWorkUnit();

                /// @brief Begin loading a list of files based on their names.
                /// @param Filenames_ A vector of Strings that correspond to either relative or absolute filenames that will be loaded.
                /// @details When the file is loaded its contents are placed in a @ref RawFile and can be retrieved with @ref GetFile
                /// or @ref GetFile member functions.
                /// @n @n
                /// This starts by clearing the previous list of loaded files. If that list has not been deleted then
                /// this will cause a memory leak. Use @ref DeleteLoadedFiles to clear this list or copy all the pointers elsewhere before
                /// calling this and delete them manually at your convience. When this work unit is destroyed it deletes any loaded files from
                /// the last call of this method.
                /// @n @n
                /// If this is called while it is loading files the behavior is undefined, most likely it will crash or fail silently, either
                /// way no good can come from it. Use @ref IsWorkDone to see if the work is @ref Complete "Complete". If the work is complete
                /// then this can be called after any call to this class's @ref DoWork member function.
                /// @warning This can leak memory if not used in conjuction with memory management or @ref DeleteLoadedFiles and read the details.
                /// @warning This can fail in horrible ways if called twice without waiting for the first call to finish, use @ref IsWorkDone and read the details.
                /// @return This returns @ref Starting "Starting" when loading is successful started and who knows what it returns if it fails, likely a segfault or GPF.
                RunningState BeginLoading(const std::vector<String>& Filenames_);

                /// @brief This checks if Asynchronous loading thread has completed and if so it cleans up that threads' resources.
                virtual void DoWork(DefaultThreadSpecificStorage::Type&);

                /// @brief Get the @ref RunningState of the file loading
                /// @details This can return any @ref RunningState and the meaning applies to the current state of the files being loaded.
                ///     - @ref NotStarted "NotStarted" - Either @ref BeginLoading has not been called or it has been called and has not progressed to the point where it sets this. Not useful for making logical decisions without knowledge of how many times @ref BeginLoading has been called.
                ///     - @ref Starting "Starting" - This is returned from @ref BeginLoading is success conditions, and is never returned from this.
                ///     - @ref Running "Running" - This means that the loading thread has started but not yet completed.
                ///     - @ref Complete "Complete" - The loading thread has completed its work, and new loading can definitely begin after the next call to @ref DoWork method.
                ///     - @ref Failed "Failed" - If somekind of recoverable error occured, this will be returned, there are no guarantees about the state of the loaded files.
                /// @return The running state as of the time of this call, though it is subject to immediate thread unsafe change.
                virtual RunningState IsWorkDone();

                /// @brief Get a loaded @ref RawFile in linear time.
                /// @details This searches the list of files names to determine the index of the filename then calls @ref GetFile "GetFile".
                /// @param FileName The file to retrieve.
                /// @return A pointer to a @ref RawFile or 0 if it cannot be retrieved for any reason.
                RawFile* GetFile(const String& FileName) const;

                /// @brief Retrieve a pointer to file contents in constant time.
                /// @param Index The index of the File Name when it was passed into @ref BeginLoading "BeginLoading".
                /// @return This returns a null pointer if the correct @ref RawFile cannot be retrieved, otherwise a pointer to the @ref RawFile with index corresponing to the index of a passed in filename is returned.
                /// @warning Do no call this before @ref IsWorkDone() indicates that the work is @ref Complete "Complete". These memory addresses are subject to change until all loading is complete.
                RawFile* GetFile(const Whole& Index) const;

                /// @brief This deletes all the loaded files from the last call of @ref BeginLoading "BeginLoading" .
                void DeleteLoadedFiles();
        };//AsynchronousFileLoadWorkUnit
    } // \Threading
}// \Mezzanine

#endif
