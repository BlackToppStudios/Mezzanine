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
#ifndef _rayquerytool_cpp
#define _rayquerytool_cpp

using namespace std;

#include "rayquerytool.h"
#include "actorbase.h"
#include "actormanager.h"
#include "Graphics/graphicsmanager.h"
#include "eventmanager.h"
#include "entresol.h"
#include "cameramanager.h"
#include "scenemanager.h"
#include "Input/inputmanager.h"
#include "camera.h"
#include "Graphics/gamewindow.h"
#include "Graphics/viewport.h"
#include "plane.h"
#include "ray.h"
#include "vector3wactor.h"
#include "Input/mouse.h"

#include <Ogre.h>

#include <exception>

#ifdef GetObject
#undef GetObject
#endif

namespace Mezzanine
{
    Ogre::RaySceneQuery* RayQueryTool::RayQuery = 0;

    RayQueryTool::RayQueryTool()
    {
    }

    RayQueryTool::~RayQueryTool()
    {
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Raycasting Nonsense goe here
    Vector3WActor* RayQueryTool::GetFirstActorOnRayByPolygon(Ray ActorRay, Whole ObjectFlags)
    {
        VerifyRayQuery();
        Ogre::Ray Ooray = ActorRay.GetOgreRay();

        if(NULL != RayQuery)          //Double check that the Rayquery is valid
        {
            RayQuery->setRay(Ooray);
            //RayQuery->setSortByDistance(true);
            RayQuery->setQueryMask(-1);
            if( RayQuery->execute().size() <= 0 ) //Did we hit anything
            {
                return NULL;
            }
        }else{                          //Whoopsie something Failed
            MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to run a query on Null RaySceneQuery");
        }

        // at this point we have raycast to a series of different objects bounding boxes.
        // we need to test these different objects to see which is the first polygon hit.
        // there are some minor optimizations (distance based) that mean we wont have to
        // check all of the objects most of the time, but the worst case scenario is that
        // we need to test every triangle of every object.
        Ogre::Real closest_distance = -1.0f;
        Vector3 closest_result;
        Ogre::RaySceneQueryResult &query_result = RayQuery->getLastResults();
        Vector3WActor* ClosestActor = new Vector3WActor();
        for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
        {
            // stop checking if we have found a raycast hit that is closer than all remaining entities
            if ( (0.0f <= closest_distance) && (closest_distance < query_result[qr_idx].distance))
                { break; }

            // only check this result if its a hit against an entity
            if ((NULL != query_result[qr_idx].movable) && (0 == query_result[qr_idx].movable->getMovableType().compare("Entity")))
            {
                // get the entity to check
                Ogre::Entity *pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable);

                try
                {
                    WorldObject* HitMetaInfo = Ogre::any_cast<WorldObject*>(pentity->getUserAny());
                    if(HitMetaInfo->GetType() & ObjectFlags)
                    {
                        // mesh data to retrieve
                        size_t vertex_count;
                        size_t index_count;
                        Ogre::Vector3 *vertices;
                        unsigned long *indices;

                        // get the mesh information
                        GetMeshInformation(pentity, vertex_count, vertices, index_count, indices,
                                          pentity->getParentNode()->_getDerivedPosition(),
                                          pentity->getParentNode()->_getDerivedOrientation(),
                                          pentity->getParentNode()->_getDerivedScale());

                        // test for hitting individual triangles on the mesh
                        bool new_closest_found = false;
                        for (size_t i = 0; i < index_count; i += 3)
                        {
                            // check for a hit against this triangle
                            std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(Ooray, vertices[indices[i]], vertices[indices[i+1]], vertices[indices[i+2]], true, false);

                            // if it was a hit check if its the closest
                            if (hit.first && ((0.0f > closest_distance) || (hit.second < closest_distance)) )
                            {
                                    closest_distance = hit.second;                        // this is the closest so far, save it off
                                    new_closest_found = true;
                            }
                        }

                        // free the vertices and indices memory
                        delete[] vertices;
                        delete[] indices;

                        // if we found a new closest raycast for this object, update the closest_result before moving on to the next object.
                        if (new_closest_found)
                        {
                            closest_result = Ooray.getPoint(closest_distance);
                            WorldObject* WO = Ogre::any_cast<WorldObject*>(pentity->getUserAny());
                            ClosestActor->Actor = static_cast<ActorBase*>( WO );
                        }

                    } // \if WSO_ActorRigid
                }catch(...){
                    MEZZ_EXCEPTION(Exception::INTERNAL_EXCEPTION,"Failed during cast in actor raycast.");
                }
            } // \if entity
        } // \if qr_idx

        //Change the closest point into a point relative to the Actor
        if (ClosestActor->Actor != NULL)
            //{ ClosestActor->Vector = closest_result - ClosestActor->Actor->GetLocation(); }
            { ClosestActor->Vector = ClosestActor->Actor->GetOrientation() * ((closest_result - ClosestActor->Actor->GetLocation()) * ClosestActor->Actor->GetScaling()); }
        return ClosestActor;
    }

