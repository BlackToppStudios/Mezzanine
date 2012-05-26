//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
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
#ifndef _smartptr_h
#define _smartptr_h

#include "datatypes.h"

#include <memory>

#ifdef MEZZDEBUG
#include "world.h"
#include <iostream>
#endif

/// @file This file describes and implements any pointers or typedefs to pointers that the Mezzanine uses and exposes in its API

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This exists once per object managed by a group of shared pointer to track items in memory.
    /// @details This exists to track the pointer to the managed object, and stores the single
    /// counter of existing references. Only one of these should be created for each group of
    /// pointers managing the same object.
    /// @note It should be extremely rare to need to create one of these outside of pointer implemenation.
    ///////////////////////////////////////////////////////////////////////////////
    template <class TypePointedTo> struct ReferenceCounter
    {
        /// @brief Constructor
        /// @param InitialPointer A pointer to the type of this template. This defaults to 0 if not provided
        /// @param InitialCount The number of references to default to this defaults to 1 if not provided.
        ReferenceCounter(TypePointedTo* InitialPointer = 0, unsigned InitialCount = 1)
            : ptr(InitialPointer), Count(InitialCount)
            {}

        /// @brief  The Pointer that is the crux of this class, At this smart pointer's core there must be a raw pointer.
        TypePointedTo*          ptr;

        /// @brief This is the Counter that stores how many references exist
        Whole Count;
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A single threaded referencing counting pointer.
    /// @details This is a pointer that automatically deallocates the object it manages when
    /// all counted_ptr intances managing it are destroyed or fall out of scope. This is a
    /// simpler version of std::shared_ptr.
    /// @warning This is not thread safe in any way.
    /// @note The basis of this class originated externally, please see the counted pointer
    /// from http://ootips.org/yonat/4dev/smart-pointers.htm0 which came with written permission
    /// for use stated as "Feel free to use my own smart pointers in your code" on that page.
    ///////////////////////////////////////////////////////////////////////////////
    template <class TypePointedTo> class CountedPtr
    {
        private:

            /// @brief This is the only data on this class, a pointer to the counter and the managed object.
            ReferenceCounter<TypePointedTo> *itsCounter;

        protected:

            /// @brief Used when Creating a new CountedPtr but the reference already exists
            /// @param CounterToAcquire The ReferenceCounter that this pointer will use
            /// @warning This does not Release the previous Reference counter. This means it is possible leak memory if a ReferenceCounter is acquired that differs from the previous one without plans to manage the original.
            void Acquire(ReferenceCounter<TypePointedTo>* CounterToAcquire)
            {
                #ifdef MEZZDEBUG
                std::ostream *Log;
                if (World::GetWorldPointerValidity())
                    { Log = & (World::GetWorldPointer()->LogStream); }
                else
                    { Log = & std::cout; }
                *Log << "Acquiring ReferenceCounter, previous counter " << (!itsCounter?"did not exist.":("had "+ToString(itsCounter->Count)+" references.")) << std::endl;
                #endif

                itsCounter = CounterToAcquire;

                #ifdef MEZZDEBUG
                *Log << "ReferenceCounter assigned, reference count is now " << (!itsCounter?"invalid.":(ToString(itsCounter->Count)+".")) << std::endl;
                #endif

                if (CounterToAcquire) ++CounterToAcquire->Count; // I am concerned that some platform somewhere will try to increment the struct pointer, then dereference it.

                #ifdef MEZZDEBUG
                *Log << "ReferenceCounter incremented, reference count is now " << (!itsCounter?"invalid.":(ToString(itsCounter->Count)+".")) << std::endl;
                #endif
            }

            /// @brief This decrements the reference count and deletes the managed items if there are no remaining reference
            void Release()
            {
                #ifdef MEZZDEBUG
                std::ostream *Log;
                if (World::GetWorldPointerValidity())
                    { Log = & (World::GetWorldPointer()->LogStream); }
                else
                    { Log = & std::cout; }
                *Log << "Releasing ReferenceCounter, counter " << (!itsCounter?"did not exist.":("had reference count of "+ToString(itsCounter->Count)+".")) << std::endl;
                #endif

                if (itsCounter) {
                    if (--itsCounter->Count == 0) {
                        #ifdef MEZZDEBUG
                        *Log << "ReferenceCounter deleted and " << (!itsCounter?"did not exist.":("had reference count of "+ToString(itsCounter->Count)+" after decrementing.")) << std::endl;
                        #endif
                        delete itsCounter->ptr;
                        delete itsCounter;
                    }
                    #ifdef MEZZDEBUG
                    else { *Log << "ReferenceCounter removed and " << (!itsCounter?"did not exist.":("had reference count of "+ToString(itsCounter->Count)+".")) << std::endl; }
                    #endif

                    itsCounter = 0;
                }
                #ifdef MEZZDEBUG
                else { *Log << "ReferenceCounter not deleted or removed and " << (!itsCounter?"did not exist.":("had "+ToString(itsCounter->Count)+"references.")) << std::endl; }
                #endif


            }

        public:
            /// @brief This makes referencing the type of the point object easier.
            typedef TypePointedTo element_type;

            /// @brief Initial Constructor
            /// @details This should only be used for initial creation of a shared pointer group. This will allocate the raw
            /// pointer and the ReferenceCounter that will be used to track the pointer passed. This will only be explicitly
            /// called to prevent accidental premature deletion of the item managed.
            /// @n @n It is quite easy to accidentally
            /// make a secondary grup of counted pointers if not using the new statement inline with this constructor, and it is
            /// not recomended to use this in any other way. Here is an exmple of the reccomended way to use new inline with this:
            /// "Mezzanine::CountedPtr<Mezzanine::Vector3> VecPtr (new Mezzanine::Vector3);"
            /// @param PointerTarget The item that will be deleted once all the pointer of this group disappear.
            explicit CountedPtr(TypePointedTo* PointerTarget = 0) : itsCounter(0)
                {if (PointerTarget) itsCounter = new ReferenceCounter<TypePointedTo>(PointerTarget);}

            /// @brief Deconstructor, Just calls Release().
            ~CountedPtr()
                {Release();}

            /// @brief Copy constructor
            /// @param Original The pointer being copied. This fresh pointer will use the same ReferenceCounter as the original.
            CountedPtr(const CountedPtr& Original) : itsCounter(NULL)
                {Acquire(Original.itsCounter);}

            /// @brief Get the current count of references.
            /// @note This name was chosen to match standard compliant names, and should be usable in templates that require this function.
            /// @return The amount of reference which still exist, or 0 if the reference counter is somehow invalid.
            Whole use_count()
            {
                if (itsCounter)
                    { return itsCounter->Count; }
                else
                    { return 0; }
            }

            CountedPtr& operator=(const CountedPtr& r)
            {
                if (this != &r) {
                    Release();
                    Acquire(r.itsCounter);
                }
                return *this;
            }

            //template <class Y> friend class Mezzanine::counted_ptr<Y>;
            template <class Y> CountedPtr(const CountedPtr<Y>& r) throw()
                {Acquire(r.itsCounter);}
            template <class Y> CountedPtr& operator=(const CountedPtr<Y>& r)
            {
                if (this != &r) {
                    Release();
                    Acquire(r.itsCounter);
                }
                return *this;
            }

            TypePointedTo& operator*()  const throw()   {return *itsCounter->ptr;}
            TypePointedTo* operator->() const throw()   {return itsCounter->ptr;}
            TypePointedTo* get()        const throw()   {return itsCounter ? itsCounter->ptr : 0;}
            bool unique()   const throw()
                {return (itsCounter ? itsCounter->count == 1 : true);}


    };


} // \Mezzanine

#endif // _smartptr_h
