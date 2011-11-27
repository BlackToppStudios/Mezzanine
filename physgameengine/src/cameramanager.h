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
#ifndef _CameraManager_h
#define _CameraManager_h

#include "managerbase.h"
#include "singleton.h"
#include "quaternion.h"
#include "ray.h"
#include "vector3.h"

#include <map>

namespace Ogre
{
    class Camera;
    class SceneNode;
    class Viewport;
}

namespace phys
{
    class Camera;
    class CameraController;
    class SceneManager;
    class GraphicsManager;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class CameraManager
    /// @headerfile cameramanager.h
    /// @brief This is the manager class for all camera functions.
    /// @details This class contains all the functionality of the use and manipulation of the camera. @n
    /// All functions that manipulate the camera will default to the default camera, so if you only use
    /// one camera you should never have to name the camera you want to use. @n
    /// This class should only be created after the SceneManager has been created.
    ///////////////////////////////////////////////////////////////////////////////
    class PHYS_LIB CameraManager : public ManagerBase, public Singleton<CameraManager>
    {
        protected:
            friend class GraphicsManager;
            friend class World;
            friend class Camera;

            Camera* DefaultCamera;
            std::vector< Camera* > Cameras;
            std::map< Camera* , CameraController* > CameraControllers;
            Camera* FindCamera(const String& Name);
        public:
            /// @internal
            /// @brief Used to reference the appropriate scene
            phys::SceneManager* SManager;

            /// @brief Class Constructor.
            /// @details This is the class constructor.  This is automatcally called in the World.CreateRenderWindow()
            /// function and should never need to be called manually.
            /// @param SceneManagerIndex The SceneManager to user as indexed by phys::World
            // /// @param SceneManagerName Name of the created SceneManager for this camera manager to use.
            // /// @param SManager A pointer to the Scenemanager where you will be creating/manipulating all the cameras.
            CameraManager(Whole SceneManagerIndex);
            /// @brief Class Destructor.
            /// @details The calss Destuctor
            virtual ~CameraManager();
            /// @brief Creates (or recreates) the default camera for this manager.
            /// @details If this function is called while there is a valid default camera already created, it will delete that camera.
            /// @return Returns a pointer to the created camera.
            Camera* CreateDefaultCamera();
            /// @brief Gets the default camera if it has been initialized.
            /// @return Returns the Default Camera or a null point if it hasn't been created yet.
            Camera* GetDefaultCamera();
            /// @brief Destroys the default camera.
            void DestroyDefaultCamera();
            /// @brief Creates a camera and returns a pointer.
            /// @details This function does the same as the other CreateCamera function but will also return a pointer to
            /// the camera class instead of a string(being the name of the camera).
            Camera* CreateCamera(const String& Name);
            /// @brief Gets an already created camera by name.
            /// @return Returns a pointer to the camera of the specified name.
            Camera* GetCamera(const String& Name);
            /// @brief Gets an already created camera by index.
            /// @return Returns a pointer to the camera at the specified index.
            Camera* GetCamera(const Whole& Index);
            /// @brief Gets the number of cameras created and stored in this manager.
            /// @return Returns the number of cameras this manager is storing.
            Whole GetNumCameras();
            /// @brief Deletes all cameras except for the first camera.
            /// @details This will clear the container of cameras.  The default camera is not stored in this container
            /// however, so it is spared from this wipe.
            /// @param DefaultAlso Whether or not to also destroy the default camera when deleting all camera's.
            void DestroyAllCameras(bool DefaultAlso = true);

            /// @brief Gets a camera controller if it exists, otherwise creates it.
            /// @param Controlled The camera that will be controlled by the controller returned.
            /// @return Returns a pointer to the created or retrieved camera controller for the camera.
            CameraController* GetOrCreateCameraController(Camera* Controlled);
            /// @brief Destroys a cameracontroller.
            /// @param ToBeDestroyed Pointer to the cameracontrolled you want destroyed.
            void DestroyCameraController(CameraController* ToBeDestroyed);
            /// @brief Destroys a cameracontroller by camera.
            /// @param ControlledCam The camera who's controller will be destroyed.  This doesn't do anything to the camera.
            void DestroyCameraController(Camera* ControlledCam);
            /// @brief Destroys all camera controllers being stored in this manager.
            void DestroyAllCameraControllers();

            //Inherited From ManagerBase
            /// @brief Empty Initializor.
            /// @details This specific initializor is unneeded, but we implement it for compatibility. It also exists
            /// in case a derived class wants to override it for some reason.
            virtual void Initialize();

            /// @brief Empty MainLoopItems.
            /// @details This class implements this for the sake of extension and compatibility.  This function does nothing.
            virtual void DoMainLoopItems();

            /// @brief This returns the type of this manager.
            /// @return This returns ManagerTypeName::CameraManager.
            virtual ManagerTypeName GetType() const;
    };
}//phys
#endif
