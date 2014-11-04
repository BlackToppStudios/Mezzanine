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
#ifndef _graphicsproceduraltexturemodifiers_h
#define _graphicsproceduraltexturemodifiers_h

#include "Graphics/Procedural/texturebuffer.h"

#include "Graphics/Procedural/Texture/texturemodifier.h"

#include "vector2.h"
#include "matrix3x3.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB BlitModifier : public TextureModifier
            {
            protected:
            public:
                /// @brief Blank constructor.
                BlitModifier();
                /// @brief Class destructor.
                virtual ~BlitModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

            };//BlitModifier

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB BlurModifier : public TextureModifier
            {
            protected:
            public:
                /// @brief Blank constructor.
                BlurModifier();
                /// @brief Class destructor.
                virtual ~BlurModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

            };//BlurModifier

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB ChannelModifier : public TextureModifier
            {
            protected:
            public:
                /// @brief Blank constructor.
                ChannelModifier();
                /// @brief Class destructor.
                virtual ~ChannelModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

            };//ChannelModifier

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB CircleModifier : public TextureModifier
            {
            protected:
            public:
                /// @brief Blank constructor.
                CircleModifier();
                /// @brief Class destructor.
                virtual ~CircleModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

            };//CircleModifier

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB ColoursModifier : public TextureModifier
            {
            protected:
            public:
                /// @brief Blank constructor.
                ColoursModifier();
                /// @brief Class destructor.
                virtual ~ColoursModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

            };//ColoursModifier

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB CombineModifier : public TextureModifier
            {
            protected:
            public:
                /// @brief Blank constructor.
                CombineModifier();
                /// @brief Class destructor.
                virtual ~CombineModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

            };//CombineModifier

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB CrackModifier : public TextureModifier
            {
            protected:
            public:
                /// @brief Blank constructor.
                CrackModifier();
                /// @brief Class destructor.
                virtual ~CrackModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

            };//CrackModifier

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB CycloidModifier : public TextureModifier
            {
            protected:
            public:
                /// @brief Blank constructor.
                CycloidModifier();
                /// @brief Class destructor.
                virtual ~CycloidModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

            };//CycloidModifier

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB DistortModifier : public TextureModifier
            {
            protected:
            public:
                /// @brief Blank constructor.
                DistortModifier();
                /// @brief Class destructor.
                virtual ~DistortModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

            };//DistortModifier

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB EdgeDetectionModifier : public TextureModifier
            {
            protected:
            public:
                /// @brief Blank constructor.
                EdgeDetectionModifier();
                /// @brief Class destructor.
                virtual ~EdgeDetectionModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

            };//EdgeDetectionModifier

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB EllipseModifier : public TextureModifier
            {
            protected:
            public:
                /// @brief Blank constructor.
                EllipseModifier();
                /// @brief Class destructor.
                virtual ~EllipseModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

            };//EllipseModifier

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB GlowModifier : public TextureModifier
            {
            protected:
            public:
                /// @brief Blank constructor.
                GlowModifier();
                /// @brief Class destructor.
                virtual ~GlowModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

            };//GlowModifier

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB JitterModifier : public TextureModifier
            {
            protected:
            public:
                /// @brief Blank constructor.
                JitterModifier();
                /// @brief Class destructor.
                virtual ~JitterModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

            };//JitterModifier

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB LerpModifier : public TextureModifier
            {
            protected:
            public:
                /// @brief Blank constructor.
                LerpModifier();
                /// @brief Class destructor.
                virtual ~LerpModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

            };//LerpModifier

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB LightBakeModifier : public TextureModifier
            {
            protected:
                /// @internal
                /// @brief
                ColourValue AmbientColour;
                /// @internal
                /// @brief
                ColourValue DiffuseColour;
                /// @internal
                /// @brief
                ColourValue SpecularColour;
                /// @internal
                /// @brief
                Vector3 LightPosition;
                /// @internal
                /// @brief
                TextureBuffer* NormalMap;
                /// @internal
                /// @brief
                UInt8 SpecularPower;
                /// @internal
                /// @brief
                UInt8 BumpPower;
            public:
                /// @brief Blank constructor.
                LightBakeModifier();
                /// @brief Class destructor.
                virtual ~LightBakeModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

            };//LightBakeModifier
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
