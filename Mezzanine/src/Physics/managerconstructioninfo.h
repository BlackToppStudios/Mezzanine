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
#ifndef _physicsmanagerconstructioninfo_h
#define _physicsmanagerconstructioninfo_h

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a helper class storing information needed for the construction of a PhysicsManager.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB ManagerConstructionInfo
        {
        public:
            /// @enum ManagerConstructionFlags
            /// @brief This is an enum used by the physics manager to determine what internal
            /// classes should be used when creating the world.
            enum ManagerConstructionFlags
            {
                PCF_SoftRigidWorld = 1,   ///< Enables support for soft bodies in the simulation.
                PCF_LimitlessWorld = 2,   ///< Enables support for very large simulations.
                PCF_Multithreaded = 4     ///< Enables multi-threaded acceleration structures.
            };//ManagerConstructionFlags

            /// @brief The flags to initialize the physics system with.
            Whole PhysicsFlags;
            /// @brief The maximum number of Actors and Area Effects you expect to have in the world at one time.
            /// @note This member is ignored if the "PCF_LimitlessWorld" flag is passed in.
            Whole MaxProxies;
            /// @brief The lower limits of the worlds AABB.
            /// @note This member is ignored if the "PCF_LimitlessWorld" flag is passed in.
            Vector3 GeographyLowerBounds;
            /// @brief The upper limits of the worlds AABB.
            /// @note This member is ignored if the "PCF_LimitlessWorld" flag is passed in.
            Vector3 GeographyUpperBounds;
            /// @brief The gravity to set for the world.
            Vector3 Gravity;

            /// @brief Class constructor.
            ManagerConstructionInfo() :
                PhysicsFlags(0),
                MaxProxies(0),
                GeographyLowerBounds(Vector3()),
                GeographyUpperBounds(Vector3()),
                Gravity(Vector3()) {  }
            /// @brief Class destructor.
            ~ManagerConstructionInfo() {  }
            /// @brief Assignment Operator.
            /// @param Other The other PhysicsConstructionInfo to be copied.
            /// @return Returns a reference to this.
            ManagerConstructionInfo& operator=(const ManagerConstructionInfo& Other)
            {
                this->PhysicsFlags = Other.PhysicsFlags;
                this->MaxProxies = Other.MaxProxies;
                this->GeographyLowerBounds = Other.GeographyLowerBounds;
                this->GeographyUpperBounds = Other.GeographyUpperBounds;
                this->Gravity = Other.Gravity;
                return *this;
            }
        };//ManagerConstructionInfo
    }//Physics
}//Mezzanine

#endif
