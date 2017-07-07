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

    ///////////////////////////////////////////////////////////////////////////////
    // Working with the World

    ///////////////////////////////////////////////////////////////////////////////
    // LevelZone Properties

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
};//StartArea

///////////////////////////////////////////////////////////////////////////////
/// @brief A factory type for the creation of StartArea objects.
///////////////////////////////////////
class StartAreaFactory : public AreaEffectFactory
{
public:
    /// @brief Class constructor.
    StartAreaFactory();
    /// @brief Class destructor.
    virtual ~StartAreaFactory();

    /// @copydoc Mezzanine::EntityFactory::GetTypeName() const
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

    /// @copydoc Mezzanine::EntityFactory::CreateEntity(const String&, World*, const NameValuePairMap&)
    virtual Entity* CreateEntity(const String& Name, World* TheWorld, const NameValuePairMap& Params) override;
    /// @copydoc Mezzanine::EntityFactory::CreateEntity(const XML::Node&)
    virtual Entity* CreateEntity(const XML::Node& XMLNode, World* TheWorld) override;
    /// @copydoc Mezzanine::EntityFactory::DestroyEntity(Entity*)
    virtual void DestroyEntity(Entity* ToBeDestroyed) override;
};//StartAreaFactory

#endif
