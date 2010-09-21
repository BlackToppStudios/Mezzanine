//© Copyright 2010 BlackTopp Studios Inc.
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
#ifndef _scenemanager_h
#define _scenemanager_h

#include "datatypes.h"
#include "managerbase.h"

namespace Ogre
{
    class SceneManager;
}

namespace phys
{
    class World;
    class Light;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class SceneManager
    /// @headerfile scenemanager.h
    /// @brief This class contains utilities and functions to allow the manipulation of the Graphical
    /// scene, rather then the physics inside, or the object inside.
    /// @details This class contains functions that allow the manipulation of lighting, skyboxes, internal
    /// scenemanager types, and more.
    ///////////////////////////////////////
    class SceneManager : public ManagerBase
    {
        public:
            enum SceneManagerType
            {
                Generic,
                Exterior,
                ExteriorRealFar,
                Interior
            };
        protected:
            /// @brief Pointer for the Ogre Scenemanager, where this manager gets it's functionality.
            Ogre::SceneManager* OgreManager;
            std::vector< Light* > Lights;
        public:
            /// @brief Class Constructor.
            /// @details Standard class initialization constructor.
            /// @param Name Name for the created Scene Manager.
            /// @param ManagerType Type of Scene Manager to be created.
            /// @param world Pointer to the world which will work with this Manager.
            SceneManager(const String& Name, SceneManager::SceneManagerType ManagerType, World* world);
            /// @brief Class Destructor.
            /// @details The class destructor.
            ~SceneManager();
            /// @brief Inherited from ManagerBase.
            void Initialize();
            /// @brief Inherited from ManagerBase.
            void DoMainLoopItems();
            /// @brief Sets the ambient light for the scene.
            /// @details Not all scene's will need ambient light.  Ambient light is light that hits all objects from
            /// all directions.
            /// @param Red The value representing the amount of red color in the ambient light.
            /// @param Green The value representing the amount of green color in the ambient light.
            /// @param Blue The value representing the amount of blue color in the ambient light.
            /// @param Alpha The value representing the transparency of the color in the ambient light.
            void SetAmbientLight(Real Red=1.0, Real Green=1.0, Real Blue=1.0, Real Alpha=1.0);
            /// @brief Creates a dynamic light.
            /// @param Name The name to be given to this light.
            /// @return Returns a pointer to the light class which was created by this function.
            Light* CreateLight(const String& Name);
            /// @brief Gets an already created light by name.
            /// @return Returns a pointer to the light of the specified name.
            Light* GetLight(const String& Name);
            /// @brief Deletes a light and removes all trace of it from the manager.
            /// @param light The light to be destroyed.
            void DestroyLight(Light* light);
            /// @brief Gets the name of this manager.
            /// @return Returns the name of this manager.
            String& GetName();
            /// @brief Gets the type of manager that this manager is.
            /// @return Returns an enum value representing the type of manager that this manager is.
            ManagerBase::ManagerTypeName GetType() const;
            /// @brief Gets the internal Ogre Scene Manager pointer.
            /// @details This function returns the internal Ogre Scene Manager pointer.
            /// @return Returns a pointer to the ogre Scene Manager.
            Ogre::SceneManager* GetGraphicsWorldPointer();
    };//scenemanager
}//phys

#endif
