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
#ifndef _rayquerytool_cpp
#define _rayquerytool_cpp

using namespace std;

#include "rayquerytool.h"
#include "actor.h"
#include "actormanager.h"
#include "areaeffect.h"
#include "areaeffectmanager.h"
#include "debris.h"
#include "debrismanager.h"
#include "eventmanager.h"
#include "worldobject.h"
#include "plane.h"
#include "ray.h"
#include "MathTools/mathtools.h"
#include "managedptr.h"
#include "serialization.h"
#include "entresol.h"
#include "world.h"

#include "Graphics/graphicsmanager.h"
#include "Graphics/scenemanager.h"
#include "Graphics/cameraproxy.h"
#include "Graphics/gamewindow.h"
#include "Graphics/viewport.h"
#include "Graphics/renderableproxy.h"

#include "Input/mouse.h"
#include "Input/inputmanager.h"

#include "Internal/meshtools.h.cpp"

#include <Ogre.h>

#include <exception>

#ifdef GetObject
#undef GetObject
#endif

namespace Mezzanine
{

    namespace
    {
        /// @internal
        /// @brief Ogre demands the use of special functions to delete a Ogre::RaySceneQuery, this handles that with RAII
        class RayQueryHandle
        {
        public:
            /// @brief This will work with a raw pointer to a Ogre::RaySceneQuery to manage a Ogre::RaySceneQuery.
            typedef Ogre::RaySceneQuery* TargetPtrType;
            /// @brief This will manage a Ogre::RaySceneQuery
            typedef Ogre::RaySceneQuery TargetType;
            /// @brief The actual ogre object we want to use.
            Ogre::RaySceneQuery* RayQuery;
            /// @brief The world in which to perform the ray query.
            World* ParentWorld;

            /// @brief Class constructor.
            /// @param QueryWorld The world in which to perform the ray query.
            explicit RayQueryHandle(World* QueryWorld) :
                ParentWorld(QueryWorld)
                {  }

            /// @brief Create the ogre specific handle and sort items for raycasting.
            void Construct()
            {
                this->RayQuery = this->ParentWorld->GetSceneManager()->_GetGraphicsWorldPointer()->createRayQuery(Ogre::Ray(), Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
                this->RayQuery->setSortByDistance(true);
            }

            /// @brief CAll the Ogre API to clean up this wierd handle thing
            void Deconstruct()
            {
                if( this->GetPointer() )
                    { this->ParentWorld->GetSceneManager()->_GetGraphicsWorldPointer()->destroyQuery(this->RayQuery); }
            }

            /// @brief This is what ManagedPtr will use in copy and assignment operations as well as invaliding handles.
            /// @param Value The new value for the pointer. If NULL the only thing that the ManagedPtr will do to the handle is call its deconstruct method.
            void SetPointer(TargetPtrType Value)
                { this->RayQuery = Value; }

            /// @brief This is what the ManagedPtr with use for dereferencing.
            /// @return The pointer to the managed data. This is expected to return a value that resolves to false when used as a condition when invalid.
            TargetPtrType GetPointer()
                { return this->RayQuery; }
        };//RayQueryHandle

        typedef ManagedPtr<RayQueryHandle> ManagedRayQuery;

