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

#ifndef _playerview_h
#define _playerview_h

#include "datatypes.h"
#include "transform.h"

namespace Mezzanine
{
    namespace Graphics
    {
        class CameraProxy;
    }
    class World;
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A class used to control the player camera.
    ///////////////////////////////////////
    class PlayerView
    {
    protected:
        /// @brief A pointer to the camera used by this view.
        Graphics::CameraProxy* ViewCam;
    public:
        /// @brief Class constructor.
        PlayerView();
        /// @brief Class destructor.
        virtual ~PlayerView();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Creates all the necessary objects to represent the player in the game world.
        /// @param GameWorld A pointer to the world in which to create the player objects.
        void InitializeInWorld(World* GameWorld);

        /// @brief Sets the camera to update.
        /// @param View A pointer to the camera of the players view.
        void SetCamera(Graphics::CameraProxy* View);
        /// @brief Gets the camera to update.
        /// @return Returns a pointer to the players camera.
        Graphics::CameraProxy* GetCamera() const;

        /// @brief Sets the transform the camera system should use to determine the transform of the camera.
        /// @remarks Depending on the exact camera system this transform could have different meanings.  With
        /// an orbital camera this would be where the camera would look at/orbit, while omitting the rotation.
        /// In first person cameras this could be analogous to the camera transform.
        /// @param TargetTrans The transform the camera system should use for positioning the camera.
        virtual void SetTarget(const Transform& TargetTrans) = 0;
        /// @brief Gets the transform the camera system is using to determine the position of the camera.
        /// @return Returns a Transform containing the transform used to determine the camera position.
        virtual Transform GetTarget() const = 0;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Performs any object specific behaviors that need to be updated per tick.
        /// @param Target The transform the camera system should use for positioning the camera.
        /// @param Delta The amount of time since the last update in microseconds.
        virtual void Update(const Transform& Target, const Whole Delta) = 0;
    };//PlayerView

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A simple camera control system that fixes the camera to an offset.
    ///////////////////////////////////////
    class FixedPlayerView
    {
    protected:
        /// @brief The offset transform to apply to the target transform.
        Transform Offset;
    public:
        /// @brief Class constructor.
        FixedPlayerView();
        /// @brief Class destructor.
        virtual ~FixedPlayerView();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Sets the offset to be applied to the target transform to get the final camera transform.
        /// @param OffsetTrans The offset to be applied to calculate the camera transform.
        virtual void SetOffset(const Transform& OffsetTrans);
        /// @brief Gets the offset to be applied to the target transform to get the final camera transform.
        /// @return Returns a Transform containing the offset to be applied to the camera.
        virtual Transform GetOffset() const;

        /// @copydoc PlayerView::SetTargetTransform(const Transform&)
        virtual void SetTarget(const Transform& TargetTrans);
        /// @copydoc PlayerView::GetTargetTransform() const
        virtual Transform GetTarget() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @copydoc PlayerView::Update(const Transform&, const Whole)
        virtual void Update(const Transform& Target, const Whole Delta);
    };//FixedPlayerView
}//Mezzanine

#endif
