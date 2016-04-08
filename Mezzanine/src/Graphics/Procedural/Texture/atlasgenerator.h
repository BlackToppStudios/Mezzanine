// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
/*
 Portions of the algorithms contained in this file are derived from the work of
 Jukka Jylänki which was released to the public domain.

 The original source code can be found at this repo:
 https://github.com/juj/RectangleBinPack
*/
#ifndef _graphicsproceduralatlasgenerator_h
#define _graphicsproceduralatlasgenerator_h

#include "Graphics/Procedural/Texture/texturegenerator.h"

#include "rect.h"
#include "Graphics/image.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A generator that will produce an image that is a number of other images placed side by side with no overlap.
            /// @details The use of this Generator is a little bit different than most generators, but only a little.  When everything is set
            /// you still call the final generate methods as you would normally, but it's expected to have a lot of setup.  Adding images
            /// must be done individually so the user may be informed of the position the image has been placed in the Atlas.  This
            /// generator does not know or care what the names or ID's of these images are, so if either is required they must be tracked
            /// elsewhere.
            ///////////////////////////////////////
            class MEZZ_LIB AtlasGenerator : public TextureGenerator
            {
            public:
                /// @brief A convenience type for the storage of Rects by this class.
                typedef std::vector<WholeRect>           RectContainer;
                /// @brief An iterator type for Images to be Rects by this class.
                typedef RectContainer::iterator          ImageIterator;
                /// @brief A const iterator type for Images to be Rects by this class.
                typedef RectContainer::const_iterator    ConstImageIterator;
            protected:
                /// @internal
                /// @brief A container storing all of the available Rects in the texture to be generated.
                RectContainer FreeRects;
                /// @internal
                /// @brief A container storing all of the used Rects in the texture to be generated.
                RectContainer UsedRects;
                /// @internal
                /// @brief The amount of extra space to enforce around every Image to be Atlas'd.
                Whole ImagePadding;
            public:
                /// @brief Class constructor.
                AtlasGenerator();
                /// @brief Class destructor.
                virtual ~AtlasGenerator();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureGenerator::AddToTextureBuffer(TextureBuffer&) const
                virtual void AddToTextureBuffer(TextureBuffer& Buffer) const;
                /// @copydoc TextureGenerator::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Adds an Image to be combined into the final Texture Atlas.
                /// @param ToAdd The loaded Image to add.
                /// @return Returns a WholeRect containing the position on the Atlas the image was placed.  An invalid (zero dim) rect means there was no room.
                WholeRect AddImage(const Graphics::Image& ToAdd);
                /// @brief Adds an Image to be combined into the final Texture Atlas.
                /// @remarks This method is identical to calling the dual String overload, but putting "UI" for the asset group.
                /// @param AssetName The name of the Image as it would be found in an asset group.
                /// @return Returns a WholeRect containing the position on the Atlas the image was placed.  An invalid (zero dim) rect means there was no room.
                WholeRect AddImage(const String& AssetName);
                /// @brief Adds an Image to be combined into the final Texture Atlas.
                /// @param AssetName The name of the Image in an asset group.
                /// @param AssetGroup The name of the asset group where the named Image can be found.
                /// @return Returns a WholeRect containing the position on the Atlas the image was placed.  An invalid (zero dim) rect means there was no room.
                WholeRect AddImage(const String& AssetName, const String& AssetGroup);

                /// @brief Sets extra padding around every Image added to the final Atlas.
                /// @param Padding The amount of blank pixels to ensure every edge of every added Image has.
                /// @return Returns a reference to this.
                AtlasGenerator& SetPadding(const Whole Padding);
            };//AtlasGenerator
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
