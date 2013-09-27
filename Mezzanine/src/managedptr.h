// © Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _managedptr_h
#define _managedptr_h

/// @file
/// @brief The interface for a class useful for applying RAII when it should have been used but wasn't.

#include "datatypes.h"


namespace Mezzanine
{
    /// @brief Sample implementation for a ManagedPtr
    /// @details This one is pretty useless, and serves as just an example. Any
    /// class that implements at least the member function this implements can be used
    /// with the ManagedPtr to get smart pointer semantics.
    class MEZZ_LIB IntHandle
    {
        public:
            /// @brief This is type ManagedPtr will use to work with a handle
            typedef int* TargetPtrType;

            /// @brief The ManagedPtr never access objects directly, so this is just an implementation detail
            TargetPtrType Pointer;

            /// @brief This is expected to leave Pointer in some valid state for dereferencing.
            void Construct()
                { Pointer = new int; }
            /// @brief This can perform any cleanup, but should not fail if Pointer is NULL
            /// @details Copying or assigning a ManagedPtr just changes which instance of the
            /// ManagedPtr will cleanup Ptr. This movement of ownership allows passing or
            /// returning ManagedPtr by value or putting it in a standard container. Only the
            /// most recent copy will delete the target and none of the earlier copies will.
            /// This also stops the target from being deleted multiple times.
            void Deconstruct()
                { delete Pointer; } //deleting NULL is a no-op, otherwise a check would be needed.

            /// @brief This is what ManagedPtr will use in copy and assignment operations as well as invaliding handles.
            /// @param Value The new value for the pointer. If NULL the only thing that the ManagedPtr will do to the handle is call its deconstruct method.
            void SetPointer(TargetPtrType Value)
                { Pointer = Value; }
            /// @brief This is what the ManagedPtr with use for dereferencing.
            /// @return The pointer to the managed data. This is expected to return a value that resolves to false when used as a condition when invalid.
            TargetPtrType GetPointer()
                { return Pointer; }

    };


    /// @brief This is used to get RAII semantics when not provided inherently.
    /// @details Some APIs have functions that return raw pointers and expect the call to clean
    /// up the mess. Even worse sometimes doing this involves more than
    /// This is exactly the size of the handle class used. It could be as small as raw pointer
    /// if constructed carefully.
    template <class Handle>
    class MEZZ_LIB ManagedPtr
    {
        private:
            /// @brief This handles the details of contructing and managing the raw pointer.
            /// @detail This is similar to the Pimpl idiom in that much of the implemention could be abstracted into this.
            /// This is expected to have members:
            ///    - TargetPtrType - The type of the underlying pointer.
            ///    - void Construct() - Called on initial construction.
            ///    - void SetPointer(TargetPtrType Value) - called for copy construction, assignements and with NULL Value for invalidations.
            ///    - TargetPtrType GetPointer() - for dereferencing.
            ///    - void Deconstruct() - For cleanup/deconstruction.
            Handle Target;

        public:

            /// @brief Constructs the target after passing it the desired state information
            /// @param StatefulHandle A way to pass parameters into the constructors of the underlying class being targeted.
            /// @details The Handle class can be implemented any way that is appropriate for the underlying data. So it is
            /// not limited to the members defined here. This constructor will call the copy copystructor of the handle before
            /// Calling Construct() on it.
            ManagedPtr(Handle StatefulHandle = Handle())
                : Target(StatefulHandle)
                { Target.Construct(); }

            /// @brief Creates the Target from an already constructed one but invalidates Other ManagedPtr
            /// @details This transfer ownership (who will deconstruct the handle) from the Other ManagedPtr to this one. This
            /// Does not call Construct().
            /// @param Other The ManagedPtr to copy then invalidate.
            ManagedPtr(ManagedPtr& Other)
                : Target(Other.Target.GetPointer())
                { Other.Target.SetPointer(NULL); }

            /// @brief Assigns the Target but invalidates Other ManagedPtr
            /// @details This transfer ownership (who will deconstruct the handle) from the Other ManagedPtr to this one. This
            /// Does not call Construct().
            /// @param Other The ManagedPtr to copy then invalidate.
            ManagedPtr& operator= (ManagedPtr& Other)
            {
                if(Target)
                    { Target.Deconstruct(); }
                Target.SetPointer(Other.Target.GetPointer());
                Other.Target.SetPointer(NULL);
            }

            // operator=
            // &
            // ->
            // * const
            // operator bool

            /// @brief Destroy the target and invalidate it.
            ~ManagedPtr()
            {
                Target.Deconstruct();
                Target.SetPointer(NULL);
            }
    };


} // /namespace Mezzanine

#endif // Include guard
