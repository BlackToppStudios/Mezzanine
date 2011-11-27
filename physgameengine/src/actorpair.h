//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#ifndef _actorpair_h
#define _actorpair_h

namespace phys
{
    class ActorBase;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorPair
    /// @headerfile actorpair.h
    /// @brief This is a basic utility class for tracking pairs of actors.
    /// @details Most commonly this class is used in collision tracking.
    ///////////////////////////////////////
    class ActorPair
    {
        public:
            /// @brief The first actor in the pair.
            ActorBase* ActorA;
            /// @brief The second actor in the pair.
            ActorBase* ActorB;

            /// @brief No initialization constructor.
            ActorPair();
            /// @brief Class Constructor.
            /// @param A The first actor to be applied to this pair.
            /// @param B The second actor to be applied to this pair.
            ActorPair(ActorBase* A, ActorBase* B);
            /// @brief Pointer copy constructor.
            /// @param OtherPair The other pair to be copied.
            ActorPair(ActorPair* OtherPair);
            /// @brief Reference copy constructor.
            /// @param OtherPair The other pair to be copied.
            ActorPair(const ActorPair& OtherPair);
            /// @brief Checks to see if this pair is the same as another pair.
            /// @param A The first actor in the other pair.
            /// @param B The second actor in the other pair.
            /// @return Returns true if the pairs match regardless of ordering, false otherwise.
            bool PairsMatch(ActorBase* A, ActorBase* B) const;
            /// @brief Checks to see if this pair is the same as another pair.
            /// @param Pair The other pair to compare against.
            /// @return Returns true if the pairs match regardless of ordering, false otherwise.
            bool PairsMatch(ActorPair* Pair) const;
            /// @brief Checks to see if this pair is the same as another pair.
            /// @param Pair The other pair to compare against.
            /// @return Returns true if the pairs match regardless of ordering, false otherwise.
            bool operator==(const ActorPair& Pair) const;
            /// @brief Checks to see if this pair is less than another pair.
            /// @remarks This check is based on combining the names of the actors and performing a std::string::compare on them.
            /// @param Pair The other pair to compare against.
            /// @return Returns true if this pair is found to be less then the passed in pair, false otherwise.
            bool operator<(const ActorPair& Pair) const;
            /// @brief Checks to see if this pair is greater than another pair.
            /// @remarks This check is based on combining the names of the actors and performing a std::string::compare on them.
            /// @param Pair The other pair to compare against.
            /// @return Returns true if this pair is found to be greater then the passed in pair, false otherwise.
            bool operator>(const ActorPair& Pair) const;
    };//ActorPair
}//phys

#endif
