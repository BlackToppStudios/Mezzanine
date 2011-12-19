//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _meshmanager_cpp
#define _meshmanager_cpp

#include "vector2.h"
#include "meshmanager.h"
#include "mesh.h"

#include <Ogre.h>

namespace Mezzanine
{
    template<> MeshManager* Singleton<MeshManager>::SingletonPtr = 0;

    MeshManager::MeshManager()
    {
        this->Priority = 35;
    }

    MeshManager::~MeshManager()
    {
        DestroyAllGeneratedMeshes();
    }

    Mesh* MeshManager::GetMesh(const String& MeshName)
    {
        std::map<String,Mesh*>::iterator lit = LoadedMeshes.find(MeshName);
        if(lit != LoadedMeshes.end())
            return (*lit).second;
        std::map<String,Mesh*>::iterator git = GeneratedMeshes.find(MeshName);
        if(git != GeneratedMeshes.end())
            return (*git).second;
        return NULL;
    }

    void MeshManager::DestroyAllMeshes()
    {
        DestroyAllGeneratedMeshes();
        UnloadAllLoadedMeshes();
    }

    Mesh* MeshManager::LoadMesh(const String& MeshName, const String& Group)
    {
        std::map<String,Mesh*>::iterator it = LoadedMeshes.find(MeshName);
        if( it != LoadedMeshes.end() )
            return (*it).second;
        Mesh* Loaded = new Mesh(Ogre::MeshManager::getSingleton().load(MeshName,Group));
        LoadedMeshes[MeshName] = Loaded;
        return Loaded;
    }

    void MeshManager::UnloadMesh(const String& MeshName)
    {
        std::map<String,Mesh*>::iterator it = LoadedMeshes.find(MeshName);
        if( it == LoadedMeshes.end() )
            return;
        Ogre::MeshManager::getSingleton().unload(MeshName);
        delete (*it).second;
        LoadedMeshes.erase(it);
    }

    Whole MeshManager::GetNumLoadedMeshes()
    {
        return LoadedMeshes.size();
    }

    void MeshManager::UnloadAllLoadedMeshes()
    {
        for( std::map<String,Mesh*>::iterator it = LoadedMeshes.begin() ; it != LoadedMeshes.end() ; it++ )
        {
            delete (*it).second;
        }
        LoadedMeshes.clear();
        Ogre::MeshManager::getSingleton().unloadAll();
    }

    Whole MeshManager::GetNumGeneratedMeshes()
    {
        return GeneratedMeshes.size();
    }

    void MeshManager::DestroyGeneratedMesh(const String& MeshName)
    {
        std::map<String,Mesh*>::iterator it = GeneratedMeshes.find(MeshName);
        if(it == GeneratedMeshes.end())
            return;
        Ogre::MeshManager::getSingletonPtr()->remove((*it).second->GetName());
        delete (*it).second;
        GeneratedMeshes.erase(it);
        return;
    }

