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
#ifndef _graphicsbillboardsetproxy_h
#define _graphicsbillboardsetproxy_h

/// @file
/// @brief This file contains the declaration for the World proxy wrapping billboard functionality.

#include "Graphics/renderableproxy.h"
#include "Graphics/graphicsenumerations.h"
#include "colourvalue.h"

namespace Ogre
{
    class BillboardSet;
}

namespace Mezzanine
{
    namespace Graphics
    {
        class Billboard;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the proxy class for placing and manipulating a set of 2D billboards in the scene.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB BillboardSetProxy : public RenderableProxy
        {
        public:
            /// @brief Basic container type for Billboard storage by this class.
            typedef std::list<Billboard*>                BillboardContainer;
            /// @brief Iterator type for Billboard instances stored by this class.
            typedef BillboardContainer::iterator         BillboardIterator;
            /// @brief Const Iterator type for Billboard instances stored by this class.
            typedef BillboardContainer::const_iterator   ConstBillboardIterator;
        protected:
            /// @internal
            /// @brief Container storing all of the Billboard instances created by this manager.
            BillboardContainer Billboards;
            /// @internal
            /// @brief A pointer to the internal BillboardSet this proxy is based on.
            Ogre::BillboardSet* GraphicsBillboardSet;

            /// @internal
            /// @param InitialPoolSize The number of billboards to reserve space for.
            /// @brief Creates an internal BillboardSet to be used by the calling instance.
            virtual void CreateBillboardSet(const UInt32 InitialPoolSize);
            /// @internal
            /// @brief Destroys the internal BillboardSet in use by this proxy.
            virtual void DestroyBillboardSet();
        public:
            /// @brief Class constructor.
            /// @param InitialPoolSize The number of billboards to reserve space for.
            /// @param Creator A pointer to the manager that created this proxy.
            BillboardSetProxy(const UInt32 InitialPoolSize, SceneManager* Creator);
            /// @brief XML constructor.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            /// @param Creator A pointer to the manager that created this proxy.
            BillboardSetProxy(const XML::Node& SelfRoot, SceneManager* Creator);
            /// @brief Class destructor.
            virtual ~BillboardSetProxy();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc WorldProxy::GetProxyType() const
            virtual Mezzanine::ProxyType GetProxyType() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Billboard Management

            /// @brief Creates a new Billboard for this set.
            /// @note In most situations you will want the colour to stay white.  The material will still render normally without modification if it is left white.
            /// @param Location The position relative to the center of the set where this billboard will be placed.
            /// @param Colour The base colour to give to the created billboard.
            /// @return Returns a pointer to the created billboard, or NULL if this set has reached it's max billboards and autoextend is disabled.
            virtual Billboard* CreateBillboard(const Vector3& Location, const ColourValue& Colour = ColourValue::White());
            /// @brief Gets a Billboard instance by index.
            /// @param Index The index of the Billboard to be retrieved.
            /// @return Returns a pointer to the Billboard at the specified index.
            virtual Billboard* GetBillboard(UInt32 Index);
            /// @brief Gets the number of Billboard instances in this set.
            /// @return Returns a UInt32 representing the number of Billboard instances contained in this set.
            virtual UInt32 GetNumBillboards() const;
            /// @brief Deletes a Billboard.
            /// @param ToBeDestroyed A pointer to the Billboard you want deleted.
            virtual void DestroyBillboard(Billboard* ToBeDestroyed);
            /// @brief Deletes all stored Billboard instances.
            virtual void DestroyAllBillboards();

            ///////////////////////////////////////////////////////////////////////////////
            // BillboardSet Properties

            /// @brief Sets whether or not this BillboardSetProxy will increase the size of it's billboard pool automatically when it's limit is reached.
            /// @param AutoExtend A bool indicating whether or not this proxy is allowed to resize it's own billboard pool.
            virtual void SetAutoExtend(const Bool AutoExtend);
            /// @brief Gets whether or not this BillboardSetProxy will increase the size of it's billboard pool automatically when it's limit is reached.
            /// @return Returns true if the billboard pool will automatically be resized when it reaches it's limit, false otherwise.
            virtual Bool GetAutoExtend() const;
            /// @brief Sets whether direction vectors will be calculated using a more expensive but more accurate algorithm.
            /// @param AccFace True to increase accuracy of billboard directions, false for the more efficient method.
            virtual void SetAccurateFacing(const Bool AccFace);
            /// @brief Gets whether direction vectors are being calculated using a more expensive but more accurate algorithm.
            /// @return Returns true if this BillboardSetProxy is using the more accurate and more expensive method for directional vectors, false otherwise.
            virtual Bool GetAccurateFacing() const;
            /// @brief Sets a new size of the billboard pool in this set.
            /// @param Size The size the billboard pool in this set should be resized to.
            virtual void SetPoolSize(const UInt32 Size);
            /// @brief Gets the current size of the billboard pool in this set.
            /// @return Returns a UInt32 representing the current size of the billboard pool in this set.
            virtual UInt32 GetPoolSize() const;

