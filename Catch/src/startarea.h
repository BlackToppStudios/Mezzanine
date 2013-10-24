#ifndef _startarea_h
#define _startarea_h

#include "levelzone.h"

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a level zone representing the starting area for throwables in the level where they can be manipulated.
/// @details
///////////////////////////////////////
class StartArea : public LevelZone
{
protected:
    /// @brief A pointer to the particle system portion of the Catch start area.
    Graphics::ParticleSystemProxy* ParSysProx;

    /// @brief Common constructor method for StartArea base class.
    /// @param HalfAreaSize Half of the size on each axis of the start area.
    virtual void CreateStartArea(const Vector3& HalfAreaSize);
    /// @brief Common destructor method for StartArea base class.
    virtual void DestroyStartArea();
    /// @brief Gets the offset from the bottom of this start area to the position of the particle system.
    /// @return Returns a Vector3 representing the world space offset from this start area to the particle system in use.
    virtual Vector3 GetParticleSystemOffset() const;
public:
    /// @brief Blank constructor.
    /// @param TheWorld A pointer to the world this object belongs to.
    StartArea(World* TheWorld);
    /// @brief Class constructor.
    /// @param Name The name to be given to this object.
    /// @param TheWorld A pointer to the world this object belongs to.
    /// @param HalfAreaSize Half of the size on each axis of the start area.
    StartArea(const String& Name, World* TheWorld, const Vector3& HalfAreaSize);
    /// @brief XML constructor.
    /// @param SelfRoot An XML::Node containing the data to populate this class with.
    /// @param TheWorld A pointer to the world this object belongs to.
    StartArea(const XML::Node& SelfRoot, World* TheWorld);
    /// @brief Class destructor.
    virtual ~StartArea();

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    /// @brief Gets a pointer to the particle portion of this zone.
    /// @return Returns a pointer to the ParticleSystem proxy used by this zone.
    virtual Graphics::ParticleSystemProxy* GetParticleSystemProxy() const;

    /// @brief Sets the minimum amount of time seconds will exist after being created by this zone.
    /// @param MinTTL The minimum amount of time particles will be allowed to exist in seconds.
    virtual void SetParticleMinimumTimeToLive(const Real MinTTL);
    /// @brief Gets the minimum amount of time seconds will exist after being created by this zone.
    /// @return Returns a Real representing the minimum amount of time particles will exist in seconds.
    virtual Real GetParticleMinimumTimeToLive() const;
    /// @brief Sets the maximum amount of time seconds will exist after being created by this zone.
    /// @param MaxTTL The maximum amount of time particles will be allowed to exist in seconds.
    virtual void SetParticleMaximumTimeToLive(const Real MaxTTL);
    /// @brief Gets the maximum amount of time seconds will exist after being created by this zone.
    /// @return Returns a Real representing the maximum amount of time particles will exist in seconds.
    virtual Real GetParticleMaximumTimeToLive() const;

    /// @copydoc Mezzanine::AreaEffect::ApplyEffect()
    virtual void ApplyEffect();

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
};//StartArea

///////////////////////////////////////////////////////////////////////////////
/// @brief A factory type for the creation of StartArea objects.
/// @details
///////////////////////////////////////
class StartAreaFactory : public AreaEffectFactory
{
public:
    /// @brief Class constructor.
    StartAreaFactory();
    /// @brief Class destructor.
    virtual ~StartAreaFactory();

    /// @copydoc Mezzanine::WorldObjectFactory::GetTypeName() const
    virtual String GetTypeName() const;

    /// @brief Creates a StartArea object.
    /// @param Name The name to be given to this object.
    /// @param TheWorld A pointer to the world this object belongs to.
    /// @param HalfAreaSize Half of the size on each axis of the start area.
    virtual StartArea* CreateStartArea(const String& Name, World* TheWorld, const Vector3& HalfAreaSize);
    /// @brief Creates a StartArea object.
    /// @param XMLNode The node of the xml document to construct from.
    /// @param TheWorld A pointer to the world this object belongs to.
    virtual StartArea* CreateStartArea(const XML::Node& XMLNode, World* TheWorld);

    /// @copydoc Mezzanine::WorldObjectFactory::CreateAreaEffect(const String&, World*, const NameValuePairMap&)
    virtual AreaEffect* CreateAreaEffect(const String& Name, World* TheWorld, const NameValuePairMap& Params);
    /// @copydoc Mezzanine::WorldObjectFactory::CreateAreaEffect(const XML::Node&)
    virtual AreaEffect* CreateAreaEffect(const XML::Node& XMLNode, World* TheWorld);
    /// @copydoc Mezzanine::WorldObjectFactory::DestroyAreaEffect(AreaEffect*)
    virtual void DestroyAreaEffect(AreaEffect* ToBeDestroyed);
};//StartAreaFactory

#endif