        /// @internal
        /// @brief Exact an Ogre::RayQuery with some default parameters and see if we hit anything
        /// @param RayQuery A ManagedRayQuery
        /// @param Ooray The Ray to follow and see if it hits something
        /// @return True if something is hit, false otherwise.
        Boole ExecuteQuery(ManagedRayQuery& RayQuery, Ogre::Ray& Ooray)
        {
            if( RayQuery ) {        //Double check that the Rayquery is valid
                RayQuery->setRay(Ooray);
                RayQuery->setQueryMask(-1); // GetFirstActorOnRayByAABB did not do this

                if( RayQuery->execute().size() <= 0 ) return false;//Did we hit anything
                else return true;
            }else{                          // Something Failed
                MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to run a query on Null RaySceneQuery");
            }
            return false;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // RayQueryTool Methods

    RayQueryTool::RayQueryTool() :
        IntersectedObject(NULL),
        ParentWorld(NULL),
        ValidResult(false)
        { this->ClearReturns(); }

    RayQueryTool::RayQueryTool(World* QueryWorld) :
        IntersectedObject(NULL),
        ParentWorld(QueryWorld),
        ValidResult(false)
        { this->ClearReturns(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void RayQueryTool::SetWorld(World* QueryWorld)
        { this->ParentWorld = QueryWorld; }

    ///////////////////////////////////////////////////////////////////////////////
    // World Ray Query Results

    Boole RayQueryTool::ClearReturns()
    {
        this->ValidResult = false;
        this->Offset = Vector3();
        this->IntersectedObject = NULL;
        return this->ValidResult;
    }

    Boole RayQueryTool::LastQueryResultsValid() const
        { return this->ValidResult; }

    Vector3 RayQueryTool::LastQueryResultsOffset() const
        { return this->Offset; }

    WorldObject* RayQueryTool::LastQueryResultsObjectPtr() const
        { return this->IntersectedObject; }

    ///////////////////////////////////////////////////////////////////////////////
    // Ray Queries

    Boole RayQueryTool::GetFirstObjectOnRayByPolygon(Ray ObjectRay, Whole ObjectFlags)
    {
        RayQueryHandle ray( this->ParentWorld );
        ManagedRayQuery RayQuery( ray );
        Ogre::Ray Ooray = ObjectRay.GetOgreRay();

        if(!ExecuteQuery(RayQuery, Ooray))
            { return ClearReturns(); }

        // at this point we have raycast to a series of different objects bounding boxes.
        // we need to test these different objects to see which is the first polygon hit.
        // there are some minor optimizations (distance based) that mean we wont have to
        // check all of the objects most of the time, but the worst case scenario is that
        // we need to test every triangle of every object.
        Ogre::Real closest_distance = -1.0f;
        Vector3 closest_result;
        IntersectedObject=NULL;
        Ogre::RaySceneQueryResult &query_result = RayQuery->getLastResults();
        for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
        {
            // stop checking if we have found a raycast hit that is closer than all remaining entities
            if( (0.0f <= closest_distance) && (closest_distance < query_result[qr_idx].distance) )
                { break; }

            // only check this result if its a hit against an entity
            /// @todo Modify this so we can check for more movable types than just entities.
            if( (NULL != query_result[qr_idx].movable) && (0 == query_result[qr_idx].movable->getMovableType().compare("Entity")) ) {
                // get the entity to check
                Ogre::Entity *pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable);

                try
                {
                    WorldObject* HitMetaInfo = Ogre::any_cast<Graphics::RenderableProxy*>(pentity->getUserAny())->GetParentObject();
                    if( HitMetaInfo && ( HitMetaInfo->GetType() & ObjectFlags ) ) {
                        // Data containers to be populated
                        Internal::MeshTools::Vector3Vec vertices;
                        Internal::MeshTools::IntVec indices;

                        // get the mesh information
                        Internal::MeshTools::GetTransformedMeshData( pentity, vertices, indices );

                        // test for hitting individual triangles on the mesh
                        Boole new_closest_found = false;
                        for( size_t i = 0 ; i < indices.size() ; i += 3 )
                        {
                            // check for a hit against this triangle
                            std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(Ooray, vertices[indices[i]], vertices[indices[i+1]], vertices[indices[i+2]], true, false);

                            // if it was a hit check if its the closest
                            if( hit.first && ( (0.0f > closest_distance) || (hit.second < closest_distance) ) ) {
                                    closest_distance = hit.second;                        // this is the closest so far, save it off
                                    new_closest_found = true;
                            }
                        }

                        // if we found a new closest raycast for this object, update the closest_result before moving on to the next object.
                        if( new_closest_found ) {
                            closest_result = Ooray.getPoint(closest_distance);
                            IntersectedObject = Ogre::any_cast<Graphics::RenderableProxy*>(pentity->getUserAny())->GetParentObject();
                        }

                    } // \if WSO_ActorRigid
                }catch(...){
                    ClearReturns();
                    MEZZ_EXCEPTION(Exception::INTERNAL_EXCEPTION,"Failed during cast in object raycast.");
                }
            } // \if entity
        } // \if qr_idx

        // Change the closest point into a point relative to the Actor
        if (IntersectedObject) {
            Offset = IntersectedObject->GetOrientation() * ((closest_result - IntersectedObject->GetLocation()) * IntersectedObject->GetScale());
            ValidResult=true;
            return ValidResult;
        }else{
            return ClearReturns();
        }
    }

    Boole RayQueryTool::GetFirstObjectOnRayByAABB(Ray ObjectRay, Whole ObjectFlags)
    {
        RayQueryHandle ray( this->ParentWorld );
        ManagedRayQuery RayQuery( ray );
        Ogre::Ray Ooray = ObjectRay.GetOgreRay();
        if(!ExecuteQuery(RayQuery, Ooray))
            { return ClearReturns(); }

        Ogre::RaySceneQueryResult &query_result = RayQuery->getLastResults();

        if (0 < query_result.size())
        {
            Ogre::Entity *pentity = static_cast<Ogre::Entity*>(query_result[0].movable);
            IntersectedObject = Ogre::any_cast<Graphics::RenderableProxy*>(pentity->getUserAny())->GetParentObject();
            Offset = Vector3();
            ValidResult = true;
            /// @todo TODO: The function WorldQueryTool::GetFirstActorOnRayByAABB does not return an valid offset. This needs to be calculated somehow.
            /// @todo TODO: The function WorldQueryTool::GetFirstActorOnRayByAABB has not been tested and needs to be tested
            /// @todo TODO: The function WorldQueryTool::GetFirstActorOnRayByAABB does not take other obstructions into account
            return ValidResult;
        }else{
            return ClearReturns();
        }
    }

    Boole RayQueryTool::RayPlaneIntersection(const Ray& QueryRay, const Plane& QueryPlane)
    {
        MathTools::Point3DTestResult Result = MathTools::Intersects(QueryPlane,QueryRay);
        this->Offset = Result.second;
        this->IntersectedObject = NULL;
        return Result.first;
        /*try{
            Vector3 u = QueryRay.Destination - QueryRay.Origin;
            Vector3 p0 = Vector3(0,0,0);

            if(QueryPlane.Normal.X == 0 && QueryPlane.Normal.Y == 0 && QueryPlane.Normal.Z == 0)
                { return ClearReturns(); }
            else
            {
                if(QueryPlane.Normal.X != 0)
                    { p0 = Vector3(QueryPlane.Distance,0,0); }
                else if(QueryPlane.Normal.Y != 0)
                    { p0 = Vector3(0,QueryPlane.Distance,0); }
                else
                    { p0 = Vector3(0,0,QueryPlane.Distance); }
            }

            Vector3 w = QueryRay.Origin - p0;

            Real D = u.DotProduct(QueryPlane.Normal);
            Real N = -1 * w.DotProduct(QueryPlane.Normal);

            Real SMALL_NUM = 0.00000001;

            if( (D<0? -D : D) < SMALL_NUM)  // Checks if the Plane behind the RAy
            {
                if(N == 0)
                {
                    Offset=QueryRay.Origin;
                    IntersectedObject=NULL;
                    ValidResult=true;
                    return ValidResult;
                }
                else
                    { return ClearReturns(); }
            }

            Real sI = N/D;

            if(sI < 0 || sI > 1) // Checks if the ray is too long
                { return ClearReturns(); }

            Vector3 return_vector(QueryRay.Origin + (u * sI));

            Real distance = return_vector.Distance(QueryRay.Origin);

            if(distance > QueryRay.Origin.Distance(QueryRay.Destination))
                { return ClearReturns(); }

            Offset=return_vector;
            IntersectedObject=NULL;
            ValidResult=true;
            return ValidResult;
        } catch(exception e) {
            //In case we divide b
            Entresol::GetSingletonPtr()->Log("WorldQueryTool Error:Failed while calculating Ray/Plane Intersection, Assuming no valid intersection. Error follows:");
            Entresol::GetSingletonPtr()->Log(e.what());
            return ClearReturns();
        }//*/
    }

    Ray RayQueryTool::GetMouseRay()
    {
        Graphics::Viewport* HoveredViewport = Input::InputManager::GetSingletonPtr()->GetSystemMouse()->GetHoveredViewport();
        Vector2 MousePos = Input::InputManager::GetSingletonPtr()->GetSystemMouse()->GetViewportPosition();
        Ray MouseRay;
        if(HoveredViewport) {
            MouseRay = Ray( HoveredViewport->GetCamera()->GetCameraToViewportRay(
                            MousePos.X / (Real)(HoveredViewport->GetActualWidth()),
                            MousePos.Y / (Real)(HoveredViewport->GetActualHeight()) ) );
        }
        return MouseRay;
    }

    void RayQueryTool::ProtoSerialize(XML::Node& CurrentRoot) const
    {
        Mezzanine::XML::Node RayQueryToolNode = CurrentRoot.AppendChild(SerializableName());
        RayQueryToolNode.SetName(SerializableName());

        Mezzanine::XML::Attribute VersionAttr = RayQueryToolNode.AppendAttribute("Version");
        if( VersionAttr && VersionAttr.SetValue("1"))
        {
            Mezzanine::XML::Attribute ResultAttr = RayQueryToolNode.AppendAttribute("ValidResult");
            if( ResultAttr && ResultAttr.SetValue(ValidResult))
                {}
            else
                { SerializeError("Create XML Attribute for ValidResult", SerializableName(), true); }

            Mezzanine::XML::Node OffsetNode = RayQueryToolNode.AppendChild("Offset");
            if( OffsetNode )
                { Offset.ProtoSerialize(OffsetNode); }
            else
                { SerializeError("Create XML Node for Offset", SerializableName(), true); }

            Mezzanine::XML::Attribute WorldObjectAttr = RayQueryToolNode.AppendAttribute("WorldObject");
            if( WorldObjectAttr )
            {
                if( IntersectedObject )
                {
                    if(WorldObjectAttr.SetValue(IntersectedObject->GetName().c_str()))
                        {}
                    else
                        { SerializeError("Create XML Node for Offset", SerializableName(),true); }
                }
            }
            else
                { SerializeError("Create XML Attribute for Offset", SerializableName(),true); }
        }else{
            SerializeError("Create XML Version Attibute", SerializableName(),true);
        }
    }

    void RayQueryTool::ProtoDeSerialize(const XML::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==Mezzanine::String(SerializableName()) )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                ValidResult=OneNode.GetAttribute("ValidResult").AsBool();

                XML::Node VecNode = OneNode.GetChild("Offset");
                if(!VecNode)
                    { DeSerializeError("Could not Deserialize Offset",SerializableName()); }
                Offset.ProtoDeSerialize(VecNode);

                String WorldObjectName(OneNode.GetAttribute("WorldObject").AsString());
                if (WorldObjectName.size()) {
                    /// @todo This is temporary code that should be replaced with something more robust to find the proper world object.
                    IntersectedObject = this->ParentWorld->GetDebrisManager()->GetDebris(WorldObjectName);
                    if( IntersectedObject == NULL ) {
                        IntersectedObject = this->ParentWorld->GetActorManager()->GetActor(WorldObjectName);
                    }
                    if( IntersectedObject == NULL ) {
                        IntersectedObject = this->ParentWorld->GetAreaEffectManager()->GetAreaEffect(WorldObjectName);
                    }
                }else{
                    IntersectedObject = NULL;
                }
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + SerializableName() + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(Exception::II_IDENTITY_INVALID_EXCEPTION,"Attempting to deserialize a " + SerializableName() + ", found a " + String(OneNode.Name()) + ".");
        }
    }

    Mezzanine::String Mezzanine::RayQueryTool::SerializableName()
        { return "RayQueryTool"; }
}

#endif