    Vector3WActor* RayQueryTool::GetFirstActorOnRayByAABB(Ray ActorRay, Whole ObjectFlags)
    {
        VerifyRayQuery();
        Ogre::Ray Ooray = ActorRay.GetOgreRay();

        if(NULL != RayQuery)          //Double check that the Rayquery is valid
        {
            RayQuery->setRay(Ooray);
            if( RayQuery->execute().size() <= 0 ) //Did we hit anything
            {
                return NULL;
            }
        }else{                          //Whoopsie something Failed
            MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to run a query on Null RaySceneQuery.");
        }

        Ogre::RaySceneQueryResult &query_result = RayQuery->getLastResults();

        if (0 < query_result.size())
        {
            Ogre::Entity *pentity = static_cast<Ogre::Entity*>(query_result[0].movable);
            Vector3WActor* ClosestActor = new Vector3WActor();
            WorldObject* WO = Ogre::any_cast<WorldObject*>(pentity->getUserAny());
            ClosestActor->Actor = static_cast<ActorBase*>( WO );
            /// @todo TODO: The function WorldQueryTool::GetFirstActorOnRayByAABB does not return an valid offset. This needs to be calculated somehow.
            /// @todo TODO: The function WorldQueryTool::GetFirstActorOnRayByAABB has not been tested and needs to be tested
            /// @todo TODO: The function WorldQueryTool::GetFirstActorOnRayByAABB does not take other obstructions into account
            return ClosestActor;
        }else{
            return 0;
        }
    }

    Vector3WActor* RayQueryTool::GetActorUnderMouse(Whole ObjectFlags, Real RayLength, bool UsePolygon)
    {
        VerifyRayQuery();
        Vector3WActor* Results = 0;

        Ray* MouseRay = GetMouseRay(RayLength);

        if (UsePolygon)
        {
            Results = GetFirstActorOnRayByPolygon(*MouseRay,ObjectFlags);
        }else{
            Results = GetFirstActorOnRayByAABB(*MouseRay,ObjectFlags);
        }

        delete MouseRay;
        return Results;
    }

    Vector3* RayQueryTool::RayPlaneIntersection(const Ray &QueryRay, const Plane &QueryPlane)
    {
        VerifyRayQuery();
        try{
            Vector3 u = QueryRay.To - QueryRay.From;
            Vector3 p0 = Vector3(0,0,0);

            if(QueryPlane.Normal.X == 0 && QueryPlane.Normal.Y == 0 && QueryPlane.Normal.Z == 0)
            {
                return 0;
            }
            else{
                if(QueryPlane.Normal.X != 0)
                {
                     p0 = Vector3(QueryPlane.Distance,0,0);
                }
                else if(QueryPlane.Normal.Y != 0)
                {
                     p0 = Vector3(0,QueryPlane.Distance,0);
                }
                else
                {
                     p0 = Vector3(0,0,QueryPlane.Distance);
                }
            }

            Vector3 w = QueryRay.From - p0;

            Real D = u.DotProduct(QueryPlane.Normal);
            Real N = -1 * w.DotProduct(QueryPlane.Normal);

            Real SMALL_NUM = 0.00000001;

            if( (D<0? -D : D) < SMALL_NUM)  //Checks if the Plane behind the RAy
            {
                if(N == 0)
                {
                    return new Vector3(QueryRay.From);
                }
                else
                {
                    return 0;
                }
            }

            Real sI = N/D;

            if(sI < 0 || sI > 1) //checks if the ray is too long
            {
                return 0;
            }

            Vector3 test =  Vector3(QueryRay.From + (u * sI));

            Vector3* return_vector = new Vector3(QueryRay.From + (u * sI));

            Real distance = return_vector->Distance(QueryRay.From);

            if(distance > QueryRay.From.Distance(QueryRay.To))
            {
                return 0;
            }

            return return_vector;
        } catch(exception e) {
            //In case we divide b
            Entresol::GetSingletonPtr()->Log("WorldQueryTool Error:Failed while calculating Ray/Plane Intersection, Assuming no valid intersection. Error follows:");
            Entresol::GetSingletonPtr()->Log(e.what());
            return 0;
        }
    }

