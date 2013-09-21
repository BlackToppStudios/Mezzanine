//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _collisionshape_cpp
#define _collisionshape_cpp

#include "Physics/collisionshape.h"
#include "collisionshapemanager.h"
#include "stringtool.h"

// For Utility functions
#include "Physics/boxcollisionshape.h"
#include "Physics/capsulecollisionshape.h"
#include "Physics/conecollisionshape.h"
#include "Physics/convexhullcollisionshape.h"
#include "Physics/cylindercollisionshape.h"
#include "Physics/multispherecollisionshape.h"
#include "Physics/spherecollisionshape.h"
#include "Physics/dynamicmeshcollisionshape.h"
#include "Physics/heightfieldcollisionshape.h"
#include "Physics/planecollisionshape.h"
#include "Physics/softcollisionshape.h"
#include "Physics/staticmeshcollisionshape.h"
#include "Physics/compoundcollisionshape.h"

#include "btBulletDynamicsCommon.h"

namespace Mezzanine
{
    namespace Physics
    {
        /////////////////////////////////////////
        // CollisionShape Functions

        CollisionShape::CollisionShape() :
            ShapeBase(NULL)
        {
        }

        CollisionShape::~CollisionShape()
        {
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        const String& CollisionShape::GetName() const
        {
            return this->Name;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Configuration Methods

        void CollisionShape::SetMargin(const Real& Margin)
        {
            this->ShapeBase->setMargin(Margin);
        }

        Real CollisionShape::GetMargin() const
        {
            return this->ShapeBase->getMargin();
        }

        void CollisionShape::SetScaling(const Vector3& Scaling)
        {
            this->ShapeBase->setLocalScaling(Scaling.GetBulletVector3());
        }

        Vector3 CollisionShape::GetScaling() const
        {
            return Vector3(this->ShapeBase->getLocalScaling());
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void CollisionShape::ProtoSerialize(XML::Node& CurrentRoot) const
        {
            XML::Node CollisionNode = CurrentRoot.AppendChild(this->CollisionShape::SerializableName());
            if (!CollisionNode) { SerializeError("create CollisionNode",this->CollisionShape::SerializableName());}

            XML::Attribute Version = CollisionNode.AppendAttribute("Version");
            if (Version)
                { Version.SetValue(1); }
            else
                { SerializeError("Create Version Attribute", SerializableName()); }

            XML::Attribute NameAttr = CollisionNode.AppendAttribute("Name");
            if(!NameAttr)
                { SerializeError("Create Name Attribute", SerializableName()); }
            NameAttr.SetValue(this->GetName());

            XML::Attribute MarginAttr = CollisionNode.AppendAttribute("Margin");
            if(!MarginAttr)
                { SerializeError("Create Margin Attribute", SerializableName()); }
            MarginAttr.SetValue(this->GetMargin());

            XML::Node ScalingNode = CollisionNode.AppendChild("Scaling");
            if (!ScalingNode) { SerializeError("Create Name Attribute", SerializableName()); }
            this->GetScaling().ProtoSerialize(ScalingNode);
        }

        void CollisionShape::ProtoDeSerialize(const XML::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==this->CollisionShape::SerializableName() )
            {
                if(OneNode.GetAttribute("Version").AsInt() == 1)
                {
                    if(OneNode.GetAttribute("Name"))
                        { this->Name = OneNode.GetAttribute("Name").AsString(); }
                    this->SetMargin(OneNode.GetAttribute("Margin").AsReal());

                    XML::Node ScalingNode = OneNode.GetChild("Scaling").GetFirstChild();
                    if(!ScalingNode)
                        { DeSerializeError("locate Scaling node",SerializableName()); }
                    this->SetScaling(Vector3(ScalingNode));
                }else{
                    DeSerializeError("find usable serialization version",SerializableName());
                }
            }else{
                DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
            }
        }

        String CollisionShape::SerializableName()
            { return String("CollisionShape"); }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        void CollisionShape::_SetShapeName(const String& NewName)
        {
            this->Name = NewName;
        }

        btCollisionShape* CollisionShape::_GetInternalShape() const
        {
            return this->ShapeBase;
        }

        /////////////////////////////////////////
        // Utility Functions

        CollisionShape* CreateShape(CollisionShape::ShapeType ShapeToCreate, const String& Name_, btCollisionShape* ShapeToModel)
        {
            if (ShapeToModel)
            {
                switch(ShapeToCreate)
                {
                    case CollisionShape::ST_Box:
                        return new BoxCollisionShape(Name_,(btBoxShape*)ShapeToModel);
                    case CollisionShape::ST_Capsule:
                        return new CapsuleCollisionShape(Name_,(btCapsuleShape*)ShapeToModel);
                    case CollisionShape::ST_Cone:
                        return new ConeCollisionShape(Name_,(btConeShape*)ShapeToModel);
                    case CollisionShape::ST_ConvexHull:
                        return new ConvexHullCollisionShape(Name_,(btConvexHullShape*)ShapeToModel);
                    case CollisionShape::ST_Cylinder:
                        return new CylinderCollisionShape(Name_,(btCylinderShape*)ShapeToModel);
                    case CollisionShape::ST_MultiSphere:
                        return new MultiSphereCollisionShape(Name_,(btMultiSphereShape*)ShapeToModel);
                    case CollisionShape::ST_Sphere:
                        return new SphereCollisionShape(Name_,(btSphereShape*)ShapeToModel);
                    case CollisionShape::ST_DynamicTriMesh:
                        return new DynamicMeshCollisionShape(Name_,(btGImpactMeshShape*)ShapeToModel);
                    case CollisionShape::ST_Heightfield:
                        return new HeightfieldCollisionShape(Name_,(btHeightfieldTerrainShape*)ShapeToModel);
                    case CollisionShape::ST_Plane:
                        return new PlaneCollisionShape(Name_,(btStaticPlaneShape*)ShapeToModel);
                    case CollisionShape::ST_Soft:
                        return new SoftCollisionShape(Name_,(btSoftBodyCollisionShape*)ShapeToModel);
                    case CollisionShape::ST_StaticTriMesh:
                        return new StaticMeshCollisionShape(Name_,(btBvhTriangleMeshShape*)ShapeToModel);
                    case CollisionShape::ST_Compound: // holy recursive batman
                        return new CompoundCollisionShape(Name_,(btCompoundShape*)ShapeToModel);
                    default:
                        { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to convert an unsupported/unwrapped Collision Shape type into a CollisionShape instance."); }
                }
            }else{
                MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to convert an empty Internal Collision Shape type into a CollisionShape instance.");
            }
        }

        CollisionShape* CreateShape(XML::Node OneNode)
        {
            CollisionShape::ShapeType ShapeToCreate = StringToShapeType(OneNode.Name());

            String Name_("Defunct");

            switch(ShapeToCreate)
            {
                case CollisionShape::ST_Box:
                    return new BoxCollisionShape(OneNode);
                case CollisionShape::ST_Capsule:
                    return new CapsuleCollisionShape(OneNode);
                case CollisionShape::ST_Cone:
                    return new ConeCollisionShape(OneNode);
                case CollisionShape::ST_ConvexHull:
                    return new ConvexHullCollisionShape(OneNode);
                case CollisionShape::ST_Cylinder:
                    return new CylinderCollisionShape(OneNode);
                case CollisionShape::ST_MultiSphere:
                    return new MultiSphereCollisionShape(OneNode);
                case CollisionShape::ST_Sphere:
                    return new SphereCollisionShape(OneNode);
                case CollisionShape::ST_DynamicTriMesh:     /// @todo Complete CreateShape function once DynamicMeshCollisionShape can be deserialized
                    //return new DynamicMeshCollisionShape(Name_,(btGImpactMeshShape*)ShapeToModel);
                    { MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"Attempting to convert an unsupported/unwrapped Collision Shape type into a CollisionShape instance, specifically a DynamicMeshCollisionShape."); }
                case CollisionShape::ST_Heightfield:
                    return new HeightfieldCollisionShape(Name_);
                case CollisionShape::ST_Plane:
                    return new PlaneCollisionShape(Name_);
                case CollisionShape::ST_Soft:          /// @todo Complete CreateShape function once SoftCollisionShape can be deserialized
                    //return new SoftCollisionShape(Name_,(btSoftBodyCollisionShape*)ShapeToModel);
                    { MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"Attempting to convert an unsupported/unwrapped Collision Shape type into a CollisionShape instance, specifically a SoftCollisionShape."); }
                case CollisionShape::ST_StaticTriMesh:      /// @todo Complete CreateShape function once StaticMeshCollisionShape can be deserialized
                    //return new StaticMeshCollisionShape(Name_,(btBvhTriangleMeshShape*)ShapeToModel);
                    { MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"Attempting to convert an unsupported/unwrapped Collision Shape type into a CollisionShape instance, specifically a StaticMeshShape."); }
                case CollisionShape::ST_Compound: // holy recursive batman
                    return new CompoundCollisionShape(Name_);
                default:
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to convert an unsupported/unwrapped Collision Shape type into a CollisionShape instance."); }
            }
        }

