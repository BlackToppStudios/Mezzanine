// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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

#include "exception.h"

namespace Mezzanine
{
    /// @brief Sample implementation for a ManagedPtr
    /// @details This one is pretty useless, and serves as just an example. Any
    /// class that implements at least the member function this implements can be used
    /// with the ManagedPtr to get smart pointer semantics.
    class IntHandle // This class should serve as an example and should not be exported.
    {
        public:
            /// @brief This is type ManagedPtr will use to work with a handle
            typedef int* TargetPtrType;

            /// @brief The Type this handle is pointer at.
            typedef int TargetType;

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
    /// @n @n
    /// To use this a 'Handle' must be created first. A good handle is trivial copyable, assignable
    /// and implements a few functions. This is similar to the Pimpl idiom in that the details of the
    /// implemention could be abstracted into this 'Handle'. The 'Handle' will encapsulate/have the
    /// knowledge of how to correctly create and destroy the pointer while the ManagedPtr provides
    /// The pointer-like semantics and esures it is destructed exactly once.
    /// This is expected to have members:
    ///    - TargetPtrType - The type of the underlying pointer.
    ///    - void Construct() - Called on initial construction.
    ///    - void SetPointer(TargetPtrType Value) - called for copy construction, assignements and with NULL Value for invalidations.
    ///    - TargetPtrType GetPointer() - for dereferencing.
    ///    - void Deconstruct() - For cleanup/deconstruction.
    ///
    /// The semantics of this class also presume that copying and assigning a Handle is safe. To do that
    /// uses these functions as well. It is intended that pointer copies be shallow copies.
    ///    - operator= - For copying one handle to another, used in construction.
    ///    - Sane default construction that can immediately allow for calls to the the other functions.
    template <class Handle>
    class MEZZ_LIB ManagedPtr
    {
        private:
            /// @brief This handles the details of contructing and managing the raw pointer.
            Handle Target;

            /// @brief Copy the state of one handle to this handle, and invalidate the old one
            /// @param StatefulHandle the state to copy onto our handle.
            void Copy(Handle& StatefulHandle)
            {
                if(StatefulHandle.GetPointer()!=Target.GetPointer())
                {
                    Target.Deconstruct();
                    Target=StatefulHandle;
                    Invalidate(StatefulHandle);
                }
            }

            /// @brief Take whatever actions are required to invalidate a handle.
            /// @param StatefulHandle The Handle to invalidate.
            static void Invalidate(Handle& StatefulHandle)
                { StatefulHandle.SetPointer(NULL); }
            /// @brief Invalidate the handle on this ManagedPtr.
            void Invalidate()
                { Invalidate(Target); }

        public:

            /// @brief Used to set the return type of pointer like returns (could be a smartpointer)
            typedef typename Handle::TargetPtrType TargetPtrType;
            /// @brief Used to set the value and reference type for returns.
            typedef typename Handle::TargetType TargetType;

            /// @brief Constructs the target after passing it the desired state information
            /// @param StatefulHandle A way to pass parameters into the constructors of the underlying class being targeted.
            /// @details The Handle class can be implemented any way that is appropriate for the underlying data. So it is
            /// not limited to the members defined here. This constructor will call the copy copystructor of the handle before
            /// Calling Construct() on it.
            ManagedPtr(Handle StatefulHandle = Handle())
                : Target( StatefulHandle )
            {
                Target.Construct();
            }

            /// @brief Creates the Target from an already constructed one but invalidates Other ManagedPtr
            /// @details This transfer ownership (who will deconstruct the handle) from the Other ManagedPtr to this one. This
            /// Does not call Construct().
            /// @param Other The ManagedPtr to copy then invalidate.
            ManagedPtr(ManagedPtr& Other)
            {
                Target=Other.Target;
                Other.Target.SetPointer(NULL);
            }

            /// @brief Assigns the Target but invalidates Other ManagedPtr
            /// @details This transfer ownership (who will deconstruct the handle) from the Other
            /// ManagedPtr to this one. This Does not call Construct().
            /// @param Other The ManagedPtr to copy then invalidate.
            /// @return A reference to this ManagedPtr to allow operator chaining.
            ManagedPtr& operator= (ManagedPtr& Other)
            {
                Copy(Other.Target);
                return *this;
            }

            /// @brief Dereference operator.
            /// @return The managed object is returned by reference.
            /// @throw If the pointerNothing This member function does not throw exceptions unless the underlying handle does.
            TargetType& operator*()
            {
                TargetPtrType Results=Target.GetPointer();
                if(Results)
                    { return *Results; }
                else
                    { MEZZ_EXCEPTION(ExceptionBase::MM_EXCEPTION,"Attempted to Dereference a NULL ManagedPtr."); }
            }

            /// @brief The Structure dereference operator.
            /// @return Makes it appear, syntactically, as though you are dereferencing the raw pointer.
            TargetPtrType operator->()
            {
                TargetPtrType Results=Target.GetPointer();
                if(Results)
                    { return Results; }
                else
                    { MEZZ_EXCEPTION(ExceptionBase::MM_EXCEPTION,"Attempted to Dereference a NULL ManagedPtr."); }
            }

            /// @brief Get the raw pointer to the managed object.
            /// @return The raw pointer to the managed object or 0 if this pointer is invalid.
            /// @note This name was chosen to match standard compliant names, and should be usable in templates that require this function.
            TargetPtrType Get()
                { return Target.GetPointer(); }

            /// @copydoc Get
            /// @note Provided to match method names on standard smart pointers
            TargetPtrType get()
                { return Get(); }

            /// @brief Returns true if this pointer points to something.
            /// @warning Without C++11 this can be accidentally easily be cast to a bool and can do silly things, like bogus pointer multiplication.
            operator Boole()
            { return 0 != Target.GetPointer(); }

            /// @brief Compares the underlying pointer of this and another ManagedPtr, (This had better return false)
            /// @param Other The right hand portion of the comparison
            /// @return Bool Hopefully thi is false, becase
            Boole operator== (ManagedPtr& Other)
                { return Target.GetPointer() == Other.Target.GetPointer(); }

            /// @brief Destroy the target and invalidate it.
            ~ManagedPtr()
            {
                Target.Deconstruct();
                Target.SetPointer(NULL);
            }
    };


} // /namespace Mezzanine

#endif // Include guard
