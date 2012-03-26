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

namespace Mezzanine
{

    // pointer from http://ootips.org/yonat/4dev/smart-pointers.html
    // with written permission for use "Feel free to use my own smart pointers in your code." on that page
    template <class X> class counted_ptr
    {
    private:
        struct counter {
            counter(X* p = 0, unsigned c = 1) : ptr(p), count(c) {}
            X*          ptr;
            unsigned int count;
        }* itsCounter;

    public:
        typedef X element_type;

        // allocate a new counter
        explicit counted_ptr(X* p = 0) : itsCounter(0)
            {if (p) itsCounter = new counter(p);}

        ~counted_ptr()
            {release();}

        counted_ptr(const counted_ptr& r) throw()
            {acquire(r.itsCounter);}

        unsigned int use_count()
            { if (itsCounter) return itsCounter->count; }

        counted_ptr& operator=(const counted_ptr& r)
        {
            if (this != &r) {
                release();
                acquire(r.itsCounter);
            }
            return *this;
        }

        //template <class Y> friend class Mezzaninecounted_ptr<Y>;
        template <class Y> counted_ptr(const counted_ptr<Y>& r) throw()
            {acquire(r.itsCounter);}
        template <class Y> counted_ptr& operator=(const counted_ptr<Y>& r)
        {
            if (this != &r) {
                release();
                acquire(r.itsCounter);
            }
            return *this;
        }

        X& operator*()  const throw()   {return *itsCounter->ptr;}
        X* operator->() const throw()   {return itsCounter->ptr;}
        X* get()        const throw()   {return itsCounter ? itsCounter->ptr : 0;}
        bool unique()   const throw()
            {return (itsCounter ? itsCounter->count == 1 : true);}

    private:

        void acquire(counter* c) throw()
        { // increment the count
            itsCounter = c;
            if (c) ++c->count;
        }

        void release()
        { // decrement the count, delete if it is 0
            if (itsCounter) {
                if (--itsCounter->count == 0) {
                    delete itsCounter->ptr;
                    delete itsCounter;
                }
                itsCounter = 0;
            }
        }
    };



