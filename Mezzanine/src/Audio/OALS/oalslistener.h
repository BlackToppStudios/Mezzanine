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
#ifndef _audiooalslistener_h
#define _audiooalslistener_h

#include "Audio/listener.h"

// OALS forward declares
struct ALCcontext;

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief This is the OpenALSoft implemenation for an Audio listener.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB Listener : public iListener
            {
            protected:
                /// @internal
                /// @brief This is a pointer to the OALS context this listener belongs to.
                ALCcontext* Context;
                /// @internal
                /// @brief This stores the modifier to be applied to the volume of all sounds heard by this listener.
                Real VolumeModifier;
                /// @internal
                /// @brief This stores the amount of meters to be assumed in one unit of world space.
                Real MPU;
                /// @internal
                /// @brief This stores the current location of this listener in 3D space.
                Vector3 Location;
                /// @internal
                /// @brief This stores the current velocity of this listener.
                Vector3 Velocity;
                /// @internal
                /// @brief This stores the current orientation of this listener.
                Quaternion Orientation;
                /// @internal
                /// @brief Converts our orientation into something OpanAL can read/use.
                void ConvertBuffer(Real* OrientationArray);
                /// @internal
                /// @brief Makes the context this listener belongs to the current context.
                void MakeCurrent();
            public:
                /// @brief Internal constructor.
                /// @param ListenContext The context this listener belongs to.
                Listener(ALCcontext* ListenContext);
                /// @brief Class destructor.
                virtual ~Listener();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc iListener::SetVelocity(const Vector3& Vel)
                virtual void SetVelocity(const Vector3& Vel);
                /// @copydoc iListener::GetVelocity() const
                virtual Vector3 GetVelocity() const;

                /// @copydoc iListener::SetVolumeModifier(const Real Vol)
                virtual void SetVolumeModifier(const Real Vol);
                /// @copydoc iListener::GetVolumeModifier() const
                virtual Real GetVolumeModifier() const;
                /// @copydoc iListener::SetMetersPerUnit(const Real Meters)
                virtual void SetMetersPerUnit(const Real Meters);
                /// @copydoc iListener::GetMetersPerUnit() const
                virtual Real GetMetersPerUnit() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Transform Methods

                /// @copydoc WorldProxy::SetLocation(const Vector3& Loc)
                virtual void SetLocation(const Vector3& Loc);
                /// @copydoc WorldProxy::SetLocation(const Real X, const Real Y, const Real Z)
                virtual void SetLocation(const Real X, const Real Y, const Real Z);
                /// @copydoc WorldProxy::GetLocation() const
                virtual Vector3 GetLocation() const;
                /// @copydoc WorldProxy::SetOrientation(const Quaternion& Ori)
                virtual void SetOrientation(const Quaternion& Ori);
                /// @copydoc WorldProxy::SetOrientation(const Real X, const Real Y, const Real Z, const Real W)
                virtual void SetOrientation(const Real X, const Real Y, const Real Z, const Real W);
                /// @copydoc WorldProxy::GetOrientation() const
                virtual Quaternion GetOrientation() const;
                /// @copydoc WorldProxy::SetScale(const Vector3& Sc)
                virtual void SetScale(const Vector3& Sc);
                /// @copydoc WorldProxy::SetScale(const Real X, const Real Y, const Real Z)
                virtual void SetScale(const Real X, const Real Y, const Real Z);
                /// @copydoc WorldProxy::GetScale() const
                virtual Vector3 GetScale() const;

                /// @copydoc WorldProxy::Translate(const Vector3& Trans)
                virtual void Translate(const Vector3& Trans);
                /// @copydoc WorldProxy::Translate(const Real X, const Real Y, const Real Z)
                virtual void Translate(const Real X, const Real Y, const Real Z);
                /// @copydoc WorldProxy::Yaw(const Real Angle)
                virtual void Yaw(const Real Angle);
                /// @copydoc WorldProxy::Pitch(const Real Angle)
                virtual void Pitch(const Real Angle);
                /// @copydoc WorldProxy::Roll(const Real Angle)
                virtual void Roll(const Real Angle);
                /// @copydoc WorldProxy::Rotate(const Vector3& Axis, const Real Angle)
                virtual void Rotate(const Vector3& Axis, const Real Angle);
                /// @copydoc WorldProxy::Rotate(const Quaternion& Rotation)
                virtual void Rotate(const Quaternion& Rotation);
                /// @copydoc WorldProxy::Scale(const Vector3& Scale)
                virtual void Scale(const Vector3& Scale);
                /// @copydoc WorldProxy::Scale(const Real X, const Real Y, const Real Z)
                virtual void Scale(const Real X, const Real Y, const Real Z);
            };//Listener
        }//OALS
    }//Audio
}//Mezzanine

#endif
