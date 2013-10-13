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
#ifndef _doublebufferedresource_h
#define _doublebufferedresource_h

#include "datatypes.h"

/// @file
/// @brief This file defines the template double buffered resources that can be attached to a thread.

namespace Mezzanine
{
    namespace Threading
    {
#ifndef SWIG
        /// @brief Double buffered resources are identified by a @ref Whole which is their type ID, This number identifies an invalid entry.
        const static Whole DBRInvalid = 10000000;

        /// @brief Double buffered resources are identified by a @ref Whole which is their type ID, This number identifies logging.
        const static Whole DBRLogger = 0;

        /// @brief Double buffered resources are identified by a @ref Whole which is their type ID, All System Type IDs will be less than this amount.
        /// @note Do not use the value of this directly, it is subject to change. In code using this, use: @code const static whole FreshDBRType = BDRUser+YourValue; @endcode
        const static Whole DBRUser = 1;
#endif
        /// @brief A thread specific resource that uses double buffering to avoid multithreaded synchronization mechanisms.
        /// @details It is intended for a Mezzanine::Threading::iWorkUnit "iWorkUnit" like the to provide asynchronous
        /// access to resources that to not need immediate atomicity. For example, it is ok if logs are gathered asynchronously
        /// and aggregated later as long no log messages are damaged, lost or taken out of order. This allows IO and other potentially
        /// expensive tasks to be used without blocking the current thread.
        template<typename T>
        class DoubleBufferedResource
        {
            private:
                /// @brief One of the resources to be buffered
                T* ResourceA;

                /// @brief The other of the resources to be buffered
                T* ResourceB;

            public:
                /// @brief A Constructor that creates default versions of the resources.
                /// @details This calls the default constructors for type T.
                DoubleBufferedResource() : ResourceA(0), ResourceB(0)
                {
                    try
                    {
                        ResourceA = new T;
                        ResourceB = new T;
                    }catch(std::exception& e){
                        if (ResourceA)
                            { delete ResourceA; ResourceA=0; }
                        if (ResourceB)
                            { delete ResourceB; ResourceB=0; }
                        throw e;
                    }

                }

                /// @brief A constructor that takes ownership of the the resources passed.
                /// @param Current This will be used as the first resource for threads to use.
                /// @param Buffer This will wait for the other thread
                DoubleBufferedResource(T* Current, T* Buffer) : ResourceA(Current), ResourceB(Buffer)
                    {}

                /// @brief Make the buffered resource the active and vice versa.
                void SwapUsableAndCommitable()
                    { std::swap(ResourceA,ResourceB); }

                /// @brief Get a pointer to the resource that can be used
                /// @return A pointer to the usable resource
                T* GetUsablePointer() const
                    { return ResourceA; }

                /// @brief Get a pointer to the resource that can be committed.
                /// @return A pointer to the commitable resource
                T* GetCommittablePointer() const
                    { return ResourceB; }

                /// @brief Get a reference to the resource that can be used
                /// @return A reference to the usable resource
                T& GetUsable()
                    { return *ResourceA; }

                /// @brief Get a reference to the resource that can be committed.
                /// @return A reference to the commitable resource
                T& GetCommittable()
                    { return *ResourceB; }

                /// @brief Destructor, this deletes both the resources managed. Regardless of how it got a hold of the pointers.
                ~DoubleBufferedResource()
                {
                    delete ResourceA;
                    delete ResourceB;
                }
        };//DoubleBufferedResource


        /// @brief A better default name for the Default Logger instance.
        typedef DoubleBufferedResource<std::stringstream> DoubleBufferedLogger;

        // forward declarations
        class FrameScheduler;

        /// @brief A thread specific collection of double-buffered and algorithm specific resources.
        /// @details This class is intended to be expanded via derived classes if it is to return extra resource types
        class MEZZ_LIB ThreadSpecificStorage
        {
            private:
                /// @brief Stores the pounters to the thread local resources.
                std::vector<ConvertiblePointer> ThreadResources;

                /// @brief A pointer to the FrameScheduler that this belongs to.
                FrameScheduler* Scheduler;

            public:
                /// @brief A constructor that automatically creates the resources it supports
                ThreadSpecificStorage(FrameScheduler* Scheduler_) :
                    Scheduler(Scheduler_)
                {
                    this->ThreadResources.push_back((ConvertiblePointer)new DoubleBufferedLogger);  // must comes first so it lands in spot 0
                }

                /// @brief Get a Specific kind of double buffered resource.
                /// @param ResourceID The ID the resource provided.
                /// @return A reference to the resource if it exists, or undefined behavior and maybe an exception if it doesn't.
                template <typename DBR>
                DBR& GetResource(const Whole& ResourceID)
                    { return *((DBR*)(this->ThreadResources[ResourceID])); }

                /// @brief Get the usable logger for this thread specific resource
                /// @return A reference to the logger.
                /// @note A function like this should be provided for any other resources added to derived versions of this class. This encourages getting resources for this thread, and not providing a method to get the commitable resource gently discourages doing it unless its really required.
                Logger& GetUsableLogger()
                    { return GetResource<DoubleBufferedLogger>(DBRLogger).GetUsable(); }

                /// @brief Get a pointer to the FrameScheduler that owns this resource.
                /// @details This is not required very often by application code, but is used
                /// in places in this scheduler library. This is primarily used to gain access
                /// to commitable parts of double buffered resources. This pointer does not
                /// confer ownership.
                /// @return A pointer to the FrameScheduler that owns this resource.
                FrameScheduler* GetFrameScheduler()
                    { return Scheduler; }

                /// @brief The commitable and usable version of every double buffered resource for this thread specific storage will be swapped.
                void SwapAllBufferedResources()
                    { GetResource<DoubleBufferedLogger>(DBRLogger).SwapUsableAndCommitable(); }

                /// @brief Deletes all the resources.
                virtual ~ThreadSpecificStorage()
                {
                    delete (DoubleBufferedLogger*)this->ThreadResources[DBRLogger]; // Items must wind up in the correct spot in the vector for this to work. Be careful to get the order right if you overload this.
                }

        };//ThreadSpecificStorage

        /// @brief Use this to change the default resource type.
        /// @details use "DefaultThreadSpecificStorage::Type" as Type to instantiate an instance of whatever type this is.
        struct DefaultThreadSpecificStorage
        {
            /// @brief A single point at which all the thread specific resources can be changed.
            typedef ThreadSpecificStorage Type;
        };//DefaultThreadSpecificStorage
    }//Threading
}//Mezzanine

#endif
