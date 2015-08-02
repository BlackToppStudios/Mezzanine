// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
#ifndef _uisprite_h
#define _uisprite_h

#include "vector2.h"
#include "UI/uienumerations.h"
#include "UI/textureatlas.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief Basic class used to describe a portion of a texture to be applied to a Quad.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB Sprite
        {
        public:
            /// @brief Class constructor.
            /// @brief The name to be given to this sprite.
            /// @param UVTop The top position of this sprite on the Atlas.
            /// @param UVLeft The left position of this sprite on the Atlas.
            /// @param UVBottom The bottom position of this sprite on the Atlas.
            /// @param UVRight The right position of this sprite on the Atlas.
            /// @param ParentAtlas The TextureAtlas this sprite belongs to.
            Sprite(const String& SpriteName, const Real UVTop, const Real UVLeft, const Real UVBottom, const Real UVRight, TextureAtlas* ParentAtlas) :
                Atlas(ParentAtlas),
                Name(SpriteName),
                Top(UVTop),
                Left(UVLeft),
                Bottom(UVBottom),
                Right(UVRight)
                {  }
            /// @brief Class destructor.
            ~Sprite()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Public Data Members

            /// @brief The TextureAtlas this sprite belongs to.
            TextureAtlas* Atlas;
            /// @brief The name of this sprite.
            const String Name;
            /// @brief The top position of this sprite on the Atlas.
            const Real Top;
            /// @brief The left position of this sprite on the Atlas.
            const Real Left;
            /// @brief The bottom position of this sprite on the Atlas.
            const Real Bottom;
            /// @brief The right position of this sprite on the Atlas.
            const Real Right;

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Gets the name of the atlas this sprite belongs to.
            inline const String& GetAtlasName() const
                { return this->Atlas->GetName(); }

            ///////////////////////////////////////////////////////////////////////////////
            // Position and Size Methods

            /// @brief Gets the Top coordinate on the Atlas texture.
            /// @return Returns a Real representing the top position of this sprite on the Atlas texture in pixels.
            inline Real GetUVTop() const
                { return this->Top * this->Atlas->GetTextureSize().Y; }
            /// @brief Gets the Bottom coordinate on the Atlas texture.
            /// @return Returns a Real representing the bottom position of this sprite on the Atlas texture in pixels.
            inline Real GetUVBottom() const
                { return this->Bottom * this->Atlas->GetTextureSize().Y; }
            /// @brief Gets the Left coordinate on the Atlas texture.
            /// @return Returns a Real representing the left position of this sprite on the Atlas texture in pixels.
            inline Real GetUVLeft() const
                { return this->Left * this->Atlas->GetTextureSize().X; }
            /// @brief Gets the Right coordinate on the Atlas texture.
            /// @return Returns a Real representing the right position of this sprite on the Atlas texture in pixels.
            inline Real GetUVRight() const
                { return this->Right * this->Atlas->GetTextureSize().X; }
            /// @brief Gets the position of the sprite on the Atlas texture.
            /// @return Returns a Vector2 containing the top-left position of this sprite on the Atlas texture in pixels.
            inline Vector2 GetPosition() const
                { return Vector2(this->Left,this->Top) * this->Atlas->GetTextureSize(); }
            /// @brief Gets the size of the sprite on the Atlas texture.
            /// @return Returns a Vector2 containing the size of this sprite on the Atlas Texture in pixels.
            inline Vector2 GetSize() const
                { return Vector2(this->GetWidth(),this->GetHeight()); }
            /// @brief Gets the sprite's height on the Atlas texture.
            /// @return Returns a Real representing the height of this sprite on the Atlas Texture in pixels.
            inline Real GetHeight() const
                { return this->GetUVBottom() - this->GetUVTop(); }
            /// @brief Gets the sprite's width on the Atlas texture.
            /// @return Returns a Real representing the width of this sprite on the Atlas Texture in pixels.
            inline Real GetWidth() const
                { return this->GetUVRight() - this->GetUVLeft(); }
            /// @brief Gets the pixel position on the Atlas of a corner belonging to this Sprite.
            /// @param Corner The corner to retrieve the coordinates for.
            /// @return Returns a Vector2 containing the Atlas pixel position of the specific corner.
            inline Vector2 GetAtlasCoords(const UI::QuadCorner Corner) const
            {
                switch(Corner)
                {
                    case UI::QC_TopLeft:      return Vector2(this->Left,this->Top) * this->Atlas->GetTextureSize();      break;
                    case UI::QC_TopRight:     return Vector2(this->Right,this->Top) * this->Atlas->GetTextureSize();     break;
                    case UI::QC_BottomLeft:   return Vector2(this->Left,this->Bottom) * this->Atlas->GetTextureSize();   break;
                    case UI::QC_BottomRight:  return Vector2(this->Right,this->Bottom) * this->Atlas->GetTextureSize();  break;
                    default:
                    { MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Invalid QuadCorner value used to query Atlas Coordinates.");  break; }
                }
            }
            /// @brief Gets the relative position on the Atlas of a corner belonging to this Sprite.
            /// @param Corner The corner to retrieve the coordinates for.
            /// @return Returns a Vector2 containing the Atlas relative position of the specific corner.
            inline Vector2 GetRelativeAtlasCoords(const UI::QuadCorner Corner) const
            {
                switch(Corner)
                {
                    case UI::QC_TopLeft:      return Vector2(this->Left,this->Top);      break;
                    case UI::QC_TopRight:     return Vector2(this->Right,this->Top);     break;
                    case UI::QC_BottomLeft:   return Vector2(this->Left,this->Bottom);   break;
                    case UI::QC_BottomRight:  return Vector2(this->Right,this->Bottom);  break;
                    default:
                    { MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Invalid QuadCorner value used to query Atlas Coordinates.");  break; }
                }
            }
        };//Sprite
    }//UI
}//Mezzanine

#endif
