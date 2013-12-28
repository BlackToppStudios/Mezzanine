// © Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _physicsmultispherecollisionshape_cpp
#define _physicsmultispherecollisionshape_cpp

#include "Physics/multispherecollisionshape.h"
#include "collisionshapemanager.h"
#include "stringtool.h"

#include "btBulletDynamicsCommon.h"

namespace Mezzanine
{
    namespace Physics
    {
        /////////////////////////////////////////
        // MultiSphereCollisionShape Functions

        void MultiSphereCollisionShape::Construct(const String& Name, const std::vector<Vector3>& Locations, const std::vector<Real>& Radii)
        {
            if(Locations.size() != Radii.size())
                { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Vector size mismatch between Locations and Radii in MultiSphereCollisionShape constructor."); }
            Whole Spheres = Locations.size();
            btVector3* BulletLocs = new btVector3[Spheres];
            btScalar* BulletRadii = new btScalar[Spheres];
            for( Whole X = 0 ; X < Spheres ; X++ )
            {
                BulletLocs[X] = Locations[X].GetBulletVector3();
                BulletRadii[X] = Radii[X];
            }

            Construct(Name,new btMultiSphereShape(BulletLocs,BulletRadii,Spheres));

            delete[] BulletLocs;
            delete[] BulletRadii;
        }

        void MultiSphereCollisionShape::Construct(const String& Name, btMultiSphereShape* BulletShape)
        {
            this->Name = Name;
            SetPointers(BulletShape);
            this->GetMultiSphereShape()->setImplicitShapeDimensions(Vector3(0,0,0).GetBulletVector3());
        }


        MultiSphereCollisionShape::MultiSphereCollisionShape(const String& Name, const std::vector<Vector3>& Locations, const std::vector<Real>& Radii)
        {
            Construct(Name,Locations,Radii);
        }

        MultiSphereCollisionShape::MultiSphereCollisionShape(const String& Name, btMultiSphereShape* BulletShape)
        {
            this->Name = Name;
            SetPointers(BulletShape);
        }

        MultiSphereCollisionShape::MultiSphereCollisionShape(XML::Node OneNode)
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                XML::Attribute OneName = OneNode.GetChild("PrimitiveCollisionShape").GetChild("CollisionShape").GetAttribute("Name");               // get name
                if(!OneName) { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Could not find Name Attribute on CollsionShape Node during preparation for deserialization"); }

                XML::Node Spheres = OneNode.GetChild("Spheres");
                if(!Spheres)
                    { DeSerializeError("locate Spheres node",SerializableName()); }

                std::vector<Vector3> Locations;
                std::vector<Real> Radii;
                XML::Node OneSphere = Spheres.GetFirstChild();
                while (OneSphere)
                {
                    Locations.push_back(Vector3(OneSphere.GetFirstChild()));
                    Radii.push_back(OneSphere.GetFirstAttribute().AsReal());

                    OneSphere = OneSphere.GetNextSibling();
                }

                Construct(OneName.AsString(),Locations,Radii);

                this->ProtoDeSerialize(OneNode);
            }else{
                DeSerializeError("find usable serialization version",MultiSphereCollisionShape::SerializableName());
            }
        }

        MultiSphereCollisionShape::~MultiSphereCollisionShape()
        {
            delete GetMultiSphereShape();
        }

        Vector3 MultiSphereCollisionShape::GetSphereLocation(const Whole& Index) const
        {
            Vector3 Loc(GetMultiSphereShape()->getSpherePosition(Index));
            return Loc;
        }

        Real MultiSphereCollisionShape::GetSphereRadius(const Whole& Index) const
        {
            return GetMultiSphereShape()->getSphereRadius(Index);
        }

        Whole MultiSphereCollisionShape::GetNumSpheres() const
        {
            return GetMultiSphereShape()->getSphereCount();
        }

        CollisionShape::ShapeType MultiSphereCollisionShape::GetType() const
        {
            return CollisionShape::ST_MultiSphere;
        }

        btMultiSphereShape* MultiSphereCollisionShape::GetMultiSphereShape() const
            { return static_cast<btMultiSphereShape*>(ShapeBase); }

        void MultiSphereCollisionShape::ProtoSerialize(XML::Node& CurrentRoot) const
        {
            XML::Node CollisionNode = CurrentRoot.AppendChild(this->MultiSphereCollisionShape::SerializableName());
            if (!CollisionNode) { SerializeError("create CollisionNode",this->MultiSphereCollisionShape::SerializableName());}

            XML::Attribute Version = CollisionNode.AppendAttribute("Version");
            if (Version)
                { Version.SetValue(1); }
            else
                { SerializeError("Create Version Attribute", SerializableName()); }

            XML::Node PointsNode = CollisionNode.AppendChild("Spheres");
            if (!PointsNode) { SerializeError("create Spheres",this->MultiSphereCollisionShape::SerializableName());}

            for(Whole c=0; c<this->GetNumSpheres(); ++c)
            {
                XML::Node Sphere = PointsNode.AppendChild("Sphere");
                if (!Sphere) { SerializeError(String("create Sphere ")+ToString(c),this->MultiSphereCollisionShape::SerializableName());}

                XML::Attribute Radius = Sphere.AppendAttribute("Radius");
                if (!Radius) { SerializeError(String("Append readius to Sphere ")+ToString(c),this->MultiSphereCollisionShape::SerializableName());}
                Radius.SetValue( this->GetSphereRadius(c) );

                this->GetSphereLocation(c).ProtoSerialize(Sphere);
            }

            this->PrimitiveCollisionShape::ProtoSerialize(CollisionNode);
        }

        void MultiSphereCollisionShape::ProtoDeSerialize(const XML::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==this->MultiSphereCollisionShape::SerializableName() )
            {
                if(OneNode.GetAttribute("Version").AsInt() == 1)
                {
                    XML::Node CollisionNode = OneNode.GetChild(this->PrimitiveCollisionShape::SerializableName());
                    if(!CollisionNode)
                        { DeSerializeError("locate PrimitiveCollisionShape node",SerializableName()); }
                    this->PrimitiveCollisionShape::ProtoDeSerialize(CollisionNode);

                }else{
                    DeSerializeError("find usable serialization version",SerializableName());
                }
            }else{
                DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
            }
        }

        String MultiSphereCollisionShape::SerializableName()
            {   return String("MultiSphereCollisionShape"); }
    }//Physics
}//Mezzanine

std::ostream& operator << (std::ostream& stream, const Mezzanine::Physics::MultiSphereCollisionShape& ShapeToSerialize)
    { Mezzanine::Serialize(stream, ShapeToSerialize); return stream; }

std::istream& operator >> (std::istream& stream, Mezzanine::Physics::MultiSphereCollisionShape& x)
    { return Mezzanine::DeSerialize(stream, x); }

void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::MultiSphereCollisionShape& x)
    { x.ProtoDeSerialize(OneNode); }

#endif
