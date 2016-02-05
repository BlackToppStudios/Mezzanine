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
#ifndef _eventarguments_h
#define _eventarguments_h

#include "datatypes.h"
#include "countedptr.h"

namespace Mezzanine
{
	///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a common class to represent all possible arguments for a given event that is fired.
    /// @details
    ///////////////////////////////////////
	class MEZZ_LIB EventArguments
	{
    private:
        /// @brief This is count of the number of references to this object.
        Whole RefCount;
    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Public Data Members

        /// @brief The name of the event being fired.
        const String EventName;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Class constructor.
        /// @param Name The name of the event being fired.
        EventArguments(const String& Name) :
            EventName(Name) {  }
        /// @brief Class destructor.
        virtual ~EventArguments() {  }

        ///////////////////////////////////////////////////////////////////////////////
        // CountedPtr Functionality

        /// @brief Increase the reference count by one and return the updated count.
        /// @return Returns a Whole representing the updated count.
        Whole IncrementReferenceCount()
            { return ++this->RefCount; }
        /// @brief Decrease the reference count by one and return the updated count.
        /// @return Returns a Whole representing the updated count.
        Whole DecrementReferenceCount()
            { return --this->RefCount; }
        /// @brief Get the current amount of references.
        /// @return A Whole with the current reference count
        Whole GetReferenceCount() const
            { return this->RefCount; }

        /// @brief Gets the actual pointer to the target of the base type.
        /// @return Returns a pointer of the targeted type to the object being managed.
        EventArguments* GetReferenceCountTargetAsPointer()
            { return this; }
        /// @brief Get a pointer to the most Derived type of this instance.
        /// @return Returns a pointer of the most derived type of this.
        virtual EventArguments* GetMostDerived()
            { return this; }
	};//EventArguments

	///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a metaprogramming traits class used by EventArguments.
    /// @details This is need for an intrusive CountedPtr implementation.  Should a working external reference count be made this
    /// could be dropped in favor of a leaner implementation.
    ///////////////////////////////////////
	template <>
    class ReferenceCountTraits<EventArguments>
    {
    public:
        /// @brief Typedef communicating the reference count type to be used.
        typedef EventArguments RefCountType;

        /// @brief Method responsible for creating a reference count for a CountedPtr of the templated type.
        /// @param Target A pointer to the target class that is to be reference counted.
        /// @return Returns a pointer to a new reference counter for the templated type.
        static RefCountType* ConstructionPointer(RefCountType* Target)
            { return Target; }

        /// @brief Enum used to decide the type of casting to be used by a reference counter of the templated type.
        enum { IsCastable = CastStatic };
    };//ReferenceCountTraits<EventArguments>

	/// @brief Convenience typedef for passing around EventArguments.
	typedef CountedPtr<EventArguments> EventArgumentsPtr;
}//Mezzanine

#endif
