// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
#ifndef _collisionshape_h
#define _collisionshape_h

#include "datatypes.h"
#ifndef SWIG
    #include "serialization.h"
    #include "transform.h"
#endif

class btCollisionShape;

namespace Mezzanine
{
    namespace Physics
    {
        class CollisionShapeManager;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class CollisionShape
        /// @headerfile collisionshape.h
        /// @brief This is the base class for all collision shapes.
        /// @details Currently there are a total of 13 collision shape classes inheriting from
        /// 3 other base classes.  Collision shapes are shape representations for Actors, AreaEffects,
        /// and other classes with bodies in the physics engine.  @n @n
        /// It's important to note that Collision shapes can be created and then re-used in as many World
        /// objects(at the same time) as you need, and it is encouraged to do this.
        ///////////////////////////////////////
        class MEZZ_LIB CollisionShape
        {
        public:
            /// @enum ShapeType
            /// @brief This enum describes what kind of shape you are currently working with.
            /// @note These are number primarily for Serialization purposes. These corresponding numbers could vary wildly. Any use of corresponding raw number in serialization will be done with object serialization version in mind.
            enum ShapeType
            {
                ST_Box = 0,               ///< Indicates the class is a BoxCollisionShape
                ST_Capsule = 1,           ///< Indicates the class is a CapsuleCollisionShape
                ST_Compound = 2,          ///< Indicates the class is a CompoundCollisionShape
                ST_Cone = 3,              ///< Indicates the class is a ConeCollisionShape
                ST_ConvexHull = 4,        ///< Indicates the class is a ConvexHullCollisionShape
                ST_Cylinder = 5,          ///< Indicates the class is a CylinderCollisionShape
                ST_MultiSphere = 6,       ///< Indicates the class is a MultiSphereCollisionShape
                ST_Sphere = 7,            ///< Indicates the class is a SphereCollisionShape
                ST_DynamicTriMesh = 8,    ///< Indicates the class is a DynamicMeshCollisionShape
                ST_Heightfield = 9,       ///< Indicates the class is a HeightfieldCollisionShape
                ST_Plane = 10,            ///< Indicates the class is a PlaneCollisionShape
                ST_Soft = 11,             ///< Indicates the class is a SoftCollisionShape
                ST_StaticTriMesh = 12     ///< Indicates the class is a StaticMeshCollisionShape
            };
        protected:
            /// @brief A pointer to the bullet collision this uses.
            btCollisionShape* ShapeBase;
            /// @brief Storage for the name of this class instance.
            String Name;
        public:
            /// @brief Class Constructor.
            CollisionShape();
            /// @brief Class Destructor.
            virtual ~CollisionShape();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the name of this shape.
            /// @return Returns a const reference string containing the name of this collision shape.
            virtual const String& GetName() const;
            /// @brief Gets the type of Collision shape this is.
            /// @return Returns an enum value indicating the type of collision shape this is.
            virtual CollisionShape::ShapeType GetType() const = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration Methods

