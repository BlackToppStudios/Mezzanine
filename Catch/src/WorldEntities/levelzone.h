#ifndef _levelzone_h
#define _levelzone_h

#include <areaeffect.h>

using namespace Mezzanine;

///////////////////////////////////////////////////////////////////////////////
/// @brief This is the base class for the various zones presented in Catch levels.
/// @details
///////////////////////////////////////
class LevelZone : public AreaEffect
{
protected:
    /// @brief The half size of this zone
    Vector3 ZoneSize;

    /// @brief Common constructor method for LevelZone base class.
    virtual void CreateLevelZone();
    /// @brief Common destructor method for LevelZone base class.
    virtual void DestroyLevelZone();
public:
    /// @brief Blank constructor.
    /// @param TheWorld A pointer to the world this LevelZone belongs to.
    LevelZone(World* TheWorld);
    /// @brief Class constructor.
    /// @param EntID The unique ID of the LevelZone.
    /// @param TheWorld A pointer to the world this LevelZone belongs to.
    /// @param HalfZoneSize Half of the size on each axis of the zone.
    LevelZone(const EntityID& EntID, World* TheWorld, const Vector3& HalfZoneSize);
    /// @brief Class constructor.
    virtual ~LevelZone();

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    /// @copydoc Mezzanine::AreaEffect::GetEntityType() const
    virtual Mezzanine::EntityType GetEntityType() const;
    /// @brief Gets a pointer to the graphics portion of this zone.
    /// @return Returns a pointer to the Item proxy representing the graphics portion of this zone.
    virtual Graphics::ItemProxy* GetItemProxy() const;

    /// @brief Checks if an object is inside this zone.
    /// @param Object The object to check inside this zone for.
    /// @return Returns true if the provided object is currently in this zone, false otherwise.
    virtual Boole IsInside(Entity* Object);
    /// @brief Checks if this zone is false.
    /// @return Returns true if this zone contains no objects, false otherwise.
    virtual Boole IsEmpty();

    ///////////////////////////////////////////////////////////////////////////////
    // Working with the World

    ///////////////////////////////////////////////////////////////////////////////
    // LevelZone Properties

    /// @brief Gets the half-size of this zone.
    /// @return Returns a vector3 containing the half-size of this zone.
    virtual Vector3 GetZoneSize() const;

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    /// @copydoc Mezzanine::WorldEntity::ProtoSerializeProperties(XML::Node&) const
    virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
    /// @copydoc Mezzanine::WorldEntity::ProtoDeSerializeProperties(const XML::Node&)
    virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

    /// @copydoc Mezzanine::WorldEntity::GetDerivedSerializableName() const
    virtual String GetDerivedSerializableName() const;
    /// @copydoc Mezzanine::WorldEntity::GetSerializableName()
    static String GetSerializableName();
};//LevelZone

#endif
