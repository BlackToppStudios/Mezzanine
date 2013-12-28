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
    /// @brief A pointer to the graphics representation of this zone.
    Graphics::EntityProxy* EntProx;

    /// @brief Common constructor method for LevelZone base class.
    virtual void CreateLevelZone();
    /// @brief Common destructor method for LevelZone base class.
    virtual void DestroyLevelZone();
public:
    /// @brief Blank constructor.
    /// @param TheWorld A pointer to the world this object belongs to.
    LevelZone(World* TheWorld);
    /// @brief Class constructor.
    /// @param Name The name to be given to this object.
    /// @param TheWorld A pointer to the world this object belongs to.
    /// @param HalfZoneSize Half of the size on each axis of the zone.
    LevelZone(const String& Name, World* TheWorld, const Vector3& HalfZoneSize);
    /// @brief Class constructor.
    virtual ~LevelZone();

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    /// @copydoc Mezzanine::AreaEffect::GetType() const
    virtual Mezzanine::WorldObjectType GetType() const;
    /// @brief Gets a pointer to the graphics portion of this zone.
    /// @return Returns a pointer to the Entity proxy representing the graphics portion of this zone.
    virtual Graphics::EntityProxy* GetEntityProxy() const;

    /// @copydoc Mezzanine::AreaEffect::ApplyEffect()
    virtual void ApplyEffect() = 0;
    /// @brief Checks if an object is inside this zone.
    /// @param Object The object to check inside this zone for.
    /// @return Returns true if the provided object is currently in this zone, false otherwise.
    virtual Boolean IsInside(WorldObject* Object);
    /// @brief Checks if this zone is false.
    /// @return Returns true if this zone contains no objects, false otherwise.
    virtual Boolean IsEmpty();

    /// @copydoc Mezzanine::WorldObject::GetProxies(ProxyContainer&)
    virtual void GetProxies(ProxyContainer& Proxies);
    /// @copydoc Mezzanine::WorldObject::GetProxies(const UInt32, ProxyContainer&)
    virtual void GetProxies(const UInt32 Types, ProxyContainer& Proxies);

    ///////////////////////////////////////////////////////////////////////////////
    // Working with the World

    /// @copydoc Mezzanine::WorldObject::AddToWorld()
    virtual void AddToWorld();
    /// @copydoc Mezzanine::WorldObject::RemoveFromWorld()
    virtual void RemoveFromWorld();

    ///////////////////////////////////////////////////////////////////////////////
    // LevelZone Properties

    ///////////////////////////////////////////////////////////////////////////////
    // Transform Methods

    /// @copydoc Mezzanine::TransformableObject::SetLocation(const Vector3&)
    virtual void SetLocation(const Vector3& Loc);
    /// @copydoc Mezzanine::TransformableObject::SetLocation(const Real, const Real, const Real)
    virtual void SetLocation(const Real X, const Real Y, const Real Z);
    /// @copydoc Mezzanine::TransformableObject::GetLocation() const
    virtual Vector3 GetLocation() const;
    /// @copydoc Mezzanine::TransformableObject::SetOrientation(const Quaternion&)
    virtual void SetOrientation(const Quaternion& Ori);
    /// @copydoc Mezzanine::TransformableObject::SetOrientation(const Real, const Real, const Real, const Real)
    virtual void SetOrientation(const Real X, const Real Y, const Real Z, const Real W);
    /// @copydoc Mezzanine::TransformableObject::GetOrientation() const
    virtual Quaternion GetOrientation() const;
    /// @copydoc Mezzanine::TransformableObject::SetScale(const Vector3&)
    virtual void SetScale(const Vector3& Sc);
    /// @copydoc Mezzanine::TransformableObject::SetScale(const Real, const Real, const Real)
    virtual void SetScale(const Real X, const Real Y, const Real Z);
    /// @copydoc Mezzanine::TransformableObject::GetScale() const
    virtual Vector3 GetScale() const;

    /// @copydoc Mezzanine::TransformableObject::Translate(const Vector3&)
    virtual void Translate(const Vector3& Trans);
    /// @copydoc Mezzanine::TransformableObject::Translate(const Real, const Real, const Real)
    virtual void Translate(const Real X, const Real Y, const Real Z);
    /// @copydoc Mezzanine::TransformableObject::Yaw(const Real)
    virtual void Yaw(const Real Angle);
    /// @copydoc Mezzanine::TransformableObject::Pitch(const Real)
    virtual void Pitch(const Real Angle);
    /// @copydoc Mezzanine::TransformableObject::Roll(const Real)
    virtual void Roll(const Real Angle);
    /// @copydoc Mezzanine::TransformableObject::Rotate(const Vector3&, const Real)
    virtual void Rotate(const Vector3& Axis, const Real Angle);
    /// @copydoc Mezzanine::TransformableObject::Rotate(const Quaternion&)
    virtual void Rotate(const Quaternion& Rotation);
    /// @copydoc Mezzanine::TransformableObject::Scale(const Vector3&)
    virtual void Scale(const Vector3& Scale);
    /// @copydoc Mezzanine::TransformableObject::Scale(const Real, const Real, const Real)
    virtual void Scale(const Real X, const Real Y, const Real Z);

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    /// @copydoc Mezzanine::WorldObject::ProtoSerializeProperties(XML::Node&) const
    virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
    /// @copydoc Mezzanine::WorldObject::ProtoSerializeProxies(XML::Node&) const
    virtual void ProtoSerializeProxies(XML::Node& SelfRoot) const;

    /// @copydoc Mezzanine::WorldObject::ProtoDeSerializeProperties(const XML::Node&)
    virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);
    /// @copydoc Mezzanine::WorldObject::ProtoDeSerializeProxies(const XML::Node&)
    virtual void ProtoDeSerializeProxies(const XML::Node& SelfRoot);

    /// @copydoc Mezzanine::WorldObject::GetDerivedSerializableName() const
    virtual String GetDerivedSerializableName() const;
    /// @copydoc Mezzanine::WorldObject::GetSerializableName()
    static String GetSerializableName();

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    /// @copydoc Mezzanine::WorldObject::_NotifyProxyDestroyed(WorldProxy*)
    virtual void _NotifyProxyDestroyed(WorldProxy* ToBeDestroyed);
};//LevelZone

#endif
