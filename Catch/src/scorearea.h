#ifndef _scorearea_h
#define _scorearea_h

#include "levelzone.h"

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a level zone representing the end zone where throwables contribute to the level score.
/// @details
///////////////////////////////////////
class ScoreArea : public LevelZone
{
protected:
    /// @brief The score modifier applied to the score values of each throwable in this zone.
    Real ScoreMultiplier;

    /// @brief Common constructor method for ScoreArea base class.
    /// @param HalfAreaSize Half of the size on each axis of the score area.
    virtual void CreateScoreArea(const Vector3& HalfAreaSize);
    /// @brief Common destructor method for ScoreArea base class.
    virtual void DestroyScoreArea();
public:
    /// @brief Blank constructor.
    /// @param TheWorld A pointer to the world this object belongs to.
    ScoreArea(World* TheWorld);
    /// @brief Class constructor.
    /// @param Name The name to be given to this object.
    /// @param TheWorld A pointer to the world this object belongs to.
    /// @param HalfAreaSize Half of the size on each axis of the score area.
    ScoreArea(const String& Name, World* TheWorld, const Vector3& HalfAreaSize);
    /// @brief XML constructor.
    /// @param SelfRoot An XML::Node containing the data to populate this class with.
    /// @param TheWorld A pointer to the world this object belongs to.
    ScoreArea(const XML::Node& SelfRoot, World* TheWorld);
    /// @brief Class destructor.
    virtual ~ScoreArea();

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    /// @copydoc Mezzanine::AreaEffect::ApplyEffect()
    virtual void ApplyEffect();
    /// @brief Gets whether or not all objects in this zone are at rest.
    /// @return Returns true if all objects in this score zone are at rest, false otherwise.
    virtual Bool AllObjectsAtRest();

    ///////////////////////////////////////////////////////////////////////////////
    // LevelZone Properties

    /// @brief Sets the score modifier for throwables in this zone.
    /// @param Multiplier The score modifier that will be applied to each throwable in this zone.
    virtual void SetScoreMultiplier(const Real Multiplier);
    /// @brief Gets the score modifier for throwables in this zone.
    /// @return Returns a Real representing the score modifier for all objects in this zone.
    virtual Real GetScoreMultiplier() const;

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    /// @copydoc Mezzanine::WorldObject::ProtoSerializeProperties(XML::Node&) const
    virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
    /// @copydoc Mezzanine::WorldObject::ProtoDeSerializeProperties(const XML::Node&)
    virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

    /// @copydoc Mezzanine::WorldObject::GetDerivedSerializableName() const
    virtual String GetDerivedSerializableName() const;
    /// @copydoc Mezzanine::WorldObject::GetSerializableName()
    static String GetSerializableName();
};//ScoreArea

///////////////////////////////////////////////////////////////////////////////
/// @brief A factory type for the creation of ScoreArea objects.
/// @details
///////////////////////////////////////
class ScoreAreaFactory : public AreaEffectFactory
{
public:
    /// @brief Class constructor.
    ScoreAreaFactory();
    /// @brief Class destructor.
    virtual ~ScoreAreaFactory();

    /// @copydoc Mezzanine::WorldObjectFactory::GetTypeName() const
    virtual String GetTypeName() const;

    /// @brief Creates a ScoreArea object.
    /// @param Name The name to be given to this object.
    /// @param TheWorld A pointer to the world this object belongs to.
    /// @param HalfAreaSize Half of the size on each axis of the start area.
    virtual ScoreArea* CreateScoreArea(const String& Name, World* TheWorld, const Vector3& HalfAreaSize);
    /// @brief Creates a ScoreArea object.
    /// @param XMLNode The node of the xml document to construct from.
    /// @param TheWorld A pointer to the world this object belongs to.
    virtual ScoreArea* CreateScoreArea(const XML::Node& XMLNode, World* TheWorld);

    /// @copydoc Mezzanine::WorldObjectFactory::CreateAreaEffect(const String&, World*, const NameValuePairMap&)
    virtual AreaEffect* CreateAreaEffect(const String& Name, World* TheWorld, const NameValuePairMap& Params);
    /// @copydoc Mezzanine::WorldObjectFactory::CreateAreaEffect(const XML::Node&)
    virtual AreaEffect* CreateAreaEffect(const XML::Node& XMLNode, World* TheWorld);
    /// @copydoc Mezzanine::WorldObjectFactory::DestroyAreaEffect(AreaEffect*)
    virtual void DestroyAreaEffect(AreaEffect* ToBeDestroyed);
};//ScoreAreaFactory

#endif
