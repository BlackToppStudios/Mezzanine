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

#ifndef _uisimplerenderer_h
#define _uisimplerenderer_h

#include "UI/vertex.h"
#include "XML/xml.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class SimpleRenderer
        /// @headerfile simplerenderer.h
        /// @brief A simple class providing basic methods to generate vertices with.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB SimpleRenderer
        {
        protected:
            /// @internal
            /// @brief This determines whether or not the verticies in this renderer need to be refreshed.
            Boole Dirty;
            /// @internal
            /// @brief This contains the name of the atlas that will be used as default when one isn't specified.
            String PriAtlas;
            /// @internal
            /// @brief This is a container storing all the Verticies generated by this renderer.
            std::vector<VertexData> RenderVertices;
            /// @internal
            /// @brief Provides the class specific implementation for regenerating vertices for this renderable.
            virtual void RedrawImpl(Boole Force) = 0;
            /// @internal
            /// @brief Collects all the relevant information for a single vertex and pushes it to a vector.
            virtual void PushVertex(const Real& X, const Real& Y, const Vector2& UV, const ColourValue& Colour, const String& Atlas);
            /// @internal
            /// @brief Pushes vertex information for a triangle to a vector.  Equivalent to calling "PushVertex" three times.
            virtual void PushTriangle(const Vector2& A, const Vector2& B, const Vector2& C, const Vector2& UV, const ColourValue& Colour, const String& Atlas);
        //public:
            /// @brief Class constructor.
            SimpleRenderer();
            /// @brief Class destructor.
            virtual ~SimpleRenderer();
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Sets the Atlas to be assumed when one isn't provided for atlas related tasks.
            /// @param Atlas The name of the atlas to be used.
            virtual void SetPrimaryAtlas(const String& Atlas);
            /// @brief Gets the currently set primary atlas.
            /// @return Returns a string containing the name of the primary atlas that is set, or an empty string if none.
            virtual String GetPrimaryAtlas() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert this class to an XML::Node ready for serialization.
            /// @param ParentNode The point in the XML hierarchy that all this renderable should be appended to.
            virtual void ProtoSerialize(XML::Node& ParentNode) const;
            /// @brief Convert the properties of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;

            /// @brief Take the data stored in an XML Node and overwrite this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerialize(const XML::Node& SelfRoot);
            /// @brief Take the data stored in an XML Node and overwrite the properties of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @brief Gets the most derived serializable name of this Renderable.
            /// @note When creating a new Renderable class verify this method has a valid return for it in order for serialization to work properly.
            /// @return Returns the name of the XML tag from the most derived class of "this".
            virtual String GetDerivedSerializableName() const;
            /// @brief Get the name of the the XML tag the Renderable class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Marks this renderable as well as all parent objects as dirty.
            virtual void _MarkDirty() = 0;
            /// @internal
            /// @brief Gets whether or not this renderer is dirty.
            /// @return Returns true if this renderer is dirty, false otherwise.
            Boole _IsDirty();
            /// @internal
            /// @brief Regenerates the verticies in this renderable.
            /// @param Force If true this will force this object to redraw it's verticies regardless of whether it is dirty.
            void _Redraw(Boole Force);
            /// @internal
            /// @brief Appends the vertices of this renderable to another vector.
            /// @param Vertices The vector of vertex's to append to.
            void _AppendVertices(std::vector<VertexData>& Vertices);
        };//SimpleRenderer
    }//UI
}//Mezzanine

#endif