            /// @brief Sets the padding that will be applied when checking for collisions.
            /// @param Margin A real in world units representing how much padding is to be applied to this shape.
            virtual void SetMargin(const Real& Margin);
            /// @brief Gets the amount of padding currently being applied to the collision shape.
            /// @return Returns the amount of padding, in world units, is being applied to the collision shape.
            virtual Real GetMargin() const;
            /// @brief Scales the collision shape on each of it's axes.
            /// @param Scaling A vector3 representing how much scaling should be applied on each of the shapes 3 axes.
            virtual void SetScaling(const Vector3& Scaling);
            /// @brief Gets the current scaling being applied to the collision shape.
            /// @return Returns a vector3 representing the amount of scaling being applied to the shape.
            virtual Vector3 GetScaling() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert this class to an XML::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this collision shape should be appended to.
            virtual void ProtoSerialize(XML::Node& CurrentRoot) const;
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and XML::Node containing the data.
            /// @warning A precondition of using this is that all of the actors intended for use must already be Deserialized.
            virtual void ProtoDeSerialize(const XML::Node& OneNode);
            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "CollisionShape"
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Sets the name of this collision shape.
            /// @remarks This method should be used with extreme care when it is stored by the collision shape manager.
            /// @param NewName The new name to be given to this shape.
            virtual void _SetShapeName(const String& NewName);
            /// @internal
            /// @brief Gets the internal shape pointer this collision shape is based on.
            /// @return Returns a pointer to the internal collision shape.
            virtual btCollisionShape* _GetInternalShape() const;
        };//CollisionShape

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Functions
        ///////////////////////////////////////
        /// @internal
        /// @brief Create A shape of a type and optionally model it after an example.
        /// @param ShapeToCreate The Type of Shape to create.
        /// @param Name the name of the new shape.
        /// @param ShapeToModel An optional pointer to a bullet shape to include instead of creating one.
        /// @return A new Mezzanine::CollisionShape with either default Values or the values of the bullet shape if one is provided.
        /// @warning The only checking done on the bullet shape is to verify it is not a null pointer. If the Internal shape fails to be of the appropriate kind correlating to ShapeToCreate then *undefined behavior* will occur.
        CollisionShape* MEZZ_LIB CreateShape(CollisionShape::ShapeType ShapeToCreate, const String& Name_, btCollisionShape* ShapeToModel);
        /// @brief Create a CollisionShape from a snippet of xml
        /// @param OneNode A Node for any Collision Shape that can be instanstiated
        /// @return A pointer to a CollisionShape of the Correct Type with the values stored in the XML.
        CollisionShape* MEZZ_LIB CreateShape(XML::Node OneNode);
        /// @internal
        /// @brief Convert from a Internal Collision Shape to a CollisionShape::ShapeType
        /// @param BulletShapeType The ShapeType to Convert
        /// @return The corresponding CollisionShape::ShapeType to the passed in bullet Shape Type if asuitable match exists
        /// @throw The Mezzanine engine only uses a subset of Bullets shapes, a Mezzanine::Exception with be thrown in the event an unsupported one is passed in.
        CollisionShape::ShapeType MEZZ_LIB InternalShapeTypeToShapeType(int InternalShapeType);
        /// @brief Get a string suitable for human eyes from a CollisionShape::ShapeType, may not be suitable for endusers.
        /// @param ShapeToConvert The kind of shape you want a string for.
        /// @return A String with human readable contents corresponding to the passed in type.
        String MEZZ_LIB ShapeTypeToString(CollisionShape::ShapeType ShapeToConvert);
        /// @brief Convert a human readable string (as provided by ShapeTypeToString()) and convert it to a CollisionShape::ShapeType
        /// @param TypeName A String that matches exactly what returns from ShapeTypeToString().
        /// @return A valid CollisionShape::ShapeType if possible
        /// @throw This throws a Mezzanine::Exception in the event of a gibberish name.
        CollisionShape::ShapeType MEZZ_LIB StringToShapeType(const String& TypeName);

        ///////////////////////////////////////////////////////////////////////////////
        /// @class CollisionShapeDeSerializer
        /// @brief A tool to aid in deserialization for the specific instances that DeSerialization CollisionShapes other ways does not make sense
        ///////////////////////////////////////
        class MEZZ_LIB CollisionShapeDeSerializer : public DeSerializer <CollisionShape>
        {
        protected:
            /// @internal
            /// @brief This Performs the work of Deserializing that DeSerialize and DeSerializeAndRetrieve need to do
            /// @param Stream the stream to deserialize from.
            /// @return a pointer to the shape just created, this may or may not be added to the collision shape manager depending on implementation details
            virtual CollisionShape* PerformDeSerialization(std::istream& Stream);
        public:
            /// @brief Convert An XML Node containing and one collision shape into a CollisionShape of the corresponding type
            /// @param OneNode A reference to the XML node to reconstitute into a live class instance.
            /// @details All items deserialized here will be added to the collision shape manager.
            /// @return A pointer to the freshly deserialized and created class instance.
            virtual CollisionShape* ProtoDeSerialize(const XML::Node& OneNode);
            /// @brief Create a collision shape from the serialized version in a stream.
            /// @param Stream The std::istream to get the data from.
            /// @details This performs less checking than the original to allow for DeSerialization of multiple kinds
            /// of xml elements. Rather all the specific checking is done closer to the actual instantion of classes.
            /// This add the DeSerialized shape to the collsion shape manager.
            /// @return This returns the input stream after the xml document has been extracted from it.
            virtual std::istream& DeSerialize(std::istream& Stream);
            /// @brief Create a collision shape from the serialized version in a stream.
            /// @param Stream The std::istream to get the data from.
            /// @details This adds the DeSerialized shape to the collsion shape manager.
            /// @return This returns a pointer to the freshly created collsion shape
            virtual CollisionShape* DeSerializeAndRetrieve(std::istream& Stream);
            /// @brief This will return the Name of the element that Contains multiple of the items to be DeSerialized
            /// @return A String containing "Shapes"
            virtual String ContainerName() const;
        };// CollisionShapeDeSerializer
    }//Physics
}//Mezzanine

#ifndef SWIG
/// @brief Serialize an CollisionShape and send it to a stream
/// @param ShapeToSerialize The CollisionShape serialize
/// @param stream the std::ostream to send the CollisionShape xml to.
/// @return The ostream after the new data has been inserted.
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Physics::CollisionShape& ShapeToSerialize);
/// @brief Get an actor from an XML stream.
/// @param stream The stream to get it out of.
/// @param x The it you will get out of the stream.
/// @return This returns the input stream to allow operator chaining.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Physics::CollisionShape& x);
/// @brief Converts an XML Node into a functional in memory construct.
/// @param OneNode The xml node that contains the deserialize class instance.
/// @param x The class instance to overwrite witht the proto serialized version in the node.
void MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::CollisionShape& x);
#endif


#endif
