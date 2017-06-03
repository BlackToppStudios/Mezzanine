// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _graphicsproceduralroundedboxgenerator_cpp
#define _graphicsproceduralroundedboxgenerator_cpp

#include "Graphics/Procedural/Mesh/roundedboxgenerator.h"
#include "Graphics/Procedural/Mesh/planegenerator.h"

#include "MathTools/mathtools.h"
#include "exception.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            RoundedBoxGenerator::RoundedBoxGenerator(const Vector3& Size, const Real ChamferSize, const Whole SegX, const Whole SegY, const Whole SegZ, const Whole ChamferSeg)
            {
                this->SetSize(Size);
                this->SetChamferSize(ChamferSize);
                this->SetNumSegX(SegX);
                this->SetNumSegY(SegY);
                this->SetNumSegZ(SegZ);
                this->SetNumChamferSeg(ChamferSeg);
            }

            RoundedBoxGenerator::RoundedBoxGenerator(const Real SizeX, const Real SizeY, const Real SizeZ, const Real ChamferSize, const Whole SegX, const Whole SegY, const Whole SegZ, const Whole ChamferSeg)
            {
                this->SetSizeX(SizeX);
                this->SetSizeY(SizeY);
                this->SetSizeZ(SizeZ);
                this->SetChamferSize(ChamferSize);
                this->SetNumSegX(SegX);
                this->SetNumSegY(SegY);
                this->SetNumSegZ(SegZ);
                this->SetNumChamferSeg(ChamferSeg);
            }

            RoundedBoxGenerator::~RoundedBoxGenerator()
                {  }

            void RoundedBoxGenerator::_AddEdge(TriangleBuffer& Buffer, Int16 XPos, Int16 YPos, Int16 ZPos) const
            {
                Integer Offset = 0;

                Vector3 centerPosition = ( Vector3::Unit_X() * ( 0.5 * XPos * this->BoxSize.X ) ) + ( Vector3::Unit_Y() * ( 0.5 * YPos * this->BoxSize.Y ) ) + ( Vector3::Unit_Z() * ( 0.5 * ZPos * this->BoxSize.Z ) );
                Vector3 vy0 =  Vector3::Unit_X() * ( 1.f - MathTools::Abs(XPos) ) + Vector3::Unit_Y() * ( 1.f - MathTools::Abs(YPos) ) + Vector3::Unit_Z() * ( 1.f - MathTools::Abs(ZPos) );//extrusion direction

                Vector3 vx0 = vy0.GetAntiPermute();
                Vector3 vz0 = vy0.GetPermute();
                if( vx0.DotProduct(centerPosition) < 0.0 )
                    vx0 = -vx0;
                if( vz0.DotProduct(centerPosition) < 0.0 )
                    vz0 = -vz0;
                if( vx0.CrossProduct(vy0).DotProduct(vz0) < 0.0 )
                    vy0 = -vy0;

                Real height = ( 1 - MathTools::Abs(XPos) ) * this->BoxSize.X + ( 1 - MathTools::Abs(YPos) ) * this->BoxSize.Y + ( 1 - MathTools::Abs(ZPos) ) * this->BoxSize.Z;
                Vector3 OffsetPosition = centerPosition - ( vy0 * ( 0.5 * height ) );
                Integer numSegHeight = 1;

                if( XPos == 0 )
                    numSegHeight = this->NumSegX;
                else if( YPos == 0 )
                    numSegHeight = this->NumSegY;
                else if( ZPos == 0 )
                    numSegHeight = this->NumSegZ;

                Real deltaAngle = ( MathTools::GetHalfPi() / this->NumChamferSeg );
                Real deltaHeight = height / (Real)numSegHeight;


                Buffer.RebaseOffset();
                Buffer.EstimateIndexCount( 6 * numSegHeight * this->NumChamferSeg );
                Buffer.EstimateVertexCount( ( numSegHeight + 1 ) * ( this->NumChamferSeg + 1 ) );

                for( Integer i = 0 ; i <= numSegHeight ; i++ )
                {
                    for( Whole j = 0 ; j <= this->NumChamferSeg ; j++ )
                    {
                        Real x0 = this->BoxChamferSize * cosf( j * deltaAngle );
                        Real z0 = this->BoxChamferSize * sinf( j * deltaAngle );
                        this->AddPoint( Buffer, Vector3( ( vx0 * x0 ) + ( vy0 * ( i * deltaHeight ) ) + ( vz0 * z0 ) + OffsetPosition ),
                                        ( vx0 * x0 + vz0 * z0 ).GetNormal(),
                                        Vector2( j / (Real)this->NumChamferSeg, i / (Real)numSegHeight ) );

                        if( i != numSegHeight && j != this->NumChamferSeg ) {
                            Buffer.AddIndex( Offset + this->NumChamferSeg + 2 );
                            Buffer.AddIndex( Offset );
                            Buffer.AddIndex( Offset + this->NumChamferSeg + 1 );
                            Buffer.AddIndex( Offset + this->NumChamferSeg + 2 );
                            Buffer.AddIndex( Offset + 1 );
                            Buffer.AddIndex( Offset );
                        }
                        Offset ++;
                    }
                }
            }

            void RoundedBoxGenerator::_AddCorner(TriangleBuffer& Buffer, Boole IsXPositive, Boole IsYPositive, Boole IsZPositive) const
            {
                Buffer.RebaseOffset();
                Buffer.EstimateVertexCount( ( this->NumChamferSeg + 1) * ( this->NumChamferSeg + 1 ) );
                Buffer.EstimateIndexCount( this->NumChamferSeg * this->NumChamferSeg * 6 );
                Integer Offset = 0;

                Vector3 OffsetPosition( ( IsXPositive ? 1 : -1 ) * .5f * this->BoxSize.X, ( IsYPositive ? 1 : -1 ) * .5f * this->BoxSize.Y, ( IsZPositive ? 1 : -1 ) * .5f * this->BoxSize.Z );
                Real deltaRingAngle = ( MathTools::GetHalfPi() / this->NumChamferSeg );
                Real deltaSegAngle = ( MathTools::GetHalfPi() / this->NumChamferSeg );
                Real OffsetRingAngle = IsYPositive ? 0 : MathTools::GetHalfPi();
                Real OffsetSegAngle;
                if ( IsXPositive && IsZPositive )
                    OffsetSegAngle = 0;
                if ( ( !IsXPositive ) && IsZPositive )
                    OffsetSegAngle = 1.5f * MathTools::GetPi();
                if ( IsXPositive && ( !IsZPositive ) )
                    OffsetSegAngle = MathTools::GetHalfPi();
                if ( ( !IsXPositive ) && ( !IsZPositive ) )
                    OffsetSegAngle = MathTools::GetPi();

                // Generate the group of rings for the sphere
                for( Whole ring = 0 ; ring <= this->NumChamferSeg ; ring++ )
                {
                    Real r0 = this->BoxChamferSize * sinf( ring * deltaRingAngle + OffsetRingAngle );
                    Real y0 = this->BoxChamferSize * cosf( ring * deltaRingAngle + OffsetRingAngle );

                    // Generate the group of segments for the current ring
                    for( Whole seg = 0 ; seg <= this->NumChamferSeg ; seg++ )
                    {
                        Real x0 = r0 * sinf( seg * deltaSegAngle + OffsetSegAngle );
                        Real z0 = r0 * cosf( seg * deltaSegAngle + OffsetSegAngle );

                        // Add one vertex to the strip which makes up the sphere
                        this->AddPoint( Buffer, Vector3( x0 + OffsetPosition.X, y0 + OffsetPosition.Y, z0 + OffsetPosition.Z ),
                                        Vector3( x0, y0, z0 ).GetNormal(),
                                        Vector2( (Real)seg / (Real)this->NumChamferSeg, (Real) ring / (Real)this->NumChamferSeg ) );

                        if( ( ring != this->NumChamferSeg ) && ( seg != this->NumChamferSeg ) ) {
                            // each vertex (except the last) has six indices pointing to it
                            Buffer.AddIndex( Offset + this->NumChamferSeg + 2 );
                            Buffer.AddIndex( Offset );
                            Buffer.AddIndex( Offset + this->NumChamferSeg + 1 );
                            Buffer.AddIndex( Offset + this->NumChamferSeg + 2 );
                            Buffer.AddIndex( Offset + 1 );
                            Buffer.AddIndex( Offset );
                        }

                        Offset++;
                    }//for each segment
                }//for each ring
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void RoundedBoxGenerator::AddToTriangleBuffer(TriangleBuffer& Buffer) const
            {
                // Generate the pseudo-box shape
                PlaneGenerator PG;
                PG.SetUTile(this->UTile).SetVTile(this->VTile);
                if( this->GeneratorOpts | GO_Transform ) {
                    PG.SetScale(this->Scale);
                    PG.SetOrientation(this->Orientation);
                }

                PG.SetNumSegX( this->NumSegY ).SetNumSegY( this->NumSegX ).SetSizeX( this->BoxSize.Y ).SetSizeY( this->BoxSize.X )
                .SetNormal( Vector3::Neg_Unit_Z() )
                .SetPosition( ( this->Orientation * Vector3::Neg_Unit_Z() ) * ( 0.5 * this->BoxSize.Z + this->BoxChamferSize ) )
                .AddToTriangleBuffer( Buffer );
                Buffer.RebaseOffset();
                PG.SetNumSegX( this->NumSegY ).SetNumSegY( this->NumSegX ).SetSizeX( this->BoxSize.Y ).SetSizeY( this->BoxSize.X )
                .SetNormal( Vector3::Unit_Z() )
                .SetPosition( ( this->Orientation * Vector3::Unit_Z() ) * ( 0.5 * this->BoxSize.Z + this->BoxChamferSize ) )
                .AddToTriangleBuffer( Buffer );
                Buffer.RebaseOffset();
                PG.SetNumSegX( this->NumSegZ ).SetNumSegY( this->NumSegX ).SetSizeX( this->BoxSize.Z ).SetSizeY( this->BoxSize.X )
                .SetNormal( Vector3::Neg_Unit_Y() )
                .SetPosition( ( this->Orientation * Vector3::Neg_Unit_Y() ) * ( 0.5 * this->BoxSize.Y + this->BoxChamferSize ) )
                .AddToTriangleBuffer( Buffer );
                Buffer.RebaseOffset();
                PG.SetNumSegX( this->NumSegZ ).SetNumSegY( this->NumSegX ).SetSizeX( this->BoxSize.Z ).SetSizeY( this->BoxSize.X )
                .SetNormal( Vector3::Unit_Y() )
                .SetPosition( ( this->Orientation * Vector3::Unit_Y() ) * ( 0.5 * this->BoxSize.Y + this->BoxChamferSize ) )
                .AddToTriangleBuffer( Buffer );
                Buffer.RebaseOffset();
                PG.SetNumSegX( this->NumSegZ ).SetNumSegY( this->NumSegY ).SetSizeX( this->BoxSize.Z ).SetSizeY( this->BoxSize.Y )
                .SetNormal( Vector3::Neg_Unit_X() )
                .SetPosition( ( this->Orientation * Vector3::Neg_Unit_X() ) * ( 0.5 * this->BoxSize.X + this->BoxChamferSize ) )
                .AddToTriangleBuffer( Buffer );
                Buffer.RebaseOffset();
                PG.SetNumSegX( this->NumSegZ ).SetNumSegY( this->NumSegY ).SetSizeX( this->BoxSize.Z ).SetSizeY( this->BoxSize.Y )
                .SetNormal( Vector3::Unit_X() )
                .SetPosition( ( this->Orientation * Vector3::Unit_X() ) * ( 0.5 * this->BoxSize.X + this->BoxChamferSize ) )
                .AddToTriangleBuffer( Buffer );

                // Generate the corners
                this->_AddCorner( Buffer, true,  true,  true );
                this->_AddCorner( Buffer, true,  true,  false );
                this->_AddCorner( Buffer, true,  false, true );
                this->_AddCorner( Buffer, true,  false, false );
                this->_AddCorner( Buffer, false, true,  true );
                this->_AddCorner( Buffer, false, true,  false );
                this->_AddCorner( Buffer, false, false, true );
                this->_AddCorner( Buffer, false, false, false );

                // Generate the edges
                this->_AddEdge( Buffer, -1,-1, 0 );
                this->_AddEdge( Buffer, -1, 1, 0 );
                this->_AddEdge( Buffer,  1,-1, 0 );
                this->_AddEdge( Buffer,  1, 1, 0 );
                this->_AddEdge( Buffer, -1, 0,-1 );
                this->_AddEdge( Buffer, -1, 0, 1 );
                this->_AddEdge( Buffer,  1, 0,-1 );
                this->_AddEdge( Buffer,  1, 0, 1 );
                this->_AddEdge( Buffer,  0,-1,-1 );
                this->_AddEdge( Buffer,  0,-1, 1 );
                this->_AddEdge( Buffer,  0, 1,-1 );
                this->_AddEdge( Buffer,  0, 1, 1 );
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            RoundedBoxGenerator& RoundedBoxGenerator::SetSizeX(const Real SizeX)
            {
                if( SizeX <= 0.0 )
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Size on an axis for a generated box mesh must be greater than zero.");

                this->BoxSize.X = SizeX;
                return *this;
            }

            RoundedBoxGenerator& RoundedBoxGenerator::SetSizeY(const Real SizeY)
            {
                if( SizeY <= 0.0 )
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Size on an axis for a generated box mesh must be greater than zero.");

                this->BoxSize.Y = SizeY;
                return *this;
            }

            RoundedBoxGenerator& RoundedBoxGenerator::SetSizeZ(const Real SizeZ)
            {
                if( SizeZ <= 0.0 )
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Size on an axis for a generated rounded box mesh must be greater than zero.");

                this->BoxSize.Z = SizeZ;
                return *this;
            }

            RoundedBoxGenerator& RoundedBoxGenerator::SetSize(const Vector3& Size)
            {
                this->SetSizeX(Size.X);
                this->SetSizeY(Size.Y);
                this->SetSizeZ(Size.Z);
                return *this;
            }

            RoundedBoxGenerator& RoundedBoxGenerator::SetChamferSize(const Real ChamferSize)
            {
                if( ChamferSize <= 0.0 )
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Size for radius of rounded edge on a generated rounded box mesh must be greater than zero.");

                this->BoxChamferSize = ChamferSize;
                return *this;
            }

            RoundedBoxGenerator& RoundedBoxGenerator::SetNumSegX(const Whole SegX)
            {
                if( SegX == 0 )
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Number of segments for generated rounded box mesh must be greater than zero.");

                this->NumSegX = SegX;
                return *this;
            }

            RoundedBoxGenerator& RoundedBoxGenerator::SetNumSegY(const Whole SegY)
            {
                if( SegY == 0 )
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Number of segments for generated rounded box mesh must be greater than zero.");

                this->NumSegY = SegY;
                return *this;
            }

            RoundedBoxGenerator& RoundedBoxGenerator::SetNumSegZ(const Whole SegZ)
            {
                if( SegZ == 0 )
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Number of segments for generated rounded box mesh must be greater than zero.");

                this->NumSegZ = SegZ;
                return *this;
            }

            RoundedBoxGenerator& RoundedBoxGenerator::SetNumChamferSeg(const Whole ChamferSeg)
            {
                if( ChamferSeg == 0 )
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Number of segments for generated rounded box mesh must be greater than zero.");

                this->NumChamferSeg = ChamferSeg;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