            /// @brief Sets the part of the billboard that will be treated as the origin for all children of this set.
            /// @param Origin A BillboardOrigin enum value expressing which part of the billboard will be considered the origin for all billboards in this set.
            virtual void SetBillboardOrigin(const Graphics::BillboardOrigin Origin);
            /// @brief Gets the part of the billboard that is currently being treated as the origin for all children of this set.
            /// @return Returns a BillboardOrigin enum value expressing which part of the billboard is being treated as the origin.
            virtual Graphics::BillboardOrigin GetBillboardOrigin() const;
            /// @brief Sets the type of rotation to apply when a rotation is made to a billboard.
            /// @param Rotation A BillboardRotation enum value expressing the underlying method for rotation in billboards belonging to this set.
            virtual void SetBillboardRotation(const Graphics::BillboardRotation Rotation);
            /// @brief Gets the type of rotation to apply when a rotation is made to a billboard.
            /// @return Returns a BillboardRotation enum value expressing how billboards in this set are to be rotated.
            virtual Graphics::BillboardRotation GetBillboardRotation() const;
            /// @brief Sets the type of BillboardSetProxy this is.
            /// @param Type A BillboardType enum value expressing how billboards in this set are to be oriented.
            virtual void SetBillboardType(const Graphics::BillboardType Type);
            /// @brief Gets the type of BillboardSetProxy this is.
            /// @return Returns a BillboardType enum value expressing how billboards in this set are being oriented.
            virtual Graphics::BillboardType GetBillboardType() const;

            /// @brief Sets the default width and height all billboards in this set are to be configured with when constructed.
            /// @param Width The default width in world units.
            /// @param Height The default height in world units.
            virtual void SetDefaultDimensions(const Real Width, const Real Height);
            /// @brief Sets the default width all billboards in this set are to be configured with when constructed.
            /// @param Width The default width in world units.
            virtual void SetDefaultWidth(const Real Width);
            /// @brief Gets the default width all billboards in this set are constructed with.
            /// @return Returns a Real representing the default width of all billboards in this set in world units.
            virtual Real GetDefaultWidth() const;
            /// @brief Sets the default height all billboards in this set are to be configured with when constructed.
            /// @param Height The default height in world units.
            virtual void SetDefaultHeight(const Real Height);
            /// @brief Gets the default height all billboards in this set are constructed with.
            /// @return Returns a Real representing the default height of all billboards in this set in world units.
            virtual Real GetDefaultHeight() const;

            /// @brief Sets the common facing direction for all billboards in this set.
            /// @remarks This only needs to be set if this billboard type is set to "BBT_Oriented_Common" or "BBT_Perpendicular_Common".
            /// @param Dir The facing axis for all billboards in this set.
            virtual void SetCommonDirection(const Vector3& Dir);
            /// @brief Gets the common facing direction for all billboards in this set.
            /// @return Returns a Vector3 representing the forward axis of billboards in this set.
            virtual Vector3 GetCommonDirection() const;
            /// @brief Sets the common up direction for all billboards in this set.
            /// @remarks This only needs to be set if this billboard type is set to "BBT_Perpendicular_Common" or "BBT_Perpendicular_Self".
            /// @param UpDir The up axis for all billboards in this set.
            virtual void SetCommonUpVector(const Vector3& UpDir);
            /// @brief Gets the common up direction for all billboards in this set.
            /// @return Returns a Vector3 representing the up axis of billboards in this set.
            virtual Vector3 GetCommonUpVector() const;

            /// @brief Sets a material for this set by name.
            /// @todo These by-name methods should be removed when we have a working material wrapper.
            /// @param MatName The name of the material to apply.
            virtual void SetMaterialName(const String& MatName);
            /// @brief Gets the name of the material being used by this set.
            /// @todo These by-name methods should be removed when we have a working material wrapper.
            /// @return Returns a string containing the name of the material being used to render this BillboardSetProxy.
            virtual String GetMaterialName() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc WorldProxy::ProtoSerialize(XML::Node&) const
            virtual void ProtoSerialize(XML::Node& ParentNode) const;
            /// @copydoc WorldProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @brief Convert the Billboards of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeBillboards(XML::Node& SelfRoot) const;

            /// @copydoc WorldProxy::ProtoDeSerialize(const XML::Node&)
            virtual void ProtoDeSerialize(const XML::Node& SelfRoot);
            /// @copydoc WorldProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);
            /// @brief Take the data stored in an XML Node and overwrite the Billboards of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeBillboards(const XML::Node& SelfRoot);

            /// @copydoc WorldProxy::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const;
            /// @copydoc WorldProxy::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Accessor for the internal billboard set.
            /// @return Returns a pointer to the internal billboard set this proxy is based on.
            virtual Ogre::BillboardSet* _GetGraphicsObject() const;
            /// @copydoc RenderableProxy::_GetBaseGraphicsObject() const
            virtual Ogre::MovableObject* _GetBaseGraphicsObject() const;
        };//BillboardSetProxy
    }//Graphics
}//Mezzanine

#endif
