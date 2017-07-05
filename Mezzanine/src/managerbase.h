// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _managerbase_h
#define _managerbase_h

#include "datatypes.h"
#include "crossplatformexport.h"
#ifndef SWIG
    #include "XML/xml.h"
#endif

namespace Mezzanine
{
    class Entresol;
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is the base class from which all the Entresol and World Managers inherit.
    /// @details This creates a base set of functions that Managers are all
    /// expected to implement.
    ///////////////////////////////////////
    class MEZZ_LIB ManagerBase
    {
    public:
        /// @brief A listing of Manager Types.
        /// @details These will be returned by ManagerBase::GetType(), and will allow
        /// code using this to determine what type of Manager class they are working with
        /// and use this information to more safely cast to the correct manager if needed.
        /// @n @n
        /// When extending the Mezzanine with additional managers, you should try to stay
        /// within the ranges specified (where applicable) as some systems use range-based
        /// checks.
        enum ManagerType
        {
            // Utility Types

            MT_Undefined               = 0,

            // Entresol Managers              // Namespaces

            MT_AudioManager            = 1,   // Audio
            MT_AnimationManager        = 2,   // Graphics
            MT_CollisionShapeManager   = 3,   // Physics
            MT_CompositorManager       = 4,   // Graphics
            MT_GraphicsManager         = 5,   // Graphics
            MT_InputManager            = 7,   // Input
            MT_LogManager              = 8,   // Mezzanine
            MT_MaterialManager         = 9,   // Graphics
            MT_MeshManager             = 10,  // Graphics
            MT_NetworkManager          = 11,  // Network
            MT_PlayerManager           = 12,  // Mezzanine
            MT_ResourceManager         = 13,  // Resource
            MT_ScriptingManager        = 14,  // Scripting
            MT_TextureManager          = 15,  // Graphics
            MT_UIManager               = 16,  // UI

            // World Managers                 // Namespaces

            MT_EntityComponentManagerFirst = 100,

            MT_PhysicsManager              = 105, // Physics
            MT_SceneManager                = 106, // Graphics
            MT_SoundScapeManager           = 107, // Audio

            MT_EntityComponentManagerLast  = 199,
            MT_EntityManagerFirst = 200,

            MT_ActorManager            = 201, // Mezzanine
            MT_AreaEffectManager       = 202, // Mezzanine
            MT_DebrisManager           = 203, // Mezzanine
            MT_TerrainManager          = 208, // Mezzanine
            MT_VehicleManager          = 209, // Mezzanine

            MT_EntityManagerLast  = 299,

            // Other Managers                 // Namespaces

            MT_UserManagerFirst        = 1000,

            MT_UserCreated             = 1001
        };
    protected:
        /// @internal
        /// @brief The actual pointer to the Entresol core class.
        Entresol* TheEntresol;
        /// @internal
        /// @brief Simple Boole indicating whether or not this manager has been initialized.
        Boole Initialized;
    public:
        /// @brief Class constructor.
        ManagerBase();
        /// @brief Class destructor.
        virtual ~ManagerBase();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        ///////////////////////////////////////////////////////////////////////////////
        // Initialization Methods

        /// @brief Configures this manager for use prior to entering the main loop.
        virtual void Initialize() = 0;
        /// @brief Removes this manager from any necessary configuration so it can be safely disposed of.
        virtual void Deinitialize() = 0;
        /// @brief Gets whether or not this manager has been initialized.
        /// @return Returns true if this manager has been initialized, false otherwise.
        Boole IsInitialized() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        /// @brief This returns the type of this manager.
        /// @details This is intended to make using and casting from Manager base easier. With this is is possible to cast from
        /// ManagerBase to the correct Manager Type.
        /// @return This returns a ManagerTypeName to identify what this can be safely cast to.
        virtual ManagerType GetInterfaceType() const = 0;
        /// @brief This Allows any manager name to be sent to a stream. Primarily used for logging
        /// @return This returns a String that contains the name.
        virtual String GetImplementationTypeName() const = 0;
        /// @brief Gets a string of the interface type of this manager.
        /// @return Returns a string containing the interface name of this manager.
        virtual String GetInterfaceTypeAsString() const;

        /// @brief Gets the string form of the type of manager.
        /// @return Returns a string containing the name of the requested type of manager.
        static String GetTypeAsString(const ManagerType& ManagerType);
        /// @brief Gets the type of manager requested from a string.
        /// @remarks This function does not try to compare the full string for the sake of speed.  Instead it'll check the first couple letters for a potential match.
        /// This function is also not case sensative.  Providing the string "ac" will return an ActorManager value, for example.  Additionally if it does not find a
        /// match it will throw an exception.  So be careful about what you put into this.
        /// @return Returns a ManagerTypeName cooresponding to the string provided.
        static ManagerType GetTypeFromString(const String& ManagerName);
    };//ManagerBase
}//Mezzanine

#endif