    Ray* RayQueryTool::GetMouseRay(Real Length)
    {
        VerifyRayQuery();
        Graphics::Viewport* HoveredViewport = Input::InputManager::GetSingletonPtr()->GetSystemMouse()->GetHoveredViewport();
        Vector2 MousePos = Input::InputManager::GetSingletonPtr()->GetSystemMouse()->GetViewportPosition();
        Ray* MouseRay = NULL;
        if(HoveredViewport)
        {
            MouseRay = new Ray( HoveredViewport->GetViewportCamera()->GetCameraToViewportRay(
                                MousePos.X / (Real)(HoveredViewport->GetActualWidth()),
                                MousePos.Y / (Real)(HoveredViewport->GetActualHeight()) ) );
            (*MouseRay) *= Length;
        }
        return MouseRay;
    }


    // Private Members
    void RayQueryTool::VerifyRayQuery()
    {
        if(!RayQuery)
        {
            RayQuery = SceneManager::GetSingletonPtr()->GetGraphicsWorldPointer()->createRayQuery(Ogre::Ray(), Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
            RayQuery->setSortByDistance(true);
        }
    }

    void RayQueryTool::GetMeshInformation( Ogre::Entity *entity,
                                size_t &vertex_count,
                                Ogre::Vector3* &vertices,
                                size_t &index_count,
                                unsigned long* &indices,
                                const Ogre::Vector3 &position,
                                const Ogre::Quaternion &orient,
                                const Ogre::Vector3 &scale)
    {
        bool added_shared = false;
        size_t current_offset = 0;
        size_t shared_offset = 0;
        size_t next_offset = 0;
        size_t index_offset = 0;
        vertex_count = index_count = 0;

        Ogre::MeshPtr mesh = entity->getMesh();


        bool useSoftwareBlendingVertices = entity->hasSkeleton();

        if (useSoftwareBlendingVertices)
        {                                   //10,000th line of code
            entity->_updateAnimation();
        }

        // Calculate how many vertices and indices we're going to need
        for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
        {
            Ogre::SubMesh* submesh = mesh->getSubMesh( i );

            // We only need to add the shared vertices once
            if(submesh->useSharedVertices)
            {
                if( !added_shared )
                {
                    vertex_count += mesh->sharedVertexData->vertexCount;
                    added_shared = true;
                }
            }
            else
            {
                vertex_count += submesh->vertexData->vertexCount;
            }

            // Add the indices
            index_count += submesh->indexData->indexCount;
        }


        // Allocate space for the vertices and indices
        vertices = new Ogre::Vector3[vertex_count];
        indices = new unsigned long[index_count];

        added_shared = false;

        // Run through the submeshes again, adding the data into the arrays
        for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
        {
            Ogre::SubMesh* submesh = mesh->getSubMesh(i);

            //Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
            Ogre::VertexData* vertex_data;

            //When there is animation:
            if(useSoftwareBlendingVertices)
            {
                //    #ifdef BUILD_AGAINST_AZATHOTH
                //             vertex_data = submesh->useSharedVertices ? entity->_getSharedBlendedVertexData() : entity->getSubEntity(i)->_getBlendedVertexData();
                //    #else
                vertex_data = submesh->useSharedVertices ? entity->_getSkelAnimVertexData() : entity->getSubEntity(i)->_getSkelAnimVertexData();
                //    #endif
            }else{
                vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
            }

            if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
            {
                if(submesh->useSharedVertices)
                {
                    added_shared = true;
                    shared_offset = current_offset;
                }

                const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
                Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
                unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

                // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
                //  as second argument. So make it float, to avoid trouble when Ogre::Real will
                //  be comiled/typedefed as double:
                //      Ogre::Real* pReal;
                float* pReal;

                for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
                {
                    posElem->baseVertexPointerToElement(vertex, &pReal);
                    Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
                    vertices[current_offset + j] = (orient * (pt * scale)) + position;
                }

                vbuf->unlock();
                next_offset += vertex_data->vertexCount;
            }


            Ogre::IndexData* index_data = submesh->indexData;
            size_t numTris = index_data->indexCount / 3;
            Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

            bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

            unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
            unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);


            size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;
            size_t index_start = index_data->indexStart;
            size_t last_index = numTris*3 + index_start;

            if (use32bitindexes)
            {
                for (size_t k = index_start; k < last_index; ++k)
                    { indices[index_offset++] = pLong[k] + static_cast<unsigned long>( offset ); }
            }else{
                for (size_t k = index_start; k < last_index; ++k)
                    { indices[ index_offset++ ] = static_cast<unsigned long>( pShort[k] ) + static_cast<unsigned long>( offset ); }
            }

            ibuf->unlock();
            current_offset = next_offset;
        }
    }
}


#endif