        CollisionShape::ShapeType InternalShapeTypeToShapeType(int InternalShapeType)
        {
            switch(InternalShapeType)
            {
                case BOX_SHAPE_PROXYTYPE:           return CollisionShape::ST_Box;
                case CAPSULE_SHAPE_PROXYTYPE:       return CollisionShape::ST_Capsule;
                case CONE_SHAPE_PROXYTYPE:          return CollisionShape::ST_Cone;
                case CONVEX_HULL_SHAPE_PROXYTYPE:   return CollisionShape::ST_ConvexHull;
                case CYLINDER_SHAPE_PROXYTYPE:      return CollisionShape::ST_Cylinder;
                case MULTI_SPHERE_SHAPE_PROXYTYPE:  return CollisionShape::ST_MultiSphere;
                case SPHERE_SHAPE_PROXYTYPE:        return CollisionShape::ST_Sphere;
                case GIMPACT_SHAPE_PROXYTYPE:       return CollisionShape::ST_DynamicTriMesh;
                case TERRAIN_SHAPE_PROXYTYPE:       return CollisionShape::ST_Heightfield;
                case STATIC_PLANE_PROXYTYPE:        return CollisionShape::ST_Plane;
                case SOFTBODY_SHAPE_PROXYTYPE:      return CollisionShape::ST_Soft;
                case TRIANGLE_MESH_SHAPE_PROXYTYPE: return CollisionShape::ST_StaticTriMesh;
                case COMPOUND_SHAPE_PROXYTYPE:      return CollisionShape::ST_Compound;
                default:
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to convert an unsupported/unwrapped Internal Collision Shape type into a Physics::CollisionShapeShapeType."); }
            }
        }

