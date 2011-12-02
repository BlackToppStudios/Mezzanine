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
#ifndef _objectpair_h
#define _objectpair_h

#include "datatypes.h"

namespace Mezzanine
{
    class ActorBase;
    class TerrainBase;
    class AreaEffect;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ObjectPair
    /// @headerfile objectpair.h
    /// @brief This is an interface class for pairs of world objects.
    /// @details Most commonly this class is used in collision tracking.
    ///////////////////////////////////////
    class MEZZ_LIB ObjectPair
    {
        protected:
            /// @internal
            /// @brief Gets a pointer to the first object in this pair.
            virtual void* GetObjectA() const = 0;
            /// @internal
            /// @brief Gets a pointer to the second object in this pair.
            virtual void* GetObjectB() const = 0;
            /// @internal
            /// @brief Gets a string containing the combined names of the objects in this pair.
            virtual String GetCombinedName() const = 0;
        public:
            /// @brief Checks to see if this pair is the same as another pair.
            /// @param Pair The other pair to compare against.
            /// @return Returns true if the pairs match regardless of ordering, false otherwise.
            virtual bool PairsMatch(const ObjectPair& Pair) const;
            /// @brief Checks to see if this pair is the same as another pair.
            /// @param Pair The other pair to compare against.
            /// @return Returns true if the pairs match regardless of ordering, false otherwise.
            virtual bool operator==(const ObjectPair& Pair) const;
            /// @brief Checks to see if this pair is less than another pair.
            /// @remarks This check is based on combining the names of the objects and performing a std::string::compare on them.
            /// @param Pair The other pair to compare against.
            /// @return Returns true if this pair is found to be less then the passed in pair, false otherwise.
            virtual bool operator<(const ObjectPair& Pair) const;
            /// @brief Checks to see if this pair is greater than another pair.
            /// @remarks This check is based on combining the names of the objects and performing a std::string::compare on them.
            /// @param Pair The other pair to compare against.
            /// @return Returns true if this pair is found to be greater then the passed in pair, false otherwise.
            virtual bool operator>(const ObjectPair& Pair) const;
    };//ObjectPair
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorPair
    /// @headerfile objectpair.h
    /// @brief This is a basic utility class for tracking pairs of actors.
    /// @details Most commonly this class is used in collision tracking.
    ///////////////////////////////////////
    class MEZZ_LIB ActorPair : public ObjectPair
    {
        protected:
            /// @internal
            /// @brief Gets a pointer to the first object in this pair.
            void* GetObjectA() const;
            /// @internal
            /// @brief Gets a pointer to the second object in this pair.
            void* GetObjectB() const;
            /// @internal
            /// @brief Gets a string containing the combined names of the objects in this pair.
            String GetCombinedName() const;
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
            /// @brief Reference copy constructor.
            /// @param OtherPair The other pair to be copied.
            ActorPair(const ActorPair& OtherPair);
            /// @brief Checks to see if this pair is the same as another pair.
            /// @param A The first actor in the other pair.
            /// @param B The second actor in the other pair.
            /// @return Returns true if the pairs match regardless of ordering, false otherwise.
            bool PairsMatch(ActorBase* A, ActorBase* B) const;
    };//ActorPair
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorTerrainPair
    /// @headerfile objectpair.h
    /// @brief This is a basic utility class for tracking pairs of actors.
    /// @details Most commonly this class is used in collision tracking.
    ///////////////////////////////////////
    class MEZZ_LIB ActorTerrainPair : public ObjectPair
    {
        protected:
            /// @internal
            /// @brief Gets a pointer to the first object in this pair.
            void* GetObjectA() const;
            /// @internal
            /// @brief Gets a pointer to the second object in this pair.
            void* GetObjectB() const;
            /// @internal
            /// @brief Gets a string containing the combined names of the objects in this pair.
            String GetCombinedName() const;
        public:
            /// @brief The first actor in the pair.
            ActorBase* Actor;
            /// @brief The second actor in the pair.
            TerrainBase* Terrain;

            /// @brief No initialization constructor.
            ActorTerrainPair();
            /// @brief Class Constructor.
            /// @param A The first actor to be applied to this pair.
            /// @param B The second actor to be applied to this pair.
            ActorTerrainPair(ActorBase* A, TerrainBase* B);
            /// @brief Reference copy constructor.
            /// @param OtherPair The other pair to be copied.
            ActorTerrainPair(const ActorTerrainPair& OtherPair);
    };//ActorPair
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorAreaEffectPair
    /// @headerfile objectpair.h
    /// @brief This is a basic utility class for tracking pairs of actors.
    /// @details Most commonly this class is used in collision tracking.
    ///////////////////////////////////////
    class MEZZ_LIB ActorAreaEffectPair : public ObjectPair
    {
        protected:
            /// @internal
            /// @brief Gets a pointer to the first object in this pair.
            void* GetObjectA() const;
            /// @internal
            /// @brief Gets a pointer to the second object in this pair.
            void* GetObjectB() const;
            /// @internal
            /// @brief Gets a string containing the combined names of the objects in this pair.
            String GetCombinedName() const;
        public:
            /// @brief The first actor in the pair.
            ActorBase* Actor;
            /// @brief The second actor in the pair.
            AreaEffect* AE;

            /// @brief No initialization constructor.
            ActorAreaEffectPair();
            /// @brief Class Constructor.
            /// @param A The first actor to be applied to this pair.
            /// @param B The second actor to be applied to this pair.
            ActorAreaEffectPair(ActorBase* A, AreaEffect* B);
            /// @brief Reference copy constructor.
            /// @param OtherPair The other pair to be copied.
            ActorAreaEffectPair(const ActorAreaEffectPair& OtherPair);
    };//ActorPair
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorPair
    /// @headerfile objectpair.h
    /// @brief This is a basic utility class for tracking pairs of actors.
    /// @details Most commonly this class is used in collision tracking.
    ///////////////////////////////////////
    class MEZZ_LIB AreaEffectTerrainPair : public ObjectPair
    {
        protected:
            /// @internal
            /// @brief Gets a pointer to the first object in this pair.
            void* GetObjectA() const;
            /// @internal
            /// @brief Gets a pointer to the second object in this pair.
            void* GetObjectB() const;
            /// @internal
            /// @brief Gets a string containing the combined names of the objects in this pair.
            String GetCombinedName() const;
        public:
            /// @brief The first actor in the pair.
            AreaEffect* AE;
            /// @brief The second actor in the pair.
            TerrainBase* Terrain;

            /// @brief No initialization constructor.
            AreaEffectTerrainPair();
            /// @brief Class Constructor.
            /// @param A The first actor to be applied to this pair.
            /// @param B The second actor to be applied to this pair.
            AreaEffectTerrainPair(AreaEffect* A, TerrainBase* B);
            /// @brief Reference copy constructor.
            /// @param OtherPair The other pair to be copied.
            AreaEffectTerrainPair(const AreaEffectTerrainPair& OtherPair);
    };//ActorPair
}//Mezzanine

#endif
