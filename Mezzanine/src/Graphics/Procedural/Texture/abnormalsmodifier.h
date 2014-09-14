// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
/*
 -----------------------------------------------------------------------------
 This source file is part of ogre-procedural

 For the latest info, see http://code.google.com/p/ogre-procedural/

 Copyright (c) 2010-2013 Michael Broutin

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */
#ifndef _graphicsproceduralabnormalsmodifier_h
#define _graphicsproceduralabnormalsmodifier_h

#include "Graphics/Procedural/Texture/texturemodifier.h"

#include "vector3.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A modifier that will "rotate" the colours of a buffer using a preset Quaternion, a parameter image, or both.
            /// @details Colours don't represent positions in 2D or 3D space, so the idea of rotating them is very unintuitive.  The description that
            /// the colours are being rotated comes from the process of this modifier where it will take the 4 values of a ColourValue and place them
            /// into a normalized Quaternion, and then rotate the Quaternion, and then convert it back to a colour.
            ///////////////////////////////////////
            class MEZZ_LIB AbnormalsModifier : public TextureModifier
            {
            public:
                /// @enum AbnormalsCompensation
                /// @brief An enum used to describe how to interpret the AbnormalsTexture member.
                enum AbnormalsCompensation
                {
                    AC_Normal = 0,      ///< Use AbnormalsTexture as a Normal Map.
                    AC_Height = 1,      ///< Use AbnormalsTexture as a Height Map.
                    AC_Quaternion = 2   ///< Use AbnormalsTexture as a Quaternion list.
                };
                /// @enum AbnormalsMirror
                /// @brief An enum used to describe which axis or axes to flip the channel intensity on.
                enum AbnormalsMirrorFlags
                {
                    AMF_None = 0,       ///< Do not mirror any axis.
                    AMF_X = 1,          ///< Mirror the X axis.
                    AMF_Y = 2,          ///< Mirror the Y axis.
                    AMF_Z = 4           ///< Mirror the Z axis.
                };
            protected:
                /// @internal
                /// @brief The axis of the colour rotation.  This is what is primarily responsible for colours changing.
                Vector3 Axis;
                /// @internal
                /// @brief The angle of the colour rotation in radians.
                Real Angle;
                /// @internal
                /// @brief A pointer to the optional texture to further modify colours from.  Interpretation is set by the Compensation member.
                TextureBuffer* AbnormalsTexture;
                /// @internal
                /// @brief A bitfield describing the axes to be flipped.
                Whole Mirror;
                /// @internal
                /// @brief An AbnormalsCompensation enum value describing how to interpret the AbnormalsTexture member.
                AbnormalsCompensation Compensation;
                /// @internal
                /// @brief Adjusts the weight of the pixels in the optional "Abnormals" texture when determining the amount of colour rotation to apply.  Range: 0-255.
                UInt8 Sensitivity;
            public:
                /// @brief Blank constructor.
                AbnormalsModifier();
                /// @brief Class destructor.
                virtual ~AbnormalsModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                /// @exception The AbnormalsTexture and the Buffer to be modified must be the same size.  Otherwise an INVALID_STATE_EXCEPTION will be thrown.
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the optional texture that will be used to update the colours of the modifier buffer.
                /// @param Abnormals A pointer to the optional texture to further modify colours from.  Interpretation is set by the Compensation member.  Initial value: NULL.
                /// @return Returns a reference to this.
                AbnormalsModifier& SetAbnormalsTexture(TextureBuffer* Abnormals);
                /// @brief Sets the angle of the colour "rotation".
                /// @param ModifierAngle The angle of the colour rotation in radians.  Initial value: 0.
                /// @return Returns a reference to this.
                AbnormalsModifier& SetAngle(const Real ModifierAngle);
                /// @brief Sets the axis of the colour "rotation".
                /// @param ModifierAxis The axis of the colour rotation.  This is what is primarily responsible for colours changing.  Initial value: 0,0,1.
                /// @return Returns a reference to this.
                AbnormalsModifier& SetAxis(const Vector3& ModifierAxis);
                /// @brief Sets the axis of the colour "rotation".
                /// @param X The X component of the colour rotation axis.  Initial value: 0.
                /// @param Y The Y component of the colour rotation axis.  Initial value: 0.
                /// @param Z The Z component of the colour rotation axis.  Initial value: 1.
                /// @return Returns a reference to this.
                AbnormalsModifier& SetAxis(const Real X, const Real Y, const Real Z);
                /// @brief Sets the weight of the values read from the AbnormalsTexture.  This is ignored if the AbnormalsTexture is NULL.
                /// @param ModifierSensitivity Adjusts the weight of the pixels in the optional "Abnormals" texture when determining the amount of colour rotation to apply.  Range: 0-255.  Initial value: 127.
                /// @return Returns a reference to this.
                AbnormalsModifier& SetSensitivity(const UInt8 ModifierSensitivity);
                /// @brief Sets how the optional Abnormals texture is to be interpretted.
                /// @param AbnormComp An AbnormalsCompensation enum value describing how to interpret the AbnormalsTexture member.  Initial value: AC_Normal.
                /// @return Returns a reference to this.
                AbnormalsModifier& SetCompensation(const AbnormalsCompensation AbnormComp);
                /// @brief Sets which axis or axes should be mirrored within the expected range(0-1).
                /// @param AbnormMirror A bitfield describing the axes to be flipped.  Initial value: AMF_None.
                /// @return Returns a reference to this.
                AbnormalsModifier& SetMirror(const Whole AbnormMirror);
            };//AbnormalsModifier
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