        String ShapeTypeToString(CollisionShape::ShapeType ShapeToConvert)
        {
            switch(ShapeToConvert)
            {
                case CollisionShape::ST_Box:            return String("BoxCollisionShape");
                case CollisionShape::ST_Capsule:        return String("CapsuleCollisionShape");
                case CollisionShape::ST_Cone:           return String("ConeCollisionShape");
                case CollisionShape::ST_ConvexHull:     return String("ConvexHullCollisionShape");
                case CollisionShape::ST_Cylinder:       return String("CylinderCollisionShape");
                case CollisionShape::ST_MultiSphere:    return String("MultiSphereCollisionShape");
                case CollisionShape::ST_Sphere:         return String("SphereCollisionShape");
                case CollisionShape::ST_DynamicTriMesh: return String("DynamicMeshCollisionShape");
                case CollisionShape::ST_Heightfield:    return String("HeightfieldCollisionShapeString(");
                case CollisionShape::ST_Plane:          return String("PlaneCollisionShape");
                case CollisionShape::ST_Soft:           return String("SoftCollisionShape");
                case CollisionShape::ST_StaticTriMesh:  return String("StaticMeshCollisionShape");
                case CollisionShape::ST_Compound:       return String("CompoundCollisionShape");
                default:
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to convert an unsupported/unwrapped Collision Shape type into a String."); }
            }
        }

