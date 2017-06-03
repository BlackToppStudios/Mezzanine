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
#ifndef _physicsconvexhullcollisionshape_cpp
#define _physicsconvexhullcollisionshape_cpp

#include "Physics/convexhullcollisionshape.h"
#include "collisionshapemanager.h"
#include "stringtool.h"

#include "btBulletDynamicsCommon.h"

namespace Mezzanine
{
    namespace Physics
    {
        /////////////////////////////////////////
        // ConvexHullCollisionShape Functions

        ConvexHullCollisionShape::ConvexHullCollisionShape(const String& Name, const std::vector<Vector3>& Points)
        {
            btScalar* BulletPoints = new btScalar[Points.size() * 3];
            for( Whole X = 0 ; X < Points.size() ; ++X )
            {
                BulletPoints[X*3]     = Points[X][0];
                BulletPoints[X*3+1]   = Points[X][1];
                BulletPoints[X*3+2]   = Points[X][2];
            }

            this->Name = Name;
            SetPointers(new btConvexHullShape(BulletPoints,Points.size(),3*sizeof(btScalar)));
            this->GetBulletHullShape()->setImplicitShapeDimensions(Vector3(0,0,0).GetBulletVector3());
            delete[] BulletPoints;
        }

        ConvexHullCollisionShape::ConvexHullCollisionShape(const String& Name, btConvexHullShape* BulletShape)
        {
            this->Name = Name;
            SetPointers(BulletShape);
            this->GetBulletHullShape()->setImplicitShapeDimensions(Vector3(0,0,0).GetBulletVector3());
        }

        ConvexHullCollisionShape::ConvexHullCollisionShape(XML::Node OneNode)
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                XML::Attribute OneName = OneNode.GetChild("PrimitiveCollisionShape").GetChild("CollisionShape").GetAttribute("Name");               // get name
                if(!OneName) { MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Could not find Name Attribute on CollsionShape Node during preparation for deserialization"); }

                this->Name = OneName.AsString();

                SetPointers(new btConvexHullShape());

                this->ProtoDeSerialize(OneNode);
            }else{
                DeSerializeError("find usable serialization version", ConvexHullCollisionShape::GetSerializableName());
            }
        }

        ConvexHullCollisionShape::~ConvexHullCollisionShape()
        {
            delete GetBulletHullShape();
        }

        void ConvexHullCollisionShape::AddPoint(const Vector3& Point)
        {
            GetBulletHullShape()->addPoint(Point.GetBulletVector3());
        }

        Vector3 ConvexHullCollisionShape::GetUnscaledPoint(const Whole& Index) const
        {

            return Vector3 ( *(GetBulletHullShape()->getUnscaledPoints()+Index) );
        }

        Vector3 ConvexHullCollisionShape::GetPoint(const Whole& Index) const
        {
            return Vector3(GetBulletHullShape()->getScaledPoint(Index));
        }

        Whole ConvexHullCollisionShape::GetNumPoints() const
        {
            return GetBulletHullShape()->getNumPoints();
        }

        Boole ConvexHullCollisionShape::IsInside(const Vector3& Location, const Real& Tolerance) const
        {
            return GetBulletHullShape()->isInside(Location.GetBulletVector3(),Tolerance);
        }

        CollisionShape::ShapeType ConvexHullCollisionShape::GetType() const
        {
            return CollisionShape::ST_ConvexHull;
        }

        btConvexHullShape* ConvexHullCollisionShape::GetBulletHullShape() const
            { return static_cast<btConvexHullShape*>(ShapeBase); }

        void ConvexHullCollisionShape::ProtoSerialize(XML::Node& CurrentRoot) const
        {
            XML::Node CollisionNode = CurrentRoot.AppendChild(this->ConvexHullCollisionShape::GetSerializableName());
            if (!CollisionNode) { SerializeError("create CollisionNode",this->ConvexHullCollisionShape::GetSerializableName());}

            XML::Attribute Version = CollisionNode.AppendAttribute("Version");
            if (Version)
                { Version.SetValue(1); }
            else
                { SerializeError("Create Version Attribute", GetSerializableName()); }

            XML::Node PointsNode = CollisionNode.AppendChild("UnscaledPoints");
            if (!PointsNode) { SerializeError("create UnscaledPoints",this->ConvexHullCollisionShape::GetSerializableName());}

            for(Whole c=0; c<this->GetNumPoints(); ++c)
            {
                this->GetUnscaledPoint(c).ProtoSerialize(PointsNode);
            }

            this->PrimitiveCollisionShape::ProtoSerialize(CollisionNode);
        }

        void ConvexHullCollisionShape::ProtoDeSerialize(const XML::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==this->ConvexHullCollisionShape::GetSerializableName() )
            {
                if(OneNode.GetAttribute("Version").AsInt() == 1)
                {
                    XML::Node CollisionNode = OneNode.GetChild(this->PrimitiveCollisionShape::GetSerializableName());
                    if(!CollisionNode)
                        { DeSerializeError("locate PrimitiveCollisionShape node",GetSerializableName()); }
                    this->PrimitiveCollisionShape::ProtoDeSerialize(CollisionNode);

                    XML::Node UnscaledPoints = OneNode.GetChild("UnscaledPoints");
                    if(!UnscaledPoints)
                        { DeSerializeError("locate UnscaledPoints node",GetSerializableName()); }

                    XML::Node OnePoint = UnscaledPoints.GetFirstChild();
                    while (OnePoint)
                    {
                        this->AddPoint(Vector3(OnePoint));
                        OnePoint = OnePoint.GetNextSibling();
                    }

                }else{
                    DeSerializeError("find usable serialization version",GetSerializableName());
                }
            }else{
                DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),GetSerializableName());
            }
        }

        String ConvexHullCollisionShape::GetSerializableName()
            {   return String("ConvexHullCollisionShape"); }

    }//Physics
}//Mezzanine

    std::ostream& operator << (std::ostream& stream, const Mezzanine::Physics::ConvexHullCollisionShape& ShapeToSerialize)
        { Mezzanine::Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, Mezzanine::Physics::ConvexHullCollisionShape& x)
        { return Mezzanine::DeSerialize(stream, x); }

    void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::ConvexHullCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

#endif
