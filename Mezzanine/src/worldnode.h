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
#ifndef _worldnode_h
#define _worldnode_h

#include "attachable.h"
#include "crossplatformexport.h"
#include "datatypes.h"
#include "vector3.h"
#include "quaternion.h"
#include "xml.h"

/// @file worldnode.h
/// @brief The declaration of the WorldNode, A class to facilitate navigation in 3d environment

namespace Ogre
{
    class SceneNode;
}

namespace Mezzanine
{
    class SceneManager;
    class Camera;
    class Light;
    class ParticleEffect;
    class ActorBase;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class WorldNode
    /// @headerfile worldnode.h
    /// @brief This is a helper class which non-physics objects, such as lights, cameras, ribbon trails, and
    /// particle effects may be attached to for enhanced effects.
    /// @details This class simply contains a group of functions for easy manipulation of movement and orientation
    /// as well as attaching elements to them.  This class should only be created through the scene manager. @n
    /// There are 4 types of nodes: @n
    /// Center - Works in conjunction with the orbit node and serves as it's center point for the orbit node to
    /// rotate around. @n
    /// Orbit - Counterpart to the Center node, this type does the orbiting. @n
    /// Stand - Stationary node that points in one direction, that is it. @n
    /// Free - A node with no rules governing how it moves. @n
    /// Note:  There are no rules restricting the use of nodes anyway, this enum is here simply to help
    /// indicate the intended use upon creation.
    ///////////////////////////////////////
    class MEZZ_LIB WorldNode : public Attachable
    {
        public:
            /// @brief Kinds of movement (or non-movement modes) a world node could have
            enum NodeType
            {
                Free    = 0,     ///< Not bound by a any kind of movement
                Center  = 1,     ///< Designed to be be rotate to simulate orbits in a simple way
                Orbit   = 2,     ///< A node that orbits aroud a center
                Stand   = 3      ///< Stationary/Standing
            };
        protected:
            /// @brief Pointer to the ogre scenenode from which this class gets it's functionality.
            Ogre::SceneNode* OgreNode;
            /// @brief Pointer to the manager that created this class.
            SceneManager* Manager;
            /// @brief Enum value storing the type of node this class is.
            WorldNode::NodeType Type;
            /// @brief Vector storing all attached cameras, lights, etc... .
            std::vector< Attachable* > Elements;

        public:
            /// @brief Standard initialization constructor.
            /// @param Name The name of this node.
            /// @param manager Pointer to the manager that this node is to be used in.
            WorldNode(const String& Name, SceneManager* manager);
            /// @internal
            /// @brief Internal constructor.
            /// @details This constructor should not be called on manually.
            /// @param snode Pointer to the Ogre SceneNode this class is based on.
            /// @param manager Pointer to the manager that this node is to be used in.
            WorldNode(Ogre::SceneNode* snode, SceneManager* manager);
            /// @brief Class destructor.
            ~WorldNode();

            ///////////////////////////////////////////////////////////////////////////////
            // Navigation

            /// @brief Sets the Location of this node.
            /// @param Location A vector3 representing the location of this node.
            virtual void SetLocation(const Vector3& Location);
            /// @brief Gets the Location of this node.
            /// @return Returns a vector3 representing the location of this node.
            virtual Vector3 GetLocation() const;

            /// @brief Sets the orientation of this node.
            /// @param Orientation A Quaternion representing the orientation of this node.
            void SetOrientation(const Quaternion& Orientation);
            /// @brief Gets the orientation of this node.
            /// @return Returns a quaternion representing the orientation of this node.
            Quaternion GetOrientation() const;

            /// @brief Automatically sets the orientation needed to point this node at a location in the world.
            /// @param LookAt The location in world space to point at.
            void LookAt(Vector3 LookAt);

            /// @brief Increments the orbit of this node if this is an orbiting node.
            /// @details This function has no effect if this isn't an orbiting node.
            /// @param Radians A real reprsenting the amount of orbit to increment in radians.
            void IncrementOrbit(Real Radians);

            ///////////////////////////////////////////////////////////////////////////////
            // Auto tracking

            /// @brief Makes this node autotrack another node.
            /// @details This function will make this node update it's orientation every frame automatically so
            /// that it's always facing the target node.
            /// @param node The node to be tracked.
            /// @param Offset A vector3 representing the offset from the nodes location to be tracked.
            /// @todo TODO: The WorldNode Does not Correctly Serialize AutoTracking.
            void SetAutoTracking(WorldNode* node, Vector3 Offset=Vector3());
            /// @brief Makes this node autotrack an actor.
            /// @details This function will make this node update it's orientation every frame automatically so
            /// that it's always facing the target actor.
            /// @param Actor The actor to be tracked.
            /// @param Offset A vector3 representing the offset from the actor's location to be tracked.
            void SetAutoTracking(ActorBase* Actor, Vector3 Offset=Vector3());
            /// @brief Disables any active autotracking for this node.
            void DisableAutoTracking();

