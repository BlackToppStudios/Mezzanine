#ifndef _levelzones_h
#define _levelzones_h

#include <areaeffect.h>

using namespace Mezzanine;

class LevelZone : public AreaEffect
{
    protected:
    public:
        LevelZone(const String& name, const Vector3& Location);
        virtual ~LevelZone();
        virtual void ApplyEffect() = 0;
        virtual bool IsInside(ActorBase* Actor);
        virtual bool IsEmpty();
        Mezzanine::WorldObjectType GetType() const;
};//LevelZone

class StartArea : public LevelZone
{
    protected:
        bool Init;
    public:
        StartArea(const String& name, const Vector3& Location);
        ~StartArea();
        void ApplyEffect();
};

class ScoreArea : public LevelZone
{
    protected:
        Real ScoreMultiplier;
    public:
        ScoreArea(const String& name, const Vector3& Location);
        ~ScoreArea();
        void ApplyEffect();
        bool AllObjectsAtRest();
        void SetScoreMultiplier(const Real& Multiplier);
        Real GetScoreMultiplier() const;
};

#endif
