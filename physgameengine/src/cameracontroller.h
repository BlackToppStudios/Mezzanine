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
#ifndef _cameracontroller_h
#define _cameracontroller_h

namespace phys
{
    class Camera;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class CameraController
    /// @headerfile cameracontroller.h
    /// @brief This is a simplified controller class for use with cameras.
    /// @details This class is useful for manipulating cameras to move around in simple ways,
    /// such as flying through a scene.
    ///////////////////////////////////////
    class CameraController
    {
        public:
            /// @enum MovementMode
            /// @brief Possible options for determining how the camera should move relative to the world.
            /// @details Options are as follows: @n
            /// CCM_Fly: This is the default option for every Camera Controller.  Allows the camera unrestrained movement throughout the scene. @n
            /// CCM_Walk: This forces the camera to be only a certain distance above the terrain.
            enum MovementMode
            {
                CCM_Fly,
                CCM_Walk
            };
        protected:
            Camera* Controlled;
            MovementMode CurrentMode;
            Real HoverHeight;
        public:
            /// @brief Class constructor.
            /// @param ToBeControlled The camera this controller is controlling.
            CameraController(Camera* ToBeControlled);
            /// @brief Class destructor.
            ~CameraController();
            /// @brief Gets the camera this controller is controlling.
            /// @return Returns a camera pointer for the camera this controller is applied to.
            Camera* GetControlledCamera();
            /// @brief Sets the movement mode for this camera/controller.
            /// @param MoveMode The MovementMode value for which mode you want applied.  See MovementMode enum for more info.
            void SetMovementMode(MovementMode MoveMode);
            /// @brief Gets the currently set movement mode.
            /// @return Returns an enum value representing the current movement mode.  See MovementMode enum for more info.
            MovementMode GetMovementMode();
            /// @brief Sets the hover distance for the camera while it's moving.
            /// @details Hover is only applied in CCM_Walk mode.  Default: 1.0.
            /// @param Hover The distance above the ground to hover, in world units.
            void SetHoverHeight(Real Hover);
            /// @brief Gets the distance the camera hovers over terrain while in CCM_Walk mode.
            /// @return Returns a Real represening the distance above terrain the camera is to hover, in world units.
            Real GetHoverHeight();
        };//cameracontroller
}//phys

#endif
