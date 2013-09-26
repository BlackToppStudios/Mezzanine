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
#ifndef _graphicsrenderableproxy_h
#define _graphicsrenderableproxy_h

/// @file
/// @brief This file contains the declaration for the base class from which graphics proxies inherit.

#include "worldproxy.h"
#include "axisalignedbox.h"

namespace Ogre
{
    class MovableObject;
}

namespace Mezzanine
{
    namespace Graphics
    {
        class SceneManager;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the base proxy class for world proxies wrapping functionality of the graphics subsystem.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB RenderableProxy : public WorldProxy
        {
        protected:
            /// @internal
            /// @brief This is a pointer to the scene manager that created and owns this proxy.
            SceneManager* Manager;
        public:
            /// @brief Class constructor.
            /// @param Creator A pointer to the manager that created this proxy.
            RenderableProxy(SceneManager* Creator);
            /// @brief Class destructor.
            virtual ~RenderableProxy();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets this proxies AABB.
            /// @note This will only return valid values if this proxy is in the world.  A proxy outside of the world has no AABB.
            /// @return Returns an AxisAlignedBox containing the AABB of this graphics proxy.
            virtual AxisAlignedBox GetAABB() const;

            /// @copydoc WorldProxy::AddToWorld()
            virtual void AddToWorld() = 0;
            /// @copydoc WorldProxy::RemoveFromWorld()
            virtual void RemoveFromWorld() = 0;
            /// @copydoc WorldProxy::IsInWorld() const
            virtual Bool IsInWorld() const = 0;

            /// @copydoc PhysicsProxy::GetCreator() const
            virtual WorldManager* GetCreator() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Graphics Properties

            /// @brief Sets whether or not this proxy is visible.
            /// @param Visible True to allow this proxy to render, false otherwise.
            virtual void SetVisible(const Bool Visible);
            /// @brief Gets whether or not this proxy is visible.
            /// @return Returns true if this proxy is being rendered, false otherwise.
            virtual Bool GetVisible() const;
            /// @brief Sets whether or not this proxy can cast a shadow.
            /// @param CastShadows True to allow this proxy to cast a shadow, false to prevent it from casting a shadow.
            virtual void SetCastShadows(const Bool CastShadows);
            /// @brief Gets whether or not this proxy can cast a shadow.
            /// @return Returns true if this proxy can cast a shadow, false otherwise.
            virtual Bool GetCastShadows() const;
            /// @brief Gets whether or not this proxy can be rendered with a shadow casted on it.
            /// @return Returns true if this proxy can receive shadows, false otherwise.
            virtual Bool GetReceiveShadows() const;
            /// @brief Sets which types of lights will affect this proxy.
            /// @param Mask A bitmask used to indicate which types of lights will be applied to this proxy.
            virtual void SetLightMask(const UInt32 Mask);
            /// @brief Gets which types of lights will affect this proxy.
            /// @return Returns a bitmask indicating the types of lights that will affect this proxies rendering.
            virtual UInt32 GetLightMask() const;

            /// @brief Sets the distance at which the proxy will stop rendering.
            /// @param Distance The distance in world units from the camera when the proxy will stop being rendered.
            virtual void SetRenderDistance(const Real Distance);
            /// @brief Gets the distance at which the proxy will stop rendering.
            /// @return Returns a Real representing the max distance from the camera at which the proxy will be rendered.
            virtual Real GetRenderDistance() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc WorldProxy::ProtoSerialize(XML::Node&) const
            virtual void ProtoSerialize(XML::Node& ParentNode) const;
            /// @brief Convert the properties of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;

            /// @copydoc WorldProxy::ProtoDeSerialize(const XML::Node&)
            virtual void ProtoDeSerialize(const XML::Node& SelfRoot);
            /// @brief Take the data stored in an XML Node and overwrite the properties of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @copydoc WorldProxy::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const;
            /// @copydoc WorldProxy::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Accessor for the internal graphics object.
            /// @return Returns a pointer to the internal object of this proxy.
            virtual Ogre::MovableObject* _GetBaseGraphicsObject() const = 0;
        };//RenderableProxy
    }//Graphics
}//Mezzanine

#endif
