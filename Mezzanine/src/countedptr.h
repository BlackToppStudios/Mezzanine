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
#ifndef _countedptr_h
#define _countedptr_h

#include "datatypes.h"

/// @file
/// @brief This file describes and implements a reference counted pointer that is NOT threadsafe
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
        CastNoneError        = -2,      ///< No Casting, any cast attempt results in a compilation Error.
        CastNoneReturnZero   = -1,      ///< No Casting, 0 is returned. Useful when types are unknown and dynamic casts are already used and checked.
        CastImplicit         = 1,       ///< Does no casting, but keeps types distinct. This should allow implicit casts to more base types, and disallow casts to derived types.
        CastStatic           = 2,       ///< A static cast from the pointer as provided with no attempt to calls functions on the pointer target, this is default for externally reference counted pointers.
        CastDynamic          = 3        ///< A dynamic cast from the pointer as provided with no attempt to calls functions on the pointer target, this is default for externally reference counted pointers.
    };

    #ifndef SWIG
    // Forward Declares Just about everything in this file
    // The Pointer itself
    template <typename TypePointedTo> class CountedPtr;

    // Externally callable casts
    template <typename ReturnType, typename OtherPointerTargetType> CountedPtr<ReturnType> CountedPtrCast(CountedPtr<OtherPointerTargetType>& Original);
    template <typename ReturnType, typename OtherPointerTargetType> CountedPtr<ReturnType> CountedPtrStaticCast(CountedPtr<OtherPointerTargetType>& Original);
    template <typename ReturnType, typename OtherPointerTargetType> CountedPtr<ReturnType> CountedPtrDynamicCast(CountedPtr<OtherPointerTargetType>& Original);

    // Casts the pointer uses internally for consistency.
    template <typename ReturnPointer, typename OriginalPointer, CountedPointerCastingState> class CountedPtrCastImpl;
    template <typename OriginalPointer> class CountedPtrCastImpl <OriginalPointer, OriginalPointer, CastNoneError>;
    template <typename ReturnPointer, typename OriginalPointer> class CountedPtrCastImpl <ReturnPointer, OriginalPointer, CastNoneReturnZero>;
    template <typename ReturnPointer, typename OriginalPointer> class CountedPtrCastImpl <ReturnPointer, OriginalPointer, CastStatic>;
    template <typename ReturnPointer, typename OriginalPointer> class CountedPtrCastImpl <ReturnPointer, OriginalPointer, CastDynamic>;

    template <typename Pointer> Pointer* CountedPtrInternalCast(Pointer* Original);
    template <typename ReturnPointer, typename OriginalPointer> ReturnPointer* CountedPtrInternalCast(const OriginalPointer* Original);

    template <typename T> class ReferenceCountTraits;

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
            virtual ~ReferenceCount()
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
            TypePointedTo* GetReferenceCountTargetAsPointer()
                { return Target; }

            /// @brief Get the current amount of references.
            /// @return A Whole with the current reference count
            Whole GetReferenceCount()
                { return RefCount; }

            /// @brief Get a pointer to the Target as a The most derived type in of this object*
            /// @return A pointer, for use with CountedPtrCast
            virtual TypePointedTo* GetMostDerived()
                { return Target; }
    };

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
            IntrusiveRefCount* GetReferenceCountTargetAsPointer()
                { return this; }

            /// @brief Get the current amount of references.
            /// @return A Whole with the current reference count
            Whole GetReferenceCount() const
                { return RefCount; }

            /// @brief Get a pointer to the most Derived type of this class
            /// @return A pointer of the most derived pointing to this.
            virtual IntrusiveRefCount* GetMostDerived()
                { return this; }

            /// @brief Virtual Deconstructor to prevent issues with deletion with incomplete type information
            virtual ~IntrusiveRefCount() {}
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is used to deduce at compile if a specific class has built-in reference counting or needs an external reference count.
    /// @details Every class that does not implement its own reference count gets this default one.
    /// The Default reference count is not thread-safe, and requires that every dereferencing
    /// of the smart pointer has the cost of a double dereference. The double dereference is caused because
    /// the reference counter has to store a native pointer to the managed object. In benchmarks
    /// included with the Unit tests this seems to increase dereference time by about double.
    /// @n @n
    /// Any class that provides TypePointedTo* GetReferenceCountTargetAsPointer(), Whole GetReferenceCount(),
    /// Whole IncrementReferenceCount(), Whole DecrementReferenceCount()
    /// and something* GetMostDerived() (which need to be virtual if inheritance is used to be useful) can be used as a reference
    /// counter. If it provides these then a specialization of ReferenceCountTraits should be implemented for a given class
    /// and CountedPtr will use the type defined by ReferenceCountTraits<T>::RefCountType as the counter. @n @n
    /// The Mezzanine provides a Reference count class that can be used with any type at the cost of
    /// extra dereferences. Some types (Scripts) have their own internal reference count that when used
    /// will increase the locality (since the reference count is part of the managed object) and reduce
    /// dereferences to exactly one. Since the CountedPtr is the size of a native pointer if it is used
    /// with an internal reference count, the only signifigant overhead should be the counting itself.
    template <typename T>
    class ReferenceCountTraits
    {
        public:
            /// @brief This is type of the ReferenceCounter, The type of the class if reference counting is instrusive.
            typedef ReferenceCount<T> RefCountType;

            /// @brief This will somehow return a pointer to the reference count
            /// @param Target A pointer the freshly created object.
            /// @return This will return a pointer to a valid reference counter.
            /// @note The default implemetation call new with a matching delete in CounterPtr::~CountedPtr . If no allocations are required this may simply return the passed parameter.
            static RefCountType* ConstructionPointer(T* Target)
                { return new RefCountType(Target); }

            /// @brief Used to determine if the data a CountedPtr is managing can be cast
            /// @details This uses the @ref CountedPointerCastingState to enter a value the Template MetaProgramming Machinery will understand.
            enum { IsCastable = CastStatic };
    };

    /// @brief An Example ReferenceCountTraits implmentation.
    /// @details Anything with an internal Reference should implement a specialization of this template
    /// that defines PtrType to be a pointer to the structure with the reference count interface in
    /// to the items defined in the main ReferenceCountTraits documentation.
    template <>
    class ReferenceCountTraits <IntrusiveRefCount>
    {
        public:
            /// @brief The sample internal reference count needs to indicate that the managed type is the same as the ReferenceCountType, because it will have a counter.
            typedef IntrusiveRefCount RefCountType;

            /// @brief Because The reference count is allocated when the caller created the target this does not need to allocate a Reference count separetaly.
            /// @param Target The already created object to manage.
            /// @return This is expected to return a valid reference count, since the reference count is in the target, this returns whatever was passed in,
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief What kind of casting should be done when the target must be down(or side) cast.
            enum { IsCastable = CastStatic};
    };

    /// @internal
    /// @brief This exists to conceal internal type detection mechanisms making internal and external reference couting different.
    namespace
    {
        /// @brief This handles the case of needing to adjust the reference count of covariant pointers all covariant pointer must use internal reference counts
        template <typename CurrentReferenceCountType, typename OtherReferenceCountType>
        class ReferenceCountAdjuster
        {
            public:
                /// @brief Gain a copy of the reference counter with appropriate type data.
                /// @param UpdateCounter A reference to a pointer that the CounterPtr wants to point at the correct reference count
                /// @param OtherCounter A reference to a pointer that is pointing to the refence counter we want to acquire
                static void Acquire(CurrentReferenceCountType* & UpdateCounter, OtherReferenceCountType* & OtherCounter)
                    { UpdateCounter = CountedPtrInternalCast<CurrentReferenceCountType>(OtherCounter->GetMostDerived()); }
        };

        /// @brief The logic is much simpler when covariant types are not involved, this will acwuire a reference counter
        /// @details Incidentally this should work for internal reference counts that are of identical type and all external reference counts.
        template <typename InternalReferenceCount>
        class ReferenceCountAdjuster<InternalReferenceCount,InternalReferenceCount>
        {
            public:
                /// @brief Gain a copy of the reference counter which already has appropriate type data.
                /// @param UpdateCounter A reference to a pointer that the CounterPtr wants to point at the correct reference count.
                /// @param OtherCounter A reference to a pointer that is pointing to the refence counter we want to acquire.
                static void Acquire(InternalReferenceCount* & UpdateCounter, InternalReferenceCount* & OtherCounter)
                    { UpdateCounter = OtherCounter; }
        };
    }
    #endif // \SWIG

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A simple reference counting pointer.
    /// @details This is a pointer that automatically deallocates the object it manages when
    /// all CountedPtr intances managing it are destroyed or fall out of scope. This is a
    /// simpler version of std::shared_ptr.
    /// @warning This is not thread safe by default.
    /// @warning If you intead to have covariant pointers (Multiple pointers to the same item but with different type date, like more base or more derived pointers) all classes pointed to must have virtual deconstructors
    /// @note The basis of this class originated externally, please see the counted pointer
    /// from http://ootips.org/yonat/4dev/smart-pointers.html which came with written permission
    /// for use stated as "Feel free to use my own smart pointers in your code" on that page.
    ///////////////////////////////////////////////////////////////////////////////
    template <typename TypePointedTo>
    class CountedPtr
    {
        public:
            /// @brief The non pointer version of PtrType
            typedef typename ReferenceCountTraits<TypePointedTo>::RefCountType RefCountType;

            /// @brief This makes referencing the type of the pointer object easier for external classes.
            typedef TypePointedTo element_type;

            /// @brief declare all pointers as friends so they can reference eachother _ReferenceCounter.
            template <typename OtherPointer> friend class CountedPtr;

            /// @brief Static casting can access internals also
            template <typename ReturnType, typename OtherPointerTargetType> friend CountedPtr<ReturnType> CountedPtrStaticCast(CountedPtr<OtherPointerTargetType>& Original);
        protected:
            /// @brief This is the only data on this class, a pointer to the counter and the managed object.
            RefCountType* _ReferenceCounter;

            /// @brief Have this pointer point at the same thing another pointer points to,
            /// @param CounterToAcquire The ReferenceCounter that this pointer will use.
            /// @warning This does not Release the previous Reference counter. This means it is possible leak memory if a ReferenceCounter is acquired that differs from the previous one without plans to manage the original.
            /// @throw Nothing This member function does not throw exceptions, unless debug logging is enabled, then it can throw any exception the logger can throw.
            template <typename AnyReferenceCountType>
            void Acquire(AnyReferenceCountType* CounterToAcquire) throw()
            {
                ReferenceCountAdjuster<RefCountType, AnyReferenceCountType>::Acquire(_ReferenceCounter, CounterToAcquire);
                if (_ReferenceCounter)
                       { _ReferenceCounter->IncrementReferenceCount(); }
            }

            /// @brief This decrements the reference count and deletes the managed items if there are no remaining references.
            void Release() throw()
            {
                if (_ReferenceCounter)
                {
                    if (_ReferenceCounter->DecrementReferenceCount() == 0)
                    {
                        delete _ReferenceCounter; // deleting a ReferenceCount should clean up the target object in its destructor, if we are deleting something intrusively reference counted this does
                        _ReferenceCounter = 0;
                    }
                }
            }

            /// @brief If required create a reference counter and point this at the passed pointer.
            /// @param PointerTarget the Pointer to take ownership of.
            void FirstAcquire(TypePointedTo* PointerTarget)
                { Acquire( ReferenceCountTraits<TypePointedTo>::ConstructionPointer(PointerTarget) ); }

        public:
            /// @brief Initializing Constructor
            /// @details This should only be used for initial creation of a shared pointer group. This will allocate the raw
            /// pointer and the ReferenceCounter that will be used to track the pointer passed. This will only be explicitly
            /// called to prevent accidental premature deletion of the item managed.
            /// @n @n It is quite easy to accidentally
            /// make a secondary group of counted pointers if not using the new statement inline with this constructor, and it is
            /// not recomended to use this in any other way. Here is an example of the recommended way to use new inline with this:
            /// "Mezzanine::CountedPtr<Mezzanine::Vector3> VecPtr (new Mezzanine::Vector3);"
            /// @param PointerTarget The item that will be deleted once all the pointer of this group disappear.
            explicit CountedPtr(TypePointedTo* PointerTarget = 0) throw()
            {
                if(PointerTarget)
                    { FirstAcquire(PointerTarget); }
                else
                    { _ReferenceCounter = NULL; }
            }

            /// @brief Deconstructor, Just calls Release().
            ~CountedPtr() throw()
                { Release(); }

            /// @brief Copy constructor
            /// @param Original The pointer being copied. This fresh pointer will use the same ReferenceCounter as the original.
            /// @throw Nothing This member function does throws exceptions.
            CountedPtr(const CountedPtr& Original) throw()
                { Acquire( Original._ReferenceCounter ); }

            /// @brief Casting copy constructor
            /// @param Original The CountedPtr being copied. This fresh pointer will use the same ReferenceCounter as the original.
            /// @throw Nothing This member function does throws exceptions, unless the casting mechanism throws, which it shouldn't.
            template <typename OtherPointer>
            CountedPtr(const CountedPtr<OtherPointer>& Original) throw() : _ReferenceCounter(NULL)
            {
                //Acquire( CountedPtr<TypePointedTo>(Original)._ReferenceCounter );
                Acquire( Original._ReferenceCounter );
            }

            /// @brief Reset this to point at nothing.
            void Reset()
            {
                Release();
                _ReferenceCounter = NULL;
            }

            /// @copydoc Reset()
            /// @note Provided to match method names on standard smart pointers
            void reset()
                { Reset(); }

            /// @brief Reset this to point at the same as another CountedPtr of the same type.
            /// @param Other Another CountedPtr which will share ownership of the target.
            void Reset(const CountedPtr<TypePointedTo>& Other)
            {
                if (this != &Other) {
                    Release();
                    Acquire(Other._ReferenceCounter);
                }
            }

            /// @copydoc Reset(const CountedPtr<TypePointedTo>&)
            /// @note Provided to match method names on standard smart pointers
            void reset(const CountedPtr<TypePointedTo>& Other)
                { Reset(Other); }

            /// @brief Take ownership of the passed pointer.
            /// @param PointerTarget The pointer to assume ownership of.
            void Reset(TypePointedTo* PointerTarget)
            {
                if(PointerTarget)
                    { FirstAcquire(PointerTarget); }
                else
                    { _ReferenceCounter = NULL; }
            }

            /// @copydoc Reset(TypePointedTo*)
            /// @note Provided to match method names on standard smart pointers
            void reset(TypePointedTo* PointerTarget)
                { Reset (PointerTarget); }

            /// @brief Get the current count of references.
            /// @note This name was chosen to match standard compliant names, and should be usable in templates that require this function.
            /// @return The amount of references which still exist, or 0 if the reference counter is somehow invalid.
            /// @throw Nothing This member function does not throws exceptions.
            Whole UseCount() const throw()
                { return _ReferenceCounter ? _ReferenceCounter->GetReferenceCount() : 0; }

            /// @copydoc UseCount
            /// @note Provided to match method names on standard smart pointers
            Whole use_count() const throw()
                { return UseCount(); }

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
                { return *(_ReferenceCounter->GetReferenceCountTargetAsPointer()); }

            /// @brief The Structure dereference operator.
            /// @return Makes it appear, syntactically, as though you are dereferencing the raw pointer.
            /// @throw Nothing This member function does not throw exceptions.
            TypePointedTo* operator->() const throw()
                { return CountedPtrInternalCast<TypePointedTo>(_ReferenceCounter->GetMostDerived()); }

            /// @brief Get the raw pointer to the managed object.
            /// @return The raw pointer to the managed object or 0 if this pointer is invalid.
            /// @throw Nothing This member function does not throw exceptions.
            /// @note This name was chosen to match standard compliant names, and should be usable in templates that require this function.
            TypePointedTo* Get() const throw()
                { return CountedPtrInternalCast<TypePointedTo>(_ReferenceCounter->GetMostDerived()); }

            /// @copydoc Get
            /// @note Provided to match method names on standard smart pointers
            TypePointedTo* get() const throw()
                { return Get(); }

            /// @brief Is this the only pointer to the managed object
            /// @return True if use_count() == 1 or if the pointer is invalid
            /// @throw Nothing This member function does not throw exceptions.
            /// @note This name was chosen to match standard compliant names, and should be usable in templates that require this function.
            Boole Unique() const throw()
                { return _ReferenceCounter ? _ReferenceCounter->GetReferenceCount()==1 : true; }

            /// @copydoc Unique
            /// @note Provided to match method names on standard smart pointers
            Boole unique() const throw()
                { return Unique(); }

            /// @brief A comparision of two CountedPtr instances
            /// @details This is used to see if this and another CountedPtr are managing the same
            /// object and are part of the same group of CountedPtr managing that object.
            /// @param Other The CountedPtr on the right hand side of the ==
            /// @return This returns true if this and Other use the same reference count and pointer.
            Boole operator==(const CountedPtr& Other) const throw()
                { return Other._ReferenceCounter == _ReferenceCounter; }

            /// @brief Returns true if this pointer points to something.
            /// @warning Without C++11 this can be accidentally easily be cast to a Boole and can do sill things.
            operator Boole() const throw()
                { return 0 != _ReferenceCounter; }

            /// @internal
            /// @brief Get the internal Reference count
            /// @return A pointer to internal reference count of this pointer.
            RefCountType* GetReferenceCount() const throw()
                { return _ReferenceCounter; }
    };

    #ifndef SWIG
    /// @internal
    /// @brief This is used as to determine how a CountedPtr performs castin between pointer types internally.
    /// @details The default implementation for internal casting in the CountedPtr  Simply throws an exception.
    /// @n @n
    /// This Should never actually get called. Though it is the default template instance
    /// the default ReferenceCountTraits uses the CastNoneError specialization of this template.
    /// @n @n
    /// The ReferenceCountTraits for the CountedPtr<Type> should declare what kind castint should be used by
    /// declaring an enumeration called 'IsCastable' and assigning the desired casting type.
    /// @code
    /// enum { IsCastable = CastStaticVirtual };
    /// @endcode
    /// @tparam ReturnPointer The type of pointer that the functions on the CountedPtr must return
    /// @tparam OriginalPointer The actual type the CountedPtr is maintaining as a target.
    /// @tparam CountedPointerCastingState Any value in the @ref CountedPointerCastingState to indicate kind of casting.
    /// @details For any case that uses CastNoneError including the default.
    template <typename ReturnPointer, typename OriginalPointer, CountedPointerCastingState>
    class CountedPtrCastImpl
    {
        public:
            /// @internal
            /// @brief Do not cast, rather fail.
            /// @return Always fails by throwing
            /// @throw Always throws a 0
            static ReturnPointer Cast(OriginalPointer)
            {
                // #error Invalid Casting value for CountedPtr cast
                throw(0); /// @todo make this conditionally fail to compile.
                return ReturnPointer(0);
            }
    };

    /// @internal
    /// @brief An implementation of the CountedPtrCast that always return the pointer passed reached via CastNoneError in the ReferenceCountTraits of the target class.
    /// @tparam OriginalPointer The type of the original pointer and return value must match exactly
    /// @note Since this requires both matching pointer types and CastNoneError to be present in the ReferenceCountTraits This should produce a compilation error when somethign is set wrong.
    template <typename OriginalPointer>
    class CountedPtrCastImpl <OriginalPointer, OriginalPointer, CastNoneError>
    {
        public:
            /// @internal
            /// @brief This literally does nothing and returns the value passed, so simple all but the dumbest compilers will optimize it away at compile time.
            /// @return Whatever was passed.
            static OriginalPointer Cast(OriginalPointer Original)
                { return Original; }
    };

    /// @internal
    /// @brief An implementation of the CountedPtrCast that always returns 0 cast to the original pointer type. Reached by putting CastNoneReturnZero in the ReferenceCountTraits of the target class.
    /// @param ReturnPointer The type of the NULL pointer to create.
    /// @param OriginalPointer The type of the pointer prior to casting.
    template <typename ReturnPointer, typename OriginalPointer>
    class CountedPtrCastImpl <ReturnPointer, OriginalPointer, CastNoneReturnZero>
    {
        public:
            /// @internal
            /// @brief This accepts a parameter solely for compatibility with other templates and always returns 0.
            /// @return Always 0
            static ReturnPointer Cast(OriginalPointer)
                { return ReturnPointer(0); }
    };

    /// @internal
    /// @brief An implementation of the CountedPtrCast that casts the passed pointer. Reached by putting CastStatic in the ReferenceCountTraits of the target class.
    /// @param ReturnPointer The type of the pointer to target with the static cast
    /// @param OriginalPointer The type of the pointer prior to casting.
    template <typename ReturnPointer, typename OriginalPointer>
    class CountedPtrCastImpl <ReturnPointer, OriginalPointer, CastImplicit>
    {
        public:
            /// @internal
            /// @brief Simply static cast the passed pointer.
            /// @param Original the passed pointer.
            /// @return The pointer after the cast.
            /// @note This will almost certainly never actually exists as a function, in even basic testing the compiles completely optimizes this out during compilation.
            static ReturnPointer Cast(OriginalPointer Original)
                { return static_cast<ReturnPointer>(Original); }
    };

    /// @internal
    /// @brief An implementation of the CountedPtrCast that naively static casts the passed pointer. Reached by putting CastStatic in the ReferenceCountTraits of the target class.
    /// @param ReturnPointer The type of the pointer to target with the static cast
    /// @param OriginalPointer The type of the pointer prior to casting.
    template <typename ReturnPointer, typename OriginalPointer>
    class CountedPtrCastImpl <ReturnPointer, OriginalPointer, CastStatic>
    {
        public:
            /// @internal
            /// @brief Simply static cast the passed pointer.
            /// @param Original the passed pointer.
            /// @return The pointer after the cast.
            /// @note This will almost certainly never actually exists as a function, in even basic testing the compiles completely optimizes this out during compilation.
            static ReturnPointer Cast(OriginalPointer Original)
                { return dynamic_cast<ReturnPointer>(Original); }
    };

    /// @internal
    /// @brief An implementation of the CountedPtrCast that naively dynamic casts the passed pointer. Reached by putting CastDynamic in the ReferenceCountTraits of the target class.
    /// @param ReturnPointer The type of the pointer to target with the dynamic cast.
    /// @param OriginalPointer The type of the pointer prior to casting.
    template <typename ReturnPointer, typename OriginalPointer>
    class CountedPtrCastImpl <ReturnPointer, OriginalPointer, CastDynamic>
    {
        public:
            /// @internal
            /// @brief Simply dynamic cast the passed pointer.
            /// @param Original the passed pointer.
            /// @return The pointer after the cast.
            /// @note This will almost certainly never actually exists as a function, tests show this as performant as a normal call to dynamic_cast.
            static ReturnPointer Cast(OriginalPointer Original)
                { return dynamic_cast<ReturnPointer>(Original); }
    };

    /// @internal
    /// @brief Used internally by CounterPtr to abstract away casts in the case where the return type would *NOT* match the input type.
    /// @tparam ReturnPointer The type of the pointer to target after using the Casting Strategy described in the classes ReferenceCountTraits.
    /// @tparam OriginalPointer The type of the pointer prior to casting.
    /// @note The function argument Original is of type const OriginalPointer. When search for possible matching functions an template a compiler prioritizes functions that are off by just a CV qualifier ahead of those that it would need to implicitly cast to call. So if the to pointer types match exactly this will not be called.
    /// @param Original The Pointer to cast which must be exactly of the type OriginalPointer(Which might be inferable).
    /// @return A pointer of the type ReturnPointer after the desired casting strategy has been used.
    template <typename ReturnPointer, typename OriginalPointer>
    ReturnPointer* CountedPtrInternalCast(const OriginalPointer* Original)
    {
        return  CountedPtrCastImpl
            <
                ReturnPointer*,
                OriginalPointer*,
                CountedPointerCastingState
                    (
                        ReferenceCountTraits<OriginalPointer>::IsCastable
                    )
            >::Cast(
                        const_cast<OriginalPointer*>(Original) // cast away the constness we just added
                    );
    }

    /// @internal
    /// @brief Used internally by CounterPtr to abstract away casts in the case where the return type would match the input type.
    /// @details This enforces the logic that cast should only be performed inside the CountedPtr when a cast is required. This
    /// also has the advantage of not performing meaningless extra casts if a runtime cast is used, so it coudl enhance performance.
    /// @tparam Pointer The only type of pointers involved.
    /// @param Original The pointer that needs to be checked to see if it should be cast and this compilation case does not require it.
    /// @return The value passed in Original.
    template <typename Pointer>
    Pointer* CountedPtrInternalCast(Pointer* Original)
        { return  Original; }

    /// @brief A compile time cast that uses only the implicit conversion of the underlying raw pointers
    /// @tparam ReturnType The type to be returned, must be specified
    /// @tparam OtherPointerTargetType The type of the provided pointer, this can be infered and should not be provided.
    /// @param Original The pointer to convert.
    /// @return Either a pointer of the desired or a compilation error
    template <typename ReturnType, typename OtherPointerTargetType>
    CountedPtr<ReturnType> CountedPtrCast(CountedPtr<OtherPointerTargetType>& Original)
        { return CountedPtr<ReturnType>(Original); }

    /// @brief A compile time cast that uses static casting conversion of the underlying raw pointers but only works on internally reference count types
    /// @tparam ReturnType The type to be returned, must be specified
    /// @tparam OtherPointerTargetType The type of the provided pointer, this can be infered and should not be provided.
    /// @param Original The pointer to convert.
    /// @return Either a pointer of the desired or a compilation error
    template <typename ReturnType, typename OtherPointerTargetType>
    CountedPtr<ReturnType> CountedPtrStaticCast(CountedPtr<OtherPointerTargetType>& Original)
        { return CountedPtr<ReturnType>(static_cast<ReturnType*>(Original.GetReferenceCount())); }

    /// @brief A Runtime cast that uses dynamic casting conversion of the underlying raw pointers but only works on internally reference count types
    /// @tparam ReturnType The type to be returned, must be specified
    /// @tparam OtherPointerTargetType The type of the provided pointer, this can be infered and should not be provided.
    /// @param Original The pointer to convert.
    /// @return Either a pointer of the desired or a 0 if casting is not possible.
    template <typename ReturnType, typename OtherPointerTargetType>
    CountedPtr<ReturnType> CountedPtrDynamicCast(CountedPtr<OtherPointerTargetType>& Original)
        { return CountedPtr<ReturnType>(dynamic_cast<ReturnType*>(Original.GetReferenceCount())); }
    #endif // \SWIG

} // \Mezzanine

#endif // _smartptr_h