    void MeshManager::DestroyAllGeneratedMeshes()
    {
        for( std::map<String,Mesh*>::iterator it = GeneratedMeshes.begin() ; it != GeneratedMeshes.end() ; it++ )
        {
            Ogre::MeshManager::getSingletonPtr()->remove((*it).second->GetName());
            delete (*it).second;
        }
        GeneratedMeshes.clear();
    }

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
        GeneratedMeshes[MeshName] = NewMesh;
        delete box;
        return NewMesh;
    }

    Mesh* MeshManager::CreateBoxMesh(const String& MeshName, const ColourValue& Colour, const Vector3& HalfExtents)
    {
        String MatName(MeshName+"Mat");
        CreateColouredMaterial(MatName,Colour);
        return CreateBoxMesh(MeshName,MatName,HalfExtents);
    }

    Mesh* MeshManager::CreateCylinderMesh(const String& MeshName, const String& MaterialName, const Vector3& HalfExtents, const Vector3& AxisOrientation, const Whole& CircleRes, const Whole& Segments)
    {
/// Start of MIT(Ogre Proceadural) License ///
        if(AxisOrientation != Vector3::Unit_Y())
            return NULL;
        if(HalfExtents.X != HalfExtents.Z)
            return NULL;

        Vector3 Half = HalfExtents;
        Ogre::MaterialPtr TheMaterial = Ogre::MaterialManager::getSingleton().getByName(MaterialName);
        String GroupName = TheMaterial->getGroup();

        Real deltaAngle = (Ogre::Math::TWO_PI / CircleRes);
        Real deltaHeight = (Half.Y * 2)/(Real)Segments;
        Real radius = Half.X;
        Whole offset = 0;

        Ogre::ManualObject* cylinder = new Ogre::ManualObject("TempMan");
        cylinder->begin(MaterialName);

        for (int i = 0; i <= Segments; i++)
        {
            for (int j = 0; j < CircleRes; j++)
            {
                Real x0 = radius * cosf(j*deltaAngle);
                Real z0 = radius * sinf(j*deltaAngle);
                cylinder->position(x0, (i*deltaHeight) - Half.Y, z0);
                cylinder->normal(Ogre::Vector3(x0,0,z0).normalisedCopy());
                cylinder->textureCoord(j/(Real)CircleRes, i/(Real)Segments);

                if (i != Segments)
                {
                    /// @todo These if/elses aren't really a long term solution
                    /// as they won't work for any more then 1 segment.  This needs to be addressed.
                    if(offset + CircleRes + 1 == CircleRes * 2)
                        cylinder->index(CircleRes);
                    else
                        cylinder->index(offset + CircleRes + 1);

                    cylinder->index(offset);
                    cylinder->index(offset + CircleRes);

                    if(offset + CircleRes + 1 == CircleRes * 2)
                        cylinder->index(0);
                    else
                        cylinder->index(offset + CircleRes + 1);

                    cylinder->index(offset + 1);
                    cylinder->index(offset);
                }
                offset ++;
            }
		}

		//low cap
		int centerIndex = offset;
		cylinder->position(0,-Half.Y,0);
		cylinder->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
		cylinder->textureCoord(0,1);
		offset++;
		for (int j=0;j<=CircleRes;j++)
		{
			Real x0 = radius * cosf(j*deltaAngle);
			Real z0 = radius * sinf(j*deltaAngle);

			cylinder->position(x0,-Half.Y, z0);
			cylinder->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
			cylinder->textureCoord(j/(Real)CircleRes,0.0);
			if (j!=CircleRes)
			{
				cylinder->index(centerIndex);
				cylinder->index(offset);
				cylinder->index(offset+1);
			}
			offset++;
		}

		// high cap
		centerIndex = offset;
		cylinder->position(0,Half.Y,0);
		cylinder->normal(Ogre::Vector3::UNIT_Y);
		cylinder->textureCoord(0,0);
		offset++;
		for (int j=0;j<=CircleRes;j++)
		{
			Real x0 = radius * cosf(j*deltaAngle);
			Real z0 = radius * sinf(j*deltaAngle);

			cylinder->position(x0, Half.Y, z0);
			cylinder->normal(Ogre::Vector3::UNIT_Y);
			cylinder->textureCoord(j/(Real)CircleRes,1);
			if (j!=CircleRes)
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
        GeneratedMeshes[MeshName] = NewMesh;
        delete cylinder;
        return NewMesh;
/// End of MIT(Ogre Proceadural) License ///
    }

    Mesh* MeshManager::CreateCylinderMesh(const String& MeshName, const ColourValue& Colour, const Vector3& HalfExtents, const Vector3& AxisOrientation, const Whole& CircleRes, const Whole& Segments)
    {
        String MatName(MeshName+"Mat");
        CreateColouredMaterial(MatName,Colour);
        return CreateCylinderMesh(MeshName,MatName,HalfExtents,AxisOrientation,CircleRes,Segments);
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
        GeneratedMeshes[MeshName] = NewMesh;
        delete sphere;
        return NewMesh;
    }

    Mesh* MeshManager::CreateSphereMesh(const String& MeshName, const ColourValue& Colour, const Real& Radius, const Real& Rings, const Real& Segments)
    {
        String MatName(MeshName+"Mat");
        CreateColouredMaterial(MatName,Colour);
        return CreateSphereMesh(MeshName,MatName,Radius,Rings,Segments);
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
        if(Smallest * 2 < FirstSizeCheck) Length = Smallest * 0.5;
        else Length = Smallest * 0.25;
        Ogre::MaterialPtr TheMaterial = Ogre::MaterialManager::getSingleton().getByName(MaterialName);
        String GroupName = TheMaterial->getGroup();

        Ogre::ManualObject* boxcorner = new Ogre::ManualObject("TempMan");
        boxcorner->begin(MaterialName);

        //Create a list of data we can play around with
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
        for (std::vector<Vector3>::iterator CurrentFlip=Flips.begin(); CurrentFlip!=Flips.end(); ++CurrentFlip)
        {
            for(std::vector<IdealPoint>::iterator CurrentVertex=IdealShape.begin(); CurrentVertex!=IdealShape.end(); ++CurrentVertex)
            {
                boxcorner->position( (CurrentVertex->Vertex * *CurrentFlip).GetOgreVector3() );
                boxcorner->normal( (CurrentVertex->Normal * *CurrentFlip).GetOgreVector3() );
                boxcorner->textureCoord( CurrentVertex->TextureCoord.GetOgreVector2() );
            }
        }


        /*
        // Vertex's
        // Top-Left-Front corner
        // Forward Face // 0 /
        boxcorner->position(-Half.X,Half.Y,Half.Z);                               boxcorner->normal(0,0,1);  boxcorner->textureCoord(0,0);
        boxcorner->position(-Half.X,Half.Y - Length,Half.Z);                      boxcorner->normal(0,0,1);  boxcorner->textureCoord(0,0.5);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - Length,Half.Z);       boxcorner->normal(0,0,1);  boxcorner->textureCoord(BoxThickness / Full.X,0.5);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z); boxcorner->normal(0,0,1);  boxcorner->textureCoord(BoxThickness / Full.X,BoxThickness / Full.Y);
        boxcorner->position(-Half.X + Length,Half.Y - BoxThickness,Half.Z);       boxcorner->normal(0,0,1);  boxcorner->textureCoord(0.5,BoxThickness / Full.Y);
        boxcorner->position(-Half.X + Length,Half.Y,Half.Z);                      boxcorner->normal(0,0,1);  boxcorner->textureCoord(0.5,0);
        // Upward Face // 6 /
        boxcorner->position(-Half.X,Half.Y,Half.Z);                               boxcorner->normal(0,1,0);  boxcorner->textureCoord(0,1);
        boxcorner->position(-Half.X + Length,Half.Y,Half.Z);                      boxcorner->normal(0,1,0);  boxcorner->textureCoord(0.5,1);
        boxcorner->position(-Half.X + Length,Half.Y,Half.Z - BoxThickness);       boxcorner->normal(0,1,0);  boxcorner->textureCoord(0.5,1 - (BoxThickness / Full.Y));
        boxcorner->position(-Half.X + BoxThickness,Half.Y,Half.Z - BoxThickness); boxcorner->normal(0,1,0);  boxcorner->textureCoord(BoxThickness / Full.X,1 - (BoxThickness / Full.Y));
        boxcorner->position(-Half.X + BoxThickness,Half.Y,Half.Z - Length);       boxcorner->normal(0,1,0);  boxcorner->textureCoord(BoxThickness / Full.X,0.5);
        boxcorner->position(-Half.X,Half.Y,Half.Z - Length);                      boxcorner->normal(0,1,0);  boxcorner->textureCoord(0,0.5);
        // Left Face // 12 /
        boxcorner->position(-Half.X,Half.Y,Half.Z);                               boxcorner->normal(-1,0,0); boxcorner->textureCoord(1,0);
        boxcorner->position(-Half.X,Half.Y,Half.Z - Length);                      boxcorner->normal(-1,0,0); boxcorner->textureCoord(0.5,0);
        boxcorner->position(-Half.X,Half.Y - BoxThickness,Half.Z - Length);       boxcorner->normal(-1,0,0); boxcorner->textureCoord(0.5,BoxThickness / Full.Y);
        boxcorner->position(-Half.X,Half.Y - BoxThickness,Half.Z - BoxThickness); boxcorner->normal(-1,0,0); boxcorner->textureCoord(1 - (BoxThickness / Full.X),BoxThickness / Full.Y);
        boxcorner->position(-Half.X,Half.Y - Length,Half.Z - BoxThickness);       boxcorner->normal(-1,0,0); boxcorner->textureCoord(1 - (BoxThickness / Full.X),0.5);
        boxcorner->position(-Half.X,Half.Y - Length,Half.Z);                      boxcorner->normal(-1,0,0); boxcorner->textureCoord(1,0.5);
        // Backward Faces // 18 /
        boxcorner->position(-Half.X + Length,Half.Y,Half.Z - BoxThickness);                      boxcorner->normal(0,0,-1); boxcorner->textureCoord(0.5,0);
        boxcorner->position(-Half.X + Length,Half.Y - BoxThickness,Half.Z - BoxThickness);       boxcorner->normal(0,0,-1); boxcorner->textureCoord(0.5,BoxThickness / Full.Y);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z - BoxThickness); boxcorner->normal(0,0,-1); boxcorner->textureCoord(1 - (BoxThickness / Full.X),BoxThickness / Full.Y);
        boxcorner->position(-Half.X + BoxThickness,Half.Y,Half.Z - BoxThickness);                boxcorner->normal(0,0,-1); boxcorner->textureCoord(1 - (BoxThickness / Full.X),0);
        boxcorner->position(-Half.X + BoxThickness,Half.Y,Half.Z - Length);                      boxcorner->normal(0,0,-1); boxcorner->textureCoord(1 - (BoxThickness / Full.X),0);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z - Length);       boxcorner->normal(0,0,-1); boxcorner->textureCoord(1 - (BoxThickness / Full.X),BoxThickness / Full.Y);
        boxcorner->position(-Half.X,Half.Y - BoxThickness,Half.Z - Length);                      boxcorner->normal(0,0,-1); boxcorner->textureCoord(1,BoxThickness / Full.Y);
        boxcorner->position(-Half.X,Half.Y,Half.Z - Length);                                     boxcorner->normal(0,0,-1); boxcorner->textureCoord(1,0);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z - BoxThickness); boxcorner->normal(0,0,-1); boxcorner->textureCoord(1 - (BoxThickness / Full.X),BoxThickness / Full.Y);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - Length,Half.Z - BoxThickness);       boxcorner->normal(0,0,-1); boxcorner->textureCoord(1 - (BoxThickness / Full.X),0.5);
        boxcorner->position(-Half.X,Half.Y - Length,Half.Z - BoxThickness);                      boxcorner->normal(0,0,-1); boxcorner->textureCoord(1,0.5);
        boxcorner->position(-Half.X,Half.Y - BoxThickness,Half.Z - BoxThickness);                boxcorner->normal(0,0,-1); boxcorner->textureCoord(1,BoxThickness / Full.Y);
        // Downward Faces // 30 /
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z);                boxcorner->normal(0,-1,0); boxcorner->textureCoord(BoxThickness / Full.X,0);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z - BoxThickness); boxcorner->normal(0,-1,0); boxcorner->textureCoord(BoxThickness / Full.X,BoxThickness / Full.Y);
        boxcorner->position(-Half.X + Length,Half.Y - BoxThickness,Half.Z - BoxThickness);       boxcorner->normal(0,-1,0); boxcorner->textureCoord(0.5,BoxThickness / Full.Y);
        boxcorner->position(-Half.X + Length,Half.Y - BoxThickness,Half.Z);                      boxcorner->normal(0,-1,0); boxcorner->textureCoord(0.5,0);
        boxcorner->position(-Half.X,Half.Y - Length,Half.Z);                                     boxcorner->normal(0,-1,0); boxcorner->textureCoord(0,0);
        boxcorner->position(-Half.X,Half.Y - Length,Half.Z - BoxThickness);                      boxcorner->normal(0,-1,0); boxcorner->textureCoord(0,BoxThickness / Full.Y);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - Length,Half.Z - BoxThickness);       boxcorner->normal(0,-1,0); boxcorner->textureCoord(BoxThickness / Full.X,BoxThickness / Full.Y);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - Length,Half.Z);                      boxcorner->normal(0,-1,0); boxcorner->textureCoord(BoxThickness / Full.X,0);
        boxcorner->position(-Half.X,Half.Y - BoxThickness,Half.Z - BoxThickness);                boxcorner->normal(0,-1,0); boxcorner->textureCoord(0,BoxThickness / Full.Y);
        boxcorner->position(-Half.X,Half.Y - BoxThickness,Half.Z - Length);                      boxcorner->normal(0,-1,0); boxcorner->textureCoord(0,0.5);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z - Length);       boxcorner->normal(0,-1,0); boxcorner->textureCoord(BoxThickness / Full.X,0.5);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z - BoxThickness); boxcorner->normal(0,-1,0); boxcorner->textureCoord(BoxThickness / Full.X,BoxThickness / Full.Y);
        // Right Faces // 42 /
        boxcorner->position(-Half.X + BoxThickness,Half.Y,Half.Z - BoxThickness);                boxcorner->normal(1,0,0);  boxcorner->textureCoord(BoxThickness / Full.X,0);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z - BoxThickness); boxcorner->normal(1,0,0);  boxcorner->textureCoord(BoxThickness / Full.X,BoxThickness / Full.Y);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z - Length);       boxcorner->normal(1,0,0);  boxcorner->textureCoord(0.5,BoxThickness / Full.Y);
        boxcorner->position(-Half.X + BoxThickness,Half.Y,Half.Z - Length);                      boxcorner->normal(1,0,0);  boxcorner->textureCoord(0.5,0);
        boxcorner->position(-Half.X + Length,Half.Y,Half.Z);                                     boxcorner->normal(1,0,0);  boxcorner->textureCoord(0,0);
        boxcorner->position(-Half.X + Length,Half.Y - BoxThickness,Half.Z);                      boxcorner->normal(1,0,0);  boxcorner->textureCoord(0,BoxThickness / Full.Y);
        boxcorner->position(-Half.X + Length,Half.Y - BoxThickness,Half.Z - BoxThickness);       boxcorner->normal(1,0,0);  boxcorner->textureCoord(BoxThickness / Full.X,BoxThickness / Full.Y);
        boxcorner->position(-Half.X + Length,Half.Y,Half.Z - BoxThickness);                      boxcorner->normal(1,0,0);  boxcorner->textureCoord(BoxThickness / Full.X,0);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z);                boxcorner->normal(1,0,0);  boxcorner->textureCoord(0,BoxThickness / Full.Y);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - Length,Half.Z);                      boxcorner->normal(1,0,0);  boxcorner->textureCoord(0,0.5);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - Length,Half.Z - BoxThickness);       boxcorner->normal(1,0,0);  boxcorner->textureCoord(BoxThickness / Full.X,0.5);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,Half.Z - BoxThickness); boxcorner->normal(1,0,0);  boxcorner->textureCoord(BoxThickness / Full.X,BoxThickness / Full.Y);

        // Top-Left-Back corner
        // Backward Face // 54
        boxcorner->position(-Half.X,Half.Y,-Half.Z);                               boxcorner->normal(0,0,-1); boxcorner->textureCoord(1,0);
        boxcorner->position(-Half.X + Length,Half.Y,-Half.Z);                      boxcorner->normal(0,0,-1); boxcorner->textureCoord(0.5,0);
        boxcorner->position(-Half.X + Length,Half.Y - BoxThickness,-Half.Z);       boxcorner->normal(0,0,-1); boxcorner->textureCoord(0.5,BoxThickness / Full.Y);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,-Half.Z); boxcorner->normal(0,0,-1); boxcorner->textureCoord(1 - (BoxThickness / Full.X),BoxThickness / Full.Y);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - Length,-Half.Z);       boxcorner->normal(0,0,-1); boxcorner->textureCoord(1 - (BoxThickness / Full.X),0.5);
        boxcorner->position(-Half.X,Half.Y - Length,-Half.Z);                      boxcorner->normal(0,0,-1); boxcorner->textureCoord(1,0.5);
        // Upward Face // 60
        boxcorner->position(-Half.X,Half.Y,-Half.Z);                               boxcorner->normal(0,1,0);  boxcorner->textureCoord(0,0);
        boxcorner->position(-Half.X,Half.Y,-Half.Z + Length);                      boxcorner->normal(0,1,0);  boxcorner->textureCoord(0,0.5);
        boxcorner->position(-Half.X + BoxThickness,Half.Y,-Half.Z + Length);       boxcorner->normal(0,1,0);  boxcorner->textureCoord(BoxThickness / Full.X,0.5);
        boxcorner->position(-Half.X + BoxThickness,Half.Y,-Half.Z + BoxThickness); boxcorner->normal(0,1,0);  boxcorner->textureCoord(BoxThickness / Full.X,BoxThickness / Full.Y);
        boxcorner->position(-Half.X + Length,Half.Y,-Half.Z + BoxThickness);       boxcorner->normal(0,1,0);  boxcorner->textureCoord(0.5,BoxThickness / Full.Y);
        boxcorner->position(-Half.X + Length,Half.Y,-Half.Z);                      boxcorner->normal(0,1,0);  boxcorner->textureCoord(0.5,0);
        // Left Face // 66
        boxcorner->position(-Half.X,Half.Y,-Half.Z);                               boxcorner->normal(-1,0,0); boxcorner->textureCoord(0,0);
        boxcorner->position(-Half.X,Half.Y - Length,-Half.Z);                      boxcorner->normal(-1,0,0); boxcorner->textureCoord(0,0.5);
        boxcorner->position(-Half.X,Half.Y - Length,-Half.Z + BoxThickness);       boxcorner->normal(-1,0,0); boxcorner->textureCoord(BoxThickness / Full.X,0.5);
        boxcorner->position(-Half.X,Half.Y - BoxThickness,-Half.Z + BoxThickness); boxcorner->normal(-1,0,0); boxcorner->textureCoord(BoxThickness / Full.X,BoxThickness / Full.Y);
        boxcorner->position(-Half.X,Half.Y - BoxThickness,-Half.Z + Length);       boxcorner->normal(-1,0,0); boxcorner->textureCoord(0.5,BoxThickness / Full.Y);
        boxcorner->position(-Half.X,Half.Y,-Half.Z + Length);                      boxcorner->normal(-1,0,0); boxcorner->textureCoord(0.5,0);
        // Forward Faces // 72
        boxcorner->position(-Half.X + BoxThickness,Half.Y,-Half.Z + BoxThickness);                boxcorner->normal(0,0,1);  boxcorner->textureCoord(BoxThickness / Full.X,0);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,-Half.Z + BoxThickness); boxcorner->normal(0,0,1);  boxcorner->textureCoord(BoxThickness / Full.X,BoxThickness / Full.Y);
        boxcorner->position(-Half.X + Length,Half.Y - BoxThickness,-Half.Z + BoxThickness);       boxcorner->normal(0,0,1);  boxcorner->textureCoord(0.5,BoxThickness / Full.Y);
        boxcorner->position(-Half.X + Length,Half.Y,-Half.Z + BoxThickness);                      boxcorner->normal(0,0,1);  boxcorner->textureCoord(0.5,0);
        boxcorner->position(-Half.X,Half.Y,-Half.Z + Length);                                     boxcorner->normal(0,0,1);  boxcorner->textureCoord(0,0);
        boxcorner->position(-Half.X,Half.Y - BoxThickness,-Half.Z + Length);                      boxcorner->normal(0,0,1);  boxcorner->textureCoord(0,BoxThickness / Full.Y);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,-Half.Z + Length);       boxcorner->normal(0,0,1);  boxcorner->textureCoord(BoxThickness / Full.X,BoxThickness / Full.Y);
        boxcorner->position(-Half.X + BoxThickness,Half.Y,-Half.Z + Length);                      boxcorner->normal(0,0,1);  boxcorner->textureCoord(BoxThickness / Full.X,0);
        boxcorner->position(-Half.X,Half.Y - BoxThickness,-Half.Z + BoxThickness);                boxcorner->normal(0,0,1);  boxcorner->textureCoord(0,BoxThickness / Full.Y);
        boxcorner->position(-Half.X,Half.Y - Length,-Half.Z + BoxThickness);                      boxcorner->normal(0,0,1);  boxcorner->textureCoord(0,0.5);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - Length,-Half.Z + BoxThickness);       boxcorner->normal(0,0,1);  boxcorner->textureCoord(BoxThickness / Full.X,0.5);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,-Half.Z + BoxThickness); boxcorner->normal(0,0,1);  boxcorner->textureCoord(BoxThickness / Full.X,BoxThickness / Full.Y);
        // Downward Faces
        boxcorner->position(-Half.X + Length,Half.Y - BoxThickness,-Half.Z);                      boxcorner->normal(0,-1,0); boxcorner->textureCoord(0.5,0);
        boxcorner->position(-Half.X + Length,Half.Y - BoxThickness,-Half.Z + BoxThickness);       boxcorner->normal(0,-1,0); boxcorner->textureCoord(0.5,BoxThickness / Full.Y);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,-Half.Z + BoxThickness); boxcorner->normal(0,-1,0); boxcorner->textureCoord(1 - (BoxThickness / Full.X),BoxThickness / Full.Y);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,-Half.Z);                boxcorner->normal(0,-1,0); boxcorner->textureCoord(1 - (BoxThickness / Full.X),0);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - Length,-Half.Z);                      boxcorner->normal(0,-1,0); boxcorner->textureCoord(1 - (BoxThickness / Full.X),0);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - Length,-Half.Z + BoxThickness);       boxcorner->normal(0,-1,0); boxcorner->textureCoord(1 - (BoxThickness / Full.X),BoxThickness / Full.Y);
        boxcorner->position(-Half.X,Half.Y - Length,-Half.Z + BoxThickness);                      boxcorner->normal(0,-1,0); boxcorner->textureCoord(1,BoxThickness / Full.Y);
        boxcorner->position(-Half.X,Half.Y - Length,-Half.Z);                                     boxcorner->normal(0,-1,0); boxcorner->textureCoord(1,0);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,-Half.Z + BoxThickness); boxcorner->normal(0,-1,0); boxcorner->textureCoord(1 - (BoxThickness / Full.X),BoxThickness / Full.Y);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,-Half.Z + Length);       boxcorner->normal(0,-1,0); boxcorner->textureCoord(1 - (BoxThickness / Full.X),0.5);
        boxcorner->position(-Half.X,Half.Y - BoxThickness,-Half.Z + Length);                      boxcorner->normal(0,-1,0); boxcorner->textureCoord(1,0.5);
        boxcorner->position(-Half.X,Half.Y - BoxThickness,-Half.Z + BoxThickness);                boxcorner->normal(0,-1,0); boxcorner->textureCoord(1,BoxThickness / Full.Y);
        // Right Faces
        boxcorner->position(-Half.X + BoxThickness,Half.Y,-Half.Z + Length);                      boxcorner->normal(1,0,0);  boxcorner->textureCoord(0.5,0);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,-Half.Z + Length);       boxcorner->normal(1,0,0);  boxcorner->textureCoord(0.5,BoxThickness / Full.Y);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,-Half.Z + BoxThickness); boxcorner->normal(1,0,0);  boxcorner->textureCoord(1 - (BoxThickness / Full.X),BoxThickness / Full.Y);
        boxcorner->position(-Half.X + BoxThickness,Half.Y,-Half.Z + BoxThickness);                boxcorner->normal(1,0,0);  boxcorner->textureCoord(1 - (BoxThickness / Full.X),0);
        boxcorner->position(-Half.X + Length,Half.Y,-Half.Z + BoxThickness);                      boxcorner->normal(1,0,0);  boxcorner->textureCoord(1 - (BoxThickness / Full.X),0);
        boxcorner->position(-Half.X + Length,Half.Y - BoxThickness,-Half.Z + BoxThickness);       boxcorner->normal(1,0,0);  boxcorner->textureCoord(1 - (BoxThickness / Full.X),BoxThickness / Full.Y);
        boxcorner->position(-Half.X + Length,Half.Y - BoxThickness,-Half.Z);                      boxcorner->normal(1,0,0);  boxcorner->textureCoord(1,BoxThickness / Full.Y);
        boxcorner->position(-Half.X + Length,Half.Y,-Half.Z);                                     boxcorner->normal(1,0,0);  boxcorner->textureCoord(1,0);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,-Half.Z + BoxThickness); boxcorner->normal(1,0,0);  boxcorner->textureCoord(1 - (BoxThickness / Full.X),BoxThickness / Full.Y);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - Length,-Half.Z + BoxThickness);       boxcorner->normal(1,0,0);  boxcorner->textureCoord(1 - (BoxThickness / Full.X),0.5);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - Length,-Half.Z);                      boxcorner->normal(1,0,0);  boxcorner->textureCoord(1,0.5);
        boxcorner->position(-Half.X + BoxThickness,Half.Y - BoxThickness,-Half.Z);                boxcorner->normal(1,0,0);  boxcorner->textureCoord(1,BoxThickness / Full.Y);
*/
        // Top-Right-Front corner
        // Top-Right-Back corner
        // Bottom-Left-Front corner
        // Bottom-Left-Back corner
        // Bottom-Right-Front corner
        // Bottom-Right-Back corner


        // Create Index's
        for (std::vector<Vector3>::iterator CurrentFlip=Flips.begin(); CurrentFlip!=Flips.end(); ++CurrentFlip)
        {
            Whole WhichShape = Flips.begin()-CurrentFlip;
            Whole ShapeIndex = WhichShape*IdealShape.size();

            // Top-Left-Front Forward Face
            if(1==CurrentFlip->Z)   // draw them in backward order if this shape is flipped on the Z axis
            {
                //Forward Face Unflipped
                boxcorner->triangle(0+ShapeIndex,1+ShapeIndex,2+ShapeIndex);
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

            // Top-Left-Front Forward Face
            //boxcorner->triangle(0,1,2);
            //boxcorner->triangle(0,2,3);
            //boxcorner->triangle(0,3,4);
            //boxcorner->triangle(0,4,5);
        }

        // Top-Left-Front Upward Face
        boxcorner->triangle(6,7,8);
        boxcorner->triangle(6,8,9);
        boxcorner->triangle(6,9,10);
        boxcorner->triangle(6,10,11);
        // Top-Left-Front Left Face
        boxcorner->triangle(12,13,14);
        boxcorner->triangle(12,14,15);
        boxcorner->triangle(12,15,16);
        boxcorner->triangle(12,16,17);
        // Top-Left-Front Backward Faces
        boxcorner->triangle(18,19,20);
        boxcorner->triangle(18,20,21);
        boxcorner->triangle(22,23,24);
        boxcorner->triangle(22,24,25);
        boxcorner->triangle(26,27,28);
        boxcorner->triangle(26,28,29);
        // Top-Left-Front Downward Faces
        boxcorner->triangle(30,31,32);
        boxcorner->triangle(30,32,33);
        boxcorner->triangle(34,35,36);
        boxcorner->triangle(34,36,37);
        boxcorner->triangle(38,39,40);
        boxcorner->triangle(38,40,41);
        // Top-Left-Front Right Faces
        boxcorner->triangle(42,43,44);
        boxcorner->triangle(42,44,45);
        boxcorner->triangle(46,47,48);
        boxcorner->triangle(46,48,49);
        boxcorner->triangle(50,51,52);
        boxcorner->triangle(50,52,53);


/*
        // Index's
        // Top-Left-Front Forward Face
        boxcorner->triangle(0,1,2);
        boxcorner->triangle(0,2,3);
        boxcorner->triangle(0,3,4);
        boxcorner->triangle(0,4,5);
        // Top-Left-Front Upward Face
        boxcorner->triangle(6,7,8);
        boxcorner->triangle(6,8,9);
        boxcorner->triangle(6,9,10);
        boxcorner->triangle(6,10,11);
        // Top-Left-Front Left Face
        boxcorner->triangle(12,13,14);
        boxcorner->triangle(12,14,15);
        boxcorner->triangle(12,15,16);
        boxcorner->triangle(12,16,17);
        // Top-Left-Front Backward Faces
        boxcorner->triangle(18,19,20);
        boxcorner->triangle(18,20,21);
        boxcorner->triangle(22,23,24);
        boxcorner->triangle(22,24,25);
        boxcorner->triangle(26,27,28);
        boxcorner->triangle(26,28,29);
        // Top-Left-Front Downward Faces
        boxcorner->triangle(30,31,32);
        boxcorner->triangle(30,32,33);
        boxcorner->triangle(34,35,36);
        boxcorner->triangle(34,36,37);
        boxcorner->triangle(38,39,40);
        boxcorner->triangle(38,40,41);
        // Top-Left-Front Right Faces
        boxcorner->triangle(42,43,44);
        boxcorner->triangle(42,44,45);
        boxcorner->triangle(46,47,48);
        boxcorner->triangle(46,48,49);
        boxcorner->triangle(50,51,52);
        boxcorner->triangle(50,52,53);

/*
        // Top-Left-Back Backward Face
        boxcorner->triangle(54,55,56);
        boxcorner->triangle(54,56,57);
        boxcorner->triangle(54,57,58);
        boxcorner->triangle(54,58,59);
        // Top-Left-Back Upward Face
        boxcorner->triangle(60,61,62);
        boxcorner->triangle(60,62,63);
        boxcorner->triangle(60,63,64);
        boxcorner->triangle(60,64,65);
        // Top-Left-Back Left Face
        boxcorner->triangle(66,67,68);
        boxcorner->triangle(66,68,69);
        boxcorner->triangle(66,69,70);
        boxcorner->triangle(66,70,71);
        // Top-Left-Back Front Faces
        boxcorner->triangle(72,73,74);
        boxcorner->triangle(72,74,75);
        boxcorner->triangle(76,77,78);
        boxcorner->triangle(76,78,79);
        boxcorner->triangle(80,81,82);
        boxcorner->triangle(80,82,83);
        // Top-Left-Back Downward Faces
        boxcorner->triangle(84,85,86);
        boxcorner->triangle(84,86,87);
        boxcorner->triangle(88,89,90);
        boxcorner->triangle(88,90,91);
        boxcorner->triangle(92,93,94);
        boxcorner->triangle(92,94,95);
        // Top-Left-Back Right Faces
        boxcorner->triangle(96,97,98);
        boxcorner->triangle(96,98,99);
        boxcorner->triangle(100,101,102);
        boxcorner->triangle(100,102,103);
        boxcorner->triangle(104,105,106);
        boxcorner->triangle(104,106,107);
        // Top-Right-Front
        /*boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        // Top-Right-Front
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        // Top-Right-Front
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        // Top-Right-Back
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        // Top-Right-Back
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        // Top-Right-Back
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        // Bottom-Left-Front
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        // Bottom-Left-Front
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        // Bottom-Left-Front
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        // Bottom-Left-Back
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        // Bottom-Left-Back
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        // Bottom-Left-Back
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        // Bottom-Right-Front
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        // Bottom-Right-Front
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        // Bottom-Right-Front
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        // Bottom-Right-Back
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        // Bottom-Right-Back
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        // Bottom-Right-Back
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);
        boxcorner->triangle(,,);// */

        boxcorner->end();
        Ogre::MeshPtr boxcornermesh = boxcorner->convertToMesh(MeshName, GroupName);
        boxcornermesh->_setBounds(Ogre::AxisAlignedBox(-Half.X,-Half.Y,-Half.Z,Half.X,Half.Y,Half.Z));
        Real FirstCheck = Half.X > Half.Y ? Half.X : Half.Y;
        Real Largest = FirstCheck > Half.Z ? FirstCheck : Half.Z;
        boxcornermesh->_setBoundingSphereRadius(1.4 * Largest);
        Mesh* NewMesh = new Mesh(boxcornermesh);
        GeneratedMeshes[MeshName] = NewMesh;
        delete boxcorner;
        return NewMesh;
    }

    Mesh* MeshManager::CreateBoxCornerMesh(const String& MeshName, const ColourValue& Colour, const Vector3& HalfExtents, const Real& BoxThickness)
    {
        String MatName(MeshName+"Mat");
        CreateColouredMaterial(MatName,Colour);
        return CreateBoxCornerMesh(MeshName,MatName,HalfExtents,BoxThickness);
    }

    Mesh* MeshManager::CreateMeshFromShape(const String& MeshName, const String& MaterialName, CollisionShape* Shape)
    {
        return NULL;
    }

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

    void MeshManager::Initialize()
    {}

    void MeshManager::DoMainLoopItems()
    {}

    ManagerBase::ManagerTypeName MeshManager::GetType() const
    { return ManagerBase::MeshManager; }
}

#endif
