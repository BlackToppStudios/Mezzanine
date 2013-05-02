//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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

/// @file
/// @brief This file describes and implements any pointers or typedefs to pointers that the Mezzanine uses and exposes in its API
namespace Mezzanine
{
    /// @brief CountedPointer casting implementation states.
    /// @details
    /// The CountedPtrCast uses some template metaprogramming to include the correct cast at compile time
    /// in the CountedPtr. In some situations it is difficult to cast from one kind of pointer to another
    /// even when it is conceptually sound to do so. For example, when virtual inheritance is used
    /// downcasting can be difficult even when a pointer was just returned from one member of an object
    /// into another member of that object but from a member function whose interface specified a more
    /// base return type. While possible to work around this normally, imagine the same situation if
    /// some of the types involved were templates.
    enum CountedPointerCastingState
    {
        CastNoneError = -2,       ///< No Casting, any cast attempt results in a compilation Error.
        CastNoneReturnZero   = -1,       ///< No Casting, 0 is returned. Useful when types are unknown and dynamic casts are already used and checked.
        CastDynamic          = 1,       ///< Dynamic Casting, the class must provide a 'virtual something* GetMostDerived()' and its return must be dynamically upcast to a more base class.
        CastStatic           = 2        ///< Static Casting, the class provides a 'virtual void* GetMostDerived()' and its return can be statically upcast to a more base class.
    };