            ///////////////////////////////////////////////////////////////////////////////
            // Basic Data

            /// @internal
            /// @brief Sets the type of node that this is.
            /// @details This is intended for internal/expert use only.  Manually calling this could disrupt normal function.
            /// @param type The type of node this is to be set as.
            void SetType(WorldNode::NodeType type);

            /// @brief Gets the type of node that this is.
            /// @return Returns the type of node this is set as.
            WorldNode::NodeType GetType() const;

            /// @brief What kind of Attachable is this.
            /// @return An Attachable::GetAttachableType containing Attachable::WorldNode.
            virtual Attachable::AttachableElement GetAttachableType() const;

            /// @brief Gets the name of this node.
            /// @return Returns a string containing the name given to this node.
            ConstString& GetName() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Attachment child management

            /// @brief Attaches an attachable element to this Node.
            /// @param Target The Attachable to be attached.
            void AttachObject(Attachable* Target);

            /// @brief Detaches an attachable element from this Node.
            /// @param Target The Attachable to be detached.
            /// @details Detach an item is done in linear time relative to the amount of attached items
            void DetachObject(Attachable* Target);

            /// @brief Detaches all attached cameras, lights, particle effects and anything else attached.
            void DetachAll();

            /// @brief Gets the number of elements attached to this node.
            /// @return Returns the number of elements attached to this node.
            Whole GetNumAttached() const;

            /// @brief Get a specific attached Item
            /// @param Index A number indicating which Attachable you want a pointer to. The WorldNode is like an Array starts at 0 and goes to WorldNode::GetNumAttached() - 1.
            /// @return A pointer to an Attachable Item attached to this.
            /// @throw This can throw an out of bounds std::exception if used incorrectly
            Attachable* GetAttached(const Whole& Index) const;

            /// @brief Used to make working with the attached items easier
            typedef std::vector< Attachable* >::iterator iterator;

            /// @brief Used to make working with the attached items easier, and avoid the risk of accidentally changing them
            typedef std::vector< Attachable* >::const_iterator const_iterator;

            /// @brief Get an iterator to the first item
            /// @return An Iterator to the first
            iterator begin();

            /// @brief Get an iterator to one past the last item
            /// @return An Iterator to one past the last item
            iterator end();

            /// @brief Get an const_iterator to the first item
            /// @return An Iterator to the first
            const_iterator begin() const;

            /// @brief Get an const_iterator to one past the last item
            /// @return An Iterator to one past the last item
            const_iterator end() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Functions

            /// @internal
            /// @brief Gets pointers to the internal ogre structures for this attachable.
            /// @return Returns an AttachableData struct with the internal data.
            virtual AttachableData GetAttachableData() const;
    };//node
}//Mezzanine



///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef MEZZXML

/// @brief Serializes the passed Mezzanine::WorldNode to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the Mezzanine::WorldNode to be serialized
/// @return this returns the ostream, now with the serialized data
/// @warning This does not added to the WorldNode manager. When a WorldNode manager is serialized, this data is implicitly stored by the WorldNodes location in the xml hierarchy, this is used instead. The Name of the Node is stored for possible future use.
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::WorldNode& Ev);

/// @brief Deserialize a Mezzanine::WorldNode
/// @param stream The istream to get the xml from to (re)make the Mezzanine::WorldNode.
/// @param Ev the Mezzanine::WorldNode to be deserialized.
/// @return this returns the ostream, advanced past the Mezzanine::WorldNode that was recreated onto Ev.
/// @warning This does not attempt to store the WorldNode aspect ratio. This is too often hardware dependent and may not be reliably re-serialized.
/// @warning This does not the pointer to the WorldNode manager. When a WorldNode manager is serialized, this data is implicitly stored by the WorldNodes location in the xml hierarchy, this is used instead. The Name of the manager is stored for possible future use.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::WorldNode& Ev);

/// @brief Set all values of a Mezzanine::WorldNode from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the Mezzanine::WorldNode.
/// @param Ev the Mezzanine::WorldNode to be reset.
/// @return This returns thexml::Node that was passed in.
/// @warning This does not attempt to de-serialize the name of the WorldNode. This is not currently changeable after the creation of a WorldNode. However, the WorldNodemanager will correctly create name WorldNode upon creation then deserialize the rest of the WorldNode.
/// @warning This does not throw an exception if the WorldNode could not be attached to the appropriate worldnode. It is assumed that the worldnode will be able to adjust the pointer on this if it is deserialized second.
Mezzanine::xml::Node& MEZZ_LIB operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::WorldNode& Ev);

#endif // \MEZZXML

#endif
