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
#ifndef _graphicsbillboard_h
#define _graphicsbillboard_h

/// @file
/// @brief This file contains the declaration for the Billboard class used by BillboardSetProxy.

#include "vector3.h"
#include "colourvalue.h"

namespace Ogre
{
    class Billboard;
}

namespace Mezzanine
{
    namespace Graphics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the proxy class for placing and manipulating a set of 2D billboards in the scene.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB Billboard
        {
        protected:
            /// @internal
            /// @brief A pointer to the internal Billboard.
            Ogre::Billboard* GraphicsBillboard;
        public:
            /// @brief Internal constructor.
            /// @param InternalBillboard A pointer to the internal Billboard this instance is based on.
            Billboard(Ogre::Billboard* InternalBillboard);
            /// @brief Class destructor.
            ~Billboard();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets whether or not this billboard has had it's dimensions individually configured.
            /// @return Returns true if this billboard has had it's dimensions manually altered, false otherwise.
            Boole HasOwnDimensions() const;
            /// @brief Resets whatever dimensions are on this billboard to the default provided to the parent BillboardSetProxy.
            void ResetDimensions();
            /// @brief Gets whether or not this billboard is using custom texture coordinates.
            /// @return Returns true if custom texture coordinates have been set for this billboard, false otherwise.
            Boole IsUsingTextureCoordRect() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Billboard Properties

            /// @brief Sets the location of this billboard relative to it's parent set.
            /// @param Loc The relative location to place this billboard.
            void SetLocation(const Vector3& Loc);
            /// @brief Gets the location of this billboard relative to it's parent set.
            /// @return Returns a Vector3 representing the current position of this billboard relative to it's parent.
            Vector3 GetLocation() const;
            /// @brief Sets the facing direction of this billboard.  Only useful with specific types of BillboardSetProxies.
            /// @remarks This value is only used if the parent billboard is of type: "BBT_Oriented_Self" or "BBT_Perpendicular_Self".
            /// @param Dir The facing direction of this billboard.
            void SetDirection(const Vector3& Dir);
            /// @brief Gets the facing direction of this billboard.  Only useful with specific types of BillboardSetProxies.
            /// @remarks This value is only used if the parent billboard is of type: "BBT_Oriented_Self" or "BBT_Perpendicular_Self".
            /// @return Returns a Vector3 representing the current facing direction of this billboard.
            Vector3 GetDirection() const;
            /// @brief Sets the rotation of this billboard.
            /// @param Rot The rotation in radians around the facing axis of this billboard to apply.
            void SetRotation(const Real Rot);
            /// @brief Gets the rotation of this billboard.
            /// @return Returns a Real representing the rotation around the facing axis of this billboard in radians.
            Real GetRotation() const;
            /// @brief Sets the base colour of this billboard.
            /// @note Pure white is default and what you will want to use in most situations as that will preserve the render settings set on the material used to render.
            /// @param Colour The colour that will become the base colour of this billboard.
            void SetColour(const ColourValue& Colour);
            /// @brief Gets the base colour of this billboard.
            /// @return Returns a ColourValue representing the currently set base colour of this billboard.
            ColourValue GetColour() const;

            /// @brief Sets the world unit dimensions of this billboard.
            /// @param Width The world unit width to be assigned to this specific billboard.
            /// @param Height The world unit height to be assigned to this specific billboard.
            void SetDimensions(const Real Width, const Real Height);
            /// @brief Gets the individually configured world unit Width of this billboard.
            /// @return Returns a Real representing the Width of this billboard that has been manually set.
            Real GetOwnWidth() const;
            /// @brief Gets the individually configured world unit Height of this billboard.
            /// @return Returns a Real representing the Height of this billboard that has been manually set.
            Real GetOwnHeight() const;

            /// @brief Sets the rect in texture coordinates to use for rendering this billboard.
            /// @note Texture coordinates should always be between 0.0 and 1.0.
            /// @param Left The Left texture coordinate to use for rendering this billboard.
            /// @param Top The Top texture coordinate to use for rendering this billboard.
            /// @param Right The Right texture coordinate to use for rendering this billboard.
            /// @param Bottom The Bottom texture coordinate to use for rendering this billboard.
            void SetTextureCoordRect(const Real Left, const Real Top, const Real Right, const Real Bottom);
            /// @brief Gets the Left side texture coordinate being used to render this billboard.
            /// @return Returns a Real representing the Left texture coordinate to use to render this billboard.
            Real GetLeftTextureCoord() const;
            /// @brief Gets the Top side texture coordinate being used to render this billboard.
            /// @return Returns a Real representing the top texture coordinate to use to render this billboard.
            Real GetTopTextureCoord() const;
            /// @brief Gets the Right side texture coordinate being used to render this billboard.
            /// @return Returns a Real representing the Right texture coordinate to use to render this billboard.
            Real GetRightTextureCoord() const;
            /// @brief Gets the Bottom side texture coordinate being used to render this billboard.
            /// @return Returns a Real representing the Bottom texture coordinate to use to render this billboard.
            Real GetBottomTextureCoord() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert this class to an XML::Node ready for serialization.
            /// @param ParentNode The point in the XML hierarchy that all this renderable should be appended to.
            void ProtoSerialize(XML::Node& ParentNode) const;
            /// @brief Take the data stored in an XML Node and overwrite this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            void ProtoDeSerialize(const XML::Node& SelfRoot);

            /// @brief Get the name of the the XML tag the proxy class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Accessor for the internal billboard.
            /// @return Returns a pointer to the internal billboard this class is based on.
            Ogre::Billboard* _GetGraphicsObject() const;
        };//Billboard
    }//Graphics
}//Mezzanine

#endif
