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
#ifndef _graphicsproceduralboxgenerator_cpp
#define _graphicsproceduralboxgenerator_cpp

#include "Graphics/Procedural/boxgenerator.h"
#include "Graphics/Procedural/planegenerator.h"

#include "exception.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            const String BoxGenerator::TAG_NEGX = "box.negx";
            const String BoxGenerator::TAG_NEGY = "box.negy";
            const String BoxGenerator::TAG_NEGZ = "box.negz";
            const String BoxGenerator::TAG_X = "box.x";
            const String BoxGenerator::TAG_Y = "box.y";
            const String BoxGenerator::TAG_Z = "box.z";

            BoxGenerator::BoxGenerator(const Vector3& HalfExtents, const Whole SegX, const Whole SegY, const Whole SegZ)
            {
                this->SetHalfExtents(HalfExtents);
                this->SetNumSegX(SegX);
                this->SetNumSegY(SegY);
                this->SetNumSegZ(SegZ);
            }

            BoxGenerator::BoxGenerator(const Real HalfSizeX, const Real HalfSizeY, const Real HalfSizeZ, const Whole SegX, const Whole SegY, const Whole SegZ)
            {
                this->SetHalfSizeX(HalfSizeX);
                this->SetHalfSizeY(HalfSizeY);
                this->SetHalfSizeZ(HalfSizeZ);
                this->SetNumSegX(SegX);
                this->SetNumSegY(SegY);
                this->SetNumSegZ(SegZ);
            }

            BoxGenerator::~BoxGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void BoxGenerator::AddToTriangleBuffer(TriangleBuffer& Buffer) const
            {
                PlaneGenerator PG;
                PG.SetUTile(this->UTile).SetVTile(this->VTile);
                if( this->GeneratorOpts | GO_Transform ) {
                    PG.SetScale(this->Scale);
                    PG.SetOrientation(this->Orientation);
                }

                BufferSection Sect = Buffer.BeginSection(BoxGenerator::TAG_NEGZ);
                PG.SetNumSegX(this->NumSegY).SetNumSegY(this->NumSegX).SetSizeX(this->BoxHalf.Y).SetSizeY(this->BoxHalf.X)
                .SetNormal( Vector3::Neg_Unit_Z() )
                .SetPosition( this->Scale * ( this->Position + ( this->Orientation * Vector3::Neg_Unit_Z() ) * ( this->BoxHalf.Z * 0.5 ) ) )
                .AddToTriangleBuffer(Buffer);
                Buffer.EndSection(Sect);

                Sect = Buffer.BeginSection(BoxGenerator::TAG_Z);
                PG.SetNumSegX(this->NumSegY).SetNumSegY(this->NumSegX).SetSizeX(this->BoxHalf.Y).SetSizeY(this->BoxHalf.X)
                .SetNormal( Vector3::Unit_Z() )
                .SetPosition( this->Scale * ( this->Position + ( this->Orientation * Vector3::Unit_Z() ) * ( this->BoxHalf.Z * 0.5 ) ) )
                .AddToTriangleBuffer(Buffer);
                Buffer.EndSection(Sect);

                Sect = Buffer.BeginSection(BoxGenerator::TAG_NEGY);
                PG.SetNumSegX(this->NumSegZ).SetNumSegY(this->NumSegX).SetSizeX(this->BoxHalf.Z).SetSizeY(this->BoxHalf.X)
                .SetNormal( Vector3::Neg_Unit_Y() )
                .SetPosition( this->Scale * ( this->Position + ( this->Orientation * Vector3::Neg_Unit_Y() ) * ( this->BoxHalf.Y * 0.5 ) ) )
                .AddToTriangleBuffer(Buffer);
                Buffer.EndSection(Sect);

                Sect = Buffer.BeginSection(BoxGenerator::TAG_Y);
                PG.SetNumSegX(this->NumSegZ).SetNumSegY(this->NumSegX).SetSizeX(this->BoxHalf.Z).SetSizeY(this->BoxHalf.X)
                .SetNormal( Vector3::Unit_Y() )
                .SetPosition( this->Scale * ( this->Position + ( this->Orientation * Vector3::Unit_Y() ) * ( this->BoxHalf.Y * 0.5 ) ) )
                .AddToTriangleBuffer(Buffer);
                Buffer.EndSection(Sect);

                Sect = Buffer.BeginSection(BoxGenerator::TAG_NEGX);
                PG.SetNumSegX(this->NumSegZ).SetNumSegY(this->NumSegY).SetSizeX(this->BoxHalf.Z).SetSizeY(this->BoxHalf.Y)
                .SetNormal( Vector3::Neg_Unit_X() )
                .SetPosition( this->Scale * ( this->Position + ( this->Orientation * Vector3::Neg_Unit_X() ) * ( this->BoxHalf.X * 0.5 ) ) )
                .AddToTriangleBuffer(Buffer);
                Buffer.EndSection(Sect);

                Sect = Buffer.BeginSection(BoxGenerator::TAG_X);
                PG.SetNumSegX(this->NumSegZ).SetNumSegY(this->NumSegY).SetSizeX(this->BoxHalf.Z).SetSizeY(this->BoxHalf.Y)
                .SetNormal( Vector3::Unit_X() )
                .SetPosition( this->Scale * ( this->Position + ( this->Orientation * Vector3::Unit_X() ) * ( this->BoxHalf.X * 0.5 ) ) )
                .AddToTriangleBuffer(Buffer);
                Buffer.EndSection(Sect);
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            BoxGenerator& BoxGenerator::SetHalfSizeX(const Real HalfSizeX)
            {
                if( HalfSizeX <= 0.0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Size on an axis for a generated box mesh must be greater than zero.");

                this->BoxHalf.X = HalfSizeX;
                return *this;
            }

            BoxGenerator& BoxGenerator::SetHalfSizeY(const Real HalfSizeY)
            {
                if( HalfSizeY <= 0.0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Size on an axis for a generated box mesh must be greater than zero.");

                this->BoxHalf.Y = HalfSizeY;
                return *this;
            }

            BoxGenerator& BoxGenerator::SetHalfSizeZ(const Real HalfSizeZ)
            {
                if( HalfSizeZ <= 0.0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Size on an axis for a generated box mesh must be greater than zero.");

                this->BoxHalf.Z = HalfSizeZ;
                return *this;
            }

            BoxGenerator& BoxGenerator::SetHalfExtents(const Vector3& HalfExtents)
            {
                this->SetHalfSizeX(HalfExtents.X);
                this->SetHalfSizeY(HalfExtents.Y);
                this->SetHalfSizeZ(HalfExtents.Z);
                return *this;
            }

            BoxGenerator& BoxGenerator::SetNumSegX(const Whole SegX)
            {
                if( SegX == 0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Number of segments for generated box mesh must be greater than zero.");

                this->NumSegX = SegX;
                return *this;
            }

            BoxGenerator& BoxGenerator::SetNumSegY(const Whole SegY)
            {
                if( SegY == 0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Number of segments for generated box mesh must be greater than zero.");

                this->NumSegY = SegY;
                return *this;
            }

            BoxGenerator& BoxGenerator::SetNumSegZ(const Whole SegZ)
            {
                if( SegZ == 0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Number of segments for generated box mesh must be greater than zero.");

                this->NumSegZ = SegZ;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