        CollisionShape::ShapeType StringToShapeType(const String& TypeName)
        {
            if(TypeName.size()<5)
                { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to convert a CollisionShape::ShapeType String into a CollisionShape::ShapeType which is too short to be valid."); }
            switch(TypeName.at(3))
            {
                case 'C':
                    if (String("BoxCollisionShape")==TypeName)
                        { return CollisionShape::ST_Box; }
                    else
                        { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'C'."); }
                case 's':
                    if (String("CapsuleCollisionShape")==TypeName)
                        { return CollisionShape::ST_Capsule; }
                    else
                        { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 's'."); }
                case 'e':
                    if (String("SphereCollisionShape")==TypeName)
                        { return CollisionShape::ST_Sphere; }
                    else if (String("ConeCollisionShape")==TypeName)
                        { return CollisionShape::ST_Cone; }
                    else
                        { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'e'."); }
                case 'v':
                    if (String("ConvexHullCollisionShape")==TypeName)
                        { return CollisionShape::ST_ConvexHull; }
                    else
                        { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'v'."); }
                case 'i':
                    if (String("CylinderCollisionShape")==TypeName)
                        { return CollisionShape::ST_Cylinder; }
                    else
                        { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'i'."); }
                case 't':
                    if (String("StaticMeshCollisionShape")==TypeName)
                        { return CollisionShape::ST_StaticTriMesh; }
                    else if (String("MultiSphereCollisionShape")==TypeName)
                        { return CollisionShape::ST_MultiSphere; }
                    else
                        { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 't'."); }
                case 'a':
                    if (String("DynamicMeshCollisionShape")==TypeName)
                        { return CollisionShape::ST_DynamicTriMesh; }
                    else
                        { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'a'."); }
                case 'g':
                    if (String("HeightfieldCollisionShapeString")==TypeName)
                        { return CollisionShape::ST_Heightfield; }
                    else
                        { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'g'."); }
                case 'n':
                    if (String("PlaneCollisionShape")==TypeName)
                        { return CollisionShape::ST_Plane; }
                    else
                        { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'n'."); }
                case 'o':
                    if (String("SoftCollisionShape")==TypeName)
                        { return CollisionShape::ST_Soft; }
                    else
                        { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'o'."); }
                case 'p':
                    if (String("CompoundCollisionShape")==TypeName)
                        { return CollisionShape::ST_Compound; }
                    else
                        { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'p'."); }
                default:
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType."); }
            }
        }

        ///////////////////////////////////////
        // CollisionShapeDeSerializer
        CollisionShape* CollisionShapeDeSerializer::PerformDeSerialization(std::istream& Stream)
        {
            XML::Document* Doc = new XML::Document();
            String OneTag( Mezzanine::XML::GetOneTag(Stream) );
            if(!Doc->Load(OneTag.c_str()))
            {
                delete Doc;
                MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Could not Deserialize XML Stream which should contain a Collision Shape, XML looked Like: " + OneTag + ".");
            }

            CollisionShape* Results = ProtoDeSerialize(Doc->GetFirstChild());
            delete Doc;
            return Results;
        }

        CollisionShape* CollisionShapeDeSerializer::ProtoDeSerialize(const XML::Node& OneNode)
            { return CreateShape(OneNode); }

        std::istream& CollisionShapeDeSerializer::DeSerialize(std::istream& Stream)
        {
            PerformDeSerialization(Stream);
            return Stream;
        }

        CollisionShape* CollisionShapeDeSerializer::DeSerializeAndRetrieve(std::istream& Stream)
            { return PerformDeSerialization(Stream); }

        String CollisionShapeDeSerializer::ContainerName() const
            { return String("Shapes"); }

    }//Physics
}//Mezzanine

std::ostream& operator << (std::ostream& stream, const Mezzanine::Physics::CollisionShape& ShapeToSerialize)
    { Mezzanine::Serialize(stream, ShapeToSerialize); return stream; }

std::istream& operator >> (std::istream& stream, Mezzanine::Physics::CollisionShape& x)
    { return Mezzanine::DeSerialize(stream, x); }

void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::CollisionShape& x)
    { x.ProtoDeSerialize(OneNode); }


#endif
