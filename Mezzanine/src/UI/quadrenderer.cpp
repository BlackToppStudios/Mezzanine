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
#ifndef _uiquadrenderer_cpp
#define _uiquadrenderer_cpp

#include "UI/quadrenderer.h"

namespace Mezzanine
{
    namespace UI
    {
        QuadRenderer::QuadRenderer()
        {
        }

        QuadRenderer::~QuadRenderer()
        {
        }

        void QuadRenderer::PushQuad(Vector2* Positions, Vector2* UVs, ColourValue* Colours, const String& Atlas)
        {
            PushVertex(Positions[3].X,Positions[3].Y,UVs[3],Colours[3],Atlas);
            PushVertex(Positions[1].X,Positions[1].Y,UVs[1],Colours[1],Atlas);
            PushVertex(Positions[0].X,Positions[0].Y,UVs[0],Colours[0],Atlas);

            PushVertex(Positions[3].X,Positions[3].Y,UVs[3],Colours[3],Atlas);
            PushVertex(Positions[2].X,Positions[2].Y,UVs[2],Colours[2],Atlas);
            PushVertex(Positions[1].X,Positions[1].Y,UVs[1],Colours[1],Atlas);
        }

        void QuadRenderer::PushQuad2(Vector2* Positions, Vector2* UVs, const ColourValue& Colour, const String& Atlas)
        {
            PushVertex(Positions[3].X,Positions[3].Y,UVs[3],Colour,Atlas);
            PushVertex(Positions[1].X,Positions[1].Y,UVs[1],Colour,Atlas);
            PushVertex(Positions[0].X,Positions[0].Y,UVs[0],Colour,Atlas);

            PushVertex(Positions[3].X,Positions[3].Y,UVs[3],Colour,Atlas);
            PushVertex(Positions[2].X,Positions[2].Y,UVs[2],Colour,Atlas);
            PushVertex(Positions[1].X,Positions[1].Y,UVs[1],Colour,Atlas);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void QuadRenderer::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->SimpleRenderer::ProtoSerializeProperties(SelfRoot);
        }

        void QuadRenderer::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->SimpleRenderer::ProtoDeSerializeProperties(SelfRoot);
        }

        String QuadRenderer::GetDerivedSerializableName() const
        {
            return QuadRenderer::GetSerializableName();
        }

        String QuadRenderer::GetSerializableName()
        {
            return "QuadRenderer";
        }
    }//UI
}//Mezzanine

#endif
