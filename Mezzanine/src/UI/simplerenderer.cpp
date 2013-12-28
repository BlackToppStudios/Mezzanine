//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _uisimplerenderer_cpp
#define _uisimplerenderer_cpp

#include "UI/simplerenderer.h"

#include "serialization.h"
#include "exception.h"

namespace Mezzanine
{
    namespace UI
    {
        SimpleRenderer::SimpleRenderer() :
            Dirty(true)
            {  }

        SimpleRenderer::~SimpleRenderer()
            {  }

        void SimpleRenderer::PushVertex(const Real& X, const Real& Y, const Vector2& UV, const ColourValue& Colour, const String& Atlas)
        {
            VertexData TempVert;
            TempVert.Vert.Position.X = X;
            TempVert.Vert.Position.Y = Y;
            TempVert.Vert.Position.Z = 0;
            TempVert.Vert.UV = UV;
            TempVert.Vert.Colour = Colour;
            TempVert.Atlas = Atlas;
            this->RenderVertices.push_back(TempVert);
        }

        void SimpleRenderer::PushTriangle(const Vector2& A, const Vector2& B, const Vector2& C, const Vector2& UV, const ColourValue& Colour, const String& Atlas)
        {
            this->PushVertex(A.X,A.Y,UV,Colour,Atlas);
            this->PushVertex(B.X,B.Y,UV,Colour,Atlas);
            this->PushVertex(C.X,C.Y,UV,Colour,Atlas);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        void SimpleRenderer::SetPrimaryAtlas(const String& Atlas)
            { this->PriAtlas = Atlas; }

        String SimpleRenderer::GetPrimaryAtlas() const
            { return this->PriAtlas; }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void SimpleRenderer::ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild( this->GetDerivedSerializableName() );

            this->ProtoSerializeProperties(SelfRoot);
        }

        void SimpleRenderer::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            XML::Node PropertiesNode = SelfRoot.AppendChild( SimpleRenderer::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("PriAtlas").SetValue(this->PriAtlas) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",SimpleRenderer::GetSerializableName() + "Properties",true);
            }
        }

        void SimpleRenderer::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            this->ProtoDeSerializeProperties(SelfRoot);
        }

        void SimpleRenderer::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( SimpleRenderer::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = PropertiesNode.GetAttribute("PriAtlas");
                    if( !CurrAttrib.Empty() )
                        this->PriAtlas = CurrAttrib.AsString();
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (SimpleRenderer::GetSerializableName() + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,SimpleRenderer::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String SimpleRenderer::GetDerivedSerializableName() const
            { return SimpleRenderer::GetSerializableName(); }

        String SimpleRenderer::GetSerializableName()
            { return "SimpleRenderer"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        bool SimpleRenderer::_IsDirty()
        {
            return this->Dirty;
        }

        void SimpleRenderer::_Redraw(bool Force)
        {
            if( this->Dirty || Force ) {
                this->RenderVertices.clear();
                this->RedrawImpl(Force);
                this->Dirty = false;
            }
        }

        void SimpleRenderer::_AppendVertices(std::vector<VertexData>& Vertices)
        {
            Vertices.insert(Vertices.end(),this->RenderVertices.begin(),this->RenderVertices.end());
        }
    }//UI
}//Mezzanine

#endif