    // Forward Declares for casting
    template <typename ReturnPointer, typename OriginalPointer, CountedPointerCastingState> class CountedPtrCastImpl;
    template <typename ReturnPointer, typename OriginalPointer> class CountedPtrCastImpl <ReturnPointer, OriginalPointer, CastNoneError>;
    template <typename ReturnPointer, typename OriginalPointer> class CountedPtrCastImpl <ReturnPointer, OriginalPointer, CastNoneReturnZero>;
    template <typename ReturnPointer, typename OriginalPointer> class CountedPtrCastImpl <ReturnPointer, OriginalPointer, CastDynamic>;
    template <typename ReturnPointer, typename OriginalPointer> class CountedPtrCastImpl <ReturnPointer, OriginalPointer, CastStatic>;

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This exists once per object managed by a group of shared pointer to track items in memory.
    /// @details This exists to track the pointer to the managed object, and stores the single
    /// counter of existing references. Only one of these should be created for each group of
    /// pointers managing the same object.
    /// @note It should be extremely rare to need to create one of these outside of pointer implemenation.
    ///////////////////////////////////////////////////////////////////////////////
    template <typename TypePointedTo> class ReferenceCount
    {
        private:
            /// @brief  The Pointer that is the crux of this class, At this smart pointer's core there must be a raw pointer.
            TypePointedTo* Target;

            /// @brief This is the Counter that stores how many references exist
            Whole RefCount;

        public:
            /// @brief Constructor
            /// @param InitialPointer A pointer to the type of this template. This defaults to 0 if not provided
            /// @param InitialCount The number of references to default to this defaults to 1 if not provided.
            ReferenceCount(TypePointedTo* InitialPointer = 0, unsigned InitialCount = 0)
                : Target(InitialPointer), RefCount(InitialCount)
                {}

            /// @brief Destructor, cleans up the object when the last reference deletes this
            ~ReferenceCount()
                { delete Target; }

            /// @brief Increase the reference count by one and return the updated count.
            /// @return The updated count;
            Whole IncrementReferenceCount()
                { return ++RefCount; }

            /// @brief Decrease the reference count by one and return the updated count.
            /// @return The updated count;
            Whole DecrementReferenceCount()
                { return --RefCount; }

            /// @brief Gets the actual pointer to the target.
            /// @return A Pointer of the targeted type to the object being managed.
            TypePointedTo* GetReferenceCountPointer()
                { return Target; }

            /// @brief Gets a reference to the thing pointed at by the reference counter
            /// @return A reference to the target.
            TypePointedTo& GetReferenceCountReference()
                { return *Target; }

            /// @brief Get the current amount of references.
            /// @return A Whole with the current reference count
            Whole GetReferenceCount()
                { return RefCount; }
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is used to deduce at compile if a specific class has built reference counting or
    /// specialized reference counting.
    /// @details Any class that provides TypePointedTo* GetReferenceCountPointer(), Whole GetReferenceCount(),
    /// TypePointedTo& GetReferenceCountReference(),
    /// Whole IncrementReferenceCount() and Whole DecrementReferenceCount() can be used as a reference
    /// counter. If it provides these then a specialization of ReferenceCountTraits should be
    /// implemented for a given class and CountedPtr will use the type defined by
    /// ReferenceCountTraits<T>::ManagedType as the counter. @n @n
    /// The Mezzanine provides anReference count class that can be used with any type at the cost of
    /// extra dereferences. Some types (Scripts) have their own internal reference count that when used
    /// will increase the locality (since the reference count is part of the managed object) and reduce
    /// dereferences to exactly one. Since the CountedPtr is the size of a native pointer if it is used
    /// with an internal reference count, the only signifigant overhead should be the counting itself.
    template <typename T> class ReferenceCountTraits;

    /// @brief A sample class that implements a minimal intrusive reference counting scheme.
    /// @details It might work to inherit from this, however on some platforms multiple inheritance
    /// comes with the exact same cost intrusive pointers are designed to avoid. This should be used
    /// as an example of how to implement a reference counted object.
    class IntrusiveRefCount
    {
        private:
            /// @brief This is the Counter that stores how many references exist
            Whole RefCount;

        public:
            /// @brief Constructor for instatiating the Reference Count
            IntrusiveRefCount(): RefCount(0)
                {}

            /// @brief Increase the reference count by one and return the updated count.
            /// @return The updated count;
            Whole IncrementReferenceCount()
                { return ++RefCount; }

            /// @brief Decrease the reference count by one and return the updated count.
            /// @return The updated count;
            Whole DecrementReferenceCount()
                { return --RefCount; }

            /// @brief Gets the actual pointer to the target.
            /// @return A Pointer of the targeted type to the object being managed.
            IntrusiveRefCount* GetReferenceCountPointer()
                { return this; }

            /// @brief Gets a reference to the reference counter, which on this is this
            /// @return A reference to this.
            IntrusiveRefCount& GetReferenceCountReference()
                { return *this; }

            /// @brief Get the current amount of references.
            /// @return A Whole with the current reference count
            Whole GetReferenceCount()
                { return RefCount; }

            /// @brief Get a pointer to the most Derived type of this class
            /// @return A pointer cast to a void*
            virtual void* GetMostDerived()
                { return reinterpret_cast<void*>(this); }
    };


    /// @brief An Example ReferenceCountTraits implmentation.
    /// @details Anything with an internal Reference should implement a specialization of this template
    /// that defines PtrType to be a pointer to the structure with the reference count interface
    template <>
    class ReferenceCountTraits <IntrusiveRefCount>
    {
        public:
            typedef IntrusiveRefCount ManagedType;
            typedef IntrusiveRefCount* PtrType;

            static PtrType ConstructionPointer(PtrType Target)
                { return Target; }

            enum { IsCastable = CastStatic };
    };

    /// @brief Every class that does not implement its own reference count gets this default one.
    /// @details The Default reference count is not thread-safe, and requires that every dereferencing
    /// of the smart pointer is actually a double dereference. The double dereference is caused because
    /// the reference counter has to store a native pointer to the managed object. In benchmarks
    /// included with the Unit tests this seems to increase dereference time by about double.
    template <typename T>
    class ReferenceCountTraits
    {
        public:
            /// @brief This is type of the ReferenceCounter, The type of the class if reference counting is instrusive.
            typedef ReferenceCount<T> ManagedType;

            /// @brief The type of the Pointer to the reference count, in case it is not ManageType*.
            typedef ReferenceCount<T>* PtrType;

            /// @brief This will somehow return a pointer to the reference count
            /// @param Target A pointer the freshly created object.
            /// @return This will return a pointer to a valid reference counter.
            /// @note The default implemetation call new with a matching delete in CounterPtr::~CountedPtr . If no allocations are required this may simply return the passed parameter.
            static PtrType ConstructionPointer(T* Target)
                { return new ManagedType(Target); }

            /// @brief Used to determine if the data a CountedPtr is managing can be cast
            /// @details This uses the @ref CountedPointerCastingState to enter a value the Template MetaProgramming Machinery will understand.
            enum { IsCastable = CastNoneError };
    };
    ///////////////////////////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A simple reference counting pointer.
    /// @details This is a pointer that automatically deallocates the object it manages when
    /// all CountedPtr intances managing it are destroyed or fall out of scope. This is a
    /// simpler version of std::shared_ptr.
    /// @warning This is not thread safe by default.
    /// @note The basis of this class originated externally, please see the counted pointer
    /// from http://ootips.org/yonat/4dev/smart-pointers.html which came with written permission
    /// for use stated as "Feel free to use my own smart pointers in your code" on that page.
    ///////////////////////////////////////////////////////////////////////////////
    template <class TypePointedTo> class CountedPtr
    {
        protected:
            /// @brief We don't know if we will use a pointer a reference counter, an instrusive pointer, or something else. This provides that abstraction layer.
            typedef typename ReferenceCountTraits<TypePointedTo>::PtrType PtrType;

            /// @brief The non pointer version of PtrType
            typedef typename ReferenceCountTraits<TypePointedTo>::ManagedType ManagedType;

            /// @brief This is the only data on this class, a pointer to the counter and the managed object.
            PtrType _ReferenceCounter;

            /// @brief Have this pointer point at the same thing another pointer points to,
            /// @param CounterToAcquire The ReferenceCounter that this pointer will use.
            /// @warning This does not Release the previous Reference counter. This means it is possible leak memory if a ReferenceCounter is acquired that differs from the previous one without plans to manage the original.
            /// @throw Nothing This member function does not throw exceptions, unless debug logging is enabled, then it can throw any exception the logger can throw.
            void Acquire(PtrType CounterToAcquire) throw()
            {
                _ReferenceCounter = CounterToAcquire;
                if (CounterToAcquire)
                       { CounterToAcquire->IncrementReferenceCount(); }
            }

            /// @brief This decrements the reference count and deletes the managed items if there are no remaining references.
            void Release() throw()
            {
                if (_ReferenceCounter)
                {
                    if (_ReferenceCounter->DecrementReferenceCount() == 0)
                    {
                        //delete _ReferenceCounter->GetReferenceCountPointer();
                        delete _ReferenceCounter; // if we are deleting a ReferenceCount its destructor should clean up the object, if we are deleting something intrusive this does that
                    }
                    _ReferenceCounter = NULL;
                }
            }

        public:
            /// @brief This makes referencing the type of the point object easier for external classes.
            typedef TypePointedTo element_type;

            /// @brief Initial Constructor
            /// @details This should only be used for initial creation of a shared pointer group. This will allocate the raw
            /// pointer and the ReferenceCounter that will be used to track the pointer passed. This will only be explicitly
            /// called to prevent accidental premature deletion of the item managed.
            /// @n @n It is quite easy to accidentally
            /// make a secondary group of counted pointers if not using the new statement inline with this constructor, and it is
            /// not recomended to use this in any other way. Here is an example of the recommended way to use new inline with this:
            /// "Mezzanine::CountedPtr<Mezzanine::Vector3> VecPtr (new Mezzanine::Vector3);"
            /// @param PointerTarget The item that will be deleted once all the pointer of this group disappear.
            explicit CountedPtr(TypePointedTo* PointerTarget = 0)
                { Acquire( ReferenceCountTraits<TypePointedTo>::ConstructionPointer(PointerTarget) ); }

            /// @brief Deconstructor, Just calls Release().
            ~CountedPtr() throw()
                { Release(); }

            /// @brief Copy constructor
            /// @param Original The pointer being copied. This fresh pointer will use the same ReferenceCounter as the original.
            /// @throw Nothing This member function does not throws exceptions.
            CountedPtr(const CountedPtr& Original) throw() : _ReferenceCounter(NULL)
                { Acquire( Original._ReferenceCounter ); }

            /// @brief Get the current count of references.
            /// @note This name was chosen to match standard compliant names, and should be usable in templates that require this function.
            /// @return The amount of references which still exist, or 0 if the reference counter is somehow invalid.
            /// @throw Nothing This member function does not throws exceptions.
            Whole use_count() throw()
                { return _ReferenceCounter ? _ReferenceCounter->GetReferenceCount() : 0; }

            /// @brief Assignement operator
            /// @details This safely handles the semantics or release the previously assigned object and acquiring the new
            /// managed object. This performs basic checks as expected.
            /// @param Other The Item on the right hand side of the '=', which this class instance will copy.
            /// @return A reference to this.
            CountedPtr& operator=(const CountedPtr& Other) throw()
            {
                if (this != &Other) {
                    Release();
                    Acquire(Other._ReferenceCounter);
                }
                return *this;
            }

            /// @brief Dereference operator.
            /// @return The managed object is returned by reference.
            /// @throw Nothing This member function does not throw exceptions.
            TypePointedTo& operator*() const throw()
                { return _ReferenceCounter->GetReferenceCountReference(); }

            /// @brief The Structure dereference operator.
            /// @return Makes it appear, syntactically, as though you are dereferencing the raw pointer.
            /// @throw Nothing This member function does not throw exceptions.
            TypePointedTo* operator->() const throw()
                { return static_cast<TypePointedTo*>(_ReferenceCounter->GetReferenceCountPointer()); }

            /// @brief Get the raw pointer to the managed object.
            /// @return The raw pointer to the managed object or 0 if this pointer is invalid.
            /// @throw Nothing This member function does not throw exceptions.
            /// @note This name was chosen to match standard compliant names, and should be usable in templates that require this function.
            TypePointedTo* get() const throw()
                { return static_cast<TypePointedTo*>(_ReferenceCounter ? _ReferenceCounter->GetReferenceCountPointer() : 0); }

            /// @brief Is this the only pointer to the managed object
            /// @return True if use_count() == 1 or if the pointer is invalid
            /// @throw Nothing This member function does not throw exceptions.
            /// @note This name was chosen to match standard compliant names, and should be usable in templates that require this function.
            bool unique() const throw()
                { return _ReferenceCounter ? _ReferenceCounter->GetReferenceCount()==1 : true; }

            /// @brief A comparision of two CountedPtr instances
            /// @details This is used to see if this and another CountedPtr are managing the same
            /// object and are part of the same group of CountedPtr managing that object.
            /// @param Other The CountedPtr on the right hand side of the ==
            /// @return This returns true if this and Other use the same reference count and pointer.
            bool operator==(const CountedPtr& Other)
                { return Other._ReferenceCounter == _ReferenceCounter; }
    };

    /// @internal
    /// @brief The default implementation for CountedPtr casting
    /// @details For any case that uses CastNoneError including the default.
    template <typename ReturnPointer, typename OriginalPointer, CountedPointerCastingState>
    class CountedPtrCastImpl
    {
        public:
            /// @internal
            /// @brief Do not cast, rather fail to compile.
            /// @return Always fails to compile, to signal an error in the calling code attempting to cast.
            static ReturnPointer Cast(OriginalPointer)
            {
                //#error Invalid Casting value for CountedPtr cast
                return ReturnPointer(0);
            }
    };


    /// @internal
    /// @brief An implementation of the CountedPtrCast that always return
    template <typename ReturnPointer, typename OriginalPointer>
    class CountedPtrCastImpl <ReturnPointer, OriginalPointer, CastNoneError>
    {
        public:
            static ReturnPointer Cast(OriginalPointer)
            {
                //#error ReferenceCountTraits for target declare pointers of given type as non-castable.
                return ReturnPointer(0);
            }
    };

    /// @internal
    /// @brief An implementation of the CountedPtrCast that always return
    template <typename ReturnPointer, typename OriginalPointer>
    class CountedPtrCastImpl <ReturnPointer, OriginalPointer, CastNoneReturnZero>
    {
        public:
            static ReturnPointer Cast(OriginalPointer)
                { return ReturnPointer(0); }
    };

    /// @internal
    template <typename ReturnPointer, typename OriginalPointer>
    class CountedPtrCastImpl <ReturnPointer, OriginalPointer, CastDynamic>
    {
        public:
            static ReturnPointer Cast(OriginalPointer Original)
            {
                return  ReturnPointer
                        (
                            dynamic_cast<typename ReferenceCountTraits<OriginalPointer>::PtrType>
                                (Original->GetMostDerived())
                        );
            }
    };

    /// @internal
    template <typename ReturnPointer, typename OriginalPointer>
    class CountedPtrCastImpl <ReturnPointer, OriginalPointer, CastStatic>
    {
        public:
            static ReturnPointer Cast(OriginalPointer Original)
            {
                return  ReturnPointer
                        (
                            static_cast<typename ReferenceCountTraits<OriginalPointer>::PtrType>
                                (Original->GetMostDerived())
                        );
            }
    };

    ///
    template <typename ReturnType, typename CountedPointer>
    CountedPtr<ReturnType> CountedPtrCast(CountedPointer Original)
    {
        return  CountedPtr<ReturnType>
                (
                    CountedPtrCastImpl
                    <
                        ReturnType,
                        typename CountedPointer::PtrType,
                        CountedPointerCastingState
                            (
                                ReferenceCountTraits<typename CountedPointer::PtrType>::IsCastable
                            )
                    >::Cast(Original.get())
                );
    }

} // \Mezzanine

#endif // _smartptr_h
