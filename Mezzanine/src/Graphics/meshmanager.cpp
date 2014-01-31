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
#ifndef _graphicsmeshmanager_cpp
#define _graphicsmeshmanager_cpp

#include "vector2.h"
#include "Graphics/meshmanager.h"
#include "Graphics/mesh.h"

#include <Ogre.h>

namespace Mezzanine
{
    template<> Graphics::MeshManager* Singleton<Graphics::MeshManager>::SingletonPtr = NULL;

    namespace Graphics
    {
        MeshManager::MeshManager()
        {
        }

        MeshManager::MeshManager(XML::Node& XMLNode)
        {
            /// @todo This class currently doesn't initialize anything from XML, if that changes this constructor needs to be expanded.
        }

        MeshManager::~MeshManager()
        {
            this->Deinitialize();
            this->DestroyAllGeneratedMeshes();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Generic Mesh Utility

        Mesh* MeshManager::GetMesh(const String& MeshName)
        {
            MeshIterator lit = this->LoadedMeshes.find(MeshName);
            if(lit != this->LoadedMeshes.end())
                return (*lit).second;
            MeshIterator git = this->GeneratedMeshes.find(MeshName);
            if(git != this->GeneratedMeshes.end())
                return (*git).second;
            return NULL;
        }

        void MeshManager::DestroyAllMeshes()
        {
            this->DestroyAllGeneratedMeshes();
            this->UnloadAllLoadedMeshes();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Non-Generated Mesh Management

        Mesh* MeshManager::LoadMesh(const String& MeshName, const String& Group)
        {
            MeshIterator it = this->LoadedMeshes.find(MeshName);
            if( it != this->LoadedMeshes.end() )
                return (*it).second;
            Mesh* Loaded = new Mesh(Ogre::MeshManager::getSingleton().load(MeshName,Group));
            this->LoadedMeshes[MeshName] = Loaded;
            return Loaded;
        }

        void MeshManager::UnloadMesh(const String& MeshName)
        {
            MeshIterator it = this->LoadedMeshes.find(MeshName);
            if( it == this->LoadedMeshes.end() )
                return;
            Ogre::MeshManager::getSingleton().unload(MeshName);
            delete (*it).second;
            this->LoadedMeshes.erase(it);
        }

        Whole MeshManager::GetNumLoadedMeshes()
        {
            return this->LoadedMeshes.size();
        }

        void MeshManager::UnloadAllLoadedMeshes()
        {
            for( MeshIterator it = this->LoadedMeshes.begin() ; it != this->LoadedMeshes.end() ; it++ )
            {
                delete (*it).second;
            }
            this->LoadedMeshes.clear();
            Ogre::MeshManager::getSingleton().unloadAll();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Generated Mesh Management

        Whole MeshManager::GetNumGeneratedMeshes()
        {
            return this->GeneratedMeshes.size();
        }

        void MeshManager::DestroyGeneratedMesh(const String& MeshName)
        {
            MeshIterator it = this->GeneratedMeshes.find(MeshName);
            if(it == this->GeneratedMeshes.end())
                return;
            Ogre::MeshManager::getSingletonPtr()->remove((*it).second->GetName());
            delete (*it).second;
            this->GeneratedMeshes.erase(it);
            return;
        }

        void MeshManager::DestroyAllGeneratedMeshes()
        {
            for( MeshIterator it = this->GeneratedMeshes.begin() ; it != this->GeneratedMeshes.end() ; it++ )
            {
                Ogre::MeshManager::getSingletonPtr()->remove((*it).second->GetName());
                delete (*it).second;
            }
            this->GeneratedMeshes.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Mesh Generation

        Mesh* MeshManager::CreateBoxMesh(const String& MeshName, const String& MaterialName, const Vector3& HalfExtents)
        {
            Vector3 Half = HalfExtents;
            Ogre::MaterialPtr TheMaterial = Ogre::MaterialManager::getSingleton().getByName(MaterialName);
            String GroupName = TheMaterial->getGroup();

            Ogre::ManualObject* box = new Ogre::ManualObject("TempMan");
            box->begin(MaterialName);
            //Front
            box->position(-Half.X,-Half.Y,Half.Z);  box->normal(0,0,1);  box->textureCoord(0,1);
            box->position(Half.X,-Half.Y,Half.Z);   box->normal(0,0,1);  box->textureCoord(1,1);
            box->position(Half.X,Half.Y,Half.Z);    box->normal(0,0,1);  box->textureCoord(1,0);
            box->position(-Half.X,Half.Y,Half.Z);   box->normal(0,0,1);  box->textureCoord(0,0);
            //Back
            box->position(Half.X,-Half.Y,-Half.Z);  box->normal(0,0,-1); box->textureCoord(0,1);
            box->position(-Half.X,-Half.Y,-Half.Z); box->normal(0,0,-1); box->textureCoord(1,1);
            box->position(-Half.X,Half.Y,-Half.Z);  box->normal(0,0,-1); box->textureCoord(1,0);
            box->position(Half.X,Half.Y,-Half.Z);   box->normal(0,0,-1); box->textureCoord(0,0);
            //Left
            box->position(-Half.X,-Half.Y,-Half.Z); box->normal(-1,0,0); box->textureCoord(0,1);
            box->position(-Half.X,-Half.Y,Half.Z);  box->normal(-1,0,0); box->textureCoord(1,1);
            box->position(-Half.X,Half.Y,Half.Z);   box->normal(-1,0,0); box->textureCoord(1,0);
            box->position(-Half.X,Half.Y,-Half.Z);  box->normal(-1,0,0); box->textureCoord(0,0);
            //Right
            box->position(Half.X,-Half.Y,Half.Z);   box->normal(1,0,0);  box->textureCoord(0,1);
            box->position(Half.X,-Half.Y,-Half.Z);  box->normal(1,0,0);  box->textureCoord(1,1);
            box->position(Half.X,Half.Y,-Half.Z);   box->normal(1,0,0);  box->textureCoord(1,0);
            box->position(Half.X,Half.Y,Half.Z);    box->normal(1,0,0);  box->textureCoord(0,0);
            //Top
            box->position(-Half.X,Half.Y,Half.Z);   box->normal(0,1,0);  box->textureCoord(0,1);
            box->position(Half.X,Half.Y,Half.Z);    box->normal(0,1,0);  box->textureCoord(1,1);
            box->position(Half.X,Half.Y,-Half.Z);   box->normal(0,1,0);  box->textureCoord(1,0);
            box->position(-Half.X,Half.Y,-Half.Z);  box->normal(0,1,0);  box->textureCoord(0,0);
            //Bottom
            box->position(-Half.X,-Half.Y,-Half.Z); box->normal(0,-1,0); box->textureCoord(0,1);
            box->position(Half.X,-Half.Y,-Half.Z);  box->normal(0,-1,0); box->textureCoord(1,1);
            box->position(Half.X,-Half.Y,Half.Z);   box->normal(0,-1,0); box->textureCoord(1,0);
            box->position(-Half.X,-Half.Y,Half.Z);  box->normal(0,-1,0); box->textureCoord(0,0);

            box->triangle(0,1,2);    box->triangle(0,2,3);    //Front
            box->triangle(4,5,6);    box->triangle(4,6,7);    //Back
            box->triangle(8,9,10);   box->triangle(8,10,11);  //Left
            box->triangle(12,13,14); box->triangle(12,14,15); //Right
            box->triangle(16,17,18); box->triangle(16,18,19); //Top
            box->triangle(20,21,22); box->triangle(20,22,23); //Bottom

            box->end();
            Ogre::MeshPtr boxmesh = box->convertToMesh(MeshName, GroupName);
            boxmesh->_setBounds(Ogre::AxisAlignedBox(-Half.X,-Half.Y,-Half.Z,Half.X,Half.Y,Half.Z));
            Real FirstCheck = Half.X > Half.Y ? Half.X : Half.Y;
            Real Largest = FirstCheck > Half.Z ? FirstCheck : Half.Z;
            boxmesh->_setBoundingSphereRadius(1.4 * Largest);
            Mesh* NewMesh = new Mesh(boxmesh);
            this->GeneratedMeshes[MeshName] = NewMesh;
            delete box;
            return NewMesh;
        }

        Mesh* MeshManager::CreateBoxMesh(const String& MeshName, const ColourValue& Colour, const Vector3& HalfExtents)
        {
            String MatName(MeshName+"Mat");
            this->CreateColouredMaterial(MatName,Colour);
            return this->CreateBoxMesh(MeshName,MatName,HalfExtents);
        }

        Mesh* MeshManager::CreateCylinderMesh(const String& MeshName, const String& MaterialName, const Vector3& HalfExtents, const Vector3& AxisOrientation, const Whole& CircleRes, const Whole& Segments)
        {
    /// Start of MIT(Ogre Proceadural) License ///
            Ogre::Quaternion RotationModifier;
            if(AxisOrientation == Vector3::Unit_X())
                RotationModifier = Ogre::Quaternion(Ogre::Radian(0.5),Ogre::Vector3::UNIT_Z);
            else if( AxisOrientation == Vector3::Unit_Y() )
                RotationModifier = Ogre::Quaternion(1,0,0,0);
            else if( AxisOrientation == Vector3::Unit_Z() )
                RotationModifier = Ogre::Quaternion(Ogre::Radian(0.5),Ogre::Vector3::UNIT_X);

            Vector3 Half = HalfExtents;
            Ogre::MaterialPtr TheMaterial = Ogre::MaterialManager::getSingleton().getByName(MaterialName);
            String GroupName = TheMaterial->getGroup();

            Real deltaAngle = (Ogre::Math::TWO_PI / CircleRes);
            Real deltaHeight = (Half.Y * 2)/(Real)Segments;
            Real radius = Half.X;
            Whole offset = 0;

            Ogre::ManualObject* cylinder = new Ogre::ManualObject("TempMan");
            cylinder->begin(MaterialName);

            for( Whole i = 0 ; i <= Segments ; i++ )
            {
                for( Whole j = 0 ; j < CircleRes ; j++ )
                {
                    Real x0 = radius * cosf(j*deltaAngle);
                    Real z0 = radius * sinf(j*deltaAngle);
                    cylinder->position( RotationModifier * Ogre::Vector3(x0, (i*deltaHeight) - Half.Y, z0) );
                    cylinder->normal( RotationModifier * Ogre::Vector3(x0,0,z0).normalisedCopy() );
                    cylinder->textureCoord(j/(Real)CircleRes, i/(Real)Segments);

                    if(i != Segments)
                    {
                        /// @todo These if/elses aren't really a long term solution
                        /// as they won't work for any more then 1 segment.  This needs to be addressed.
                        if(offset + CircleRes == CircleRes * 2)
                            cylinder->index(CircleRes);
                        else
                            cylinder->index(offset + CircleRes + 1);

                        cylinder->index(offset);
                        cylinder->index(offset + CircleRes);

                        if(offset + CircleRes == CircleRes * 2)
                            cylinder->index(0);
                        else
                            cylinder->index(offset + CircleRes + 1);

                        cylinder->index(offset + 1);
                        cylinder->index(offset);
                    }
                    offset++;
                }
            }

            //low cap
            Whole centerIndex = offset;
            cylinder->position( RotationModifier * Ogre::Vector3(0,-Half.Y,0) );
            cylinder->normal( RotationModifier * Ogre::Vector3::NEGATIVE_UNIT_Y );
            cylinder->textureCoord(0,1);
            offset++;
            for( Whole j = 0 ; j <= CircleRes ; j++ )
            {
                Real x0 = radius * cosf(j*deltaAngle);
                Real z0 = radius * sinf(j*deltaAngle);

                cylinder->position( RotationModifier * Ogre::Vector3(x0,-Half.Y, z0) );
                cylinder->normal( RotationModifier * Ogre::Vector3::NEGATIVE_UNIT_Y );
                cylinder->textureCoord(j/(Real)CircleRes,0.0);
                if( j != CircleRes )
                {
                    cylinder->index(centerIndex);
                    cylinder->index(offset);
                    cylinder->index(offset+1);
                }
                offset++;
            }

            // high cap
            centerIndex = offset;
            cylinder->position( RotationModifier * Ogre::Vector3(0,Half.Y,0) );
            cylinder->normal( RotationModifier * Ogre::Vector3::UNIT_Y );
            cylinder->textureCoord(0,0);
            offset++;
            for(Whole j = 0 ; j <= CircleRes ; j++ )
            {
                Real x0 = radius * cosf(j*deltaAngle);
                Real z0 = radius * sinf(j*deltaAngle);

                cylinder->position( RotationModifier * Ogre::Vector3(x0, Half.Y, z0) );
                cylinder->normal( RotationModifier * Ogre::Vector3::UNIT_Y );
                cylinder->textureCoord(j/(Real)CircleRes,1);
                if( j != CircleRes )
                {
                    cylinder->index(centerIndex);
                    cylinder->index(offset+1);
                    cylinder->index(offset);
                }
                offset++;
            }

            cylinder->end();
            Ogre::MeshPtr cylindermesh = cylinder->convertToMesh(MeshName, GroupName);
            cylindermesh->_setBounds(Ogre::AxisAlignedBox(-Half.X,-Half.Y,-Half.Z,Half.X,Half.Y,Half.Z));
            Real FirstCheck = Half.X > Half.Y ? Half.X : Half.Y;
            Real Largest = FirstCheck > Half.Z ? FirstCheck : Half.Z;
            cylindermesh->_setBoundingSphereRadius(1.4 * Largest);
            Mesh* NewMesh = new Mesh(cylindermesh);
            this->GeneratedMeshes[MeshName] = NewMesh;
            delete cylinder;
            return NewMesh;
    /// End of MIT(Ogre Proceadural) License ///
        }

        Mesh* MeshManager::CreateCylinderMesh(const String& MeshName, const ColourValue& Colour, const Vector3& HalfExtents, const Vector3& AxisOrientation, const Whole& CircleRes, const Whole& Segments)
        {
            String MatName(MeshName+"Mat");
            this->CreateColouredMaterial(MatName,Colour);
            return this->CreateCylinderMesh(MeshName,MatName,HalfExtents,AxisOrientation,CircleRes,Segments);
        }

        Mesh* MeshManager::CreateSphereMesh(const String& MeshName, const String& MaterialName, const Real& Radius, const Real& Rings, const Real& Segments)
        {
            Ogre::MaterialPtr TheMaterial = Ogre::MaterialManager::getSingleton().getByName(MaterialName);
            String GroupName = TheMaterial->getGroup();

            Real RingAngle = (Ogre::Math::PI / Rings);
            Real SegAngle = (2 * Ogre::Math::PI / Segments);
            Whole VertIndex = 0;

            Ogre::ManualObject* sphere = new Ogre::ManualObject("TempMan");
            sphere->begin(MaterialName);

            for( Whole ring = 0 ; ring <= Rings ; ring++ ) {
            Real r0 = Radius * sinf (ring * RingAngle);
            Real y0 = Radius * cosf (ring * RingAngle);

                // Generate the group of segments for the current ring
                for( Whole seg = 0 ; seg <= Segments ; seg++ ) {
                    Real x0 = r0 * sinf(seg * SegAngle);
                    Real z0 = r0 * cosf(seg * SegAngle);

                    // Add one vertex to the strip which makes up the sphere
                    sphere->position( x0, y0, z0);
                    sphere->normal(Ogre::Vector3(x0, y0, z0).normalisedCopy());
                    sphere->textureCoord((Real) seg / (Real) Segments, (Real) ring / (Real) Rings);

                    if (ring != Rings) {
                        // each vertex (except the last) has six indicies pointing to it
                        sphere->index(VertIndex + Segments + 1);
                        sphere->index(VertIndex);
                        sphere->index(VertIndex + Segments);
                        sphere->index(VertIndex + Segments + 1);
                        sphere->index(VertIndex + 1);
                        sphere->index(VertIndex);
                        VertIndex++;
                    }
                }
            }

            sphere->end();
            Ogre::MeshPtr spheremesh = sphere->convertToMesh(MeshName, GroupName);
            spheremesh->_setBounds(Ogre::AxisAlignedBox(-Radius,-Radius,-Radius,Radius,Radius,Radius));
            spheremesh->_setBoundingSphereRadius(Radius);
            Mesh* NewMesh = new Mesh(spheremesh);
            this->GeneratedMeshes[MeshName] = NewMesh;
            delete sphere;
            return NewMesh;
        }

        Mesh* MeshManager::CreateSphereMesh(const String& MeshName, const ColourValue& Colour, const Real& Radius, const Real& Rings, const Real& Segments)
        {
            String MatName(MeshName+"Mat");
            this->CreateColouredMaterial(MatName,Colour);
            return this->CreateSphereMesh(MeshName,MatName,Radius,Rings,Segments);
        }

        namespace{
            /// @internal
            /// @brief used to created simple manual shapes in a quick and dirty way
            struct IdealPoint
            {
                Vector3 Vertex;
                Vector3 Normal;
                Vector2 TextureCoord;

                IdealPoint(Vector3 Vertex_, Vector3 Normal_, Vector2 TextureCoord_)
                {
                    Vertex=Vertex_;
                    Normal=Normal_;
                    TextureCoord=TextureCoord_;
                }
            };
        }

        Mesh* MeshManager::CreateBoxCornerMesh(const String& MeshName, const String& MaterialName, const Vector3& HalfExtents, const Real& BoxThickness)
        {
            Vector3 Half = HalfExtents;
            Vector3 Full = HalfExtents * 2;

            Real Length = 0.0;
            Real FirstSizeCheck = HalfExtents.X < HalfExtents.Y ? HalfExtents.X : HalfExtents.Y;
            Real Smallest = FirstSizeCheck < HalfExtents.Z ? FirstSizeCheck : HalfExtents.Z;
            if(Smallest * 2 <= FirstSizeCheck) Length = Smallest * 0.5;
            else Length = Smallest * 0.25;
            Ogre::MaterialPtr TheMaterial = Ogre::MaterialManager::getSingleton().getByName(MaterialName);
            String GroupName = TheMaterial->getGroup();

            Ogre::ManualObject* boxcorner = new Ogre::ManualObject("TempMan");
            boxcorner->begin(MaterialName);

            // Create a list of data we can play around with
            std::vector<IdealPoint> IdealShape; //Vertex                                                        //Normal            //textureCoord
            // Forward Face // 0 /
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X,Half.Y,Half.Z),                                 Vector3(0,0,1),     Vector2(0,0) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X,Half.Y - Length,Half.Z),                        Vector3(0,0,1),     Vector2(0,0.5) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y - Length,Half.Z),         Vector3(0,0,1),     Vector2(BoxThickness / Full.X,0.5) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z),   Vector3(0,0,1),     Vector2(BoxThickness / Full.X,BoxThickness / Full.Y) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + Length,Half.Y - BoxThickness,Half.Z),         Vector3(0,0,1),     Vector2(0.5,BoxThickness / Full.Y) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + Length,Half.Y,Half.Z),                        Vector3(0,0,1),     Vector2(0.5,0) ));
            // Upward Face // 6 */
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X,Half.Y,Half.Z),                                 Vector3(0,1,0),     Vector2(0,1) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + Length,Half.Y,Half.Z),                        Vector3(0,1,0),     Vector2(0.5,1) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + Length,Half.Y,Half.Z - BoxThickness),         Vector3(0,1,0),     Vector2(0.5,1 - (BoxThickness / Full.Y)) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y,Half.Z - BoxThickness),   Vector3(0,1,0),     Vector2(BoxThickness / Full.X,1 - (BoxThickness / Full.Y)) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y,Half.Z - Length),         Vector3(0,1,0),     Vector2(BoxThickness / Full.X,0.5) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X,Half.Y,Half.Z - Length),                        Vector3(0,1,0),     Vector2(0,0.5) ));
            // Left Face // 12 */
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X,Half.Y,Half.Z),                                 Vector3(-1,0,0),    Vector2(1,0) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X,Half.Y,Half.Z - Length),                        Vector3(-1,0,0),    Vector2(0.5,0) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X,Half.Y - BoxThickness,Half.Z - Length),         Vector3(-1,0,0),    Vector2(0.5,BoxThickness / Full.Y) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X,Half.Y - BoxThickness,Half.Z - BoxThickness),   Vector3(-1,0,0),    Vector2(1 - (BoxThickness / Full.X),BoxThickness / Full.Y) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X,Half.Y - Length,Half.Z - BoxThickness),         Vector3(-1,0,0),    Vector2(1 - (BoxThickness / Full.X),0.5) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X,Half.Y - Length,Half.Z),                        Vector3(-1,0,0),    Vector2(1,0.5) ));
            // Backward Faces // 18 */
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + Length,Half.Y,Half.Z - BoxThickness),                       Vector3(0,0,-1),     Vector2(0.5,0) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + Length,Half.Y - BoxThickness,Half.Z - BoxThickness),        Vector3(0,0,-1),     Vector2(0.5,BoxThickness / Full.Y) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z - BoxThickness),  Vector3(0,0,-1),     Vector2(1 - (BoxThickness / Full.X),BoxThickness / Full.Y) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y,Half.Z - BoxThickness),                 Vector3(0,0,-1),     Vector2(1 - (BoxThickness / Full.X),0) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y,Half.Z - Length),                       Vector3(0,0,-1),     Vector2(1 - (BoxThickness / Full.X),0) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z - Length),        Vector3(0,0,-1),     Vector2(1 - (BoxThickness / Full.X),BoxThickness / Full.Y) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X,Half.Y - BoxThickness,Half.Z - Length),                       Vector3(0,0,-1),     Vector2(1,BoxThickness / Full.Y) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X,Half.Y,Half.Z - Length),                                      Vector3(0,0,-1),     Vector2(1,0) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z - BoxThickness),  Vector3(0,0,-1),     Vector2(1 - (BoxThickness / Full.X),BoxThickness / Full.Y) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y - Length,Half.Z - BoxThickness),        Vector3(0,0,-1),     Vector2(1 - (BoxThickness / Full.X),0.5) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X,Half.Y - Length,Half.Z - BoxThickness),                       Vector3(0,0,-1),     Vector2(1,0.5) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X,Half.Y - BoxThickness,Half.Z - BoxThickness),                 Vector3(0,0,-1),     Vector2(1,BoxThickness / Full.Y) ));
            // Downward Faces // 30 */
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z),                 Vector3(0,-1,0),     Vector2(BoxThickness / Full.X,0) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z - BoxThickness),  Vector3(0,-1,0),     Vector2(BoxThickness / Full.X,BoxThickness / Full.Y) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + Length,Half.Y - BoxThickness,Half.Z - BoxThickness),        Vector3(0,-1,0),     Vector2(0.5,BoxThickness / Full.Y) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + Length,Half.Y - BoxThickness,Half.Z),                       Vector3(0,-1,0),     Vector2(0.5,0) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X,Half.Y - Length,Half.Z),                                      Vector3(0,-1,0),     Vector2(0,0) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X,Half.Y - Length,Half.Z - BoxThickness),                       Vector3(0,-1,0),     Vector2(0,BoxThickness / Full.Y) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y - Length,Half.Z - BoxThickness),        Vector3(0,-1,0),     Vector2(BoxThickness / Full.X,BoxThickness / Full.Y) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y - Length,Half.Z),                       Vector3(0,-1,0),     Vector2(BoxThickness / Full.X,0) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X,Half.Y - BoxThickness,Half.Z - BoxThickness),                 Vector3(0,-1,0),     Vector2(0,BoxThickness / Full.Y) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X,Half.Y - BoxThickness,Half.Z - Length),                       Vector3(0,-1,0),     Vector2(0,0.5) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z - Length),        Vector3(0,-1,0),     Vector2(BoxThickness / Full.X,0.5) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z - BoxThickness),  Vector3(0,-1,0),     Vector2(BoxThickness / Full.X,BoxThickness / Full.Y) ));
            // Right Faces // 42 */
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y,Half.Z - BoxThickness),                 Vector3(1,0,0),      Vector2(BoxThickness / Full.X,0) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z - BoxThickness),  Vector3(1,0,0),      Vector2(BoxThickness / Full.X,BoxThickness / Full.Y) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z - Length),        Vector3(1,0,0),      Vector2(0.5,BoxThickness / Full.Y) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y,Half.Z - Length),                       Vector3(1,0,0),      Vector2(0.5,0) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + Length,Half.Y,Half.Z),                                      Vector3(1,0,0),      Vector2(0,0) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + Length,Half.Y - BoxThickness,Half.Z),                       Vector3(1,0,0),      Vector2(0,BoxThickness / Full.Y) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + Length,Half.Y - BoxThickness,Half.Z - BoxThickness),        Vector3(1,0,0),      Vector2(BoxThickness / Full.X,BoxThickness / Full.Y) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + Length,Half.Y,Half.Z - BoxThickness),                       Vector3(1,0,0),      Vector2(BoxThickness / Full.X,0) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z),                 Vector3(1,0,0),      Vector2(0,BoxThickness / Full.Y) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y - Length,Half.Z),                       Vector3(1,0,0),      Vector2(0,0.5) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y - Length,Half.Z - BoxThickness),        Vector3(1,0,0),      Vector2(BoxThickness / Full.X,0.5) ));
            IdealShape.push_back( IdealPoint(   Vector3(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z - BoxThickness),  Vector3(1,0,0),      Vector2(BoxThickness / Full.X,BoxThickness / Full.Y) ));

            // Each of the flips we need to make
            std::vector<Vector3> Flips;
            Flips.push_back(Vector3(1,1,1));
            Flips.push_back(Vector3(1,1,-1));
            Flips.push_back(Vector3(1,-1,1));
            Flips.push_back(Vector3(-1,1,1));
            Flips.push_back(Vector3(1,-1,-1));
            Flips.push_back(Vector3(-1,1,-1));
            Flips.push_back(Vector3(-1,-1,1));
            Flips.push_back(Vector3(-1,-1,-1));

            // flip this set of points against the list of flips
            for( std::vector<Vector3>::iterator CurrentFlip=Flips.begin() ; CurrentFlip!=Flips.end() ; ++CurrentFlip )
            {
                for(std::vector<IdealPoint>::iterator CurrentVertex=IdealShape.begin(); CurrentVertex!=IdealShape.end(); ++CurrentVertex)
                {
                    boxcorner->position( (CurrentVertex->Vertex * *CurrentFlip).GetOgreVector3() );
                    boxcorner->normal( (CurrentVertex->Normal * *CurrentFlip).GetOgreVector3() );
                    boxcorner->textureCoord( CurrentVertex->TextureCoord.GetOgreVector2() );
                }
            }

            // Create Index's
            for( Whole FlipCount = 0 ; FlipCount<Flips.size() ; ++FlipCount )
            {
                Whole ShapeIndex = FlipCount*IdealShape.size();

                // Top-Left-Front Forward Face
                if(1==Flips.at(FlipCount).Z)   // draw them in backward order if this shape is flipped on the Z axis
                {
                    //Forward Face Unflipped
                    boxcorner->triangle(0+ShapeIndex,1+ShapeIndex,2+ShapeIndex);        // These things could be replaced by a series of loops, but this was simpler
                    boxcorner->triangle(0+ShapeIndex,2+ShapeIndex,3+ShapeIndex);
                    boxcorner->triangle(0+ShapeIndex,3+ShapeIndex,4+ShapeIndex);
                    boxcorner->triangle(0+ShapeIndex,4+ShapeIndex,5+ShapeIndex);
                    //Backward Faces Unflipped
                    boxcorner->triangle(18+ShapeIndex,19+ShapeIndex,20+ShapeIndex);
                    boxcorner->triangle(18+ShapeIndex,20+ShapeIndex,21+ShapeIndex);
                    boxcorner->triangle(22+ShapeIndex,23+ShapeIndex,24+ShapeIndex);
                    boxcorner->triangle(22+ShapeIndex,24+ShapeIndex,25+ShapeIndex);
                    boxcorner->triangle(26+ShapeIndex,27+ShapeIndex,28+ShapeIndex);
                    boxcorner->triangle(26+ShapeIndex,28+ShapeIndex,29+ShapeIndex);
                }else{
                    //Forward Face Flipped
                    boxcorner->triangle(0+ShapeIndex,1+ShapeIndex,2+ShapeIndex);
                    boxcorner->triangle(0+ShapeIndex,3+ShapeIndex,4+ShapeIndex);
                    boxcorner->triangle(0+ShapeIndex,2+ShapeIndex,3+ShapeIndex);
                    boxcorner->triangle(0+ShapeIndex,4+ShapeIndex,5+ShapeIndex);
                    //Backward Faces Flipped
                    boxcorner->triangle(26+ShapeIndex,28+ShapeIndex,29+ShapeIndex);
                    boxcorner->triangle(26+ShapeIndex,27+ShapeIndex,28+ShapeIndex);
                    boxcorner->triangle(22+ShapeIndex,24+ShapeIndex,25+ShapeIndex);
                    boxcorner->triangle(22+ShapeIndex,23+ShapeIndex,24+ShapeIndex);
                    boxcorner->triangle(18+ShapeIndex,20+ShapeIndex,21+ShapeIndex);
                    boxcorner->triangle(18+ShapeIndex,19+ShapeIndex,20+ShapeIndex);
                }

                if(1==Flips.at(FlipCount).Y)   // draw them in backward order if this shape is flipped on the Y axis
                {
                    // Upward Face Unflipped
                    boxcorner->triangle(6+ShapeIndex,7+ShapeIndex,8+ShapeIndex);
                    boxcorner->triangle(6+ShapeIndex,8+ShapeIndex,9+ShapeIndex);
                    boxcorner->triangle(6+ShapeIndex,9+ShapeIndex,10+ShapeIndex);
                    boxcorner->triangle(6+ShapeIndex,10+ShapeIndex,11+ShapeIndex);
                    // Downward Faces Unflipped
                    boxcorner->triangle(30+ShapeIndex,31+ShapeIndex,32+ShapeIndex);
                    boxcorner->triangle(30+ShapeIndex,32+ShapeIndex,33+ShapeIndex);
                    boxcorner->triangle(34+ShapeIndex,35+ShapeIndex,36+ShapeIndex);
                    boxcorner->triangle(34+ShapeIndex,36+ShapeIndex,37+ShapeIndex);
                    boxcorner->triangle(38+ShapeIndex,39+ShapeIndex,40+ShapeIndex);
                    boxcorner->triangle(38+ShapeIndex,40+ShapeIndex,41+ShapeIndex);
                }else{
                    // Upward Face Flipped
                    boxcorner->triangle(6+ShapeIndex,10+ShapeIndex,11+ShapeIndex);
                    boxcorner->triangle(6+ShapeIndex,9+ShapeIndex,10+ShapeIndex);
                    boxcorner->triangle(6+ShapeIndex,8+ShapeIndex,9+ShapeIndex);
                    boxcorner->triangle(6+ShapeIndex,7+ShapeIndex,8+ShapeIndex);
                    // Downward Faces Flipped
                    boxcorner->triangle(38+ShapeIndex,40+ShapeIndex,41+ShapeIndex);
                    boxcorner->triangle(38+ShapeIndex,39+ShapeIndex,40+ShapeIndex);
                    boxcorner->triangle(34+ShapeIndex,36+ShapeIndex,37+ShapeIndex);
                    boxcorner->triangle(34+ShapeIndex,35+ShapeIndex,36+ShapeIndex);
                    boxcorner->triangle(30+ShapeIndex,32+ShapeIndex,33+ShapeIndex);
                    boxcorner->triangle(30+ShapeIndex,31+ShapeIndex,32+ShapeIndex);
                }

                if(1==Flips.at(FlipCount).X)   // draw them in backward order if this shape is flipped on the X axis
                {
                    // Left Face Unflipped
                    boxcorner->triangle(12+ShapeIndex,13+ShapeIndex,14+ShapeIndex);
                    boxcorner->triangle(12+ShapeIndex,14+ShapeIndex,15+ShapeIndex);
                    boxcorner->triangle(12+ShapeIndex,15+ShapeIndex,16+ShapeIndex);
                    boxcorner->triangle(12+ShapeIndex,16+ShapeIndex,17+ShapeIndex);
                    // Right Faces Unflipped
                    boxcorner->triangle(42+ShapeIndex,43+ShapeIndex,44+ShapeIndex);
                    boxcorner->triangle(42+ShapeIndex,44+ShapeIndex,45+ShapeIndex);
                    boxcorner->triangle(46+ShapeIndex,47+ShapeIndex,48+ShapeIndex);
                    boxcorner->triangle(46+ShapeIndex,48+ShapeIndex,49+ShapeIndex);
                    boxcorner->triangle(50+ShapeIndex,51+ShapeIndex,52+ShapeIndex);
                    boxcorner->triangle(50+ShapeIndex,52+ShapeIndex,53+ShapeIndex);
                }else{
                    // Left Face Unflipped
                    boxcorner->triangle(12+ShapeIndex,16+ShapeIndex,17+ShapeIndex);
                    boxcorner->triangle(12+ShapeIndex,15+ShapeIndex,16+ShapeIndex);
                    boxcorner->triangle(12+ShapeIndex,14+ShapeIndex,15+ShapeIndex);
                    boxcorner->triangle(12+ShapeIndex,13+ShapeIndex,14+ShapeIndex);
                    // Right Faces Unflipped
                    boxcorner->triangle(50+ShapeIndex,52+ShapeIndex,53+ShapeIndex);
                    boxcorner->triangle(50+ShapeIndex,51+ShapeIndex,52+ShapeIndex);
                    boxcorner->triangle(46+ShapeIndex,48+ShapeIndex,49+ShapeIndex);
                    boxcorner->triangle(46+ShapeIndex,47+ShapeIndex,48+ShapeIndex);
                    boxcorner->triangle(42+ShapeIndex,44+ShapeIndex,45+ShapeIndex);
                    boxcorner->triangle(42+ShapeIndex,43+ShapeIndex,44+ShapeIndex);
                }
            }

            boxcorner->end();
            Ogre::MeshPtr boxcornermesh = boxcorner->convertToMesh(MeshName, GroupName);
            boxcornermesh->_setBounds(Ogre::AxisAlignedBox(-Half.X,-Half.Y,-Half.Z,Half.X,Half.Y,Half.Z));
            Real FirstCheck = Half.X > Half.Y ? Half.X : Half.Y;
            Real Largest = FirstCheck > Half.Z ? FirstCheck : Half.Z;
            boxcornermesh->_setBoundingSphereRadius(1.4 * Largest);
            Mesh* NewMesh = new Mesh(boxcornermesh);
            this->GeneratedMeshes[MeshName] = NewMesh;
            delete boxcorner;
            return NewMesh;
        }

        Mesh* MeshManager::CreateBoxCornerMesh(const String& MeshName, const ColourValue& Colour, const Vector3& HalfExtents, const Real& BoxThickness)
        {
            String MatName(MeshName+"Mat");
            this->CreateColouredMaterial(MatName,Colour);
            return this->CreateBoxCornerMesh(MeshName,MatName,HalfExtents,BoxThickness);
        }

        Mesh* MeshManager::CreateMeshFromShape(const String& MeshName, const String& MaterialName, Physics::CollisionShape* Shape)
        {
            return NULL;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Material Utilities - Until we can get a Material Manager

        const String& MeshManager::CreateColouredMaterial(const String& MatName, const ColourValue& Colour, const String& Group)
        {
            Ogre::ResourceManager::ResourceCreateOrRetrieveResult Result = Ogre::MaterialManager::getSingletonPtr()->createOrRetrieve(MatName, Group.empty() ? Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME : Group);
            Ogre::MaterialPtr NewMaterial = static_cast<Ogre::MaterialPtr>(Result.first);
            NewMaterial->setReceiveShadows(false);
            Ogre::Pass* pass = NewMaterial->getTechnique(0)->getPass(0);
            pass->setCullingMode(Ogre::CULL_NONE);
            pass->setDepthCheckEnabled(true);
            pass->setDepthWriteEnabled(false);
            pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
            pass->setAmbient(Colour.GetOgreColourValue());
            pass->setDiffuse(Colour.GetOgreColourValue());
            NewMaterial->prepare();
            NewMaterial->load();
            return MatName;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void MeshManager::Initialize()
            { this->Initialized = true; }

        void MeshManager::Deinitialize()
            { this->Initialized = false; }

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        ManagerBase::ManagerType MeshManager::GetInterfaceType() const
            { return ManagerBase::MT_MeshManager; }

        String MeshManager::GetImplementationTypeName() const
            { return "DefaultMeshManager"; }

        ///////////////////////////////////////////////////////////////////////////////
        // DefaultMeshManagerFactory Methods

        DefaultMeshManagerFactory::DefaultMeshManagerFactory()
        {
        }

        DefaultMeshManagerFactory::~DefaultMeshManagerFactory()
        {
        }

        String DefaultMeshManagerFactory::GetManagerTypeName() const
        {
            return "DefaultMeshManager";
        }

        ManagerBase* DefaultMeshManagerFactory::CreateManager(NameValuePairList& Params)
        {
            if(MeshManager::SingletonValid())
            {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return MeshManager::GetSingletonPtr();
            }else return new MeshManager();
        }

        ManagerBase* DefaultMeshManagerFactory::CreateManager(XML::Node& XMLNode)
        {
            if(MeshManager::SingletonValid())
            {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return MeshManager::GetSingletonPtr();
            }else return new MeshManager(XMLNode);
        }

        void DefaultMeshManagerFactory::DestroyManager(ManagerBase* ToBeDestroyed)
        {
            delete ToBeDestroyed;
        }
    }//Graphics
}//Mezzanine

#endif