    /*
    /// @class
    /// @brief This is a reference counting shared pointer.
    /// @details This is a single threaded reference counting shared pointer. This is not thread safe and is
    /// intended to be used from a single thread. This is a reipl
    template<class T> class SharedPtrSingle
	{
        protected:

            /// @brief The item pointed to and owned by the shared pointer
            T* pRep;

            /// @brief The single count of references
            unsigned int* pUseCount;

        public:
            /// @brief Constructor, does not initialise the SharedPtr.
            /// @warn Dangerous! You have to call bind() before using the SharedPtr.

            SharedPtr() : pRep(0), pUseCount(0)
                {}
	}
	// */
	/*
		/// Constructor.
		@param rep The pointer to take ownership of
		@param freeMode The mechanism to use to free the pointer

        template< class Y>
		explicit SharedPtr(Y* rep)
			: pRep(rep)
			, pUseCount(rep ? OGRE_NEW_T(unsigned int, MEMCATEGORY_GENERAL)(1) : 0)
			, useFreeMethod(inFreeMethod)
		{
            OGRE_SET_AUTO_SHARED_MUTEX_NULL
			if (rep)
			{
				OGRE_NEW_AUTO_SHARED_MUTEX
			}
		}
		SharedPtr(const SharedPtr& r)
            : pRep(0), pUseCount(0), useFreeMethod(SPFM_DELETE)
		{
			// lock & copy other mutex pointer

            OGRE_SET_AUTO_SHARED_MUTEX_NULL
            OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
            {
			    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
			    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
			    pRep = r.pRep;
			    pUseCount = r.pUseCount;
				useFreeMethod = r.useFreeMethod;
			    // Handle zero pointer gracefully to manage STL containers
			    if(pUseCount)
			    {
				    ++(*pUseCount);
			    }
            }
		}
		SharedPtr& operator=(const SharedPtr& r) {
			if (pRep == r.pRep)
				return *this;
			// Swap current data into a local copy
			// this ensures we deal with rhs and this being dependent
			SharedPtr<T> tmp(r);
			swap(tmp);
			return *this;
		}

		template< class Y>
		SharedPtr(const SharedPtr<Y>& r)
            : pRep(0), pUseCount(0), useFreeMethod(SPFM_DELETE)
		{
			// lock & copy other mutex pointer

            OGRE_SET_AUTO_SHARED_MUTEX_NULL
            OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
            {
			    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
			    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
			    pRep = r.getPointer();
			    pUseCount = r.useCountPointer();
				useFreeMethod = r.freeMethod();
			    // Handle zero pointer gracefully to manage STL containers
			    if(pUseCount)
			    {
				    ++(*pUseCount);
			    }
            }
		}
		template< class Y>
		SharedPtr& operator=(const SharedPtr<Y>& r) {
			if (pRep == r.getPointer())
				return *this;
			// Swap current data into a local copy
			// this ensures we deal with rhs and this being dependent
			SharedPtr<T> tmp(r);
			swap(tmp);
			return *this;
		}
		virtual ~SharedPtr() {
            release();
		}


		inline T& operator*() const { assert(pRep); return *pRep; }
		inline T* operator->() const { assert(pRep); return pRep; }
		inline T* get() const { return pRep; }

		/** Binds rep to the SharedPtr.
			@remarks
				Assumes that the SharedPtr is uninitialised!

		void bind(T* rep, SharedPtrFreeMethod inFreeMethod = SPFM_DELETE) {
			assert(!pRep && !pUseCount);

			pUseCount = OGRE_NEW_T(unsigned int, MEMCATEGORY_GENERAL)(1);
			pRep = rep;
			useFreeMethod = inFreeMethod;
		}

		inline bool unique() const { OGRE_LOCK_AUTO_SHARED_MUTEX assert(pUseCount); return *pUseCount == 1; }
		inline unsigned int useCount() const { OGRE_LOCK_AUTO_SHARED_MUTEX assert(pUseCount); return *pUseCount; }
		inline unsigned int* useCountPointer() const { return pUseCount; }

		inline T* getPointer() const { return pRep; }
		inline SharedPtrFreeMethod freeMethod() const { return useFreeMethod; }

		inline bool isNull(void) const { return pRep == 0; }

        inline void setNull(void) {
			if (pRep)
			{
				// can't scope lock mutex before release in case deleted
				release();
				pRep = 0;
				pUseCount = 0;
			}
        }

    protected:

        inline void release(void)
        {
			bool destroyThis = false;

            /* If the mutex is not initialized to a non-zero value, then
               neither is pUseCount nor pRep.


            OGRE_MUTEX_CONDITIONAL(OGRE_AUTO_MUTEX_NAME)
			{
				// lock own mutex in limited scope (must unlock before destroy)
				OGRE_LOCK_AUTO_SHARED_MUTEX
				if (pUseCount)
				{
					if (--(*pUseCount) == 0)
					{
						destroyThis = true;
	                }
				}
            }
			if (destroyThis)
				destroy();

            OGRE_SET_AUTO_SHARED_MUTEX_NULL
        }

        virtual void destroy(void)
        {
            // IF YOU GET A CRASH HERE, YOU FORGOT TO FREE UP POINTERS
            // BEFORE SHUTTING OGRE DOWN
            // Use setNull() before shutdown or make sure your pointer goes
            // out of scope before OGRE shuts down to avoid this.
			switch(useFreeMethod)
			{
			case SPFM_DELETE:
				OGRE_DELETE pRep;
				break;
			case SPFM_DELETE_T:
				OGRE_DELETE_T(pRep, T, MEMCATEGORY_GENERAL);
				break;
			case SPFM_FREE:
				OGRE_FREE(pRep, MEMCATEGORY_GENERAL);
				break;
			};
			// use OGRE_FREE instead of OGRE_DELETE_T since 'unsigned int' isn't a destructor
			// we only used OGRE_NEW_T to be able to use constructor
            OGRE_FREE(pUseCount, MEMCATEGORY_GENERAL);
			OGRE_DELETE_AUTO_SHARED_MUTEX
        }

		virtual void swap(SharedPtr<T> &other)
		{
			std::swap(pRep, other.pRep);
			std::swap(pUseCount, other.pUseCount);
			std::swap(useFreeMethod, other.useFreeMethod);
		}
	};

	template<class T, class U> inline bool operator==(SharedPtr<T> const& a, SharedPtr<U> const& b)
	{
		return a.get() == b.get();
	}

	template<class T, class U> inline bool operator!=(SharedPtr<T> const& a, SharedPtr<U> const& b)
	{
		return a.get() != b.get();
	}

	template<class T, class U> inline bool operator<(SharedPtr<T> const& a, SharedPtr<U> const& b)
	{
		return std::less<const void*>()(a.get(), b.get());
	}


    template<class T> class SharedPtrSingle
*/



} // \Mezzanine

#endif // _smartptr_h
